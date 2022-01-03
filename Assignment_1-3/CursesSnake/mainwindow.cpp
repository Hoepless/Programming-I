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

        int start=std::clock();
        int temp;
        while(clock()-start<=200){

            halfdelay(2-(clock()-start)/100); //700 ms
            temp=getch();
            if (temp!=ERR && this->Exceptors.count(temp)){
                this->Exceptors[temp]->KeyEnter(temp);
            }
        }
        DoCommand("SnakeWidget", "NextStep", "");
    }
}
