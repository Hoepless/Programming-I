#include <eleminterface.h>
#include <iostream>


ELEM_InfoBox::ELEM_InfoBox(__BaseELEMENT* ParentWindow, std::string Name, unsigned int x, unsigned int y, unsigned int w, unsigned int h, std::string message, std::string caption):__BaseELEMENT(ParentWindow, Name), Geometry(x, y, w, h)
{
    this->Text=ELEM_InfoBox::GetTextInBoarder(Size_t(w, h), message);
    this->CaptionOfInfo=caption;
    findMaximumLength();
    CalculateLines();
    this->CommandList["ChangeText"]=&ELEM_InfoBox::ChangeText;
    this->CommandList["Move"]=&ELEM_InfoBox::move;
    this->CommandList["Resize"]=&ELEM_InfoBox::resize;
    this->CommandList["Hide"]=&ELEM_InfoBox::Hide;
    this->CommandList["Show"]=&ELEM_InfoBox::Show;
    if (ParentWindow!=nullptr){
        UpNeedToPaint();
    }
}

void ELEM_InfoBox::MessageFunc(std::string FuncName, std::string Argv){
    //std::cout << "\n(Message)"+FuncName;
    if (CommandList.count(FuncName)){
        (this->*CommandList[FuncName])(Argv);
    }
}

void ELEM_InfoBox::Hide(std::string Argv){
    Visible=false;
    UpNeedToPaint();
    UpNeedKeyboard();
}

void ELEM_InfoBox::Show(std::string Argv){
    Visible=true;
    UpNeedToPaint();
    UpNeedKeyboard();
}

void ELEM_InfoBox::move(std::string Argv){
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

void ELEM_InfoBox::resize(std::string Argv){
    int i;
    for (i=0; i<Argv.length(); i++){
        if (Argv[i]==',') break;
    }
    int w=str_to_uint(Argv.substr(0, i));
    int h=str_to_uint(Argv.substr(i+1, Argv.length()-i));
    this->Size=Size_t(w, h);
    CalculateLines();
    GetTextInBoarder(Size, GetBigLine(Text));
    UpNeedToPaint();
}

void ELEM_InfoBox::ChangeText(std::string Argv){
    if (Argv.length()>0 && Argv[0]=='+'){
        unsigned int CurrentText=str_to_uint(GetBigLine(Text));
        unsigned int Add=str_to_uint(Argv.substr(1, Argv.length()-1));
        Argv=uint_to_str(CurrentText+Add);
    }else if (Argv.length()>0 && Argv[0]=='-'){
        unsigned int CurrentText=str_to_uint(GetBigLine(Text));
        unsigned int Add=str_to_uint(Argv.substr(1, Argv.length()-1));
        Argv=uint_to_str(CurrentText-Add);
    }

    //std::cout << "\n\n(Change Text)"+Argv;
    Text=GetTextInBoarder(Size, Argv);
    CalculateLines();
    if (Parent!=nullptr)
        UpNeedToPaint();
    //else std::cout << " Parent == nullptr";
}

void ELEM_InfoBox::CalculateLines(){
    EmptyLine="";
    for (int i=1; i<Size.Width-1; i++){
        EmptyLine+=" ";
    }
}

std::vector<std::string> ELEM_InfoBox::GetTextInBoarder(Size_t sz, std::string Message){
    std::vector<std::string> answer;
    int CountAvalibleLines=1;
    std::string NonFuncText(Message);
    int maxLenOfText=strlen(Message.c_str());
    bool ispok=false;
    for (int pok=0; pok<NonFuncText.length(); pok++){
        if (NonFuncText[pok]=='\n'){
            ispok=true;
        }

    }
    while (ispok || (NonFuncText.length()>sz.Width && CountAvalibleLines<sz.Height)){
        CountAvalibleLines++;
        int temp=CountAvalibleLines;
        maxLenOfText=0;
        int pos=std::min(sz.Width-1, (unsigned int)NonFuncText.length());
        int i;
        for (i=0; i<pos; i++){
            if (NonFuncText[i]=='\n'){
                break;
            }

        }
        if (ispok){
            answer.push_back(NonFuncText.substr(0, i));
            maxLenOfText=std::max(maxLenOfText, int(answer[answer.size()-1].length()));
            NonFuncText=NonFuncText.substr(i+1, NonFuncText.length()-i);
            ispok=false;
        }else{
            if (pos==NonFuncText.length() || NonFuncText[pos-1]==' ' || NonFuncText[pos-1]=='\t' || NonFuncText[pos-1]=='\n'){
                answer.push_back(NonFuncText.substr(0, pos));
                maxLenOfText=std::max(maxLenOfText, int(answer[answer.size()-1].length()));
                NonFuncText=NonFuncText.substr(pos+1, NonFuncText.length()-pos);
            }else{
                int leftInd=pos-1, rightInd=pos;
                //find first separator left from pos
                for (; leftInd>=0; leftInd--){
                    if (NonFuncText[leftInd]==' ' || NonFuncText[leftInd]=='\t'){
                        break;
                    }
                }
                if (leftInd==-1){
                    answer.push_back(NonFuncText.substr(0, rightInd));
                    maxLenOfText=std::max(maxLenOfText, int(answer[answer.size()-1].length()));
                    NonFuncText=NonFuncText.substr(rightInd, NonFuncText.length()-rightInd);
                }else{
                    answer.push_back(NonFuncText.substr(0, leftInd));
                    maxLenOfText=std::max(maxLenOfText, int(answer[answer.size()-1].length()));
                    NonFuncText=NonFuncText.substr(leftInd+1, NonFuncText.length()-leftInd);
                    //std::cout << NonFuncText << "\n";
                }
            }
        }
        for (int pok=0; pok<NonFuncText.length(); pok++){
            if (NonFuncText[pok]=='\n'){
                ispok=true;
            }

        }
    }
    if (NonFuncText.length()){
        answer.push_back(NonFuncText);
    }
    for (int i=0; i<answer.size(); i++){
        //std::cout << answer[i] << "\n";
    }
    return answer;
}

void ELEM_InfoBox::findMaximumLength(){
    for (std::string now : Text){
        if (now.length()>maxLength){
            maxLength=now.length();
        }
    }
    if (maxLength<CaptionOfInfo.length()){
        maxLength=CaptionOfInfo.length();
    }
}

void ELEM_InfoBox::Paint() {
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
        SomeString+=Text[i];
        for (int j=SomeString.length(); j<Size.Width-1; j++){
            SomeString+=" ";
        }

        mvprintw(y+1+(Size.Height-Text.size())/2+i, x+1, SomeString.c_str());
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
}
