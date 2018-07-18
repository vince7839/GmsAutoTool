#-------------------------------------------------
#
# Project created by QtCreator 2017-10-12T21:48:07
#
#-------------------------------------------------

QT     +=    core gui sql xml    network

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
    util/config.cpp \
    util/loadresultthread.cpp \
    util/logutil.cpp \
    util/networkutil.cpp \
    util/planutil.cpp \
    util/socketutil.cpp \
    util/sqlconnection.cpp \
    util/taskparam.cpp \
    util/xmlutil.cpp \
    main.cpp \
    view/addsolutionwidget.cpp \
    view/addtoolwidget.cpp \
    view/extendwidget.cpp \
    view/failturewidget.cpp \
    view/mainwindow.cpp \
    view/messagedialog.cpp \
    view/onlinewidget.cpp \
    view/plandialog.cpp \
    view/projecteditwidget.cpp \
    view/projectwidget.cpp \
    view/resultwidget.cpp \
    view/screenwidget.cpp \
    view/settingswidget.cpp \
    view/solutionwidget.cpp \
    view/tabstyle.cpp \
    view/taskdelegate.cpp \
    view/toolwidget.cpp \
    view/waitingwidget.cpp \
    view/warningwidget.cpp \
    view/buildtaskwidget.cpp \
    view/taskwidget.cpp \
    util/executor.cpp \
    util/cmdbuilder.cpp \
    view/devicedialog.cpp \
    util/devicelistener.cpp \
    util/tasklistener.cpp \
    view/downloadwidget.cpp


HEADERS += \
    util/config.h \
    util/loadresultthread.h \
    util/logutil.h \
    util/networkutil.h \
    util/planutil.h \
    util/socketutil.h \
    util/sqlconnection.h \
    util/taskparam.h \
    util/xmlutil.h \
    view/addsolutionwidget.h \
    view/addtoolwidget.h \
    view/extendwidget.h \
    view/failurewidget.h \
    view/mainwindow.h \
    view/messagedialog.h \
    view/onlinewidget.h \
    view/plandialog.h \
    view/projecteditwidget.h \
    view/projectwidget.h \
    view/resultwidget.h \
    view/screenwidget.h \
    view/settingswidget.h \
    view/solutionwidget.h \
    view/tabstyle.h \
    view/taskdelegate.h \
    view/toolwidget.h \
    view/updatable.h \
    view/waitingwidget.h \
    view/warningwidget.h \
    view/buildtaskwidget.h \
    view/taskwidget.h \
    util/executor.h \
    util/cmdbuilder.h \
    view/devicedialog.h \
    util/devicelistener.h \
    util/tasklistener.h \
    view/downloadwidget.h


FORMS += \
    ui/addsolutionwidget.ui \
    ui/addtoolwidget.ui \
    ui/extendwidget.ui \
    ui/failurewidget.ui \
    ui/mainwindow.ui \
    ui/messagedialog.ui \
    ui/onlinewidget.ui \
    ui/plandialog.ui \
    ui/projecteditwidget.ui \
    ui/projectwidget.ui \
    ui/resultwidget.ui \
    ui/screenwidget.ui \
    ui/settingswidget.ui \
    ui/solutionwidget.ui \
    ui/toolwidget.ui \
    ui/waitingwidget.ui \
    ui/warningwidget.ui \
    ui/buildtaskwidget.ui \
    ui/taskwidget.ui \
    ui/devicedialog.ui \
    view/downloadwidget.ui


RESOURCES += \
    icon.qrc

DISTFILES += \
    database/GmsAutoTool.db
