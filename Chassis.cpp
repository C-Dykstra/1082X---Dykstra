#include "main.cpp"

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

//FUNCTIONS
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

//Joystick Curve
float joycurve(float val){
	if (abs(val)<7){
		return 0;
	}
	return (val > 0) ? 1 : ((val < 0) ? -1 : 0) *127/(1+exp(-.13889*(abs(val)-50)));
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

void chassisInit(){
    pros::lcd::initialize();

    left_side_motors.set_brake_modes(MOTOR_BRAKE_COAST);
    right_side_motors.set_brake_modes(MOTOR_BRAKE_COAST);

    PTO_Piston.set_value(false);

    chassis.calibrate();
}