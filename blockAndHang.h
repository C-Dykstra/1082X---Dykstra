#pragma once
#include "main.h"

//Blocker
pros::ADIDigitalOut blocker1;//Blocker
pros::ADIDigitalOut blocker2;
bool BL;//False is retracted

//Hang
pros::ADIDigitalOut hang1;//Blocker
pros::ADIDigitalOut hang2;
bool HA;//False is retracted

//Blocker
void blockToggle();

//Hang
void hangToggle();

void blockHangInit();