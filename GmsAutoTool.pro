#-------------------------------------------------
#
# Project created by QtCreator 2017-10-12T21:48:07
#
#-------------------------------------------------

QT     += core gui sql xml network

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
    plandialog.cpp \
    solutionwidget.cpp \
    addsolutionwidget.cpp \
    loadresultthread.cpp \
    extendwidget.cpp \
    xmlutil.cpp \
    logutil.cpp \
    socketutil.cpp \
    onlinewidget.cpp \
    tabstyle.cpp \
    screenwidget.cpp \
    warningwidget.cpp \
    settingswidget.cpp \
    messagedialog.cpp \
    config.cpp \
    planutil.cpp \
    waitingwidget.cpp \
    failturewidget.cpp

HEADERS += \
    mainwindow.h \
    addtoolwidget.h \
    sqlconnection.h \
    addtestwidget.h \
    resultwidget.h \
    toolwidget.h \
    testwidget.h \
    plandialog.h \
    solutionwidget.h \
    addsolutionwidget.h \
    loadresultthread.h \
    extendwidget.h \
    xmlutil.h \
    logutil.h \
    socketutil.h \
    onlinewidget.h \
    tabstyle.h \
    screenwidget.h \
    warningwidget.h \
    settingswidget.h \
    messagedialog.h \
    updatable.h \
    config.h \
    planutil.h \
    waitingwidget.h \
    failurewidget.h

FORMS += \
    mainwindow.ui \
    addtoolwidget.ui \
    addtestwidget.ui \
    resultwidget.ui \
    toolwidget.ui \
    testwidget.ui \
    plandialog.ui \
    solutionwidget.ui \
    addsolutionwidget.ui \
    extendwidget.ui \
    onlinewidget.ui \
    screenwidget.ui \
    warningwidget.ui \
    settingswidget.ui \
    messagedialog.ui \
    waitingwidget.ui \
    failurewidget.ui

RESOURCES += \
    icon.qrc
