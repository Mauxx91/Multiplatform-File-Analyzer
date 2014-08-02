/**
 * File: ResultTableWidget.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/10
*/

#ifndef RESULTTABLEWIDGET_H
#define RESULTTABLEWIDGET_H

#include "ResultWidgetBase.h"

#include <vector>
#include <string>

class QTableWidget;
class QVBoxLayout;

using namespace std;

/**
 * @brief The ResultTableWidget class that implement the ResultWidgetBase displaying the results in a table
 */
class ResultTableWidget : public ResultWidgetBase
{
    Q_OBJECT

private:

    /**
     * @brief m_table
     */
    QTableWidget* m_pTable;

    //To display a widget in this widget
    QVBoxLayout* m_pLayout;

    /**
     * @brief fixedtableHeaders: contains all the table headers that is fixed
     */
    QStringList m_fixedtableHeaders;

    /**
     * @brief m_lastUpdateWasALastAdd: it's to understand when lauch the table re-initialize. In oder to don't do more than necessary.
     */
    bool m_lastUpdateWasALastAdd;

    /**
     * @brief m_allPlatformVector: all the platform for the current search. Is reinitialize every table reinitialize
     */
    //vector<string> m_allPlatformVector;

    /**
     * @brief hoursUnitDisplay: if true the time has to be displayed in hours unit otherwise in days unit
     */
    bool m_hoursUnitDisplay;

public:
    /**
     * @brief ResultTableWidget constructor that initialize the superclass with a pointer to an observable
     * @param resultData
     */
    ResultTableWidget(QWidget* parent = 0);

    /**
     * @brief ResultTableWidget destructor that destry the layout, because only one at time is setted
     */
    ~ResultTableWidget();

    /**
     * @brief Update: Inplementation of the observer interface method to update the view after a data update
     * @param updateCode: what kind of changes has happened
     */
    virtual void Update(UPDATE_CODE updateCode);

    /**
     * @brief SetHoursUnitDisplay: set if display the result in hours(true) or in days(false)
     */
    void SetHoursUnitDisplay(bool hoursUnitDisplay);

private:
    /**
     * @brief ReinizializeTable the table layout and delete all the rows
     */
    void ReinizializeTable();

    /**
     * @brief AddLastPresenceToANewRow: called after a UPDATE_LAST_ADD it add a new row with the new presence found
     */
    void AddLastPresenceToANewRow();

    qint64 GiveDifferenceInSeconds(const QString& dateBig, const QString& dateSmall);

private slots:
    /**
     * @brief OpenPathInExplorer: open the windows explorer for the path
     */
    void OpenPathInExplorer(int indexRow, int) const;

    /**
     * @brief SortByColumn: sort the table rows by the column clicked
     * @param indexColumn: the column requested
     * @param sortOrder: the sort order requested
     */
    void SortByColumn(int indexColumn, Qt::SortOrder sortOrder);

    /**
     * @brief ShowContextualMenuTable: show the contextual menu of the table
     * @param point: where the user has clicked
     */
    void ShowContextualMenuTable(QPoint point);
};

#endif // RESULTTABLEWIDGET_H
