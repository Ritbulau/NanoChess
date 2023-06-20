#ifndef _input_h
#define _input_h



void input_setup();
bool input_up();
bool input_down();
bool input_left();
bool input_right();
bool input_fire();

#ifdef SNES_CONTROLLER
bool input_start();
void getControllerData(void);
#endif

#endif
