#include "gamewidget.h"

ELEM_15Game::ELEM_15Game(__BaseELEMENT* Parent, std::string Name, int x, int y, int w, int h) : __BaseELEMENT(Parent, Name), Geometry(x,y,w,h)
{

    init_pair(7,  COLOR_WHITE,   COLOR_BLACK);
    init_pair(1,  COLOR_BLUE,     COLOR_GREEN);
    init_pair(2,  COLOR_BLUE,   COLOR_BLACK);
    board.resize(h);
    for (int i=0; i<h; i++){
        board[i].resize(w/2);
    }
    for (int i=0; i<h; i++){
        for (int j=0; j<w/2; j++){
            board[i][j]="  "; //free space
        }
    }
    for (int i=0; i<h; i++){
        board[i][0]="\xc5\xc5"; //wall
        board[i][w/2-1]="\xc5\xc5";
    }
    for (int i=1; i<w/2-1; i++){
        board[0][i]="\xc5\xc5";
        board[h-1][i]="\xc5\xc5";
    }
    ShaffleCard();
    ExceptableKeys={KEY_UP, KEY_LEFT, KEY_F(12), '\n'};
    UpNeedToPaint();
    UpNeedKeyboard();
}

void ELEM_15Game::MessageFunc(std::string FuncName, std::string Argv){
    //std::cout << "\n(Message)"+FuncName;
    if (CommandList.count(FuncName)){
        (this->*CommandList[FuncName])(Argv);
    }
}

void ELEM_15Game::move(std::string Argv){
    int i;
    for (i=0; i<Argv.length(); i++){
        if (Argv[i]==',') break;
    }
    int xt=str_to_uint(Argv.substr(0, i));
    int yt=str_to_uint(Argv.substr(i+1, Argv.length()-i));

    //std::cout << "\n(Move)["+uint_to_str(xt)+"] ["+uint_to_str(yt)+']';
    this->x=xt;
    this->y=yt;
    UpNeedToPaint();
}

void ELEM_15Game::Show(){
    Visible=true;
}

void ELEM_15Game::Hide(){
    Visible=false;
}


void ELEM_15Game::Paint(){
    for (int i=0; i<board.size(); i++){
        for (int j=0; j<board[i].size(); j++){
            attron(COLOR_PAIR(7));
            if (board[i][j]=="\xc5\xc5"){
                attron(COLOR_PAIR(Wall));
                mvaddch(y+i, x+j*2, ACS_PLUS);
                mvaddch(y+i, x+j*2+1, ACS_PLUS);
            }
        }
    }
    attron(COLOR_PAIR(7));
};

bool ELEM_15Game::Cheak(int x[4][4]){
    int a[4][4];
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++)
            a[i][j]=x[i][j];
    }
    for (int i=0; i<3; i++) a[0][i]=a[0][i+1];
    for (int i=0; i<3; i++) a[i][3]=a[i+1][3];
    a[3][3]=0;
    int temp[16];
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            temp[i*4+j]=a[i][j];
        }
    }
    int tAns=0;
    for (int i=0; i<14; i++){
        for (int j=i+1; j<15; j++){
            tAns+=int(temp[i]>temp[j]);
        }
    }
    return tAns%2;
}

void ELEM_15Game::EndGame(){
    ExceptableKeys.clear();
    UpNeedKeyboard();
    this->AddElem(new ELEM_InfoBox(this, "WinInfo", x+(Size.Width-15)/2, y+(Size.Height-6)/2, 15, 6, "You win!", "Message"));
}

bool ELEM_15Game::isWin(){
    int temp[16];
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if (Playground[i][j]==nullptr) temp[i*4+j]=16;
            else temp[i*4+j]=str_to_uint(Playground[i][j]->Text[0]);
        }
    }
    int t=0;
    for (; t<15; t++){
        if (temp[t]>temp[t+1]) break;
    }
    return t==15;
}

