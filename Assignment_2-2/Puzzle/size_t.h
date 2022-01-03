#ifndef SIZE_T_H
#define SIZE_T_H

struct Size_t{
    Size_t (unsigned int width, unsigned int height){
        this->Width=width;
        this->Height=height;
    }
    unsigned int Width=0, Height=0;
};

#endif // SIZE_T_H
