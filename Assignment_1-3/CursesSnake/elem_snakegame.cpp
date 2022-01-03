#include "elem_snakegame.h"

ELEM_SnakeGame::ELEM_SnakeGame(__BaseELEMENT* Parent, std::string Name, int x, int y, int w, int h) : __BaseELEMENT(Parent, Name), Geometry(x,y,w,h)
{
    init_pair(1,  COLOR_BLUE,     COLOR_GREEN);
    init_pair(2,  COLOR_WHITE,   COLOR_CYAN);
    init_pair(3,  COLOR_YELLOW,  COLOR_MAGENTA);
    init_pair(4,  COLOR_BLUE,    COLOR_CYAN);
    init_pair(5,  COLOR_BLUE, COLOR_MAGENTA);
    init_pair(6,  COLOR_YELLOW,    COLOR_RED);
    init_pair(7,  COLOR_WHITE,    COLOR_BLACK);
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

    int CentralHorizontalWallLength=(w/2-2)-5*2;
    int CentralHorizontalWallWidth = int(h%2==0) + 1;
    for (int i=0; i<CentralHorizontalWallWidth; i++){
        for (int j=0; j<CentralHorizontalWallLength; j++){
            board[(h-2-CentralHorizontalWallWidth)/2+1+i][6+j]="\xc5\xc5";
        }
    }
    ExceptableKeys={KEY_LEFT, KEY_RIGHT, 'a', 'd'};
    int yFirstSnake=(h-2-CentralHorizontalWallWidth)/2+1+2;
    Snake1=new Snake({3, yFirstSnake}, {{3,yFirstSnake-1}, {3, yFirstSnake-2}, {3, yFirstSnake-3}});
    Snake1->Movement=Move_vector::Down;
    board[Snake1->Head.second][Snake1->Head.first]="<>";
    for (int i=0; i<Snake1->Body.size(); i++){
        std::pair<int, int> t=Snake1->Body[i];
        board[t.second][t.first]="\xb1\xb1";
    }
    Snake2=new Snake({w/2-4, yFirstSnake-3}, {{w/2-4,yFirstSnake-2}, {w/2-4, yFirstSnake-1}, {w/2-4, yFirstSnake}});
    Snake2->Movement=Move_vector::Up;
    board[Snake2->Head.second][Snake2->Head.first]="<>";
    for (int i=0; i<Snake2->Body.size(); i++){
        std::pair<int, int> t=Snake2->Body[i];
        board[t.second][t.first]="\xb1\xb1";
    }
    CommandList["EndInitGame"]=&ELEM_SnakeGame::EndInitGame;
    AddElem(new ELEM_InfoBox_SNAKE(this, "info_SNAKE", x+(w-25)/2, y+(h-6)/2, 25, 6, "Player1 W/A/S/D ..\nPlayer2 Arrows ..", "Waiting"));
    this->GenerateApple();
    /*for (int i=0; i<h; i++){
        for (int j=0; j<w/2; j++){
            std::cerr << board[i][j];
        }
        std::cerr << "\n";
    }*/
    UpNeedKeyboard();
}

void ELEM_SnakeGame::MessageFunc(std::string FuncName, std::string Argv){
    //std::cout << "\n(Message)"+FuncName;
    if (CommandList.count(FuncName)){
        (this->*CommandList[FuncName])(Argv);
    }
}