void ELEM_15Game::ShaffleCard(){
A:{}
    FreeCords={0,0};
    std::vector<int> Variables(15);
    for (int i=1; i<16; i++) Variables[i-1]=i;

    int temp[4][4];
    for (int i=1; i<4*4; i++){
        int ind=rand()%Variables.size();
        temp[i/4][i%4]=Variables[ind];
        Variables.erase(Variables.begin()+ind);
    }
    if (Cheak(temp)) goto A;
    this->NameMap.clear();
    this->NameMap["Just<Parent>"]=this;
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if (Playground[i][j]!=nullptr)
                delete Playground[i][j];
        }
    }
    Playground[0][0]=nullptr;
    for (int j=1; j<4; j++){
        this->Playground[0][j]=new ELEM_InfoBox(this, "Cards"+uint_to_str(j), 2+((Size.Width-4)/4)*j,2,(Size.Width-4)/4-1,(Size.Height-4)/4-1, uint_to_str(temp[0][j]));
        this->AddElem(Playground[0][j]);
    }

    for (int i=1; i<4; i++){
        for (int j=0; j<4; j++){
            this->Playground[i][j]=new ELEM_InfoBox(this, "Cards"+uint_to_str(i*4+j), 2+((Size.Width-4)/4)*j,2+((Size.Height-4)/4)*i,(Size.Width-4)/4-1,(Size.Height-4)/4-1, uint_to_str(temp[i][j]));
            this->AddElem(Playground[i][j]);
        }
    }
    Parent->NameMap.erase("WinInfo");
    Parent->DoCommand("InfoBox_Score1", "ChangeText", uint_to_str(0));

}

int findInIntVector(std::vector<int> a, int Elem){
    for (int i=0; i<a.size(); i++){
        if (a[i]==Elem) return i;
    }
    return -1;
}

std::pair<int,int> findInIntMassive(int a[4][4], int Elem){
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if (a[i][j]==Elem) return {i, j};
        }
    }
}

