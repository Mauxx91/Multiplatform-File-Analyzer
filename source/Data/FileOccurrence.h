/**
 * File: FileOccurrence.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/09
*/

#ifndef FILEPRESENCE_H
#define FILEPRESENCE_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief The FileOccurrence class represent a occurence of a file in a platform or in the parent directory
 */
class FileOccurrence
{
private:
    /**
     * @brief m_relativeFileName: only the file name without the path
     */
    string m_relativeFileName;

    /**
     * @brief m_absoluteFilePath of the relativeFileName, excluded the platform name
     */
    string m_absoluteFilePath;

    /**
     * @brief platforms: list of the platform in which it was found
     */
    vector<string> m_foundPlatforms;

    /**
     * @brief m_foundPlatformsDate: list of date of the last change orderd in the same order of the m_foundPlatforms
     */
    vector<string> m_foundPlatformsDate;

public:
    /**
     * @brief FilePresence: constructor of the class.
     * @param absoluteFileName: the absolute path of the relativeFileName, excluded the platform name
     * @param relativeFileName: only the file name without the path.
     */
    FileOccurrence(const string& absoluteFilePath, const string& relativeFileName);

    /**
     * @brief AddPresence: add a presence of the file for a platform
     * @param platformName: the platform in which it was found
     * @param data: the data of the last modification
     */
    void AddPresence(const string& platformName, const string& date);

    //Getter
    const string& GetRelativeFileName() const;
    const string& GetAbsolutePath() const;
    const vector<string>& GetFoundPlatform() const;
    const vector<string>& GetFoundPlatformDates() const;
};

#endif // FILEPRESENCE_H
