#ifndef ELEM_SNAKEGAME_H
#define ELEM_SNAKEGAME_H

#include <eleminterface.h>
#include <deque>
#include <cstdlib>

#define Wall 1
#define SnakeBody1 2
#define SnakeBody2 3
#define SnakeHead1 4
#define SnakeHead2 5
#define Apple 6


enum Move_vector{
    Up,
    Down,
    Left,
    Right,
    None
};

class ELEM_InfoBox_SNAKE :  public ELEM_InfoBox{
public :ELEM_InfoBox_SNAKE(__BaseELEMENT* Parent, std::string Name, unsigned int x, unsigned int y, unsigned int w, unsigned int h, std::string message, std::string caption="")
        : ELEM_InfoBox(Parent, Name, x, y, w, h, message, caption){

        ExceptableKeys={KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, 'a', 'd', 'w', 's'};
        init_pair(10,  COLOR_BLACK,    COLOR_CYAN);
        init_pair(11,  COLOR_BLACK, COLOR_MAGENTA);
        init_pair(12,  COLOR_WHITE, COLOR_BLACK);
        UpNeedKeyboard();
    }

    void KeyEnter(int Key) override{
        if (Key>='a' && Key<='w'){
            FirstPlayerActive=true;
            Text[0][Text[0].length()-2]='O';
            Text[0][Text[0].length()-1]='K';
        }else{
            SecondPlayerActive=true;
            Text[1][Text[1].length()-2]='O';
            Text[1][Text[1].length()-1]='K';
        }
        if (FirstPlayerActive && SecondPlayerActive){
            Hide("");
            Parent->DoCommand("Just<Parent>", "EndInitGame", "");
        }
        UpNeedToPaint();
    }




    void Paint() override{

        //upLine
            mvaddch(y,x, ACS_ULCORNER);
            if (this->CaptionOfInfo==""){
                for (int i=1; i<Size.Width-1; i++)
                    mvaddch(y,x+i, ACS_HLINE);
            }else{
                int i;
                for (i=1; i<(Size.Width-CaptionOfInfo.length())/2-1; i++)
                    mvaddch(y,x+i, ACS_HLINE);
                mvaddch(y,x+i, ACS_RTEE);
                i++;
                mvaddstr(y, x+i, CaptionOfInfo.c_str()); i+=CaptionOfInfo.length();
                mvaddch(y,x+i, ACS_LTEE); i++;
                for (; i<Size.Width-1; i++)
                    mvaddch(y,x+i, ACS_HLINE);
            }
            mvaddch(y, x+Size.Width-1, ACS_URCORNER);
        //Emptys


            for (int i=1; i<=(Size.Height-Text.size())/2; i++){
                mvaddch(y+i, x, ACS_VLINE);
                mvprintw(y+i, x+1, EmptyLine.c_str());
                mvaddch(y+i, x+Size.Width-1, ACS_VLINE);
            }
            for (int i=0; i<Text.size(); i++){
                mvaddch(y+1+(Size.Height-Text.size())/2+i, x, ACS_VLINE);
                std::string SomeString;
                for (int j=1; j<(Size.Width-Text[i].length())/2; j++)
                    SomeString+=" ";
                mvprintw(y+1+(Size.Height-Text.size())/2+i, x+1, SomeString.c_str());
                if (i==0)
                    attron(COLOR_PAIR(10));
                else if (i==1) attron(COLOR_PAIR(11));
                SomeString=Text[i];
                mvprintw(y+1+(Size.Height-Text.size())/2+i, x+(Size.Width-Text[i].length())/2, SomeString.c_str());
                SomeString="";
                attron(COLOR_PAIR(12));
                for (int j=(Size.Width-Text[i].length())/2+Text[i].length(); j<Size.Width-1; j++){
                    SomeString+=" ";
                }

                mvprintw(y+1+(Size.Height-Text.size())/2+i, x+(Size.Width-Text[i].length())/2+Text[i].length(), SomeString.c_str());
                mvaddch(y+1+(Size.Height-Text.size())/2+i, x+Size.Width-1, ACS_VLINE);
            }
            for (int i=(Size.Height-Text.size())/2+Text.size()+1; i<=Size.Height-1; i++){

                mvaddch(y+i, x, ACS_VLINE);
                mvprintw(y+i, x+1, EmptyLine.c_str());
                mvaddch(y+i, x+Size.Width-1, ACS_VLINE);
            }

            //Down

            mvaddch(y+Size.Height, x, ACS_LLCORNER);
            hline(ACS_HLINE, Size.Width-2);
            mvaddch(y+Size.Height, x+Size.Width-1, ACS_LRCORNER);

        /*mvprintw(y, x, UpLine.c_str());
        for (int i=1; i<=(Size.Height-Text.size())/2; i++){
            mvprintw(y+i, x, EmptyLine.c_str());
        }
        for (int i=0; i<Text.size(); i++){
            std::string SomeString="\xb3";
            for (int j=1; j<(Size.Width-Text[i].length())/2; j++)
                SomeString+=" ";
            mvprintw(y+1+(Size.Height-Text.size())/2+i, x, SomeString.c_str());
            if (i==0)
                attron(COLOR_PAIR(10));
            else if (i==1) attron(COLOR_PAIR(11));
            SomeString=Text[i];
            mvprintw(y+1+(Size.Height-Text.size())/2+i, x+(Size.Width-Text[i].length())/2, SomeString.c_str());
            attron(COLOR_PAIR(12));
            SomeString="";
            for (int j=(Size.Width-Text[i].length())/2+Text[i].length(); j<Size.Width-1; j++){
                SomeString+=" ";
            }
            SomeString+="\xb3";
            mvprintw(y+1+(Size.Height-Text.size())/2+i, x+(Size.Width-Text[i].length())/2+Text[i].length(), SomeString.c_str());
        }
        for (int i=(Size.Height-Text.size())/2+Text.size()+1; i<=Size.Height-1; i++){
            mvprintw(y+i, x, EmptyLine.c_str());
        }
        mvprintw(y+Size.Height, x, DownLine.c_str());
        */
    }
    bool FirstPlayerActive=false;
    bool SecondPlayerActive=false;
};

