/* 
 * 2021-09-30 Minicheeta test version
 * Workspace
 */

#include "workplace.hpp"



// 1 drgree = about 7 of encoder value
/*  --test commands
unsigned char motorTest1Cmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x00, 0x00, 0x08, 0x1a};
unsigned char motorTest2Cmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x00, 0x00, 0x08, 0x09};
unsigned char motorTest3Cmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x00, 0x00, 0x07, 0xE4};
unsigned char PosG90Cmd[8] = {0x82, 0x75, 0x7f, 0xf0, 0x08, 0x33, 0x37, 0xff};
unsigned char PosG180Cmd[8] = {0x84, 0xeb, 0x7f, 0xf0, 0x08, 0x33, 0x37, 0xff};
unsigned char PosG360Cmd[8] = {0x89, 0xd6, 0x7f, 0xf0, 0x08, 0x33, 0x37, 0xff};
unsigned char PosG0Cmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x08, 0x33, 0x37, 0xff};
*/

unsigned char motorStartCmd[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfC};
unsigned char motorEndCmd[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfD};
unsigned char motorSetCmd[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfE};
unsigned char TorqueGCmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x00, 0x00, 0x07, 0xff};
unsigned char PosGTCmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x0a, 0x33, 0x37, 0xff};
unsigned char PosGCmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x0a, 0x33, 0x37, 0xff};
int mAllDegreeVal[MAXIMUM_MOTOR_NUMBER] = {0,}; //  composed degree as integer


posSt RReceivedPosData;
posSt OReceivedPosData;
posSt EPosData = {0,};
posSt OEPosData = {0,};

Thread canRecieveThread;
Thread canSendThread;
Thread uartRecieveThread;
Thread uartSendThread;

int p_gain = 1;
int D_gain = 100;

signed short posVal = 0;

//--------Main init
void mainInit(){
    hCommon::getInstance().setBoardValue();
}


//--------Communication init
void communicationInit(){
    printf("canInit()\n");
    //hCommunication::getInstance().canInit();  //can init
    hCommunication::getInstance().uartInit();   //uart init
}


//--------Motor init
void motorInit(int _MCanNum, int _SCanNum){
    printf("Motor Init CAN ID : %d, Motor ID : %d\n", _MCanNum, _SCanNum);
    hCommunication::getInstance().canSendData(_MCanNum,_SCanNum,motorEndCmd,8); //Turn off a motor
    ThisThread::sleep_for(10ms);
    hCommunication::getInstance().canSendData(_MCanNum,_SCanNum,motorStartCmd,8); //Turn on a motor
    ThisThread::sleep_for(10ms);
    hCommunication::getInstance().canSendData(_MCanNum,_SCanNum,motorSetCmd,8); // activate a motor
    ThisThread::sleep_for(10ms);
}


//--------Motor Torque Test Function
void testMotorCntTorque(){
    OReceivedPosData = RReceivedPosData;
    OEPosData = EPosData;
//    Receivedmsg = hCommunication::getInstance().canRecieveData();
    RReceivedPosData = hCommunication::getInstance().canRecievePosData();
    EPosData.data1 = TorqueGCmd[0]-RReceivedPosData.data1;
    EPosData.data2 = TorqueGCmd[1]-RReceivedPosData.data2;
    if(EPosData.data1 < 0x7f){
        TorqueGCmd[6] = 0x08;
        TorqueGCmd[7] = p_gain*EPosData.data2+D_gain*(OEPosData.data2-EPosData.data2);
    }
    else if(EPosData.data1 >= 0x7f){
        TorqueGCmd[6] = 0x07;
        TorqueGCmd[7] = 0xff-(p_gain*EPosData.data2+D_gain*(OEPosData.data2-EPosData.data2));
    }

    if(TorqueGCmd[6] >= 0x08){
        if(TorqueGCmd[7] > 0x1a){
            TorqueGCmd[7] = 0x1a;
        }
    }
    else if(TorqueGCmd[6] <= 0x07){
        if(TorqueGCmd[7] < 0xe4){
            TorqueGCmd[7] = 0xe4;
        }
    }

    if(TorqueGCmd[7]>0x1a&&TorqueGCmd[7]<0xe4){
        TorqueGCmd[6] = 0x07;
        TorqueGCmd[7] = 0xff;
    }
    hCommunication::getInstance().canSendData(1,2,TorqueGCmd,8);
}


//--------Motor Position Mode Function
void MotorCntPos(){
    for(int i = 0; i < MAXIMUM_MOTOR_NUMBER ; i++){
        hCommon::getInstance().convertingtDegree(mAllDegreeVal[i],PosGCmd);
        hCommunication::getInstance().canSendData(1,6,PosGCmd);
        ThisThread::sleep_for(1ms);
    }
}


//--------Motor Position TEST Mode Function
int testCnt = 0;
void testMotorCntPos(){
    if(testCnt%4==0){
        hCommon::getInstance().convertingtDegree(mAllDegreeVal[0],PosGTCmd);
        testCnt++;
    }
    else if(testCnt%4==1){
        hCommon::getInstance().convertingtDegree(mAllDegreeVal[1],PosGTCmd);
        testCnt++;
    }
    else if(testCnt%4==2){
        hCommon::getInstance().convertingtDegree(mAllDegreeVal[2],PosGTCmd);
        testCnt++;
    }
    else{
        hCommon::getInstance().convertingtDegree(mAllDegreeVal[3],PosGTCmd);
        testCnt=0;
    }
    hCommunication::getInstance().canSendData(1,2,PosGTCmd,8);
}


//--------Thread : Recieve deree data from common class
void recieveData(){
    printf("recieveData()_start\n");
    while(1){
        hCommon::getInstance().getAllDegreeData(mAllDegreeVal);
        ThisThread::sleep_for(10ms);
    }
    printf("recieveData()_end\n");
}


//--------Thread : Send a packet to a motor
void canSendMotorData(){
    printf("canSendMotorData()_start\n");
    while(1){
        MotorCntPos();
        ThisThread::sleep_for(10ms);
    }
    printf("canSendMotorData()_end\n");
}


//--------Thread : Recieve encoder data from a motor -- need to modify ( not use for position test )
void canRecieveMotorData(){
    printf("canRecieveMotorData()_start\n");
    while(1){
        hCommunication::getInstance().getPosInt(posVal);
        ThisThread::sleep_for(10ms);
        //printf("%x\n",PosGCmd[1]);
    }
    printf("canRecieveMotorData()_end\n");
}


//--------Main Loop involves all threads start
int mainLoop(){
    CANMessage Receivedmsg;
    char ldata22[MAXIMUM_BUFFER_SIZE] = {0,};
    
    canRecieveThread.start(canRecieveMotorData);
    canSendThread.start(canSendMotorData);
    uartRecieveThread.start(recieveData);

    while(1){
        hCommunication::getInstance().receiveUartData();

        if(0){
            break;
        }
        ThisThread::sleep_for(10ms);
    }
    return -1;
}

