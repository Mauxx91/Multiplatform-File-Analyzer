/**
 * File: ViewMainWindow.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#include "ViewMainWindow.h"

#include "../Controller/controller.h"
#include "resulttablewidget.h"
#include "../Data/resultdata.h"
#include "../version.h"

#include <QCoreApplication>
#include <QSettings>

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStatusBar>
#include <QComboBox>
#include <QPushButton>
#include <QString>
#include <QFileDialog>

ViewMainWindow::ViewMainWindow(ResultWidgetBase* resultWidget):
    m_pResultWidget(resultWidget), QMainWindow(0)
{
    m_numResult = 0;

    //Set the window title and version
    setWindowTitle("MultiplatformFileAnalyzer " VER_FILEVERSION_STR);

    //Set application
    QCoreApplication::setOrganizationName("Milestone S.r.l");
    QCoreApplication::setApplicationName("DataLastTimeAnalyzer");

    //Initialize the settings to access the stored input of the previously session
    m_pSettings = new QSettings();

    //Initialize the keys to access to the stored data
    m_keySourcePath = "k_SourcePat";
    m_keyPlatformName = "k_PlatformName";
    m_keyWarningTimeLine = "k_WarningTimeLine";
    m_keyWarningTimeCombo = "k_WarningTimeCombo";

    //Set status bar in which the status tip will be displayed
    statusBar()->showMessage("Ready");

    //Inizialize the general layout of the widget
    QVBoxLayout* verticalLayout = new QVBoxLayout;

    //Inizialize the input layout, it will be inserted first in the vertical layout
    QFormLayout* formLayout = new QFormLayout;

    //Source path input
    QLabel* sourcePathLabel = new QLabel("Source path:");
    sourcePathLabel->setStatusTip("The directory from where start the analysis. Use absolute path");
    m_pSourcePathLine = new QLineEdit();
    m_pSourcePathLine->setStatusTip("The directory from where start the analysis. Use absolute path");
    //Set the input validator
    /*QRegExp re1( "^[a-zA-Z0-9àéèìòù][._a-zA-Z0-9àéèìòù:\\\\]{0,255}$" );
    QRegExpValidator *validator1 = new QRegExpValidator(re1);
    m_pSourcePathLine->setValidator(validator1);*/
    m_pBrowseSourcePathButton = new QPushButton("browse");
    //auxiliary layout to display two items on the input field of the row of source path
    QHBoxLayout* suourcePathLayout = new QHBoxLayout;
    suourcePathLayout->addWidget(m_pSourcePathLine);
    suourcePathLayout->addWidget(m_pBrowseSourcePathButton);
    formLayout->addRow(sourcePathLabel, suourcePathLayout);

    //Platform name input
    QLabel* platformNameLabel = new QLabel("Platform names:");
    platformNameLabel->setStatusTip("Separate the names with semicolon. Names will be used in case insensitive mode");
    m_pPlatfformNameLine = new QLineEdit();
    m_pPlatfformNameLine->setStatusTip("Separate the names with semicolon. Names will be used in case insensitive mode");
    //Set the input validator
    QRegExp re2( "^[a-zA-Z0-9;]{0,255}$" );
    QRegExpValidator *validator2 = new QRegExpValidator(re2);
    m_pPlatfformNameLine->setValidator(validator2);
    formLayout->addRow(platformNameLabel, m_pPlatfformNameLine);

    //Warning time input
    QLabel* warningTimeLabel = new QLabel("Warning time:");
    warningTimeLabel->setStatusTip("Use dot to separate the decimal digits. Max 2 decimal digits");
    m_pWarningTimeLine = new QLineEdit();
    m_pWarningTimeLine->setStatusTip("Use dot to separate the decimal digits. Max 2 decimal digits");
    //Set input validator
    /*QRegExp re3( "^[0-9]{1,30}[.]{0,1}[0-9]{0,2}$" );
    QRegExpValidator *validator3 = new QRegExpValidator(re3);
    m_pWarningTimeLine->setValidator(validator3);*/
    m_pWarningTimeCombo = new QComboBox;
    QStringList timeUnits;
    timeUnits << "days" << "hours";
    m_pWarningTimeCombo->insertItems(0,timeUnits);
    //auxiliary layout to display two items on the input field of the row of warning time
    QHBoxLayout* warningTimeLayout = new QHBoxLayout;
    warningTimeLayout->addWidget(m_pWarningTimeLine);
    warningTimeLayout->addWidget(m_pWarningTimeCombo);
    formLayout->addRow(warningTimeLabel, warningTimeLayout);

    //Inizialize the start button
    m_pStartButton = new QPushButton("Start analysis");
    m_pStartButton->setToolTip("Start the analysis");

    //Inizialize the widget for dysplay the results
    QLabel* resultlabel = new QLabel("Results:   ");

    //Initialize the label where the error is written
    m_pErrorlabel = new QLabel("");
    //Set the red color for the error label
    QPalette palette = m_pErrorlabel->palette();
    palette.setColor(QPalette::WindowText, QColor(Qt::red));
    m_pErrorlabel->setPalette(palette);

    m_pResolutSummaryLabel = new QLabel("");

    //Use an auxiliary layou to display in the same line resultlabel and ResolutSummaryLabel
    QHBoxLayout* resultHorizontalLayout = new QHBoxLayout;
    resultHorizontalLayout->setAlignment(Qt::AlignLeft);
    resultHorizontalLayout->addWidget(resultlabel);
    resultHorizontalLayout->addWidget(m_pResolutSummaryLabel);

    //Set the content of the vertical layout
    verticalLayout->addItem(formLayout);
    verticalLayout->addWidget(m_pStartButton);
    verticalLayout->addItem(resultHorizontalLayout);
    verticalLayout->addWidget(m_pErrorlabel);
    verticalLayout->addWidget(m_pResultWidget);

    //Widget ausiliario in cui settare il layout da visualizzare
    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(verticalLayout);

    //Visualizza il widget ausiliario
    setCentralWidget(centralWidget);

    //Set a decent windows size
    this->resize(800, 600);

    //Initialize the input data from the previosly inserted values or insert default value -> The QVariant("") is what to return when the key is not found
    m_pSourcePathLine->setText(m_pSettings->value(m_keySourcePath, QVariant("")).toString());
    m_pPlatfformNameLine->setText(m_pSettings->value(m_keyPlatformName, QVariant("")).toString());
    m_pWarningTimeLine->setText(m_pSettings->value(m_keyWarningTimeLine, QVariant("")).toString());
    m_pWarningTimeCombo->setCurrentIndex(m_pSettings->value(m_keyWarningTimeCombo, QVariant(0)).toInt());
}

