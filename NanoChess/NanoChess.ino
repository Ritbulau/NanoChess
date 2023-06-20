#include "constants.h"
#include "level.h"
#include "sprites.h"
#include "input.h"
#include "entities.h"
#include "types.h"
#include "display.h"
#include "sound.h"

using namespace std;
// Useful macros
#define swap(a, b)            do { typeof(a) temp = a; a = b; b = temp; } while (0)
#define sign(a, b)            (double) (a > b ? 1 : (b > a ? -1 : 0))

//defines for global bools
#define B_EXIT_SCENE 1
#define B_INVERT_SCREEN 2
#define B_FADE_SCREEN 4
byte Globals = 0;

uint8_t scene = INTRO;
uint8_t SelX = 0, SelY = 0;
uint8_t i = 0;
bool second, bitSec;
unsigned long time, oldTime, oldBitTime;
//main setup
void setup(void) {

  B_FALSE(Globals, B_EXIT_SCENE);
  B_FALSE(Globals, B_INVERT_SCREEN);
  B_FALSE(Globals, B_FADE_SCREEN);
  setupDisplay();
  input_setup();
  sound_init();



  switch (scene) {
    case INTRO: {
        music = 1;

        display.drawBitmap(
          1,
          1,
          bmp_logo_bits,
          BMP_LOGO_WIDTH,
          BMP_LOGO_HEIGHT,
          1
        );

        drawText(40, 44, F("PRESS FIRE"));
        drawText(106, 4, F("NANO"));
        drawText(1, 58, F("V 0.03"));
        display.display();
        break;
      }
    case TEST: {
      loopTest();
    }
  }
}


// Jump to another scene
void jumpTo(uint8_t target_scene) {
  scene = target_scene;
  B_TRUE(Globals, B_EXIT_SCENE);
  if (target_scene == INTRO) {
  }
  else if(target_scene == TEST){
    loopTest();
  }
}



void softReset() {
  asm volatile ("jmp 0");
}
// Intro screen
void loopIntro() {
#ifdef SNES_CONTROLLER
  getControllerData();
#endif
  //playSound(mus_s1_snd, MUS_S1_SND_LEN);
  while (!B_READ(Globals, B_EXIT_SCENE)) {
    if (input_fire()) jumpTo(TEST);
  };
}

void drawFigure(char type, int8_t x, int8_t y, bool color){
  drawChar(16 + x*7, 50 + y*7, type);
  display.drawPixel(16 + x*7 + 5, 50 + y*7 + 1, color);
  display.drawPixel(16 + x*7 + 5, 50 + y*7 + 3, color);
  display.drawPixel(16 + x*7 + 5, 50 + y*7 + 5, color);
}
void drawSelector(int8_t x, int8_t y, bool sec){
  if(sec){
    display.drawPixel(16 + x*7 - 1, 50 - y*7, 2);
    display.drawPixel(16 + x*7 + 4, 50 - y*7, 2);
    display.drawPixel(16 + x*7 + 4, 50 - y*7 + 6, 2);
    display.drawPixel(16 + x*7 - 1, 50 - y*7 + 6, 2);
  }
}

void loopTest() 
{

  #ifdef SNES_CONTROLLER
  getControllerData();
  #endif
  if(input_up() && SelY<7){
    SelY++;
    second = true;
  }
  if(input_down() && SelY>0){
    SelY--;
    second = true;
  }
  if(input_right() && SelX<7){
    SelX++;
    second = true;
  }
  if(input_left() && SelX>0){
    SelX--;
    second = true;
  }
  delay(100);

  display.clearDisplay();
  //display.fillRect(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t)
  drawText(16, 57, F("A"));
  drawText(23, 57, F("B"));
  drawText(30, 57, F("C"));
  drawText(37, 57, F("D"));
  drawText(44, 57, F("E"));
  drawText(51, 57, F("F"));
  drawText(58, 57, F("G"));
  drawText(65, 57, F("H"));

  drawText(10, 1, F("8"));
  drawText(10, 8, F("7"));
  drawText(10, 15, F("6"));
  drawText(10, 22, F("5"));
  drawText(10, 29, F("4"));
  drawText(10, 36, F("3"));
  drawText(10, 43, F("2"));
  drawText(10, 50, F("1"));
  
  drawFigure('P', 0, 0, 1);
  drawSelector(SelX, SelY, second);

  for(i = 0; i < 8; i++){
    display.fillRect(15 + (i*7), 50 - (i*7), 7, 7, 2);
  }
  for(i = 0; i < 6; i++){
    display.fillRect(15 + (i*7), 36 - (i*7), 7, 7, 2);
  }
  for(i = 0; i < 4; i++){
    display.fillRect(15 + (i*7), 22 - (i*7), 7, 7, 2);
  }
  for(i = 0; i < 2; i++){
    display.fillRect(15 + (i*7), 8 - (i*7), 7, 7, 2);
  }

  for(i = 0; i < 6; i++){
    display.fillRect(29 + (i*7), 50 - (i*7), 7, 7, 2);
  }
  for(i = 0; i < 4; i++){
    display.fillRect(43 + (i*7), 50 - (i*7), 7, 7, 2);
  }
  for(i = 0; i < 2; i++){
    display.fillRect(57 + (i*7), 50 - (i*7), 7, 7, 2);
  }


  display.display();
}

unsigned long last_time;
void loop(void) {
  fps();
  time = millis();
  if (time / 1000  - oldTime == 1 ){
    second = !second;
    oldTime = time / 1000;
  }

  switch (scene) {
    case INTRO: {
        loopIntro();
        break;
      }
    case TEST: {
        loopTest();
        break;
      }
  }

  if (B_READ(Globals, B_FADE_SCREEN)) {// fade out effect
    for (uint8_t i = 0; i < GRADIENT_COUNT; i++) {
      fadeScreen(i, 0);
      display.display();
      delay(40);
    }
  }

  B_FALSE(Globals, B_EXIT_SCENE);

}
