/**
 * File: ResultTableWidget.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/10
*/

#include "ResultTableWidget.h"

#include <QTextEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QDesktopServices>
#include <QString>
#include <QMenu>

#include "../Data/resultdata.h"
#include "../Data/FileOccurrence.h"

ResultTableWidget::ResultTableWidget(QWidget* parent): ResultWidgetBase(parent)
{
    //Because at the first last add the table needs to be re-initialized
    m_lastUpdateWasALastAdd = false;

    //days as defalut
    m_hoursUnitDisplay = false;

    //Initialize the table widget to display the results
    m_pTable = new QTableWidget;
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_pTable->setToolTip("Duble click to open the directory in explorer");

    //Bind cell double clicked to open the path in exploer of window
    connect(m_pTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(OpenPathInExplorer(int,int)));
    //Bind the sort indicator changes on the horinzontal header with the function that execute the requested sort
    connect(m_pTable->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(SortByColumn(int,Qt::SortOrder)));

    //Set for the contextual menu
    m_pTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextualMenuTable(QPoint)));

    //Fill the static headers list
    m_fixedtableHeaders << "File" << "Path" << "Max Delta";

    //Initialize the layout
    m_pLayout = new QVBoxLayout;
    //Set the layout
    setLayout(m_pLayout);

    //Set the table to the layout
    m_pLayout->addWidget(m_pTable);
}

ResultTableWidget::~ResultTableWidget()
{
    delete m_pLayout;
}

void ResultTableWidget::Update(UPDATE_CODE updateCode)
{
    if(updateCode == UPDATE_ERR)
    {
        ReinizializeTable();
    }

    if(updateCode == UPDATE_ALL)
    {
        m_lastUpdateWasALastAdd = false;
        return;
    }

    if(updateCode == UPDATE_LAST_ADD)
    {
        //If false, it there were almost one UPDATE_ALL so the table need to be re-initialized
        if(m_lastUpdateWasALastAdd == false)
        {
            m_lastUpdateWasALastAdd = true;
            //Because before that there was a UPDATE_ALL a reinitialize is necessary
            ReinizializeTable();
        }

        //Add the last presence found to the table
        AddLastPresenceToANewRow();
    }
}

void ResultTableWidget::SetHoursUnitDisplay(bool hoursUnitDisplay)
{
    m_hoursUnitDisplay = hoursUnitDisplay;
}

void ResultTableWidget::ReinizializeTable()
{
    //reinitialize the platform vector with the new onw
    const vector<string>& allPlatformVector = m_pResultData->GetAllPlatformVector();

    //Copy the fixed headers
    QStringList headers = m_fixedtableHeaders;
    //Add to header all the platforms
    headers << "PARENT";
    for(unsigned int i=0; i<allPlatformVector.size(); ++i)
    {
        headers << QString::fromStdString(allPlatformVector.at(i));
    }

    m_pTable->setRowCount(0);

    //Set the new headers and the right column count: fixed + one for each platform, + 1 for the parent directory
    m_pTable->setColumnCount(m_fixedtableHeaders.size() + allPlatformVector.size() +1);
    m_pTable->setHorizontalHeaderLabels(headers);

    m_pTable->horizontalHeader()->setSortIndicatorShown(true);

    //Set the right column width
     m_pTable->setColumnWidth(0, 200);//file name
    //Platform column
    for(unsigned int i=0; i<allPlatformVector.size()+1; ++i)
    {
        m_pTable->setColumnWidth(i+m_fixedtableHeaders.size(), 120);
    }
}

void ResultTableWidget::AddLastPresenceToANewRow()
{
    const FileOccurrence& filePresence = m_pResultData->GetLastFilePresence();

    int rowIndex = m_pTable->rowCount(); //The index of the new row
    int columnIndex = 0;

    m_pTable->insertRow(rowIndex); //Insert a empty row at the bottom of the table

    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(filePresence.GetRelativeFileName())); //Relative file name
    //item->setToolTip(*it);
    m_pTable->setItem(rowIndex, columnIndex, item);
    columnIndex++;

    //Remove from the absolute path the known part
    QString& absolutePath = QString::fromStdString(filePresence.GetAbsolutePath());
    QString& sourcePath = QString::fromStdString(m_pResultData->GetSourcepath());

    absolutePath.replace(QString("\\"), QString("/"), Qt::CaseInsensitive); //Transform absolute path from \ to /
    sourcePath.replace(QString("\\"), QString("/"), Qt::CaseInsensitive); //Transform source path from \ to /
    absolutePath.replace(sourcePath, QString(), Qt::CaseInsensitive); //Remove from the absolute path the known part
    if(!absolutePath.startsWith('/'))
    {
        absolutePath.prepend('/');
    }

    item = new QTableWidgetItem(absolutePath); //Absolute path
    //item->setToolTip(*it);
    m_pTable->setItem(rowIndex, columnIndex, item);
    columnIndex++;

    //MaxDelta
    columnIndex++;

    vector<string> foundPlatform = filePresence.GetFoundPlatform();
    vector<string> foundPlatformDate = filePresence.GetFoundPlatformDates();

    QString maxPlatformDateFound = "";
    int columnOfMaxPlatformDateFound = -2; //There will find almost one correct

    QString minPlatformDateFound = "ZZZZZZZZZZZZZZZZZZZZ";
    int columnOfMinPlatformDateFound = -3; //There will find almost one correct

    //Set all the platform date
    for(unsigned int i=0; i<foundPlatform.size(); ++i)
    {
        //Where to insert the platform date. Has to be initialized with the correct value
        int platformColumn = -1;

        //Find the column that match the platform name; starts with column index for jump the fixed column that isn't platform
        for(int k=columnIndex; k<m_pTable->columnCount(); ++k)
        {
            if( QString::compare(QString::fromStdString(foundPlatform.at(i)), m_pTable->horizontalHeaderItem(k)->text(), Qt::CaseInsensitive) == 0)
            {
                platformColumn = k;
            }
        }

        QString platformDate = QString::fromStdString(foundPlatformDate.at(i));
        platformDate.replace("T", " ");
        platformDate.replace("-", "/");

        //Seach for the biggest
        if(platformDate >= maxPlatformDateFound)
        {
            maxPlatformDateFound = platformDate;
            columnOfMaxPlatformDateFound = platformColumn;
        }

        //Search for the smallest
        if(platformDate <= minPlatformDateFound)
        {
            minPlatformDateFound = platformDate;
            columnOfMinPlatformDateFound = platformColumn;
        }

        item = new QTableWidgetItem(platformDate); //Platform date
        //item->setToolTip(*it);
        m_pTable->setItem(rowIndex, platformColumn, item);
    }

    //Color the most recent platform and the worst recent
    m_pTable->item(rowIndex, columnOfMaxPlatformDateFound)->setTextColor(Qt::darkGreen);
    m_pTable->item(rowIndex, columnOfMinPlatformDateFound)->setTextColor(Qt::red);

    const qint64& deltaS = GiveDifferenceInSeconds(maxPlatformDateFound, minPlatformDateFound);

    //fill the max delta cell
    //display in hours or days
    if(m_hoursUnitDisplay)
    {
        item = new QTableWidgetItem(QString::number(deltaS/(3600)) + " h"); //Max delta
    }
    else
    {
       item = new QTableWidgetItem(QString::number(deltaS/(24*3600)) + " d"); //Max delta
    }
    m_pTable->setItem(rowIndex, 2, item);

    //Color the delta max if is greater than the warning time
    if(deltaS >= m_pResultData->GetWarningTimeS())
    {
        m_pTable->item(rowIndex, 2)->setTextColor(Qt::red);
    }
}

