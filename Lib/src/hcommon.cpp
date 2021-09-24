#include "hcommon.hpp"

hCommon* hCommon::mp_Instance=NULL;

hCommon::hCommon(){


}

hCommon::~hCommon(){
    

}



hCommon& hCommon::getInstance(void) {
    if (mp_Instance == NULL) {
        mp_Instance = new hCommon();
    }
    return *mp_Instance;
}



void hCommon::hTimerFunc(int _timer, float _time, void *_func()){


}


void hCommon::setBoardValue(){
    int lCnt=0;
    list<string>::iterator l_it=mBoardList.begin();
    string st1 = MBED_STRINGIFY(TARGET_NAME);
    for(l_it = mBoardList.begin(); l_it != mBoardList.end(); l_it++){
        if(st1.compare(*l_it)==0){
            break;
        }
        lCnt++;
    }

    mBoardValue=lCnt;
}

void hCommon::getBoardValue(int &_boardVal){
    _boardVal=mBoardValue;
}


void hCommon::boardList(){
//    mBoardList.push_back("RZ_A1H");         //0
//    mBoardList.push_back("NUCLEOF_767ZI");  //1
}