TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        eleminterfase.cpp \
        gamewidget.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
    eleminterfase.h \
    gamewidget.h \
    mainwindow.h \
    size_t.h

LIBS += /usr/lib/x86_64-linux-gnu/libncursesw.so
#LIBS += C:\MinGW64\x86_64-w64-mingw32\include\Curses36\wincon\pdcurses.a
