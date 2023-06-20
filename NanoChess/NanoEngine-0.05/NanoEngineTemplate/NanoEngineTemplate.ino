#include "constants.h"
#include "level.h"
#include "sprites.h"
#include "input.h"
#include "entities.h"
#include "types.h"
#include "display.h"
#include "sound.h"

#include "physics.h"

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
uint8_t x = 20;
uint8_t y = 20;

#define B_PLAYER_ANIM 1
byte Animation = 0;

PhysBoxCollider PlayerColl;

void setupPlayer()
{
  PlayerColl.pos1[0] = x;
  PlayerColl.pos1[1] = y;

  PlayerColl.pos2[0] = x;
  PlayerColl.pos2[1] = y + BMP_PLAYER2_HEIGHT;

  PlayerColl.pos3[0] = x + BMP_PLAYER2_WIDTH;
  PlayerColl.pos3[1] = y ;

  PlayerColl.pos4[0] = x + BMP_PLAYER2_WIDTH;
  PlayerColl.pos4[1] = y  + BMP_PLAYER2_HEIGHT;

}

//main setup
void setup(void) {

  B_FALSE(Animation, B_PLAYER_ANIM);
  B_FALSE(Globals, B_EXIT_SCENE);
  B_FALSE(Globals, B_INVERT_SCREEN);
  B_FALSE(Globals, B_FADE_SCREEN);

  setupDisplay();
  input_setup();
  sound_init();

  setupPlayer();

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
  }
}
// game
// player and entities
Player player;
Entity entity[MAX_ENTITIES];
StaticEntity static_entity[MAX_STATIC_ENTITIES];

// Jump to another scene
void jumpTo(uint8_t target_scene) {
  scene = target_scene;
  B_TRUE(Globals, B_EXIT_SCENE);
  if (target_scene == INTRO) {
    if (player.health  == 0) {
      softReset();
    }
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


void loopTest() 
{

#ifdef SNES_CONTROLLER
  getControllerData();
#endif

  display.clearDisplay();
  bool input = false;

  if (input_up())
  {
    
    y--;
    if(PlayerColl.ScreenCollideYup())
    {
      y++;
    }
    input = true;
  }
  if (input_down())
  {
    y++;
    if(PlayerColl.ScreenCollideYdown())
    {
      y--;
    }
    input = true;
  }
  if (input_left()) {
    x--;
    if(PlayerColl.ScreenCollideXleft())
    {
      x++;
    }
    input = true;
  }
  if (input_right()) {
    x++;
    if(PlayerColl.ScreenCollideXright())
    {
      x--;
    }
    input = true;
  }


  if (B_READ(Animation, B_PLAYER_ANIM) & input == true) {
    B_FALSE(Animation, B_PLAYER_ANIM);
    display.drawBitmap(x, y, bmp_player2, BMP_PLAYER2_WIDTH, BMP_PLAYER2_HEIGHT, 1);

    setupPlayer();
  }
  else {
    B_TRUE(Animation, B_PLAYER_ANIM);
    display.drawBitmap(x, y, bmp_player1, BMP_PLAYER1_WIDTH, BMP_PLAYER1_HEIGHT, 1);

    setupPlayer();
  }

  //drawPixel(PlayerColl.pos4[0], PlayerColl.pos4[1], 1, false);

  display.display();



}

unsigned long last_time;
void loop(void) {
  fps();
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
