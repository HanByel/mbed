/* 
 * 2021-09-30 Minicheeta test version
 * Common class
 * convert data / calculate data
 */

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
    
    
}


//--------convert from float degree to character data[2 bytes]
void hCommon::convertingtDegree(float _degree, unsigned char *_dataVal){
	_degree *= 7;
	_degree += 32767;
	_dataVal[0] = (short)_degree >> 8;
	_dataVal[1] = (char)_degree;
}


//--------get degree
void hCommon::getAllDegreeData(int *arr){
    for(int i = 0 ; i < MAXIMUM_MOTOR_NUMBER ; i++)         arr[i] = mDegreeBuff[i];
}


//--------set uart data to this class
void hCommon::setUartData(char *arr){
    for(int i = 0 ; i < MAXIMUM_BUFFER_SIZE ; i++)   mUartBuff[i]=arr[i];
    hCommon::getInstance().convUart2DegreeData();
}


//--------converting uart data to degreedata as integer // less than the number of three figures
void hCommon::convUart2DegreeData(){
    int convCnt=0;
    int p=0;
    for(int i = 0 ; i < MAXIMUM_MOTOR_NUMBER ; i++){
        mDegreeBuff[i] = 0;
    }
    for(int i = 0 ; i < MAXIMUM_BUFFER_SIZE ; i++){
        if(mUartBuff[i]==47||mUartBuff[i]==32){
            if(mUartBuff[i+1]==0||mUartBuff[i+1]==47||
                mUartBuff[i+1]==32||mUartBuff[i+1] == 13)                p = 0;

            else if(mUartBuff[i+2]==0||mUartBuff[i+2]==47||
                mUartBuff[i+2]==32||mUartBuff[i+2] == 13)                p = 1;

            else if(mUartBuff[i+3]==0||mUartBuff[i+3]==47||
                mUartBuff[i+3]==32||mUartBuff[i+3] == 13)                p = 2;

            else                p = 3;
               

            for(int j = p ; j > 0 ; j--){
                if(mUartBuff[i+j] !=32 && mUartBuff[i+j]!= 0 && mUartBuff[i+j] != 47 && mUartBuff[i+j] != 13){
                    mDegreeBuff[convCnt]+=(mUartBuff[i+j]-48)*(int)pow(10,p-j);
                }
                else{
                    mDegreeBuff[convCnt] += 0 ;
                }
            }
            convCnt++;
        }
        else if(mUartBuff[i]==13){
            
            break;
        }
        else if(mUartBuff[i]==0){

        }
        else{
            
        }
    }
}
