/**
 * File: QtCoreMultiplatformFileAnalyzer.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/09
*/

#include "QtCoreMultiplatformFileAnalyzer.h"

#include "../Data/resultdata.h"
#include "../Data/inputdata.h"
#include "../Data/FileOccurrence.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include <set>

void QtCoreMultiplatformFileAnalyzer::Execute(ResultData *o_pResultData, InputData* pInputData)
{
    //Set the new result data
    m_pResultData = o_pResultData;

    //Set the source path in the results. It has to be setted even if there's errors.
    m_pResultData->SetSourcePath(pInputData->GetSourcePath());

    //Set the time warning to the result object
    m_pResultData->SetWarningTimeS(pInputData->GetWarningTimeS());

    //Set the platform list in the resultData
    m_pResultData->SetPlatforms(pInputData->GetPlatformNamesVector());

    ERR_CODE errCode = ControlInputData(pInputData);

    //Set the error code in the result. Even if it's ERR_NO_ERR that means the input data are ok
    m_pResultData->SetErrorCode(errCode);

    //If there's no error start the Analysis
    if(errCode == ERR_NO_ERR)
    {
        const vector<string>& platformsVect = pInputData->GetPlatformNamesVector();

        m_platformsList.clear();
        //Store the platform names in the member parameter in Qt list for faster access.
        for(size_t i=0; i<platformsVect.size(); ++i)
        {
            m_platformsList << QString::fromStdString(platformsVect.at(i));
        }

        //Launch the real analysis from the source path
        this->AnalyzeDir(QString::fromStdString(m_pResultData->GetSourcepath()));
    }
}

ERR_CODE QtCoreMultiplatformFileAnalyzer::ControlInputData(InputData* pInputData)
{
    //For read the info of the given source path
    QFileInfo sourceDirInfo(QString::fromStdString(pInputData->GetSourcePath()));

    //Control if the path exists
    if(!sourceDirInfo.exists())
    {
        return ERR_PATH_NON_EXISTENT;
    }

    //Control if the file is a directory
    if(!sourceDirInfo.isDir())
    {
        return ERR_PATH_ISNT_DIR;
    }

    //Control if the path use an absolute notation
    if(QDir::isRelativePath(QString::fromStdString(pInputData->GetSourcePath())))
    {
        return ERR_PATH_RELATIVE;
    }

    //Control that there are almost one platform
    if(pInputData->GetPlatformNamesVector().size() == 0)
    {
        return ERR_ZERO_PLATFORMS;
    }

    return ERR_NO_ERR;
}

void QtCoreMultiplatformFileAnalyzer::AnalyzeDir(const QString &sourcePath)
{
    //From where search the platform directories
    QDir sourceDir(sourcePath);
    sourceDir.setFilter(QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot);

    //First do the job on this directory and then launch the recursive call on all subdirectory

    //Analyze the file in this dir
    TrueAnalyzeDir(sourcePath, sourceDir);

    //Analyze all the sub directory
    AnalyzeSubDir(sourceDir);
}

void QtCoreMultiplatformFileAnalyzer::TrueAnalyzeDir(const QString& sourcePath, const QDir& sourceDir)
{
    //List of all files already analysed
    QSet<QString> alreadyAnalysedFiles;

    //serach for all platform directory child of the source directory
    //This for select a platform
    for(int i=0; i<m_platformsList.size(); ++i)
    {
        //Platform name of the cycle
        const QString& platformName = m_platformsList.at(i);

        QStringList platformDirFilter;
        platformDirFilter << platformName;
        //sourceDir.setNameFilters(platformDirFilter);

        //List of the directory that match the platform chosen. Obviosly it will be one o zero because it can't exist two directoru with the same name
        const QFileInfoList& list = sourceDir.entryInfoList(platformDirFilter); //In order to filter only the selected platform directory if it exist

        //If the list is empty the platform dir doesn't exist, if exist iterate over all files inside
        if(list.size() > 0)
        {
            //Cycle over all the file inside the platform directory
            QDir platformDir(list.at(0).absoluteFilePath());
            platformDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
            //platformDir.setSorting(QDir::Name | QDir::Reversed); //Order by name, smaller name first

            const QFileInfoList& listPlatformFiles = platformDir.entryInfoList();

            for (QFileInfoList::ConstIterator filePlatformIter = listPlatformFiles.constBegin(); filePlatformIter != listPlatformFiles.constEnd(); ++filePlatformIter)
            {
                //Name of the potential file to analyse, only the name file with the path
                const QString& relativeFileName = (*filePlatformIter).fileName();

                //Check if the file was already found in others platform directory and so also already analysed
                if( ! alreadyAnalysedFiles.contains(relativeFileName)) //if it isn't contained
                {
                    //Insert the file in the set of already visited file
                    alreadyAnalysedFiles.insert(relativeFileName);

                    //Analyse file call to analyse this single file
                    AnalyzeFile(platformName, relativeFileName, sourcePath);
                }
            }
        }
    }
}

