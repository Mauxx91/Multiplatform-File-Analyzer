/**
 * File: FileOccurrence.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/09
*/

#include "FileOccurrence.h"

FileOccurrence::FileOccurrence(const string &absoluteFilePath, const string &relativeFileName)
{
    m_absoluteFilePath = absoluteFilePath;
    m_relativeFileName = relativeFileName;
}

void FileOccurrence::AddPresence(const string &platformName, const string &date)
{
    m_foundPlatforms.push_back(platformName);
    m_foundPlatformsDate.push_back(date);
}

const string& FileOccurrence::GetRelativeFileName() const
{
    return m_relativeFileName;
}

const string& FileOccurrence::GetAbsolutePath() const
{
    return m_absoluteFilePath;
}

const vector<string>& FileOccurrence::GetFoundPlatform() const
{
    return m_foundPlatforms;
}

const vector<string>& FileOccurrence::GetFoundPlatformDates() const
{
    return m_foundPlatformsDate;
}
