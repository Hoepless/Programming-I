#ifndef ELEMINTERFACE_H
#define ELEMINTERFACE_H

#include <string>
#include <cstring>
#include <vector>
#include <ncurses.h>
#include <Size_T.h>
#include <map>
#include <iostream>
#include <set>

static std::string uint_to_str(unsigned int a){
    std::string answer="";
    if (!a) return "0";
    while (a){
        answer=char(a%10+'0')+answer;
        a/=10;
    }
    return answer;
}

class Geometry{
public:
    Geometry (int x, int y, int w, int h){
        this->x=x;
        this->y=y;
        this->Size=Size_t(w, h);

    }
    int x=0, y=0;
    Size_t Size=Size_t(0,0);
};

class __BaseELEMENT{
public:

    std::string Name; //Приоритет

    std::string Type="__Base"; //тип элемента

    __BaseELEMENT(__BaseELEMENT* Parent=nullptr, std::string Name=""){ //Конструктор любого элемента принемающий родителя и имя для доступа
        this->Parent=Parent;
        this->Name=Name;
        NameMap["Just<Parent>"]=this;
        if (Parent!=nullptr){
            Parent->AddElem(this);
        }
    }

    void AddElem(__BaseELEMENT* el){ //Функция добавления дочернего элемента
        this->NameMap[el->Name]=el;
        //std::cout << "\n(AddElem)"+el->Name;
    };

    ~__BaseELEMENT(){ //деструктор
        for (auto now : NameMap){
            if (now.first!="Just<Parent>"){
                now.second->~__BaseELEMENT();
                delete now.second;
            }
        }
    }

    void UpNeedKeyboard(){

        if (Parent!=nullptr){
            Parent->UpNeedKeyboard();
        }
        else{
            UPDATE_KEYBOARD();
        }
    }

    void UPDATE_KEYBOARD(){

        Exceptors.clear();
        for (int i=0; i<ExceptableKeys.size(); i++){
            Exceptors[ExceptableKeys[i]]=this;
        }

        for (auto now : NameMap){
            if (now.second->Visible && now.first!="Just<Parent>"){
                now.second->UPDATE_KEYBOARD();
                for (auto elem : now.second->Exceptors){
                    Exceptors[elem.first]=elem.second;
                }
            }
        }
    }

    virtual void KeyEnter(int Key){};

    std::vector<int> ExceptableKeys;

    std::map<int, __BaseELEMENT*> Exceptors;


    std::map<int, __BaseELEMENT* > ElemsWhoExpectThisKey; //словарь указателея на элемент ожидающий некоторую кнопку с наибольшим приоритетом

    virtual void MessageFunc(std::string FuncName, std::string Argv){}; //Функция исполняющая функцию из словаря CommandList по переданному имени с переданными аргументами

    void UpdateGraphic(){
        Paint();
        //std::cout<<"\n(Paint)"+this->Name;
        PaintChildren();
    }

    virtual void Paint(){}; //Функция отрисовки

    void PaintChildren(){
        for (auto now : NameMap){
            if (now.first!="Just<Parent>" && now.second->Visible==true)
                now.second->UpdateGraphic();
        }
    }

    bool Visible=true;

    void UpNeedToPaint(){
        //std::cout << "\n(UpNeedToPait)"+this->Name;
        if (Parent!=nullptr){
            Parent->UpNeedToPaint();
        }else{
            clear();
            UpdateGraphic();
            refresh();
        }
    } //Сообщить наверх о необходимости перерисовки


    __BaseELEMENT* Parent=nullptr; //Указатель на родительский элемент

    std::map<std::string, __BaseELEMENT*> NameMap; //{<Name of Children>, <Children Ptr>}

    void DoCommand(std::string TargetObject, std::string FuncName, std::string Argv){ //forExample {"Score1", {"ChangeText", "+40"}}
                                                                                    //           {<Recivier name>, <command name> <argv>}

        //std::cout << "\n\n(DoCommand)"+TargetObject;
        //for (auto now : NameMap){
        //    std::cout << "\n["+now.first+"]";
        //}
        if (NameMap.count(TargetObject)){

            NameMap[TargetObject]->MessageFunc(FuncName, Argv);
        }
    }

};

class ELEM_InfoBox : public __BaseELEMENT, public Geometry
{
public:
    static std::string uint_to_str(unsigned int a){
        std::string answer="";
        if (!a) return "0";
        while (a){
            answer=char(a%10+'0')+answer;
            a/=10;
        }
        return answer;
    }
    void KeyEnter(int Key){};

    void Hide(std::string Argv);

    void Show(std::string Argv);

    void move(std::string Argv);

    void resize(std::string Argv);

    static unsigned int str_to_uint(std::string a){
        unsigned int answer=0;
        for (int i=0; i<a.length(); i++){
            answer=answer*10+a[i]-'0';
        }
        return answer;
    }

    static std::string GetBigLine(std::vector<std::string> a){
        std::string answer="";
        for (int i=0; i<a.size(); i++){
            answer+=a[i];
        }
        return answer;
    }

    ELEM_InfoBox(__BaseELEMENT* Parent, std::string Name, unsigned int x, unsigned int y, unsigned int w, unsigned int h, std::string message, std::string caption="");

    static std::vector<std::string> GetTextInBoarder(Size_t sz, std::string Message);

    void MessageFunc(std::string FuncName, std::string Argv) override;

    void Paint() override;

    void ChangeText(std::string Argv);

    void findMaximumLength();
    unsigned int maxLength=0;
    std::string CaptionOfInfo="";
    std::vector<std::string> Text;
    std::map<std::string, void (ELEM_InfoBox::*) (std::string)> CommandList;
    //for paint
    void CalculateLines();

    std::string UpLine="", DownLine="", EmptyLine="";
};

#endif // ELEMINTERFACE_H
