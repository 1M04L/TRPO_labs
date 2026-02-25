QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle


SOURCES += \
        main.cpp \
        filewatcher.cpp \
        filemanager.cpp

HEADERS += \
        filewatcher.h \
        filemanager.h \
        logger.h
