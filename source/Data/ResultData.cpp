/**
 * File: ResultData.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#include "ResultData.h"

#include "../Observer/IObserver.h"

ResultData::ResultData()
{
    Clear();
}

void ResultData::Clear()
{
    m_sourcePath = "";
    m_foundPresence.clear();
    m_allPlatformVector.clear();
    m_errorCode = ERR_NO_ERR;

    //Notify the changes to all the registered observer
    Notify(UPDATE_ALL);
}

void ResultData::SetSourcePath(const string& sourcePath)
{
    m_sourcePath = sourcePath;

    //Notify the changes to all the registered observer
    Notify(UPDATE_ALL);
}

void ResultData::SetPlatforms(const vector<string>& platformVector)
{
    m_allPlatformVector = platformVector;

    //Notify the changes to all the registered observer
    Notify(UPDATE_ALL);
}

void ResultData::SetWarningTimeS(int warningTime)
{
    m_warningTimeS = warningTime;
}

void ResultData::SetErrorCode(ERR_CODE errorCode)
{
    m_errorCode = errorCode;

    //Notify the changes to all the registered observer
    Notify(UPDATE_ERR);
}

void ResultData::AddFilePresence(const FileOccurrence& filePresence)
{
    m_foundPresence.push_back(filePresence);

    //Notify the changes to the observers
    Notify(UPDATE_LAST_ADD);
}

const string& ResultData::GetSourcepath() const
{
    return m_sourcePath;
}

const vector<string>& ResultData::GetAllPlatformVector() const
{
    return m_allPlatformVector;
}

int ResultData::GetWarningTimeS() const
{
    return m_warningTimeS;
}

ERR_CODE ResultData::GetErrorCode() const
{
    return m_errorCode;
}

const FileOccurrence& ResultData::GetLastFilePresence() const
{
    return m_foundPresence.back();
}
