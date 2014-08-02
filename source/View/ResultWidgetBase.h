/**
 * File: ResultWidgetBase.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/10
*/

#ifndef IRESULTWIDGET_H
#define IRESULTWIDGET_H

#include "../Observer/iobserver.h"

#include <QWidget>

class ResultData;

/**
 * @brief The ResultWidgetBase class. It's an abstract base for a QWidget that display the results of the analysis
 */
class ResultWidgetBase : public QWidget, public IObserver
{
protected:
    /**
     * @brief m_pResultData: result data to display
     */
    ResultData* m_pResultData;

public:
    ResultWidgetBase(QWidget* parent = 0);

    //Function update will be definited by the subclass

    /**
     * @brief Initialize the object with the result data
     * @param resultData: object in which the results will be stored
     */
    void Initialize(ResultData* resultData);

    /**
     * @brief SetHoursUnitDisplay: set if display the result in hours(true) or in days(false)
     */
    virtual void SetHoursUnitDisplay(bool hoursUnitDisplay) = 0;
};

#endif // IRESULTWIDGET_H
