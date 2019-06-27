#include "systemManager.h"

Motor::Motor(){
return;	
}

void Motor::startMotor(){
	motorOn();
}

void Motor::offMotor(){
	motorOff();
}

void Motor::drive(){
	writeAnalog(0, speed);
}

void Motor::setZero(){
	speed = 2048;
}
void Motor::setSpeed(int v){
	speed = v;
	return;
}
int Motor:: getSpeed(){
	return speed;
}
