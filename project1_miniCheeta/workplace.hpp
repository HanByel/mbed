#ifndef MINICHEETAWORKPLACE_HPP
#define MINICHEETAWORKPLACE_HPP

#include "hCommunication.hpp"
#include "hcommon.hpp"
#include <string.h>
#define MAXIMUM_MOTOR_NUMBER 12

int mainLoop();
void mainInit();
void communicationInit();
void motorInit(int _Cid=1, int _Mid=1);
void testMotorCntTorque();
void MotorCntPos();
void testMotorCntPos();




#endif //MINICHEETAWORKPLACE_HPP