ViewMainWindow::~ViewMainWindow() {}

void ViewMainWindow::Initialize(Controller* controller, ResultData* resultData)
{
    m_pController = controller;
    m_pResultData = resultData;

    //initialize the result widget with the result data
    m_pResultWidget->Initialize(resultData);

    //Bind user input with the slots
    connect(m_pSourcePathLine, SIGNAL(editingFinished()), this, SLOT(ChangedSourcePath()));
    connect(m_pBrowseSourcePathButton, SIGNAL(clicked()), this, SLOT(PressedSourcePathBrowseButton()));
    connect(m_pPlatfformNameLine, SIGNAL(editingFinished()), this, SLOT(ChangedPlatformNames()));
    connect(m_pWarningTimeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeWarningTime()));
    connect(m_pWarningTimeLine, SIGNAL(editingFinished()), this, SLOT(ChangeWarningTime()));
    connect(m_pStartButton, SIGNAL(clicked()), this, SLOT(PressedStartAnalysis()));

    //Notify the controller that the input data cab be changed due to the load
    ChangedSourcePath();
    ChangedPlatformNames();
    ChangeWarningTime();

    //Register to the result data observer
    m_pResultData->AttachObserver(this); //Don't need because a subwidget show the result
}

void ViewMainWindow::ResetResultSummary(bool update)
{
    static const QString fixedMessage = " files found.";

    if(update) //If it's to update the numer
    {
        m_pResolutSummaryLabel->setText(QString::number(m_numResult) + fixedMessage);
    }
    else //If it's to remove the string
    {
        m_pResolutSummaryLabel->setText("");
    }
}

const std::string ViewMainWindow::Trim(const std::string &stringToTrim)
{
    string finalString = "";

    char space = ' ';

    //Position of the first character non space and the right substring
    size_t indexOfFistNonSpace = stringToTrim.find_first_not_of(space);
    size_t indexOfLastNonSpace = stringToTrim.find_last_not_of(space);

    //if there are only spacese return "" else take the
    if(indexOfFistNonSpace == std::string::npos || indexOfLastNonSpace == std::string::npos)
    {
        finalString = "";
    }
    else
    {
        finalString = stringToTrim.substr(indexOfFistNonSpace, indexOfLastNonSpace - indexOfFistNonSpace +1);
    }

    return finalString;
}