void ELEM_15Game::AutoGame(){
    Parent->DoCommand("InfoBox_Score1", "ChangeText", "-1");
    int temp[4][4];
    int ans[4][4];
    bool AccessMask[4][4];
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            AccessMask[i][j]=true;
            ans[i][j]=i*4+j+1;
            if (Playground[i][j]==nullptr) temp[i][j]=0;
            else temp[i][j]=str_to_uint(Playground[i][j]->Text[0]);
        }
    }
    ans[3][3]=0;
    bool Error=false;
    int ind=0;
    for (ind=0; ind<4; ind++){
        if (temp[0][ind]!=ans[0][ind]){
            Error=true;
            break;
        }
        AccessMask[0][ind]=false;
    }
    if (Error){
        //необходимо собирать первую верхнюю строку начиная ind с символа
        std::pair<int, int> tans;
        if (ind<2)
            tans=findInIntMassive(temp, ans[0][ind]);
        else{
            if (temp[0][2]==4){

                tans=findInIntMassive(temp, 3);
                int yNeed=tans.first, xNeed=tans.second;
                int xFree=FreeCords.first, yFree=FreeCords.second;
                if (xNeed==3 && yNeed==0 && (xFree!=3 || yFree!=1)){
                    if (3-xFree) KeyEnter(KEY_LEFT);
                    else if (yFree-1) KeyEnter(KEY_DOWN);
                }else if(xNeed==3 && yNeed==0 && (xFree==3 && yFree==1)) KeyEnter(KEY_DOWN);
                else if (xNeed==3 && yNeed==1 && (xFree==3 || yFree==0)) KeyEnter(KEY_RIGHT);
                else if (xFree==3 && yFree==0 && (xNeed!=2 || yNeed!=1)) KeyEnter(KEY_UP);
                else{
                    ind=2;

                    goto GO;
                }
                goto End;
            }else if(temp[0][3]==4){
                tans=findInIntMassive(temp, 3);
                int yNeed=tans.first, xNeed=tans.second;
                int xFree=FreeCords.first, yFree=FreeCords.second;
                if(xNeed==3 && yNeed==1 && (xFree==2 && yFree==1)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==1 && (xFree==2 && yFree==0)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==1 && (xFree==2 && yFree==1)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==1 && (xFree==2 && yFree==2)) KeyEnter(KEY_LEFT);
                else if(xNeed==3 && yNeed==1 && (xFree==3 && yFree==2)) KeyEnter(KEY_DOWN);
                else if(xNeed==3 && yNeed==2 && (xFree==3 && yFree==1)) KeyEnter(KEY_RIGHT);
                else if(xNeed==2 && yNeed==1 && (xFree==2 && yFree==0)) KeyEnter(KEY_UP);
                else goto F4;
                goto End;
            }
            F4:{}
            tans=findInIntMassive(temp, ans[0][3]);
            ind=2;
        }
GO:{}
        int yNeed=tans.first, xNeed=tans.second;

        int xFree=FreeCords.first, yFree=FreeCords.second;
        if (xNeed<ind){
            int dltX=ind-xNeed, dltY=yNeed-1;
            if (xFree<xNeed){
                if (yFree<yNeed){
                    KeyEnter(KEY_LEFT);
                }else
                if (yFree>=yNeed){
                    if (yFree==yNeed && dltY==0){
                        KeyEnter(KEY_UP);
                    }else
                    if (dltY==0){
                        KeyEnter(KEY_LEFT);
                    }
                    else
                    if (xNeed-xFree<yFree-yNeed){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_DOWN);
                    }
                }
            }else
            if (xFree==xNeed){
                if (yFree>yNeed){
                    KeyEnter(KEY_LEFT);
                }else{
                    KeyEnter(KEY_UP);
                }
            }else
            if(xFree>xNeed) {
                if (yFree>yNeed){
                    KeyEnter(KEY_DOWN);
                }else
                if (yFree==yNeed){
                    KeyEnter(KEY_RIGHT);
                }else
                if (xFree-xNeed<yNeed-yFree){
                    if (xFree>ind || yFree>=1)
                        KeyEnter(KEY_RIGHT);
                    else
                        KeyEnter(KEY_UP);
                }else{
                    KeyEnter(KEY_UP);
                }
            }

        }else{
            int dltX=xNeed-ind, dltY=yNeed;
            if (dltX+dltY!=0){
                if (xFree>xNeed){
                    if (yFree<yNeed){
                        KeyEnter(KEY_RIGHT);
                    }else
                    if (yFree>=yNeed){
                        if (yFree==yNeed && dltY==0){
                            KeyEnter(KEY_UP);
                        }else
                        if (dltY==0){
                            KeyEnter(KEY_RIGHT);
                        }
                        else
                        if (xFree-xNeed<yFree-yNeed){
                            KeyEnter(KEY_RIGHT);
                        }else{
                            KeyEnter(KEY_DOWN);
                        }
                    }
                }else if (xFree==xNeed){
                    if (yFree>yNeed){
                        if (xFree!=ind)
                            KeyEnter(KEY_RIGHT);
                        else
                            KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }else if(xFree<xNeed) {

                    if (yFree>yNeed){
                        if (xFree+1==ind){
                            KeyEnter(KEY_LEFT);
                        }else
                            KeyEnter(KEY_DOWN);
                    }else

                    if (yFree==yNeed){
                        if (xFree+1==ind && xNeed==ind){
                            if (yFree>1)
                                KeyEnter(KEY_DOWN);
                            else
                                KeyEnter(KEY_UP);
                        }else{
                            KeyEnter(KEY_LEFT);
                        }
                    }else
                    if (xNeed-xFree<=yNeed-yFree){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }
            }

        }
        goto End;
    }
    Error=false;
    for (ind=0; ind<4; ind++){
        if (temp[ind][0]!=ans[ind][0]){
            Error=true;
            break;
        }
        AccessMask[ind][0]=false;
    }
    if (Error){
        Transpor=true;
        //необходимо собирать первую левый столбей начиная ind+1 с символа
        std::pair<int, int> tans;
        if (ind<2)
            tans=findInIntMassive(temp, ans[ind][0]);
        else{
            if (temp[2][0]==13){

                tans=findInIntMassive(temp, 9);
                int yNeed=tans.second, xNeed=tans.first;
                int xFree=FreeCords.second, yFree=FreeCords.first;
                if (xNeed==3 && yNeed==0 && (xFree!=3 || yFree!=1)){
                    if (3-xFree) KeyEnter(KEY_LEFT);
                    else if (yFree-1) KeyEnter(KEY_DOWN);
                }else if(xNeed==3 && yNeed==0 && (xFree==3 && yFree==1)) KeyEnter(KEY_DOWN);
                else if (xNeed==3 && yNeed==1 && (xFree==3 || yFree==0)) KeyEnter(KEY_RIGHT);
                else if (xFree==3 && yFree==0 && (xNeed!=2 || yNeed!=1)) KeyEnter(KEY_UP);
                else{
                    ind=2;
                    goto GO2;
                }
                goto End;
            }else if(temp[3][0]==13){
                tans=findInIntMassive(temp, 9);
                int yNeed=tans.second, xNeed=tans.first;
                int xFree=FreeCords.second, yFree=FreeCords.first;
                if(xNeed==3 && yNeed==1 && (xFree==2 && yFree==1)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==1 && (xFree==2 && yFree==0)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==1 && (xFree==2 && yFree==1)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==1 && (xFree==2 && yFree==2)) KeyEnter(KEY_LEFT);
                else if(xNeed==3 && yNeed==1 && (xFree==3 && yFree==2)) KeyEnter(KEY_DOWN);
                else if(xNeed==3 && yNeed==2 && (xFree==3 && yFree==1)) KeyEnter(KEY_RIGHT);
                else if(xNeed==2 && yNeed==1 && (xFree==2 && yFree==0)) KeyEnter(KEY_UP);
                else goto F42;
                goto End;
            }
            F42:{}
            tans=findInIntMassive(temp, 13);
            ind=2;
        }
GO2:{}
        int yNeed=tans.second, xNeed=tans.first;

        int xFree=FreeCords.second, yFree=FreeCords.first;
        if (xNeed<ind){
            int dltX=ind-xNeed, dltY=yNeed-1;
            if (xFree<xNeed){
                if (yFree<yNeed){
                    KeyEnter(KEY_LEFT);
                }else
                if (yFree>=yNeed){
                    if (yFree==yNeed && dltY==0){
                        KeyEnter(KEY_UP);
                    }else
                    if (dltY==0){
                        KeyEnter(KEY_LEFT);
                    }
                    else
                    if (xNeed-xFree<yFree-yNeed){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_DOWN);
                    }
                }
            }else
            if (xFree==xNeed){
                if (yFree>yNeed){
                    KeyEnter(KEY_LEFT);
                }else{
                    KeyEnter(KEY_UP);
                }
            }else
            if(xFree>xNeed) {
                if (yFree>yNeed){
                    KeyEnter(KEY_DOWN);
                }else
                if (yFree==yNeed){
                    KeyEnter(KEY_RIGHT);
                }else
                if (xFree-xNeed<yNeed-yFree){
                    if (xFree>ind || yFree>=1)
                        KeyEnter(KEY_RIGHT);
                    else
                        KeyEnter(KEY_UP);
                }else{
                    KeyEnter(KEY_UP);
                }
            }

        }else{
            int dltX=xNeed-ind, dltY=yNeed;
            if (dltX+dltY!=0){
                if (xFree>xNeed){
                    if (yFree<yNeed){
                        KeyEnter(KEY_RIGHT);
                    }else
                    if (yFree>=yNeed){
                        if (yFree==yNeed && dltY==0){
                            KeyEnter(KEY_UP);
                        }else
                        if (dltY==0){
                            KeyEnter(KEY_RIGHT);
                        }
                        else
                        if (xFree-xNeed<yFree-yNeed){
                            KeyEnter(KEY_RIGHT);
                        }else{
                            KeyEnter(KEY_DOWN);
                        }
                    }
                }else if (xFree==xNeed){
                    if (yFree>yNeed){
                        if (xFree!=ind)
                            KeyEnter(KEY_RIGHT);
                        else
                            KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }else if(xFree<xNeed) {

                    if (yFree>yNeed){
                        if (xFree+1==ind){
                            KeyEnter(KEY_LEFT);
                        }else
                            KeyEnter(KEY_DOWN);
                    }else

                    if (yFree==yNeed){
                        if (xFree+1==ind && xNeed==ind){
                            if (yFree>1)
                                KeyEnter(KEY_DOWN);
                            else
                                KeyEnter(KEY_UP);
                        }else{
                            KeyEnter(KEY_LEFT);
                        }
                    }else
                    if (xNeed-xFree<=yNeed-yFree){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }
            }

        }
        goto End;
    }
    Error=false;
    for (ind=0; ind<4; ind++){
        if (temp[1][ind]!=ans[1][ind]){
            Error=true;
            break;
        }
    }
    if (Error){
        std::pair<int, int> tans;
        if (ind<2)
            tans=findInIntMassive(temp, ans[1][ind]);
        else{
            if (temp[1][2]==8){

                tans=findInIntMassive(temp, 7);
                int yNeed=tans.first, xNeed=tans.second;
                int xFree=FreeCords.first, yFree=FreeCords.second;
                if (xNeed==3 && yNeed==1 && (xFree!=3 || yFree!=2)){
                    if (3-xFree) KeyEnter(KEY_LEFT);
                    else if (yFree-2) KeyEnter(KEY_DOWN);
                }else if(xNeed==3 && yNeed==1 && (xFree==3 && yFree==2)) KeyEnter(KEY_DOWN);
                else if (xNeed==3 && yNeed==2 && (xFree==3 || yFree==1)) KeyEnter(KEY_RIGHT);
                else if (xFree==3 && yFree==1 && (xNeed!=2 || yNeed!=2)) KeyEnter(KEY_UP);
                else{
                    ind=2;

                    goto GO3;
                }
                goto End;
            }else if(temp[1][3]==8){
                tans=findInIntMassive(temp, 7);
                int yNeed=tans.first, xNeed=tans.second;
                int xFree=FreeCords.first, yFree=FreeCords.second;
                if(xNeed==3 && yNeed==2 && (xFree==2 && yFree==2)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==2 && (xFree==2 && yFree==1)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==2 && (xFree==2 && yFree==2)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==2 && (xFree==2 && yFree==3)) KeyEnter(KEY_LEFT);
                else if(xNeed==3 && yNeed==2 && (xFree==3 && yFree==3)) KeyEnter(KEY_DOWN);
                else if(xNeed==3 && yNeed==3 && (xFree==3 && yFree==2)) KeyEnter(KEY_RIGHT);
                else if(xNeed==2 && yNeed==2 && (xFree==2 && yFree==1)) KeyEnter(KEY_UP);
                else goto F43;
                goto End;
            }
            F43:{}
            tans=findInIntMassive(temp, ans[1][3]);
            ind=2;
        }
GO3:{}
        int yNeed=tans.first, xNeed=tans.second;

        int xFree=FreeCords.first, yFree=FreeCords.second;
        if (xNeed<ind){
            int dltX=ind-xNeed, dltY=yNeed-2;
            if (xFree<xNeed){
                if (yFree<yNeed){
                    KeyEnter(KEY_LEFT);
                }else
                if (yFree>=yNeed){
                    if (yFree==yNeed && dltY==0){
                        KeyEnter(KEY_UP);
                    }else
                    if (dltY==0){
                        KeyEnter(KEY_LEFT);
                    }
                    else
                    if (xNeed-xFree<yFree-yNeed){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_DOWN);
                    }
                }
            }else
            if (xFree==xNeed){
                if (yFree>yNeed){
                    KeyEnter(KEY_LEFT);
                }else{
                    KeyEnter(KEY_UP);
                }
            }else
            if(xFree>xNeed) {
                if (yFree>yNeed){
                    KeyEnter(KEY_DOWN);
                }else
                if (yFree==yNeed){
                    KeyEnter(KEY_RIGHT);
                }else
                if (xFree-xNeed<yNeed-yFree){
                    if (xFree>ind || yFree>=2)
                        KeyEnter(KEY_RIGHT);
                    else
                        KeyEnter(KEY_UP);
                }else{
                    KeyEnter(KEY_UP);
                }
            }

        }else{
            int dltX=xNeed-ind, dltY=yNeed-1;
            if (dltX+dltY!=0){
                if (xFree>xNeed){
                    if (yFree<yNeed){
                        KeyEnter(KEY_RIGHT);
                    }else
                    if (yFree>=yNeed){
                        if (yFree==yNeed && dltY==0){
                            KeyEnter(KEY_UP);
                        }else
                        if (dltY==0){
                            KeyEnter(KEY_RIGHT);
                        }
                        else
                        if (xFree-xNeed<yFree-yNeed){
                            KeyEnter(KEY_RIGHT);
                        }else{
                            KeyEnter(KEY_DOWN);
                        }
                    }
                }else if (xFree==xNeed){
                    if (yFree>yNeed){
                        if (xFree!=ind)
                            KeyEnter(KEY_RIGHT);
                        else
                            KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }else if(xFree<xNeed) {

                    if (yFree>yNeed){
                        if (xFree+1==ind){
                            KeyEnter(KEY_LEFT);
                        }else
                            KeyEnter(KEY_DOWN);
                    }else

                    if (yFree==yNeed){
                        if (xFree+1==ind && xNeed==ind){
                            if (yFree>2)
                                KeyEnter(KEY_DOWN);
                            else
                                KeyEnter(KEY_UP);
                        }else{
                            KeyEnter(KEY_LEFT);
                        }
                    }else
                    if (xNeed-xFree<=yNeed-yFree){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }
            }

        }
        goto End;
    }
    Error=false;
    for (ind=0; ind<4; ind++){
        if (temp[ind][1]!=ans[ind][1]){
            Error=true;
            break;
        }
        AccessMask[ind][1]=false;
    }if (Error){
        Transpor=true;
        std::pair<int, int> tans;
        if (ind<2)
            tans=findInIntMassive(temp, ans[ind][1]);
        else{
            if (temp[2][1]==14){

                tans=findInIntMassive(temp, 10);
                int yNeed=tans.second, xNeed=tans.first;
                int xFree=FreeCords.second, yFree=FreeCords.first;
                if (xNeed==3 && yNeed==1 && (xFree!=3 || yFree!=2)){
                    if (3-xFree) KeyEnter(KEY_LEFT);
                    else if (yFree-2) KeyEnter(KEY_DOWN);
                }else if(xNeed==3 && yNeed==1 && (xFree==3 && yFree==2)) KeyEnter(KEY_DOWN);
                else if (xNeed==3 && yNeed==2 && (xFree==3 || yFree==1)) KeyEnter(KEY_RIGHT);
                else if (xFree==3 && yFree==1 && (xNeed!=2 || yNeed!=2)) KeyEnter(KEY_UP);
                else{
                    ind=2;

                    goto GO24;
                }
                goto End;
            }else if(temp[3][1]==14){
                tans=findInIntMassive(temp, 10);
                int yNeed=tans.second, xNeed=tans.first;
                int xFree=FreeCords.second, yFree=FreeCords.first;
                if(xNeed==3 && yNeed==2 && (xFree==2 && yFree==2)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==2 && (xFree==2 && yFree==1)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==2 && (xFree==2 && yFree==2)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==2 && (xFree==2 && yFree==3)) KeyEnter(KEY_LEFT);
                else if(xNeed==3 && yNeed==2 && (xFree==3 && yFree==3)) KeyEnter(KEY_DOWN);
                else if(xNeed==3 && yNeed==3 && (xFree==3 && yFree==2)) KeyEnter(KEY_RIGHT);
                else if(xNeed==2 && yNeed==2 && (xFree==2 && yFree==1)) KeyEnter(KEY_UP);
                else goto F424;
                goto End;
            }
            F424:{}
            tans=findInIntMassive(temp, 14);
            ind=2;
        }
GO24:{}
        int yNeed=tans.second, xNeed=tans.first;

        int xFree=FreeCords.second, yFree=FreeCords.first;
        if (xNeed<ind){
            int dltX=ind-xNeed, dltY=yNeed-2;
            if (xFree<xNeed){
                if (yFree<yNeed){
                    KeyEnter(KEY_LEFT);
                }else
                if (yFree>=yNeed){
                    if (yFree==yNeed && dltY==0){
                        KeyEnter(KEY_UP);
                    }else
                    if (dltY==0){
                        KeyEnter(KEY_LEFT);
                    }
                    else
                    if (xNeed-xFree<yFree-yNeed){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_DOWN);
                    }
                }
            }else
            if (xFree==xNeed){
                if (yFree>yNeed){
                    KeyEnter(KEY_LEFT);
                }else{
                    KeyEnter(KEY_UP);
                }
            }else
            if(xFree>xNeed) {
                if (yFree>yNeed){
                    KeyEnter(KEY_DOWN);
                }else
                if (yFree==yNeed){
                    KeyEnter(KEY_RIGHT);
                }else
                if (xFree-xNeed<yNeed-yFree){
                    if (xFree>ind || yFree>=2)
                        KeyEnter(KEY_RIGHT);
                    else
                        KeyEnter(KEY_UP);
                }else{
                    KeyEnter(KEY_UP);
                }
            }

        }else{
            int dltX=xNeed-ind, dltY=yNeed-1;
            if (dltX+dltY!=0){
                if (xFree>xNeed){
                    if (yFree<yNeed){
                        KeyEnter(KEY_RIGHT);
                    }else
                    if (yFree>=yNeed){
                        if (yFree==yNeed && dltY==0){
                            KeyEnter(KEY_UP);
                        }else
                        if (dltY==0){
                            KeyEnter(KEY_RIGHT);
                        }
                        else
                        if (xFree-xNeed<yFree-yNeed){
                            KeyEnter(KEY_RIGHT);
                        }else{
                            KeyEnter(KEY_DOWN);
                        }
                    }
                }else if (xFree==xNeed){
                    if (yFree>yNeed){
                        if (xFree!=ind)
                            KeyEnter(KEY_RIGHT);
                        else
                            KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }else if(xFree<xNeed) {

                    if (yFree>yNeed){
                        if (xFree+1==ind){
                            KeyEnter(KEY_LEFT);
                        }else
                            KeyEnter(KEY_DOWN);
                    }else

                    if (yFree==yNeed){
                        if (xFree+1==ind && xNeed==ind){
                            if (yFree>2)
                                KeyEnter(KEY_DOWN);
                            else
                                KeyEnter(KEY_UP);
                        }else{
                            KeyEnter(KEY_LEFT);
                        }
                    }else
                    if (xNeed-xFree<=yNeed-yFree){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }
            }

        }
        goto End;
    }
    Error=false;
    for (ind=0; ind<4; ind++){
        if (temp[2][ind]!=ans[2][ind]){
            Error=true;
            break;
        }
    }
    if (Error){
        std::pair<int, int> tans;
        if (ind<2)
            tans=findInIntMassive(temp, ans[2][ind]);
        else{
            if (temp[2][2]==12){

                tans=findInIntMassive(temp, 11);
                int yNeed=tans.first, xNeed=tans.second;
                int xFree=FreeCords.first, yFree=FreeCords.second;
                if (xNeed==3 && yNeed==2 && (xFree!=3 || yFree!=3)){
                    if (3-xFree) KeyEnter(KEY_LEFT);
                    else if (yFree-3) KeyEnter(KEY_DOWN);
                }else if(xNeed==3 && yNeed==2 && (xFree==3 && yFree==3)) KeyEnter(KEY_DOWN);
                else if (xNeed==3 && yNeed==3 && (xFree==3 || yFree==2)) KeyEnter(KEY_RIGHT);
                else if (xFree==3 && yFree==2 && (xNeed!=2 || yNeed!=3)) KeyEnter(KEY_UP);
                else{
                    ind=2;

                    goto GO33;
                }
                goto End;
            }else if(temp[2][3]==12){
                tans=findInIntMassive(temp, 11);
                int yNeed=tans.first, xNeed=tans.second;
                int xFree=FreeCords.first, yFree=FreeCords.second;
                if(xNeed==3 && yNeed==3 && (xFree==2 && yFree==3)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==3 && (xFree==2 && yFree==2)) KeyEnter(KEY_UP);
                else if(xNeed==3 && yNeed==3 && (xFree==2 && yFree==3)) KeyEnter(KEY_UP);
                else if(xNeed==2 && yNeed==3 && (xFree==2 && yFree==2)) KeyEnter(KEY_UP);
                else goto F433;
                goto End;
            }
            F433:{}
            tans=findInIntMassive(temp, ans[2][3]);
            ind=2;
        }
GO33:{}
        int yNeed=tans.first, xNeed=tans.second;

        int xFree=FreeCords.first, yFree=FreeCords.second;
        if (xNeed<ind){
            int dltX=ind-xNeed, dltY=yNeed-3;
            if (xFree<xNeed){
                if (yFree<yNeed){
                    KeyEnter(KEY_LEFT);
                }else
                if (yFree>=yNeed){
                    if (yFree==yNeed && dltY==0){
                        KeyEnter(KEY_UP);
                    }else
                    if (dltY==0){
                        KeyEnter(KEY_LEFT);
                    }
                    else
                    if (xNeed-xFree<yFree-yNeed){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_DOWN);
                    }
                }
            }else
            if (xFree==xNeed){
                if (yFree>yNeed){
                    KeyEnter(KEY_LEFT);
                }else{
                    KeyEnter(KEY_UP);
                }
            }else
            if(xFree>xNeed) {
                if (yFree>yNeed){
                    KeyEnter(KEY_DOWN);
                }else
                if (yFree==yNeed){
                    KeyEnter(KEY_RIGHT);
                }else
                if (xFree-xNeed<yNeed-yFree){
                    if (xFree>ind || yFree>=3)
                        KeyEnter(KEY_RIGHT);
                    else
                        KeyEnter(KEY_UP);
                }else{
                    KeyEnter(KEY_UP);
                }
            }

        }else{
            int dltX=xNeed-ind, dltY=yNeed-2;
            if (dltX+dltY!=0){
                if (xFree>xNeed){
                    if (yFree<yNeed){
                        KeyEnter(KEY_RIGHT);
                    }else
                    if (yFree>=yNeed){
                        if (yFree==yNeed && dltY==0){
                            KeyEnter(KEY_UP);
                        }else
                        if (dltY==0){
                            KeyEnter(KEY_RIGHT);
                        }
                        else
                        if (xFree-xNeed<yFree-yNeed){
                            KeyEnter(KEY_RIGHT);
                        }else{
                            KeyEnter(KEY_DOWN);
                        }
                    }
                }else if (xFree==xNeed){
                    if (yFree>yNeed){
                        if (xFree!=ind)
                            KeyEnter(KEY_RIGHT);
                        else
                            KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }else if(xFree<xNeed) {

                    if (yFree>yNeed){
                        if (xFree+1==ind){
                            KeyEnter(KEY_LEFT);
                        }else
                            KeyEnter(KEY_DOWN);
                    }else

                    if (yFree==yNeed){
                        if (xFree+1==ind && xNeed==ind){
                            KeyEnter(KEY_UP);
                        }else{
                            KeyEnter(KEY_LEFT);
                        }
                    }else
                    if (xNeed-xFree<=yNeed-yFree){
                        KeyEnter(KEY_LEFT);
                    }else{
                        KeyEnter(KEY_UP);
                    }
                }
            }

        }
        goto End;
    }
    KeyEnter(KEY_LEFT);

End:{}
}

void ELEM_15Game::KeyEnter(int Key){
    if (Transpor){
        Transpor=false;
        switch (Key) {
            case KEY_LEFT:{ Key=KEY_UP;
                            break;
            }
            case KEY_DOWN:{ Key=KEY_RIGHT;
                            break;
            }
            case KEY_UP:{ Key=KEY_LEFT;
                            break;
            }
            case KEY_RIGHT:{ Key=KEY_DOWN;
                            break;
            }
        }
    }
    if (findInIntVector(ExceptableKeys, Key)!=-1){
        Parent->DoCommand("InfoBox_Score1", "ChangeText", "+1");
        switch (Key) {
            case KEY_UP:
            {
                auto Elem=this->Playground[FreeCords.second+1][FreeCords.first];
                Elem->move(uint_to_str(Elem->x)+','+uint_to_str(Elem->y-Elem->Size.Height-1));
                Playground[FreeCords.second][FreeCords.first]=Elem;
                Playground[FreeCords.second+1][FreeCords.first]=nullptr;
                FreeCords.second++;
                break;
            }
            case KEY_LEFT:
            {
                auto Elem=this->Playground[FreeCords.second][FreeCords.first+1];
                Elem->move(uint_to_str(Elem->x-Elem->Size.Width-1)+','+uint_to_str(Elem->y));
                Playground[FreeCords.second][FreeCords.first]=Elem;
                Playground[FreeCords.second][FreeCords.first+1]=nullptr;
                FreeCords.first++;
                break;
            }
            case KEY_DOWN:{
                auto Elem=this->Playground[FreeCords.second-1][FreeCords.first];
                Elem->move(uint_to_str(Elem->x)+','+uint_to_str(Elem->y+Elem->Size.Height+1));
                Playground[FreeCords.second][FreeCords.first]=Elem;
                Playground[FreeCords.second-1][FreeCords.first]=nullptr;
                FreeCords.second--;
                break;
            }
            case KEY_RIGHT:{
                auto Elem=this->Playground[FreeCords.second][FreeCords.first-1];
                Elem->move(uint_to_str(Elem->x+Elem->Size.Width+1)+','+uint_to_str(Elem->y));
                Playground[FreeCords.second][FreeCords.first]=Elem;
                Playground[FreeCords.second][FreeCords.first-1]=nullptr;
                FreeCords.first--;
                break;
            }
            case KEY_F(12):{
                std::cerr << '23';
                ShaffleCard();
                break;
            }
            case '\n':{
                AutoGame();
            }
        }
        ExceptableKeys={'\n'};
        if (FreeCords.second!=0) ExceptableKeys.push_back(KEY_DOWN);
        if (FreeCords.second!=3) ExceptableKeys.push_back(KEY_UP);
        if (FreeCords.first!=0) ExceptableKeys.push_back(KEY_RIGHT);
        if (FreeCords.first!=3) ExceptableKeys.push_back(KEY_LEFT);

        if (isWin()){
            EndGame();
        }
        ExceptableKeys.push_back(KEY_F(12));
        UpNeedKeyboard();
        UpNeedToPaint();
    }
}
