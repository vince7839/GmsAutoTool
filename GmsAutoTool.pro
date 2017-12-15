#-------------------------------------------------
#
# Project created by QtCreator 2017-10-12T21:48:07
#
#-------------------------------------------------

QT       += core gui sql xml network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GmsAutoTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    mainwindow.cpp \
    addtoolwidget.cpp \
    sqlconnection.cpp \
    addtestwidget.cpp \
    resultwidget.cpp \
    toolwidget.cpp \
    testwidget.cpp \
    parseresultwidget.cpp \
    plandialog.cpp \
    solutionwidget.cpp \
    addsolutionwidget.cpp \
    loadresultthread.cpp \
    extendwidget.cpp \
    configquery.cpp \
    xmlutil.cpp \
    logutil.cpp

HEADERS += \
    mainwindow.h \
    addtoolwidget.h \
    sqlconnection.h \
    addtestwidget.h \
    resultwidget.h \
    toolwidget.h \
    testwidget.h \
    parseresultwidget.h \
    plandialog.h \
    solutionwidget.h \
    addsolutionwidget.h \
    loadresultthread.h \
    extendwidget.h \
    configquery.h \
    xmlutil.h \
    logutil.h

FORMS += \
    mainwindow.ui \
    addtoolwidget.ui \
    addtestwidget.ui \
    resultwidget.ui \
    toolwidget.ui \
    testwidget.ui \
    parseresultwidget.ui \
    plandialog.ui \
    solutionwidget.ui \
    addsolutionwidget.ui \
    extendwidget.ui
