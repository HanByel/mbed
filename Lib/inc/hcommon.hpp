#ifndef HCOMMON_HPP
#define HCOMMON_HPP


#include <stdio.h>
#include <string>
#include <list>
#include "mbed.h"


#define oneDegree 45
#define MAXIMUM_BUFFER_SIZE 50
#define MAXIMUM_MOTOR_NUMBER 12

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
    void convertingtDegree(float _degree, unsigned char *_dataVal);

public:
    void getAllDegreeData(int *arr);
    void setUartData(char *arr);
    void convUart2DegreeData();

private:
    void boardList();
    
private:
    char mUartBuff[MAXIMUM_BUFFER_SIZE]={0,};
    signed int mDegreeBuff[MAXIMUM_MOTOR_NUMBER]={0,};
    char mZBuff[MAXIMUM_BUFFER_SIZE]={0,};
    int Uartcnt = 0;

};

#endif //COMMON_HPP