void ViewMainWindow::Show(){
    QMainWindow::show();
}

void ViewMainWindow::Hide(){
    QMainWindow::hide();
}

void ViewMainWindow::Update(UPDATE_CODE updateCode)
{
    //If there was an error show it
    if(updateCode == UPDATE_ERR)
    {
        QString errorMessage = "";
        switch(m_pResultData->GetErrorCode())
        {
            //ERR_CODE {ERR_PATH_NON_EXISTENT, ERR_PATH_ISNT_DIR, ERR_PATH_RELATIVE, ERR_ZERO_PLATFORMS};

            case(ERR_ZERO_PLATFORMS): errorMessage = "There aren't any platform. Insert almost one.";
                break;
            case(ERR_PATH_RELATIVE): errorMessage = "The source path isn't absolute.";
                break;
            case(ERR_PATH_ISNT_DIR): errorMessage = "The source path isn't a directory.";
                break;
            case(ERR_PATH_NON_EXISTENT): errorMessage = "The source path doesn't exist.";
                break;
        }

        //Set as displayed widget the text box and write the message
        m_pErrorlabel->setText(errorMessage);

        //Hide the result summary string
        ResetResultSummary(false);
    }
    else if(updateCode == UPDATE_ALL)
    {
        m_numResult = 0;
        //Hide the result summery string
        ResetResultSummary(false);

    }
    else if(updateCode == UPDATE_LAST_ADD)
    {
        m_numResult++;
        //Show and update the result summery string
        ResetResultSummary(true);
    }

}

void ViewMainWindow::ChangedSourcePath()
{
    //Execute trim for the input data
    m_pSourcePathLine->setText(QString::fromStdString(this->Trim(m_pSourcePathLine->text().toStdString())));

    //Notify controller of the state change
    m_pController->ChangedSourcePath(m_pSourcePathLine->text().toStdString());

    //Store the new input
    m_pSettings->setValue(m_keySourcePath, m_pSourcePathLine->text());
}

void ViewMainWindow::ChangedPlatformNames()
{
    //Execute trim for the input data
    m_pPlatfformNameLine->setText(QString::fromStdString(this->Trim(m_pPlatfformNameLine->text().toStdString())));

    //Notify controller of the state change
    m_pController->ChangedPlatformNames(m_pPlatfformNameLine->text().toStdString());

    //Store the new input
    m_pSettings->setValue(m_keyPlatformName, m_pPlatfformNameLine->text());
}

void ViewMainWindow::ChangeWarningTime()
{
    //No trim needed because of no space allowed in this input field

    float timeInserted = m_pWarningTimeLine->text().toFloat();
    if(m_pWarningTimeCombo->currentIndex() == 0) //days selected
        timeInserted *= 24*3600; //convert from days to seconds unit
    else //hours selected
        timeInserted *= 3600; //convert from hours to seconds unit

    //Notify controller of the state change
    //Converted to int because it's needn't to have float precision
    m_pController->ChangeWarningTime(static_cast<int>(timeInserted));

    //Store the new input
    m_pSettings->setValue(m_keyWarningTimeLine, m_pWarningTimeLine->text());
    m_pSettings->setValue(m_keyWarningTimeCombo, m_pWarningTimeCombo->currentIndex());

    //Advert the result widget to display the result in the right way
    m_pResultWidget->SetHoursUnitDisplay(m_pWarningTimeCombo->currentIndex()); //because index 0 = days and 0 is false
}

void ViewMainWindow::PressedStartAnalysis()
{
    //Disable the start button in order to avoid multiple seach.
    m_pStartButton->setEnabled(false);

    //Display to the status bar that the analysis is started
    statusBar()->showMessage("Analysis started");

    m_pController->StartAnalysis();

    //Display to the status bar that the analysis is finished
    statusBar()->showMessage("Analysis completed");

    //Re-enable the start button
    m_pStartButton->setEnabled(true);
}

void ViewMainWindow::PressedSourcePathBrowseButton()
{
    QFileDialog dialog(this);
    //Set to accept only directories
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(m_pSourcePathLine->text());

    if(dialog.exec())
    {
        //Take the first directory selected, the others will be ignored
        QString chosenPath = dialog.selectedFiles().at(0);

        //If the selected path is != "" select in the input field
        if(chosenPath != "")
        {
            m_pSourcePathLine->setText(chosenPath);

            //Notify the potential changed of the input data to the controller
            ChangedSourcePath();
        }
    }
}
