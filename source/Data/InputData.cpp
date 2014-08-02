/**
 * File: InputData.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#include "InputData.h"

InputData::InputData()
{
   m_warningTimeS = 0;
   m_separatorCharacter = ';';
}

InputData::InputData(const string &sourcepath, const string &platformNames, int timeWarningS)
{
    m_sourcepath = sourcepath;
    m_platformNames = platformNames;
    m_warningTimeS = timeWarningS;
}

void InputData::SetSourcePath(const string& sourcePath)
{
    m_sourcepath = sourcePath;
}

void InputData::SetPlatformNames(const string& platformNames)
{
    m_platformNames = platformNames;
}

void InputData::SetWarningTimeS(int warningTimeS)
{
    m_warningTimeS = warningTimeS;
}

const string& InputData::GetSourcePath() const
{
    return m_sourcepath;
}

const vector<string> InputData::GetPlatformNamesVector() const
{
    std::vector<string> platformsVector;
    const string& platforms = m_platformNames;

    size_t indexOfStart = 0;
    size_t indexOfSeparator = platforms.find_first_of(m_separatorCharacter);

    //while is find a simicolon
    while(indexOfSeparator != std::string::npos)
    {
        platformsVector.push_back(platforms.substr(indexOfStart, indexOfSeparator-indexOfStart));

        indexOfStart = indexOfSeparator +1;
        indexOfSeparator = platforms.find_first_of(m_separatorCharacter, indexOfStart);
    }

    //if the last platform doesn't terminate with a delimiter character, control only if has size > 0
    if(platforms.size() > 0 && platforms.at( platforms.length()-1) != m_separatorCharacter) //last character of the string
    {
        size_t indexOfLastSeparator = platforms.find_last_of(m_separatorCharacter);

        if(indexOfLastSeparator != std::string::npos)
        {
            //indexOfSeparator is the last semicolon in the string so the last pltform starts after this position
            platformsVector.push_back(platforms.substr(indexOfLastSeparator+1, platforms.length()));
        }
        else
        {
            //If there are no separator we have to add all the string
            platformsVector.push_back(platforms);
        }
    }

    return platformsVector;
}

int InputData::GetWarningTimeS() const
{
    return m_warningTimeS;
}
