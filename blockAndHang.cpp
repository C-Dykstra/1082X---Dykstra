#include "main.cpp"

//Blocker
pros::ADIDigitalOut blocker1 ('E');//Blocker
pros::ADIDigitalOut blocker2 ('F');
bool BL = false;//False is retracted

//Hang
pros::ADIDigitalOut hang1 ('G');//Blocker
pros::ADIDigitalOut hang2 ('H');
bool HA = false;//False is retracted

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

void blockHangInit(){
    blocker1.set_value(false);
    blocker2.set_value(false);

    hang1.set_value(false);
    hang2.set_value(false);
}