
#include <eleminterfase.h>
#include <mainwindow.h>
#include <cstdlib>
#include <gamewidget.h>
#include <ctime>
#include <clocale>
//#include <windows.h>

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

    //SetConsoleOutputCP( CP_WINUNICODE );
    //cout<<'\u2500';
    //std::cout<<setlocale(LC_ALL, ".866");setlocale(LC_CTYPE, "");
    //std::cout << wchar_t();
    srand(time(0));
    initscr();
    noecho();
    keypad(stdscr, true);
    start_color();
    curs_set(0); //"Убиваем" курсор
    mainwindow* MW = new mainwindow();
    MW->AddElem(new ELEM_InfoBox(MW, "InfoBox_Score1", 60, 0, 20, 6, "0", "Steps"));
    MW->AddElem(new ELEM_InfoBox(MW, "InfoBox_ControllMenu", 58, 15, 30, 6, "Arrows to move\nEnter to Help to Solve\nF12 to Shuffle\nEsc to Exit", "Control menu"));
    MW->AddElem(new ELEM_15Game(MW, "PatnaWidget", 0, 0, 40, 24));
    MW->Exec();
    endwin();

    return 0;
}
