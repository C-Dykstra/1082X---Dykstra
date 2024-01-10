#include "functions.h"
#include "gif-pros/gifclass.hpp"

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}//Gif gif("/usd/spingoat.gif", lv_scr_act());}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	//gif.clean();
	pros::Task intaking(intake_run(127, 0, 600));
	chassis.moveToPoint(10, -58, 1000, 20); // move to the point (10, 0) with a timeout of 1000 ms, and a maximum speed of 50
	intaking.join();
	intaking.remove();
	pros::Task descore_auton(Descore(750, 1000));
	chassis.follow(auton1_txt, 15, 3000, false);//Follow path auton1, lookahead dist of 8 inches, timeout 3000ms, follow backwards
	descore_auton.remove();
	chassis.turnTo(60, -50, 1000);
	intake_run(-127, 0, 350);
	pros::Task intaking2(intake_run(127, 750, 1500));
	chassis.follow(auton2_txt, 15, 3000, true);
	intaking2.join();
	intaking2.remove();
	pros::Task intaking3(intake_run(-127, 650, 600));
	chassis.follow(auton3_txt, 15, 3000, true);
	intaking3.join();
	intaking3.remove();
	pros::Task intaking4(intake_run(127, 500, 1000));
	chassis.follow(auton4_txt, 15, 3000, true);
	toggle_BW();
	chassis.follow(auton5_txt, 15, 3000, true);
	toggle_BW();
	chassis.follow(auton6_txt, 15, 3000, true);
	chassis.follow(auton7_txt, 15, 3000, false);
	chassis.follow(auton8_txt, 15, 3000, true);
	blockToggle();
	intaking4.remove();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	//gif.clean();

	pros::Task SlapTask(slapperRun());
	while (true) {
		chassis.arcade(joycurve(master.get_analog(ANALOG_LEFT_Y)), joycurve(master.get_analog(ANALOG_RIGHT_X)));
		if(master.get_digital_new_press(DIGITAL_L1)&&!PTO_Engaged){//Slapper
			slapperToggle();
		}
		if(master.get_digital_new_press(DIGITAL_LEFT)){//PTO
			PTO_Switch();
		}
		if(master.get_digital(DIGITAL_R1)){//Intake
			pros::Task intakeTask(intake.move(127));
		}
		else if(master.get_digital(DIGITAL_R2)){//Outtake
			pros::Task intakeTask(intake.move(-127));
		}
		else{/*intakeTask.remove();*/intake.brake();}
		if(master.get_digital(DIGITAL_DOWN)){//Blocker
			blockToggle();
		}
		if(master.get_digital(DIGITAL_B)){//Back wings
			toggle_BW();
		}
		if(master.get_digital(DIGITAL_Y)){//Front wings
			toggle_front_wings();
		}
		if(master.get_digital(DIGITAL_A)){//Hang
			hangToggle();
		}
		if(master.get_digital(DIGITAL_X)){//Brake
			brakeToggle();
		}
		if(master.get_digital(DIGITAL_L2)){//Brake
			moveBrakeToggle();
		}
		pros::delay(5);
	}
}
