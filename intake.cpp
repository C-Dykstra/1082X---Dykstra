#include "main.cpp"

//Intake Motor
pros::Motor intake(5, true);

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

void intakeInit(){
    
}