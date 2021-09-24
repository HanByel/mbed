#ifndef HCOMMON_HPP
#define HCOMMON_HPP


#include <stdio.h>
#include <string>
#include <list>
#include "mbed.h"


struct posSt{
    unsigned char data1;
    unsigned char data2;
};


class hCommon{

public:
    hCommon();
    virtual ~hCommon();

    static hCommon &getInstance();
    static hCommon *mp_Instance;

private:
    int mBoardValue;
    list<string> mBoardList;

public:
    void setBoardValue();
    void getBoardValue(int &_boardVal);
    void hTimerFunc(int _timer, float _time, void *_func());

private:
    void boardList();

};

#endif //COMMON_HPP