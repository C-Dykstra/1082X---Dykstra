#include "main.cpp"

//Wings
pros::ADIDigitalOut back_wing_l ('C');//Back
pros::ADIDigitalOut back_wing_r ('D');//Back
bool BW = false;//True=Extended
pros::ADIDigitalOut front_wing_r ('B');//Front right
bool FR = false;
pros::ADIDigitalOut front_wing_l ('A');//Front left
bool FL = false;

void toggle_BW(){
	BW = !BW;
	back_wing_r.set_value(BW);
	back_wing_l.set_value(BW);
}

bool toggle_FR(){
	FR = !FR;
	front_wing_r.set_value(FR);
	return FR;
}

bool toggle_FL(){
	FL = !FL;
	front_wing_l.set_value(FL);
	return FL;
}

void toggle_front_wings(){
	FL=!FL;
	FR=!FR;
	front_wing_l.set_value(FL);
	front_wing_r.set_value(FR);
}

void cont_front(bool ext){
	FL = FR = ext;
	front_wing_l.set_value(FL);
	front_wing_r.set_value(FR);
}

void Descore(int delay, int wait){
	pros::delay(delay);
	toggle_BW();
	pros::delay(wait);
	toggle_BW();
}

void wingsInit(){
    back_wing_l.set_value(false);
    back_wing_r.set_value(false);
    front_wing_r.set_value(false);
    front_wing_l.set_value(false);
}