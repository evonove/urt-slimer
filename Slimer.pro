QT += core
QT -= gui
QT += serialport
QT += network

CONFIG += c++11

TARGET = Slimer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    serialreader.cpp \
    parser.cpp

HEADERS += \
    payload_state.h \
    serialreader.h \
    parser.h
