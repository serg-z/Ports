#-------------------------------------------------
#
# Project created by QtCreator 2012-09-14T20:44:06
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++0x

DEFINES += UNLINK_SHARED_MEMORY

TARGET = Ports
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    InpOut.cpp \
    pinwidget.cpp \
    portwidget.cpp \
    portlptwidget.cpp

HEADERS  += mainwindow.h \
    InpOut.h \
    pinwidget.h \
    portwidget.h \
    portlptwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    Ports.qrc

unix {
    CONFIG(debug, debug|release) {
        DESTDIR = debug
        DEFINES += DEBUG
    } else {
        DESTDIR = release
    }

    DESTDIR = $${DESTDIR}/linux

    OBJECTS_DIR = $${DESTDIR}
}

win32 {
    DESTDIR = $${DESTDIR}/win32

    RC_FILE = Ports.rc
}
