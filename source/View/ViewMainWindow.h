/**
 * File: ViewMainWindow.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#ifndef VIEW_H
#define VIEW_H

#include "iview.h"

#include <QMainWindow>

#include <string>

class QSettings;
class QLineEdit;
class QPushButton;
class QComboBox;
class QLabel;

class ResultWidgetBase;

class Controller;

/**
 * @brief The ViewMainWindow class it's a view based on a visual user interface
 */
class ViewMainWindow : public QMainWindow, public IView
{
    Q_OBJECT

private:
    //Qt object used to store input data between session.
    QSettings* m_pSettings;
    //Keys to access to the stored data
    QString m_keySourcePath;
    QString m_keyPlatformName;
    QString m_keyWarningTimeLine;
    QString m_keyWarningTimeCombo;

    //Input fields
    QLineEdit* m_pSourcePathLine;
    QLineEdit* m_pPlatfformNameLine;
    QPushButton* m_pBrowseSourcePathButton;
    QLineEdit* m_pWarningTimeLine;
    QComboBox* m_pWarningTimeCombo;
    QPushButton* m_pStartButton;

    /**
     * @brief m_errorlabel: where show the occurred errors
     */
    QLabel* m_pErrorlabel;

    /**
     * @brief m_pResolutSummary: where show the summery of the analysis
     */
    QLabel* m_pResolutSummaryLabel;

    /**
     * @brief resultWidget in which the results will be displayed
     */
    ResultWidgetBase* m_pResultWidget;

    /**
     * @brief controller pointer to notify input changes
     */
    Controller* m_pController;

    /**
     * @brief m_pResultData: where it's stored the results of an analysis
     */
    ResultData* m_pResultData;

    /**
     * @brief m_numResult: number of results found
     */
    int m_numResult;

    /**
     * @brief resetResultSummary: reset the summary string
     * @param update: if it's true update the numer of results, otherwise the string will be removed
     */
    void ResetResultSummary(bool update);

    /**
     * @brief trim removes from the string in input all the initial and final spaces
     * @param stringTotrim: string to elaborate
     * @return the string without all the initial and final spaces
     */
    const std::string Trim(const std::string& stringToTrim);

public:
    /**
     * @brief View constructor
     * @param parent: set if this view depends by another one
     */
    ViewMainWindow(ResultWidgetBase* resultWidget);

    /**
     * @brief View destructor: with the parental destructor there's no problem of destruct the inner widged used
     */
    ~ViewMainWindow();

    /**
     * @brief Initialize: set the status of a View
     * @param controller: for notify the user input
     * @param resultData: the object that stores the results of an analysis
     */
    virtual void Initialize(Controller* controller, ResultData* resultData);

    /**
     * @brief show the view using the QMainWindow method
     */
    virtual void Show();

    /**
     * @brief hide the view using the QMainWindow method
     */
    virtual void Hide();

    /**
     * @brief Update the view using the result object
     */
    virtual void Update(UPDATE_CODE updateCode);

private slots:

    //Slot that notify the controller the changes of the input data.
    void ChangedSourcePath();
    void ChangedPlatformNames();
    void ChangeWarningTime();

    //Slot for make the analysis started
    void PressedStartAnalysis();

    //Stot for browse the file system to chose the source directory
    void PressedSourcePathBrowseButton();
};

#endif // VIEW_H
