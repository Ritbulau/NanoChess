#include <Arduino.h>
#include "input.h"
#include "constants.h"

#ifdef USE_INPUT_PULLUP
  #define INPUT_MODE INPUT_PULLUP
  #define INPUT_STATE LOW
#else
  #define INPUT_MODE INPUT
  #define INPUT_STATE HIGH
#endif

#ifdef SNES_CONTROLLER
uint16_t buttons = 0;

void input_setup() {
  // Set DATA_CLOCK normally HIGH
  pinMode(DATA_CLOCK, OUTPUT);
  digitalWrite(DATA_CLOCK, HIGH);

  // Set DATA_LATCH normally LOW
  pinMode(DATA_LATCH, OUTPUT);
  digitalWrite(DATA_LATCH, LOW);

  // Set DATA_SERIAL normally HIGH
  pinMode(DATA_SERIAL, OUTPUT);
  digitalWrite(DATA_SERIAL, HIGH);
  pinMode(DATA_SERIAL, INPUT);
}

void getControllerData(void){
  // Latch for 12us
  digitalWrite(DATA_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(DATA_LATCH, LOW);
  delayMicroseconds(6);
  buttons = 0;
  // Retrieve button presses from shift register by pulling the clock high for 6us
  for(uint8_t i = 0; i < 16; ++i){
    digitalWrite(DATA_CLOCK, LOW);
    delayMicroseconds(6);
    buttons |= !digitalRead(DATA_SERIAL) << i;
    digitalWrite(DATA_CLOCK, HIGH);
    delayMicroseconds(6);
  }
}

bool input_left() {
  return buttons & LEFT;
};

bool input_right() {
  return buttons & RIGHT;
};

bool input_up() {
  return buttons & UP;
};

bool input_down() {
  return buttons & DOWN;
};

bool input_fire() {
  return buttons & Y;
};
#else

void input_setup() {
  pinMode(K_LEFT, INPUT_MODE);
  pinMode(K_RIGHT, INPUT_MODE);
  pinMode(K_UP, INPUT_MODE);
  pinMode(K_DOWN, INPUT_MODE);
  pinMode(K_FIRE, INPUT_MODE);
}

bool input_left() {
  return digitalRead(K_LEFT) == INPUT_STATE;
};

bool input_right() {
  return digitalRead(K_RIGHT) == INPUT_STATE;
};

bool input_up() {
  return digitalRead(K_UP) == INPUT_STATE;
};

bool input_down() {
  return digitalRead(K_DOWN) == INPUT_STATE;
};

bool input_fire() {
  return digitalRead(K_FIRE) == INPUT_STATE;
};
#endif

bool ifo, ifoa;
bool iuo, iuoa;
bool ido, idoa;
bool ilo, iloa;
bool iro, iroa;

bool input_fire_once(){
  bool result = false;
  if(input_fire()){
    ifo = true;
  }
  else{
    ifo = false;
    ifoa = false;
  }
  if(!ifoa && ifo){
    ifoa = true;
    result = true;
  }
  return result;
}


bool input_up_once(){
  bool result = false;
  if(input_up()){
    iuo = true;
  }
  else{
    iuo = false;
    iuoa = false;
  }
  if(!iuoa && iuo){
    iuoa = true;
    result = true;
  }
  return result;
}


bool input_down_once(){
  bool result = false;
  if(input_down()){
    ido = true;
  }
  else{
    ido = false;
    idoa = false;
  }
  if(!idoa && ido){
    idoa = true;
    result = true;
  }
  return result;
}


bool input_left_once(){
  bool result = false;
  if(input_left()){
    ilo = true;
  }
  else{
    ilo = false;
    iloa = false;
  }
  if(!iloa && ilo){
    iloa = true;
    result = true;
  }
  return result;
}

bool input_right_once(){
  bool result = false;
  if(input_right()){
    iro = true;
  }
  else{
    iro = false;
    iroa = false;
  }
  if(!iroa && iro){
    iroa = true;
    result = true;
  }
  return result;
}


bool ifu;

bool input_fire_up(){
  if(input_fire()){
    ifu = true;
    return false;
  }
  else if(!input_fire() && ifu == true){
    ifu = false;
    return true;
  }
  else{
    return false;
  }
}
