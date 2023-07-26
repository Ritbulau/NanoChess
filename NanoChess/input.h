#ifndef _input_h
#define _input_h



void input_setup();
bool input_up();
bool input_down();
bool input_left();
bool input_right();
bool input_fire();

bool input_up_once();
bool input_down_once();
bool input_left_once();
bool input_right_once();
bool input_fire_once();

bool input_up_up();
bool input_down_up();
bool input_left_up();
bool input_right_up();
bool input_fire_up();

#ifdef SNES_CONTROLLER
bool input_start();
void getControllerData(void);
#endif

#endif
