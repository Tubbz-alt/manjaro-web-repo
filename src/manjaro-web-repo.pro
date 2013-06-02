#-------------------------------------------------
#
# Project created by QtCreator 2013-05-26T15:43:49
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CXXFLAGS += -std=c++11
TARGET = manjaro-web-repo
LIBS += -L/usr/lib -lcurl -lboost_thread -lboost_system -lboost_signals -lwt -lwthttp
CONFIG   += console
CONFIG   -= app_bundl
CONFIG   += no_keywords

TEMPLATE = app


SOURCES += main.cpp \
    mainwidget.cpp \
    repositorystatepage.cpp \
    global.cpp \
    backgroundworker.cpp

HEADERS += \
    mainwidget.h \
    repositorystatepage.h \
    global.h \
    backgroundworker.h \
    const.h

target.path = /usr/bin
