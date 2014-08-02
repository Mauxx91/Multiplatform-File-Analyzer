/**
 * File: IView.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#ifndef IVIEW_H
#define IVIEW_H

#include "../Observer/iobserver.h"

class Controller;
class ResultData;

/**
 * @brief The IView interface: It represent a view in which display the user interface and take input
 */
class IView : public IObserver
{

public:
    /**
     * @brief ~IView virtual destructor
     */
    virtual ~IView();

    /**
     * @brief Initialize: set the status of a View
     * @param controller: for notify the user input
     * @param resultData: the object that stores the results of an analysis
     */
    virtual void Initialize(Controller* controller, ResultData* resultData) = 0;

    /**
     * @brief show: show the view
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the view
     */
    virtual void Hide() = 0;

};

#endif // IVIEW_H
