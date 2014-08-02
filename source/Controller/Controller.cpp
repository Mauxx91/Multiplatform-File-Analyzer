/**
 * File: Controller.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#include "Controller.h"

Controller::Controller(IView *view, ICoreMultiplatformFileAnalyzer *core)
{
    //Instantiate the data
    m_pInputData = new InputData;
    m_pResultData = new ResultData;

    //Set the chosen core
    m_pCore = core;

    //Set the chosen view and call its initialize
    m_pView = view;
    m_pView->Initialize(this, m_pResultData);
}

Controller::~Controller()
{
    //Delete all
    delete m_pInputData;
    delete m_pResultData;
    delete m_pCore;

    //delete m_view; is needn't because qt has a parent destructor that destroy all qt stuff
}

void Controller::Start()
{
    m_pView->Show();
}

void Controller::ChangedSourcePath(const string& sourcePath)
{
    m_pInputData->SetSourcePath(sourcePath);
}

void Controller::ChangedPlatformNames(const string& platformNames)
{
    m_pInputData->SetPlatformNames(platformNames);
}

void Controller::ChangeWarningTime(int warningTimeS)
{
    m_pInputData->SetWarningTimeS(warningTimeS);
}

void Controller::StartAnalysis()
{
    //Be sure that the result data is free to have the new results
    m_pResultData->Clear();

    //Real execute the opertation
    m_pCore->Execute(m_pResultData, m_pInputData);
}
