/**
 * File: ResultWidgetBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/10
*/

#include "ResultWidgetBase.h"

#include "../Data/resultdata.h"

ResultWidgetBase::ResultWidgetBase(QWidget* parent): QWidget(parent) {}

void ResultWidgetBase::Initialize(ResultData* resultData)
{
    //Attach to the observable
    resultData->AttachObserver(this);

    //Store the pointer for in the subclass show the result
    m_pResultData = resultData;
}
