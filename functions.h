#include "main.h"
#include "lemlib/api.hpp"
#include "init.h"

//Mutexes
pros::Mutex chassisMutex;
pros::Mutex backWingMutex;
pros::Mutex FRMutex;
pros::Mutex FLMutex;
pros::Mutex intakeMutex;

void PTO_Switch(){
	if (PTO_Engaged==true){//Turn off PTO
		pros::MotorGroup left_side_motors({left_m1_front, left_m2_mid, left_m3_back});
		pros::MotorGroup right_side_motors({right_m1_front, right_m2_mid, right_m3_back});
	}
	else{
		pros::MotorGroup left_side_motors({left_m1_front, left_m2_mid, left_m3_back, left_half_motor});
		pros::MotorGroup right_side_motors({right_m1_front, right_m2_mid, right_m3_back, right_half_motor});
	}
	PTO_Engaged=!PTO_Engaged;
	PTO_Piston.set_value(PTO_Engaged);
}

void toggle_BW(){
	BW = !BW;
	back_wing_r.set_value(BW);
	back_wing_l.set_value(BW);
}

bool toggle_FR(){
	FR = !FR;
	front_wing_r.set_value(FR);
	return FR;
}

bool toggle_FL(){
	FL = !FL;
	front_wing_l.set_value(FL);
	return FL;
}

void toggle_front_wings(){
	FL=!FL;
	FR=!FR;
	front_wing_l.set_value(FL);
	front_wing_r.set_value(FR);
}

void cont_front(bool ext){
	FL = FR = ext;
	front_wing_l.set_value(FL);
	front_wing_r.set_value(FR);
}

void Descore(int delay, int wait){
	pros::delay(delay);
	toggle_BW();
	pros::delay(wait);
	toggle_BW();
}

void intake_run(int power, int wait){
	intake.move(power);
	pros::delay(wait);
	intake.brake();
}

void intake_run(int power, int delay, int wait){
	pros::delay(delay);
	intake.move(power);
	pros::delay(wait);
	intake.brake();
}

//Joystick Curve
float joycurve(float val){
	if (abs(val)<7){
		return 0;
	}
	return (val > 0) ? 1 : ((val < 0) ? -1 : 0) *127/(1+exp(-.13889*(abs(val)-50)));
}

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

//Blocker
void blockToggle(){
	BL=!BL;
	blocker1.set_value(BL);
	blocker2.set_value(BL);
}

//Hang
void hangToggle(){
	HA=!HA;
	hang1.set_value(HA);
	hang2.set_value(HA);
}

//Brake Toggle
void brakeToggle(){
	brakeHold=!brakeHold;
}

//Brake Task
bool &BrakePtr=brakeHold;
void brakeRun(){
	while(true){
		if(&BrakePtr){
			left_side_motors.set_brake_modes(MOTOR_BRAKE_HOLD);
			right_side_motors.set_brake_modes(MOTOR_BRAKE_HOLD);
		}
		else{
			left_side_motors.set_brake_modes(MOTOR_BRAKE_COAST);
			right_side_motors.set_brake_modes(MOTOR_BRAKE_COAST);
		}
	}
}

bool moveBrake = false;
void moveBrakeToggle(){
	moveBrake=!moveBrake;
}

bool &moveBrakeptr = moveBrake;
void moveBraker(){
	while(true){
		while(abs(master.get_analog(ANALOG_LEFT_Y))<7){
		if(&moveBrakeptr){
			if(chassis.getPose().y>=0){
				if(chassis.getPose().x>=0){
					chassis.moveToPose(58, 45, 245, 4000);
				}
				else{chassis.moveToPose(-58, 45, 125, 4000);}
			}
			else{
				if(chassis.getPose().x>=0){
					chassis.moveToPose(58, -45, 335, 4000);
				}
				else{chassis.moveToPose(-58, -45, 25, 4000);}
			}
		}
		}
	}
}