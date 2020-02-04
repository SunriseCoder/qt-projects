QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    entities/fileentity.cpp \
    entities/question.cpp \
    entities/taskentity.cpp \
    main.cpp \
    models/taskmodel.cpp \
    ui/mainwindow.cpp \
    utils/alltasksexecutor.cpp \
    utils/convert.cpp \
    utils/filecopier.cpp \
    utils/jsonhelper.cpp \
    utils/taskexecutor.cpp \
    widgets/customaction.cpp \
    widgets/custommenu.cpp \
    widgets/customtablewidget.cpp

HEADERS += \
    entities/fileentity.h \
    entities/question.h \
    entities/taskentity.h \
    models/taskmodel.h \
    ui/mainwindow.h \
    utils/alltasksexecutor.h \
    utils/convert.h \
    utils/filecopier.h \
    utils/jsonhelper.h \
    utils/taskexecutor.h \
    widgets/customaction.h \
    widgets/custommenu.h \
    widgets/customtablewidget.h

FORMS += \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
