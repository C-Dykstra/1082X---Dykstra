#pragma once
#include "main.h"

//Intake Motor
pros::Motor intake;

void intake_run(int power, int wait);

void intake_run(int power, int delay, int wait);

void intakeInit();