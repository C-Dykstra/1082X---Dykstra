#pragma once
#include "main.h"

//Wings
pros::ADIDigitalOut back_wing_l;//Back
pros::ADIDigitalOut back_wing_r;//Back
bool BW;//True=Extended
pros::ADIDigitalOut front_wing_r;//Front right
bool FR;
pros::ADIDigitalOut front_wing_l;//Front left
bool FL;

void toggle_BW();

bool toggle_FR();

bool toggle_FL();

void toggle_front_wings();

void cont_front(bool ext);

void Descore(int delay, int wait);

void wingsInit();