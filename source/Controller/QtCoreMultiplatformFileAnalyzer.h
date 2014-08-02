/**
 * File: QtCoreMultiplatformFileAnalyzer.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/09
*/

#ifndef CORE_H
#define CORE_H

#include "icoremultiplatformfileanalyzer.h"

#include <QStringList>

class QDir;
enum ERR_CODE;

/**
 * @brief The QtCoreMultiplatformFileAnalyzer class implements the ICoreMultiplatformFileAnalyzer interface with the support of Qt classes
 */
class QtCoreMultiplatformFileAnalyzer : public ICoreMultiplatformFileAnalyzer
{
private:
    /**
     * @brief m_platformsVec: list of the platform
     */
    QStringList m_platformsList;

    /**
     * @brief m_pResultData: where to store result data. It's re-initialize every call of execute
     */
    ResultData* m_pResultData;

public:

    /**
     * @brief execute: start the analysis by control the input data and then call the real analysis function
     * @param resultData: where store the result of the analysis
     * @param inputData: input data of the analysisi
     */
    void Execute(ResultData *o_pResultData, InputData* pInputData);

private:

    ERR_CODE ControlInputData(InputData* pInputData);

    /**
     * @brief analyzeDir: recursive function the execute the analysis from a path to a directory
     * @param o_resultData: where the function stores the results
     */
    void AnalyzeDir(const QString& sourcePath);

    /**
     * @brief TrueAnalyzeDir: analyze only the source path dir
     * @param sourcePath: the directory to analyze
     */
    void TrueAnalyzeDir(const QString& sourcePath, const QDir& sourceDir);

    /**
     * @brief AnalyzeSubDir: call an analyze dir in all the subdirectory of the input directory
     * @param sourceDir: The dir from where find sub directory
     */
    void AnalyzeSubDir(const QDir& sourceDir);

    /**
     * @brief AnalyzeFile: analyze a file of a given platform and fill a FilePresence in to o_resultData
     * @param o_resultData: where put the FilePresence with the result
     * @param platformName: the name of the platform in which the file was found, so this platform is skipped during the analysis
     * @param relativeFileName: the name of the file to analyze. It has to be only the name without the path
     * @param sourcePath: the source path of the platform directory.
     *
     * @example If the relativeFileName is 'pippo.xml',the platformName is 'PS3' and the sourcePath is 'C:Data'.
     *          The complete path of the file to analyze is C:Data\PS3\pippo.xml
     */
    void AnalyzeFile(const QString& platformName, const QString& relativeFileName, const QString& sourcePath);
};

#endif // CORE_H
