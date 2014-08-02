/**
 * File: ResultData.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#ifndef RESULTDATA_H
#define RESULTDATA_H

#include "../Observer/Subject.h"

#include "FileOccurrence.h"

#include <string>
#include <deque>

using namespace std;

enum ERR_CODE {ERR_NO_ERR, ERR_PATH_NON_EXISTENT, ERR_PATH_ISNT_DIR, ERR_PATH_RELATIVE, ERR_ZERO_PLATFORMS};

/**
 * @brief The ResultData class manages the result datas of the analysis
 */
class ResultData : public Subject
{
private:
    /**
     * @brief m_sourcePath: from which the research is started
     */
    string m_sourcePath;

    /**
     * @brief m_foundPresence: list of the presence found
     */
    deque<FileOccurrence> m_foundPresence;

    /**
     * @brief m_allPlatformVector: contains all the platforms used for the search
     */
    vector<string> m_allPlatformVector;

    /**
     * @brief m_warningTimeS: warning time in seconds
     */
    int m_warningTimeS;

    /**
     * @brief m_errorString: error code
     */
    ERR_CODE m_errorCode;

public:
    /**
     * @brief ResultData constructor that set default values
     */
    ResultData();

    /**
     * @brief clear: set the state of the object as jest created. Now it's ready for a new search result
     */
    void Clear();

    //Setter for the member atributes
    void SetSourcePath(const string& sourcePath);
    void SetPlatforms(const vector<string>& platformVector);
    void SetWarningTimeS(int warningTime);
    void SetHoursUnitDisplay(bool hoursUnitDisplay);
    void SetErrorCode(ERR_CODE errorCode);

    //Add a file presence
    void AddFilePresence(const FileOccurrence& filePresence);

    //Getter for the member attributes
    const string& GetSourcepath() const;
    const vector<string>& GetAllPlatformVector() const;
    int GetWarningTimeS() const;
    ERR_CODE GetErrorCode() const;

    //Getter for the file presence
    const FileOccurrence& GetLastFilePresence() const;

};

#endif // RESULTDATA_H