void ELEM_SnakeGame::move(std::string Argv){
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

void ELEM_SnakeGame::Show(){
    Visible=true;
}

void ELEM_SnakeGame::Hide(){
    Visible=false;
}

void ELEM_SnakeGame::EndInitGame(std::string Argv){
    CommandList["NextStep"]=&ELEM_SnakeGame::NextStep;
}

void ELEM_SnakeGame::NextStep(std::string Argv){
    Snake temp1(Snake1->Head, Snake1->Body);
    Snake1->MoveSnake();
    bool FirstLoose=false, SecondLoose=false;
    if (board[Snake1->Head.second][Snake1->Head.first]=="@@"){
        Snake1->Body.push_back(temp1.Body.back());
        board[temp1.Head.second][temp1.Head.first]="\xb1\xb1";
        board[Snake1->Head.second][Snake1->Head.first]="<>";
        this->GenerateApple();
        Parent->DoCommand("InfoBox_Score1", "ChangeText", "+1");
    }else if (board[Snake1->Head.second][Snake1->Head.first]!="  "){
        //Parent->DoCommand("InfoBox_Score1", "ChangeText", "Loose");
        CommandList.erase("NextStep");
        FirstLoose=true;//lose
    }else{
        board[temp1.Body.back().second][temp1.Body.back().first]="  ";
        board[temp1.Head.second][temp1.Head.first]="\xb1\xb1";
        board[Snake1->Head.second][Snake1->Head.first]="<>";
    }
    switch (Snake1->Movement) {
        case Move_vector::Up:{
            ExceptableKeys[0]='a';
            ExceptableKeys[1]='d';
            break;
        }
        case Move_vector::Down:{
            ExceptableKeys[0]='a';
            ExceptableKeys[1]='d';
            break;
        }
        case Move_vector::Left:{
            ExceptableKeys[0]='w';
            ExceptableKeys[1]='s';
            break;
        }
        case Move_vector::Right:{
            ExceptableKeys[0]='w';
            ExceptableKeys[1]='s';
            break;
        }
    }

    UpNeedToPaint();
    Snake temp2=*Snake2;
    Snake2->MoveSnake();
    if (board[Snake2->Head.second][Snake2->Head.first]=="@@"){
        Snake2->Body.push_back(temp2.Body.back());
        board[temp2.Head.second][temp2.Head.first]="\xb1\xb1";
        board[Snake2->Head.second][Snake2->Head.first]="<>";
        this->GenerateApple();
        Parent->DoCommand("InfoBox_Score2", "ChangeText", "+1");
    }else if (board[Snake2->Head.second][Snake2->Head.first]!="  "){
        //Parent->DoCommand("InfoBox_Score2", "ChangeText", "Loose");
        CommandList.erase("NextStep");
        SecondLoose=true;//lose
    }else{
        board[temp2.Body.back().second][temp2.Body.back().first]="  ";
        board[temp2.Head.second][temp2.Head.first]="\xb1\xb1";
        board[Snake2->Head.second][Snake2->Head.first]="<>";
    }
    switch (Snake2->Movement) {
        case Move_vector::Up:{
            ExceptableKeys[3]=KEY_LEFT;
            ExceptableKeys[2]=KEY_RIGHT;
            break;
        }
        case Move_vector::Down:{
            ExceptableKeys[3]=KEY_LEFT;
            ExceptableKeys[2]=KEY_RIGHT;
            break;
        }
        case Move_vector::Left:{

            ExceptableKeys[3]=KEY_UP;
            ExceptableKeys[2]=KEY_DOWN;
            break;
        }
        case Move_vector::Right:{
            ExceptableKeys[3]=KEY_UP;
            ExceptableKeys[2]=KEY_DOWN;
            break;
        }
    }
    if (FirstLoose && SecondLoose){
        Parent->DoCommand("Result", "Show", "");
        Parent->DoCommand("Result", "ChangeText", "The game ended in a draw");
    }else if (FirstLoose){
        Parent->DoCommand("Result", "Show", "");
        Parent->DoCommand("Result", "ChangeText", "Player2 win!");
    }else if (SecondLoose){
        Parent->DoCommand("Result", "Show", "");
        Parent->DoCommand("Result", "ChangeText", "Player1 win!");
    }
    UpNeedKeyboard();
    UpNeedToPaint();
}

void ELEM_SnakeGame::Paint(){
    for (int i=0; i<board.size(); i++){
        for (int j=0; j<board[i].size(); j++){
            attron(COLOR_PAIR(7));
            if (board[i][j]=="@@")
                attron(COLOR_PAIR(Apple));
            else if (board[i][j]=="\xc5\xc5"){
                attron(COLOR_PAIR(Wall));
                mvaddch(y+i, x+j*2, ACS_PLUS);
                mvaddch(y+i, x+j*2+1, ACS_PLUS);
            }
            else if (board[i][j]=="  ")
                attron(A_NORMAL);
            else if (Snake1->Head.second==i && Snake1->Head.first==j)
                attron(COLOR_PAIR(SnakeHead1));
            else if (Snake2->Head.second==i && Snake2->Head.first==j)
                attron(COLOR_PAIR(SnakeHead2));
            else{
                bool x=false;
                for (int q=0; q<Snake1->Body.size(); q++){
                    if (Snake1->Body[q].second==i && Snake1->Body[q].first==j){
                        x=true;
                        attron(COLOR_PAIR(SnakeBody1));
                        break;
                    }
                }
                if (!x){
                    for (int q=0; q<Snake2->Body.size(); q++){
                        if (Snake2->Body[q].second==i && Snake2->Body[q].first==j){
                            attron(COLOR_PAIR(SnakeBody2));
                            break;
                        }
                    }
                }
            }

            mvprintw(y+i, x+j*2, board[i][j].c_str());
        }
    }
    attron(COLOR_PAIR(7));
}

void ELEM_SnakeGame::KeyEnter(int Key){
    switch (Key) {
    case KEY_UP:{
        Snake2->Movement=Move_vector::Up;
        break;
    }
    case KEY_LEFT:{
        Snake2->Movement=Move_vector::Left;
        break;
    }
    case KEY_DOWN:{
        Snake2->Movement=Move_vector::Down;
        break;
    }
    case KEY_RIGHT:{
        Snake2->Movement=Move_vector::Right;
        break;
    }
    case 'w':{
        Snake1->Movement=Move_vector::Up;
        break;
    }
    case 'a':{
        Snake1->Movement=Move_vector::Left;
        break;
    }
    case 's':{
        Snake1->Movement=Move_vector::Down;
        break;
    }
    case 'd':{
        Snake1->Movement=Move_vector::Right;
        break;
    }
    }
    UpNeedKeyboard();
}
