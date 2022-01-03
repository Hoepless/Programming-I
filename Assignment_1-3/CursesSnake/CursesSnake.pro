TEMPLATE = app
CONFIG += console c++11
CONFIG += curses
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += \
        elem_snakegame.cpp \
        eleminterface.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
    Size_T.h \
    elem_snakegame.h \
    eleminterface.h \
    mainwindow.h

#LIBS += C:\MinGW64\x86_64-w64-mingw32\include\Curses36\wincon\pdcurses.a
LIBS += /usr/lib/x86_64-linux-gnu/libncursesw.so