class Snake{
public:

    Snake(std::pair<int, int> Head, std::deque<std::pair<int, int> > Body){
        this->Head=Head;
        this->Body=Body;
    }
    std::pair <int, int> Head;
    std::deque <std::pair<int, int> > Body;
    Move_vector Movement=Move_vector::None;

    void MoveSnake(){

        Body.push_front(Head);
        Body.pop_back();
        switch (Movement) {
            case Up:{
                Head={Head.first, Head.second-1};
                break;
            }
            case Down:{
                Head={Head.first, Head.second+1};
                break;
            }
            case Left:{
                Head={Head.first-1, Head.second};
                break;
            }
            case Right:{
                Head={Head.first+1, Head.second};
                break;
            }
        }
    }
};

class ELEM_SnakeGame : public __BaseELEMENT, public Geometry
{
public:


    void move(std::string Argv);

    static unsigned int str_to_uint(std::string a){
        unsigned int answer=0;
        for (int i=0; i<a.length(); i++){
            answer=answer*10+a[i]-'0';
        }
        return answer;
    }

    void Show();

    void Hide();

    void EndInitGame(std::string Argv);

    void NextStep(std::string Argv);

    std::vector<std::vector<std::string> > board;
    Snake* Snake1, *Snake2;

    void MessageFunc(std::string FuncName, std::string Argv) override;

    void Paint() override;

    void KeyEnter(int Key) override;

    std::string state="InitGame";

    std::map<std::string, void (ELEM_SnakeGame::*) (std::string)> CommandList;

    std::string UpLine="", DownLine="", EmptyLine="";

    ELEM_SnakeGame(__BaseELEMENT* Parent, std::string Name, int x, int y, int w, int h);
private:
    void GenerateApple(){
        std::vector<std::pair<int, int> > MaybePosisApple;
        for (int i=1; i<Size.Height-1; i++){
            for (int j=1; j<Size.Width/2; j++){
                if (board[i][j]=="  ") MaybePosisApple.push_back({i, j});
            }
        }
        std::pair<int, int> temp=MaybePosisApple[rand()%MaybePosisApple.size()];
        board[temp.first][temp.second]="@@"; //apple;
    }
};

#endif // ELEM_SNAKEGAME_H
