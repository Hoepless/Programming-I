#include "mainwindow.h"

mainwindow::mainwindow():__BaseELEMENT(nullptr, "")
{
    this->ExceptableKeys.push_back(27); //ESC
    Name="MW";
    UpNeedKeyboard();
}

void mainwindow::KeyEnter(int Key){

    DoCommand("InfoBox_Score2", "ChangeText", uint_to_str(Key));
    if (Key==27) this->Escape=true;
}

void mainwindow::Exec(){
    while (!Escape) {

        int temp;

            temp=getch();
            if (temp!=ERR && this->Exceptors.count(temp)){
                this->Exceptors[temp]->KeyEnter(temp);
            }
        }
    }
