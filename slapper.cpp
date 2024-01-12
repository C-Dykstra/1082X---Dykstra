#include "main.cpp"

//Slapper
pros::MotorGroup Slapper({right_half_motor, left_half_motor});
bool slapRun = false;

//Slapper Toggle/Run
bool &slaptog = slapRun;
void slapperRun(){
	while(true){
		Slapper = 127*((int)&slaptog);
	}
}
void slapperToggle(){
	slapRun=!slapRun;
}

void slapperInit(){
    
}