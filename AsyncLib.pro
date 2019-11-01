TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    async.cpp \
    fs.cpp

LIBS += -pthread

HEADERS += \
    async.h \
    fs.h
