#ifndef ELEM_SNAKEGAME_H
#define ELEM_SNAKEGAME_H

#include <eleminterfase.h>
#include <deque>
#include <algorithm>
#include <cstdlib>

#define Wall 1



class ELEM_15Game : public __BaseELEMENT, public Geometry
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

    bool Transpor=false;

    ELEM_InfoBox* Playground[4][4];

    void ShaffleCard();

    bool Cheak(int x[4][4]);

    void Show();

    void Hide();

    void EndGame();

    bool isWin();

    void AutoGame();

    void NextStep(std::string Argv);

    std::pair<int,int> FreeCords={0,0};

    std::vector<std::vector<std::string> >board;


    void MessageFunc(std::string FuncName, std::string Argv) override;

    void Paint() override;

    void KeyEnter(int Key) override;

    std::string state="InitGame";

    std::map<std::string, void (ELEM_15Game::*) (std::string)> CommandList;

    std::string UpLine="", DownLine="", EmptyLine="";

    ELEM_15Game(__BaseELEMENT* Parent, std::string Name, int x, int y, int w, int h);
private:
};

#endif // ELEM_SNAKEGAME_H
