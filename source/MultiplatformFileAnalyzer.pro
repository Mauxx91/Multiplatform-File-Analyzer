#-------------------------------------------------
#
# Project created by QtCreator 2014-07-08T11:15:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiplatformFileAnalyzer
TEMPLATE = app


SOURCES += main.cpp\
    Controller/QtCoreMultiplatformFileAnalyzer.cpp \
    Observer/Subject.cpp \
    Controller/Controller.cpp \
    Controller/ICoreMultiplatformFileAnalyzer.cpp \
    Data/FileOccurrence.cpp \
    Data/InputData.cpp \
    Data/ResultData.cpp \
    Observer/IObserver.cpp \
    View/ResultWidgetBase.cpp \
    View/IView.cpp \
    View/ResultTableWidget.cpp \
    View/ViewMainWindow.cpp

HEADERS  += \
    version.h \
    Controller/QtCoreMultiplatformFileAnalyzer.h \
    Observer/Subject.h \
    Observer/IObserver.h \
    Data/ResultData.h \
    Data/InputData.h \
    Data/FileOccurrence.h \
    Controller/ICoreMultiplatformFileAnalyzer.h \
    Controller/Controller.h \
    View/ResultWidgetBase.h \
    View/IView.h \
    View/ResultTableWidget.h \
    View/ViewMainWindow.h

RC_FILE = resources.rc
