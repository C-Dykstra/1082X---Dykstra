#pragma once
#include "main.h"

//Slapper
pros::MotorGroup Slapper;
bool slapRun;

//Slapper Toggle/Run
bool &slaptog;
void slapperRun();
void slapperToggle();

void slapperInit();