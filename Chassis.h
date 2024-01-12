#pragma once
#include "main.h"

pros::Motor left_m1_front; // port 1, blue gearbox, not reversed
pros::Motor left_m2_mid;
pros::Motor left_m3_back;
pros::Motor left_half_motor;

pros::Motor right_m1_front;
pros::Motor right_m2_mid;
pros::Motor right_m3_back;
pros::Motor right_half_motor;

pros::MotorGroup left_side_motors;
pros::MotorGroup right_side_motors;

bool brakeHold;//False is coast

pros::ADIDigitalOut PTO_Piston;

bool PTO_Engaged; //True means on drive

//Odom
pros::Rotation h_track;//Horiz
pros::Rotation v_track;//Vert

//2.75" wheel diameter, -3" offset from tracking center, 1:1 gear ratio
lemlib::TrackingWheel horiz;
lemlib::TrackingWheel vert;

pros::Imu inertial_sensor; // port 11

// drivetrain settings
lemlib::Drivetrain drivetrain;

// lateral motion controller
lemlib::ControllerSettings linearController;

// angular motion controller
lemlib::ControllerSettings angularController;

// sensors for odometry
// note that in this example we use internal motor encoders (IMEs), so we don't pass vertical tracking wheels
lemlib::OdomSensors sensors;

//Chassis
lemlib::Chassis chassis;

//Pure Pursuit Assets
ASSET(auton1_txt);
ASSET(auton2_txt);
ASSET(auton3_txt);
ASSET(auton4_txt);
ASSET(auton5_txt);
ASSET(auton6_txt);
ASSET(auton7_txt);
ASSET(auton8_txt);

//FUNCTIONS
void PTO_Switch();

//Joystick Curve
float joycurve(float val);

//Brake Task
bool &BrakePtr;;
void brakeRun();

bool moveBrake;;
void moveBrakeToggle();

bool &moveBrakeptr;
void moveBraker();

void chassisInit();