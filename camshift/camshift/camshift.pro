QT += core
QT -= gui

TARGET = camshift
CONFIG += console
CONFIG -= app_bundle
CONFIG += link_pkgconfig

PKGCONFIG += opencv

TEMPLATE = app

SOURCES += main.cpp \
    camshift.cpp \
    detect.cpp \
    recognize.cpp

HEADERS += \
    recognize.hpp \
    detect.hpp \
    camshift.hpp

