/**
 * File: InputData.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief The InputData class manages the input data for the analysis
 */
class InputData
{
private:
    //Input data
    string m_sourcepath;
    string m_platformNames;
    int m_warningTimeS;

    /**
     * @brief m_separatorCharacter for divide the platform names string: m_platformNames
     */
    char m_separatorCharacter;

public:
    /**
     * @brief InputData: initialize the input data to the defalut value
     */
    InputData();

    /**
     * @brief InputData: contructor
     * @param sourcepath: from where start the analysis
     * @param platformNames: platfrom names to consider through the analysis
     * @param timeWarningS: warning time
     */
    InputData(const string& sourcepath, const string& platformNames, int timeWarningS);

    //Setter for the member attribute
    void SetSourcePath(const string& sourcePath);
    void SetPlatformNames(const string& platformNames);
    void SetWarningTimeS(int warningTimeS);

    //Getter for the menber attributes
    const string& GetSourcePath() const;
    const vector<string> GetPlatformNamesVector() const;
    int GetWarningTimeS() const;
};

#endif // INPUTDATA_H
