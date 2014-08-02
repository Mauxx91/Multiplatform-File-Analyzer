/**
 * File: main.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#include "controller/controller.h"
#include <QApplication>

#include "Controller/QtCoreMultiplatformFileAnalyzer.h"
#include "View/ResultTableWidget.h"
#include "view/ViewMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Initilize the chosen view and Core
    ResultTableWidget* resultWidget = new ResultTableWidget();
    ViewMainWindow* view = new ViewMainWindow(resultWidget);
    QtCoreMultiplatformFileAnalyzer* core = new QtCoreMultiplatformFileAnalyzer();

    Controller controller(view, core);
    controller.Start();

    return a.exec();
}
