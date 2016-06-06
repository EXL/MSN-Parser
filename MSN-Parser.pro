#-------------------------------------------------
#
# Project created by QtCreator 2016-03-27T00:23:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MSN-Parser
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

win32:RC_FILE = MSN-Parser.rc

RESOURCES += MSN-Parser.qrc

OTHER_FILES += README.md
