
#include <eleminterface.h>
#include <mainwindow.h>
#include <cstdlib>
#include <elem_snakegame.h>
#include <ctime>
#include <clocale>

using namespace std;

string int_to_str(int a){
    if (!a) return "0";
    string answer="";
    while(a){
        answer=(char)(a%10+'0')+answer;
        a/=10;
    }
    return answer;
}

int main(){
    //std::cout<<setlocale(LC_ALL, ".866");setlocale(LC_CTYPE, "");
    srand(time(0));
    initscr();
    noecho();
    keypad(stdscr, true);
    start_color();
    curs_set(0); //"Убиваем" курсор
    mainwindow* MW = new mainwindow();
    MW->AddElem(new ELEM_InfoBox(MW, "InfoBox_Score1", 60, 0, 20, 6, "0", "Score1"));
    MW->AddElem(new ELEM_InfoBox(MW, "InfoBox_Score2", 60, 18, 20, 6, "0", "Score2"));
    MW->AddElem(new ELEM_InfoBox(MW, "Result", 57, (24-5)/2, 20, 6, "Player1 Win"));
    MW->AddElem(new ELEM_SnakeGame(MW, "SnakeWidget", 0, 0, 57, 24));
    MW->DoCommand("Result", "Hide", "");
    MW->Exec();
    endwin();
    return 0;
}
