/**
 * File: Controller.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "icoremultiplatformfileanalyzer.h"
#include "../View/iview.h"
#include "../Data/inputdata.h"
#include "../Data/resultdata.h"

/**
 * @brief The Controller class. It's show the view and inizialize the application component.
 *        It calls the core to perform the user requested task.
 */
class Controller
{
private:
    //The selected view
    IView* m_pView;

    //The data
    InputData* m_pInputData;
    ResultData* m_pResultData;

    //The core that real execute the operation
    ICoreMultiplatformFileAnalyzer* m_pCore;

public:
    /**
     * @brief Controller constructor that inizialize all the component needed
     */
    Controller(IView* view, ICoreMultiplatformFileAnalyzer* core);

    /**
     * @brief Controller distructor that deallocate all the component allocated in the constructor
     */
    ~Controller();

    /**
     * @brief start: after the initialization start the program showing the selected view
     */
    void Start();

    //Function invoked by the view when the input data is changed
    void ChangedSourcePath(const string& sourcePath);
    void ChangedPlatformNames(const string& platformNames);
    void ChangeWarningTime(int timeWarningS);

    /**
     * @brief startAnalysis and insert the result in result data object
     */
    void StartAnalysis();

};

#endif // CONTROLLER_H
