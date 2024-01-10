#include "main.h"
#include "lemlib/api.hpp"
#include "../include/pros/misc.h"

pros::Motor left_m1_front(1, pros::E_MOTOR_GEARSET_06, true); // port 1, blue gearbox, not reversed
pros::Motor left_m2_mid(2, pros::E_MOTOR_GEARSET_06, true);
pros::Motor left_m3_back(4, pros::E_MOTOR_GEARSET_06, true);
pros::Motor left_half_motor(3, pros::E_MOTOR_GEARSET_06, false);

pros::Motor right_m1_front(7, pros::E_MOTOR_GEARSET_06, false);
pros::Motor right_m2_mid(8, pros::E_MOTOR_GEARSET_06, false);
pros::Motor right_m3_back(10, pros::E_MOTOR_GEARSET_06, false);
pros::Motor right_half_motor(9, pros::E_MOTOR_GEARSET_06, true);

pros::MotorGroup left_side_motors({left_m1_front, left_m2_mid, left_m3_back, left_half_motor});
pros::MotorGroup right_side_motors({right_m1_front, right_m2_mid, right_m3_back, right_half_motor});

bool brakeHold = false;//False is coast

pros::ADIDigitalOut PTO_Piston ('A');

bool PTO_Engaged = true; //True means on drive

//Odom
pros::Rotation h_track(9, false);//Horiz
pros::Rotation v_track(10, false);//Vert

//2.75" wheel diameter, -3" offset from tracking center, 1:1 gear ratio
lemlib::TrackingWheel horiz(&h_track, 2.75, -3, 1);
lemlib::TrackingWheel vert(&v_track, 2.75, 0, 1);

pros::Imu inertial_sensor(11); // port 11

// drivetrain settings
lemlib::Drivetrain drivetrain(
    &left_side_motors, // left motor group
    &right_side_motors, // right motor group
    10, // 10 inch track width
    lemlib::Omniwheel::NEW_275, // using new 2.75" omnis
    450, // drivetrain rpm is 360
    2 // chase power is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(
    10, // proportional gain (kP)
    0, // integral gain (kI)
    3, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in inches
    100, // small error range timeout, in milliseconds
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(
    2, // proportional gain (kP)
    0, // integral gain (kI)
    10, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in degrees
    100, // small error range timeout, in milliseconds
    3, // large error range, in degrees
    500, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
);

// sensors for odometry
// note that in this example we use internal motor encoders (IMEs), so we don't pass vertical tracking wheels
lemlib::OdomSensors sensors(
    &vert, // vertical tracking wheel 1, set to null
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    &horiz, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
    &inertial_sensor // inertial sensor
);

//Chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors);\

//Pure Pursuit Assets
ASSET(auton1_txt);
ASSET(auton2_txt);
ASSET(auton3_txt);
ASSET(auton4_txt);
ASSET(auton5_txt);
ASSET(auton6_txt);
ASSET(auton7_txt);
ASSET(auton8_txt);

//Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

//Intake Motor
pros::Motor intake(5, true);

//Slapper
pros::MotorGroup Slapper({right_half_motor, left_half_motor});
bool slapRun = false;

//Wings
pros::ADIDigitalOut back_wing_l ('C');//Back

pros::ADIDigitalOut back_wing_r ('D');//Back

bool BW = false;//True=Extended
pros::ADIDigitalOut front_wing_r ('B');//Front right

bool FR = false;
pros::ADIDigitalOut front_wing_l ('A');//Front left

bool FL = false;

//Blocker
pros::ADIDigitalOut blocker1 ('E');//Blocker

pros::ADIDigitalOut blocker2 ('F');

bool BL = false;//False is retracted

//Hang
pros::ADIDigitalOut hang1 ('G');//Blocker

pros::ADIDigitalOut hang2 ('H');

bool HA = false;//False is retracted

void screen() {
    // loop forever
    while (true) {
        lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
        pros::lcd::print(0, "x: %f", pose.x); // print the x position
        pros::lcd::print(1, "y: %f", pose.y); // print the y position
        pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
        pros::delay(50);
    }
}

//Initialize
void initialize(){
    pros::lcd::initialize();

    left_side_motors.set_brake_modes(MOTOR_BRAKE_COAST);
    right_side_motors.set_brake_modes(MOTOR_BRAKE_COAST);

    PTO_Piston.set_value(false);

    back_wing_l.set_value(false);
    back_wing_r.set_value(false);
    front_wing_r.set_value(false);
    front_wing_l.set_value(false);

    blocker1.set_value(false);
    blocker2.set_value(false);

    hang1.set_value(false);
    hang2.set_value(false);

    chassis.calibrate();
    chassis.setPose(12, -58, -90);
    pros::Task screenTask(screen());
}