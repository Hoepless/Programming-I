TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        OnePlayerPentris.cpp \
        Pentris.cpp \
        TwoPlayersPentris.cpp \
        main.cpp

HEADERS += \
    IPentrisPlayer.h \
    OnePlayerPentris.h \
    Pentris.h \
    TwoPlayersPentris.h

LIBS += /usr/lib/x86_64-linux-gnu/libncursesw.so
