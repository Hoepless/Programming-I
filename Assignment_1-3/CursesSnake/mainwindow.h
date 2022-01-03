#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <eleminterface.h>
#include <ctime>


class mainwindow : public __BaseELEMENT
{
public:

    mainwindow();

    void Exec();

    void KeyEnter(int Key) override;

    bool Escape=false;
};

#endif // MAINWINDOW_H

