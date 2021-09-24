#include "workplace.hpp"



unsigned char motorStartCmd[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfC};
unsigned char motorEndCmd[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfD};
unsigned char motorSetCmd[8] = {0x7f, 0xff, 0x81, 0xf0, 0x00, 0x00, 0x07, 0xff};
unsigned char motorTest1Cmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x00, 0x00, 0x08, 0x1a};
unsigned char motorTest2Cmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x00, 0x00, 0x08, 0x09};
unsigned char motorTest3Cmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x00, 0x00, 0x07, 0xE4};
unsigned char TorqueGCmd[8] = {0x7f, 0xff, 0x7f, 0xf0, 0x00, 0x00, 0x07, 0xff};
unsigned char PosGCmd[8] = {0x27, 0x10, 0x80, 0xf1, 0xeb, 0x0a, 0x08, 0x2a};

posSt RReceivedPosData;
posSt OReceivedPosData;
posSt EPosData = {0,};
posSt OEPosData = {0,};


int p_gain = 1;
int D_gain = 1;

signed short posVal = 0;

void mainInit(){
    printf("mainInit()\n");
    hCommon::getInstance().setBoardValue();
}

void canInit(){
    printf("canInit()\n");
    hCommunication::getInstance().canInit();
}

void motorInit(){
    printf("motorInit()\n");
    hCommunication::getInstance().canSendData(1,2,motorEndCmd,8);
    ThisThread::sleep_for(10ms);
    hCommunication::getInstance().canSendData(1,2,motorStartCmd,8);
    hCommunication::getInstance().canSendData(1,2,motorSetCmd,8);
}

void MotorCntTorque(){

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
    
    printf("%d, %d\n",EPosData.data1, EPosData.data2);
    hCommunication::getInstance().canSendData(1,2,TorqueGCmd,8);
}

void MotorCntPos(){

    PosGCmd[1]++;
    hCommunication::getInstance().canSendData(1,2,PosGCmd,8);

    hCommunication::getInstance().getPosInt(posVal);

    printf("%d, %d\n",EPosData.data1, EPosData.data2);
    //printf("Rdata %x,  %x  \n",RReceivedPosData.data1, RReceivedPosData.data2);
 
}


int mainLoop(){
    CANMessage Receivedmsg;
    char cntTorque = 10;
    while(1){
        MotorCntTorque();

        if(0){
            break;
        }
        ThisThread::sleep_for(100ms);
    }

    return -1;
}