void QtCoreMultiplatformFileAnalyzer::AnalyzeSubDir(const QDir& sourceDir)
{
    //Recursive call on all the other not-platform directory. sadly it doesn't exist the negative filter for Qdir list
    //Cycle on all the directory
    const QFileInfoList& listAllDir = sourceDir.entryInfoList();
    for(int i=0; i<listAllDir.size(); ++i)
    {
        //Name of the selected directory to analyze
        const QString& dirName = listAllDir.at(i).fileName();

        bool isANotPlatformDir = true;
        //Control if a directory is a platform directory
        for(int k=0; k<m_platformsList.size() && isANotPlatformDir; ++k)
        {
            if( m_platformsList.at(k) == dirName)
            {
                isANotPlatformDir = false;
            }
        }

        if(isANotPlatformDir)
        {
            AnalyzeDir(listAllDir.at(i).absoluteFilePath());
        }
    }
}

void QtCoreMultiplatformFileAnalyzer::AnalyzeFile(const QString &platformName, const QString &relativeFileName, const QString &sourcePath)
{
    //Instantiate the FilePresence with the file
    FileOccurrence filePresence(sourcePath.toStdString(), relativeFileName.toStdString());

    //Add to the filePresence the analysed file
    QFileInfo fileInfo( sourcePath+"\\"+platformName+"\\"+relativeFileName );  //TODO STRINGBUILDER
    filePresence.AddPresence(platformName.toStdString(), fileInfo.lastModified().toString(Qt::ISODate).toStdString());

    //From where search the other platform directories
    QDir sourceDir(sourcePath);
    sourceDir.setFilter(QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot);

    QStringList platformDirFilter;
    //Add in the filter of the visualized directory all the platform except the one of the file
    for(int i=0; i<m_platformsList.size(); ++i)
    {
        if(m_platformsList.at(i) != platformName)
        {
            platformDirFilter << m_platformsList.at(i);
        }
    }
    sourceDir.setNameFilters(platformDirFilter);

    //List of the directory that match the platform chosen. Obviosly it will be one o zero because it can't exist two directoru with the same name
    const QFileInfoList& otherPlatformsList = sourceDir.entryInfoList(platformDirFilter); //In order to filter only the selected platform directory if it exist

    bool foundOtherFiles = false;

    //If the list is empty the other platforms don't exist, if exist iterate all the platform searching for the file
    for(int i=0; i<otherPlatformsList.size(); ++i)
    {
        //Create a QFileInfo of the potential file in this platform directory
        QFileInfo otherFileInfo(otherPlatformsList.at(i).absoluteFilePath() + "\\" + relativeFileName);

        //If exist it will be added to the FilePresence
        if(otherFileInfo.exists())
        {
            foundOtherFiles = true;
            filePresence.AddPresence(otherFileInfo.dir().dirName().toStdString(), otherFileInfo.lastModified().toString(Qt::ISODate).toStdString());
        }

    }

    //Controlla nella parent senza platform se ce il file e aggiungilo nel caso
    QFileInfo otherFileParentInfo( sourcePath + "\\" +relativeFileName);
    if(otherFileParentInfo.exists())
    {
        foundOtherFiles = true;
        filePresence.AddPresence("PARENT", otherFileParentInfo.lastModified().toString(Qt::ISODate).toStdString());
    }

    //If it was found other files the FilePresence has to be butted on the results
    if(foundOtherFiles)
    {
        m_pResultData->AddFilePresence(filePresence);
    }
}
