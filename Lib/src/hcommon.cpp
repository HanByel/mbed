/* 
 * 2021-10-15 Minicheeta
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


/** 2021-10-15  HanByel
 * Set Board Type
 */
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


/** 2021-10-15  HanByel
 * Get Board Type
 */
void hCommon::getBoardValue(int &_boardVal){
    _boardVal=mBoardValue;
}


void hCommon::boardList(){
    
    
}


/** 2021-10-15  HanByel
 * convert from float degree to character data
 * param float _degree : Degree data
 * param unsigned char *_dataVal : 2 bit position data (array) {pos1,pos2}
 */
void hCommon::convertingtDegree(float _degree, unsigned char *_dataVal){
	_degree *= 7;
	_degree += 32767;
	_dataVal[0] = (short)_degree >> 8;
	_dataVal[1] = (char)_degree;
}


/** 2021-10-15  HanByel
 * get degree function
 * Can get ALL degree data using this function
 * param int *arr : array degree data
 */
void hCommon::getAllDegreeData(int *arr){
    for(int i = 0 ; i < MAXIMUM_MOTOR_NUMBER ; i++)         arr[i] = mDegreeBuff[i];
}


/** 2021-10-15  HanByel
 * set uart data to this class
 * Set degree data with uart interface
 */
void hCommon::setUartData(char *arr){
    for(int i = 0 ; i < MAXIMUM_BUFFER_SIZE ; i++)   mUartBuff[i]=arr[i];
    hCommon::getInstance().convUart2DegreeData();
}


//--------
/** 2021-10-15  HanByel
 * convert UART data, gained with UART, to Degree data
 * Set degree data from uart interface
 * converting uart data to degreedata as integer 
 * less than the number of three figures
 */
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
