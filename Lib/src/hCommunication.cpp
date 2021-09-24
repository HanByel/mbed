#include "hCommunication.hpp"

hCommunication* hCommunication::mp_Instance=NULL;

#ifdef TARGET_RZ_A1H
    CAN mcan1(P11_12, P11_13, 1000000);
    CAN mcan2(P6_4, P6_5, 1000000);
    static BufferedSerial serial_port(P2_14, P2_15);
#elif  TARGET_NUCLEO_F767ZI
    CAN mcan1(PD_0, PD_1, 1000000);
    CAN mcan2(PB_5, PB_6, 1000000);
    CAN mcan3(PB_3, PB_4, 1000000);
    static BufferedSerial serial_port(PB_9, PB_8);
#else
#endif

hCommunication::hCommunication(){
}

hCommunication::~hCommunication(){


}


hCommunication& hCommunication::getInstance(void) {
    if (mp_Instance == NULL) {
        mp_Instance = new hCommunication();
    }
    return *mp_Instance;
}

void hCommunication::canInit(){


}

void hCommunication::canSendData(unsigned int _canId, unsigned int _id, const unsigned char *_data, unsigned char _len, CANType _type, CANFormat _format){
    CANMessage lmsg=CANMessage(_id, _data, _len, _type, _format);
    switch(_canId){
        case 1:{
            mcan1.write(lmsg);
            break;
        }
        case 2:{
            mcan2.write(lmsg);
            break;
        }
        #ifdef TARGET_NUCLEO_F767ZI
        case 3:{
            mcan3.write(lmsg);
            break;
        }
        #endif
        default:{
        printf("canSendData() CAN_Select_Error\n");
        break;
        }
    }
}


CANMessage hCommunication::canRecieveData(){
    CANMessage msg;
    if (mcan1.read(msg)) {
        printf("Message received: %x, %x, %x, %x, %x, %x, %x, %x\n", msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
    }
    
    return msg;
}


posSt hCommunication::canRecievePosData(){
    posSt posMSG;
    CANMessage msg;
    if (mcan1.read(msg)) {
//        printf("Message received: %x, %x, %x, %x, %x, %x, %x, %x\n", msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
    posMSG.data1 = msg.data[1];
    posMSG.data2 = msg.data[2];
    }
    
    return posMSG;
}


void hCommunication::getPosInt(signed short &_IntVal){
    posSt posMSG;
    CANMessage msg;
    if (mcan1.read(msg)) {
//        printf("Message received: %x, %x, %x, %x, %x, %x, %x, %x\n", msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
    posMSG.data1 = msg.data[1];
    posMSG.data2 = msg.data[2];
    }
    _IntVal = posMSG.data1 << 8 | posMSG.data2;
    _IntVal -=32767;
    //printf("Message received: %d %d %d\n", _IntVal, msg.data[1], msg.data[2]);
}



void hCommunication::uartInit(){
    serial_port.set_baud(115200);
    serial_port.set_format(8, BufferedSerial::None, 1);
}

void hCommunication::sendUartData(const void *_buf, size_t _length){
    serial_port.write(_buf, _length);
}

void hCommunication::getUartData(char *arr){
    if(uint32_t num = serial_port.read(mBuff, sizeof(mBuff))){
        for(int i = 0; i < MAXIMUM_BUFFER_SIZE ; i++){
            arr[i] = mBuff[i];
        }
    }
}
void hCommunication::setUartData(){

}