qint64 ResultTableWidget::GiveDifferenceInSeconds(const QString& dateBig, const QString& dateSmall)
{
    //2 elements: data and time of the day
    const QStringList& dateAndTimeOfTheDay1 = dateBig.split(" ", QString::SkipEmptyParts);

    //3  elements: year, month and day
    const QStringList& yearMonthDay1 = dateAndTimeOfTheDay1.at(0).split("/", QString::SkipEmptyParts);
    const QStringList& hourMinuteSecond1 = dateAndTimeOfTheDay1.at(1).split(":", QString::SkipEmptyParts);

    QDateTime timeBig; //= QDateTime::fromString(date, "yyyy-MM-ddTHH:mm:ss");
    timeBig.setDate(QDate(yearMonthDay1.at(0).toInt(), yearMonthDay1.at(1).toInt(), yearMonthDay1.at(2).toInt()));
    timeBig.setTime(QTime(hourMinuteSecond1.at(0).toInt(), hourMinuteSecond1.at(1).toInt(), hourMinuteSecond1.at(2).toInt()));

    qint64 secondsFrom1970Big = timeBig.toMSecsSinceEpoch()/1000;

    const QStringList& dateAndTimeOfTheDay2 = dateSmall.split(" ", QString::SkipEmptyParts);

    //3  elements: year, month and day
    const QStringList& yearMonthDay2 = dateAndTimeOfTheDay2.at(0).split("/", QString::SkipEmptyParts);
    const QStringList& hourMinuteSecond2 = dateAndTimeOfTheDay2.at(1).split(":", QString::SkipEmptyParts);

    QDateTime timeSmall; //= QDateTime::fromString(date, "yyyy-MM-ddTHH:mm:ss");
    timeSmall.setDate(QDate(yearMonthDay2.at(0).toInt(), yearMonthDay2.at(1).toInt(), yearMonthDay2.at(2).toInt()));
    timeSmall.setTime(QTime(hourMinuteSecond2.at(0).toInt(), hourMinuteSecond2.at(1).toInt(), hourMinuteSecond2.at(2).toInt()));

    const qint64& secondsFrom1970Small = timeSmall.toMSecsSinceEpoch()/1000;

    return secondsFrom1970Big - secondsFrom1970Small;
}

void ResultTableWidget::OpenPathInExplorer(int indexRow, int) const
{
    const QString& basePath = QString::fromStdString(m_pResultData->GetSourcepath());
    /*if(basePath.lastIndexOf("\\") == basePath.length()-1 || basePath.lastIndexOf("/") == basePath.length()-1)
        basePath.resize(basePath.length()-1);*/

    //todo

    QDesktopServices::openUrl(QUrl("file:///"+ basePath + m_pTable->item(indexRow, 1)->text(), QUrl::TolerantMode));
}

void ResultTableWidget::SortByColumn(int indexColumn, Qt::SortOrder sortOrder)
{
    m_pTable->sortByColumn(indexColumn, sortOrder);
}

void ResultTableWidget::ShowContextualMenuTable(QPoint point)
{
    //Create a menu ad fill with the actions
    QMenu menu;
    QAction* showInExplorerAct = new QAction("Show in explorer", 0);
    menu.addAction(showInExplorerAct);

    //Disable the actions if none cell was selected
    QList<QTableWidgetItem*> selectedCellList = m_pTable->selectedItems();
    if(selectedCellList.size() < 1)
    {
        showInExplorerAct->setDisabled(true);
    }

    //Show contextual menu
    QAction* itemSelected = menu.exec(this->mapToGlobal(point));

    //Execute what the user has requested
    if( itemSelected == showInExplorerAct )
    {
        OpenPathInExplorer(selectedCellList.at(0)->row(), -1);
    }
}
