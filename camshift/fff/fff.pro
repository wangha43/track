#-------------------------------------------------
#
# Project created by QtCreator 2016-06-06T20:55:07
#
#-------------------------------------------------

QT       += core gui
TARGET = fff
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += link_pkgconfig

PKGCONFIG += libavformat libavcodec libavfilter libavutil
PKGCONFIG += opencv

TEMPLATE = app


SOURCES += main.cpp detect.cpp camshift.cpp recognize.cpp \
    tldtracker.cpp

HEADERS +=detect.hpp camshift.hpp recognize.hpp \
    tldtracker.hpp

