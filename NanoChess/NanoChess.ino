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
#define swap(a, b) \
  do { \
    typeof(a) temp = a; \
    a = b; \
    b = temp; \
  } while (0)
#define sign(a, b) (double)(a > b ? 1 : (b > a ? -1 : 0))

//defines for global bools
#define B_EXIT_SCENE 1
#define B_INVERT_SCREEN 2
#define B_FADE_SCREEN 4
byte Globals = 0;

uint8_t scene = INTRO;
int8_t SelCoords = 7;
uint8_t i = 0;
bool second, bitSec;
unsigned long time, oldBitTime;
int8_t SelectedCoords = -1;
bool team = 0;
int8_t Win;
int8_t k = 9;
bool Endable = false;
bool TimerEnabeled = true;

float DeltaTime;
unsigned long timing;
float TimeSecBlack, TimeSecWhite;
float TimeMinBlack, TimeMinWhite;
float TimeMinInc, TimeSecInc;
float oldTime;
int8_t Check;

int8_t timer_mode = 1;

uint16_t CurrMove;

Figure PawnB1('P', 1, 1, 0, 0), PawnB2('P', 1, 11, 0, 0), PawnB3('P', 1, 21, 0, 0), PawnB4('P', 1, 31, 0, 0), PawnB5('P', 1, 41, 0, 0), PawnB6('P', 1, 51, 0, 0), PawnB7('P', 1, 61, 0, 0), PawnB8('P', 1, 71, 0, 0);
Figure RookB1('R', 1, 00, 0, 0), RookB2('R', 1, 70, 0, 0);
Figure KnightB1('N', 1, 10, 0, 0), KnightB2('N', 1, 60, 0, 0);
Figure BishopB1('B', 1, 20, 0, 0), BishopB2('B', 1, 50, 0, 0);
Figure KingB('K', 1, 40, 0, 0);
Figure QueenB('Q', 1, 30, 0, 0);

Figure Pawn1('P', 0, 6, 0, 0), Pawn2('P', 0, 16, 0, 0), Pawn3('P', 0, 26, 0, 0), Pawn4('P', 0, 36, 0, 0), Pawn5('P', 0, 46, 0, 0), Pawn6('P', 0, 56, 0, 0), Pawn7('P', 0, 66, 0, 0), Pawn8('P', 0, 76, 0, 0);
Figure Rook1('R', 0, 7, 0, 0), Rook2('R', 0, 77, 0, 0);
Figure Knight1('N', 0, 17, 0, 0), Knight2('N', 0, 67, 0, 0);
Figure Bishop1('B', 0, 27, 0, 0), Bishop2('B', 0, 57, 0, 0);
Figure King('K', 0, 47, 0, 0);
Figure Queen('Q', 0, 37, 0, 0);

Figure FA[]{
  RookB1,
  KnightB1,
  BishopB1,
  QueenB,
  KingB,
  BishopB2,
  KnightB2,
  RookB2,
  PawnB1,
  PawnB2,
  PawnB3,
  PawnB4,
  PawnB5,
  PawnB6,
  PawnB7,
  PawnB8,

  Pawn1,
  Pawn2,
  Pawn3,
  Pawn4,
  Pawn5,
  Pawn6,
  Pawn7,
  Pawn8,
  Rook1,
  Knight1,
  Bishop1,
  Queen,
  King,
  Bishop2,
  Knight2,
  Rook2,
};

int8_t BKing = 4;
int8_t WKing = 28;

//main setup
void setup(void) {

  B_FALSE(Globals, B_EXIT_SCENE);
  B_FALSE(Globals, B_INVERT_SCREEN);
  B_FALSE(Globals, B_FADE_SCREEN);
  setupDisplay();
  input_setup();
  sound_init();



  switch (scene) {
    case INTRO:
      {
        music = 1;

        drawText(37, 52, F("PRESS FIRE"));

        display.drawBitmap(
          8,
          3,
          bmp_logo_bits,
          BMP_LOGO_WIDTH,
          BMP_LOGO_HEIGHT,
          1
        );

        //drawText(37, 15, F("NANO-CHESS"));
        drawText(1, 58, F("V 0.95"));
        display.display();
        break;
      }
    case TEST:
      {
        loopTest();
      }
    case SETTINGS:
      {
        loopSettings();
      }
  }
}


// Jump to another scene
void jumpTo(uint8_t target_scene) {
  scene = target_scene;
  timing = millis();
  B_TRUE(Globals, B_EXIT_SCENE);
  if (target_scene == INTRO) {
  } else if (target_scene == TEST) {
    loopTest();
  } else if (target_scene == SETTINGS) {
    loopSettings();
  }
}

//draws up('^' or 'u'), down('d'), left('<' or 'l') and right('>' or 'r') arrow, for size use only odd integer!
void drawArrow(uint8_t x, uint8_t y, int8_t size, char arrow) {
  bool top = false;
  int8_t xDraw = 0, yDraw = 0;
  if (arrow == '>' || arrow == 'r') {
    for (int8_t i = 0; i < size; i++) {
      display.drawPixel(x + xDraw, y + i, 1);
      if (xDraw < size / 2 && !top) {
        xDraw++;
      } else {
        xDraw--;
        top = true;
      }
    }
  } else if (arrow == '<' || arrow == 'l') {
    xDraw = size / 2;
    for (int8_t i = 0; i < size; i++) {
      display.drawPixel(x + xDraw, y + i, 1);
      if (xDraw > 0 && !top) {
        xDraw--;
      } else {
        xDraw++;
        top = true;
      }
    }
  } else if (arrow == '^' || arrow == 'u') {
    yDraw = size / 2;
    for (int8_t i = 0; i < size; i++) {
      display.drawPixel(x + i, y + yDraw, 1);
      if (yDraw > 0 && !top) {
        yDraw--;
      } else {
        yDraw++;
        top = true;
      }
    }
  } else if (arrow == 'd') {
    for (int8_t i = 0; i < size; i++) {
      display.drawPixel(x + i, y + yDraw, 1);
      if (yDraw < size / 2 && !top) {
        yDraw++;
      } else {
        yDraw--;
        top = true;
      }
    }
  }
}

void softReset() {
  asm volatile("jmp 0");
}

// Intro screen
void loopIntro() {
#ifdef SNES_CONTROLLER
  getControllerData();
#endif
  //playSound(mus_s1_snd, MUS_S1_SND_LEN);
  while (!B_READ(Globals, B_EXIT_SCENE)) {
    if (input_fire()) {
      delay(200);
      jumpTo(SETTINGS);
    }
  };
}

void drawFigure(char type, int8_t x, int8_t y, bool color) {
  if (x < 8 && y < 8 && x > -1 && y > -1) {
    drawChar(16 + x * 7 - k, 1 + y * 7, type);
    display.drawPixel(16 + x * 7 + 5 - k, 1 + y * 7 + 1, color);
    display.drawPixel(16 + x * 7 + 5 - k, 1 + y * 7 + 3, color);
    display.drawPixel(16 + x * 7 + 5 - k, 1 + y * 7 + 5, color);
  }
}

void drawFigureHUD(char type, int8_t x, int8_t y, bool color) {

  drawChar(x, y, type);
  display.drawPixel(x + 5, y + 1, color);
  display.drawPixel(x + 5, y + 3, color);
  display.drawPixel(x + 5, y + 5, color);
}

void drawSelector(int8_t coords, bool sec) {
  if (sec) {
    display.drawPixel(16 + (coords / 10) * 7 - 1 - k, 1 + (coords % 10) * 7, 1);
    display.drawPixel(16 + (coords / 10) * 7 + 4 - k, 1 + (coords % 10) * 7, 1);
    display.drawPixel(16 + (coords / 10) * 7 + 4 - k, 1 + (coords % 10) * 7 + 6, 1);
    display.drawPixel(16 + (coords / 10) * 7 - 1 - k, 1 + (coords % 10) * 7 + 6, 1);
  }
}

void drawSelected(int8_t coords) {
  if (coords >= 0) {
    display.drawPixel(16 + (coords / 10) * 7 - 1 - k, 1 + (coords % 10) * 7, 1);
    display.drawPixel(16 + (coords / 10) * 7 + 4 - k, 1 + (coords % 10) * 7, 1);
    display.drawPixel(16 + (coords / 10) * 7 + 4 - k, 1 + (coords % 10) * 7 + 6, 1);
    display.drawPixel(16 + (coords / 10) * 7 - 1 - k, 1 + (coords % 10) * 7 + 6, 1);
  }
}

int8_t figureFind(int8_t coords) {
  for (int8_t FLF = 0; FLF < sizeof(FA) / sizeof(Figure); FLF++) {
    if (FA[FLF].coords == coords) {
      return FLF;
      break;
    }
  }
}

int8_t FFBTAT(char type, bool team) {
  for (int8_t FLF = 0; FLF < sizeof(FA) / sizeof(Figure); FLF++) {
    if (FA[FLF].type == type && FA[FLF].team == team) {
      return FLF;
      break;
    }
  }
}

int8_t FHU(char type, bool team, int8_t coords, bool moved) {
  bool found = false;
  for (int8_t FLF = 0; FLF < sizeof(FA) / sizeof(Figure); FLF++) {
    if (FA[FLF].type == type && FA[FLF].team == team && FA[FLF].coords == coords && FA[FLF].moved == moved) {
      found = true;
      break;
    }
  }
  return found;
}

bool FigureHere(int8_t coords) {
  bool found = false;
  for (int8_t FLF = 0; FLF < sizeof(FA) / sizeof(Figure); FLF++) {
    if (FA[FLF].coords == coords) {
      found = true;
      break;
    }
  }
  return found;
}

int8_t SelFigure = -1;
uint8_t CurrFigure = -1;
bool Choice = false;
int8_t CurChoice = 0;

void CallChoice(int8_t figure) {
  drawText(80 - k, 25, F("QUEEN (Q)"));
  drawText(80 - k, 32, F("KNIGHT (N)"));
  drawText(80 - k, 39, F("BISHOP (B)"));
  drawText(80 - k, 46, F("ROOK (R)"));

  if (input_up_once() && CurChoice > 0) {
    CurChoice--;
  }
  if (input_down_once() && CurChoice < 3) {
    CurChoice++;
  }

  switch (CurChoice) {
    case 0:
      {
        drawText(74 - k, 25, F("#"));
        if (input_fire_once()) {
          FA[figure].type = 'Q';
          Choice = false;
        }
        break;
      }
    case 1:
      {
        drawText(74 - k, 32, F("#"));
        if (input_fire_once()) {
          FA[figure].type = 'N';
          Choice = false;
        }
        break;
      }
    case 2:
      {
        drawText(74 - k, 39, F("#"));
        if (input_fire_once()) {
          FA[figure].type = 'B';
          Choice = false;
        }
        break;
      }
    case 3:
      {
        drawText(74 - k, 46, F("#"));
        if (input_fire_once()) {
          FA[figure].type = 'R';
          Choice = false;
        }
        break;
      }
  }
}

int8_t DrawMovesCounter;

void FigureMove() {

  if(timer_mode == 3){
    if(!team){
      TimeSecWhite += TimeSecInc;
      TimeMinWhite += TimeMinInc;
    }
    else{
      TimeSecBlack += TimeSecInc;
      TimeMinBlack += TimeMinInc;
    }
  }

  if (FA[CurrFigure].team != team && FigureHere(SelCoords)) {
    FA[CurrFigure].coords = 126;
    playSound(eat_snd, EAT_SND_LEN);
  } else {
    playSound(move_snd, MOVE_SND_LEN);
    if (FA[SelFigure].type == 'P') {
      DrawMovesCounter = 0;
    } else {
      DrawMovesCounter++;
    }
  }
  FA[SelFigure].coords = SelCoords;
  CurrMove++;
  FA[SelFigure].last_time_moved = CurrMove;

  FA[SelFigure].moved++;
  team = !team;
  SelectedCoords = -1;
}

//FigureMove alternative for En Passant
void PawnMove() {

  if(timer_mode == 3){
    if(!team){
      TimeSecWhite += TimeSecInc;
      TimeMinWhite += TimeMinInc;
    }
    else{
      TimeSecBlack += TimeSecInc;
      TimeMinBlack += TimeMinInc;
    }
  }

  if (FA[SelFigure].team) {
    FA[figureFind(SelCoords - 1)].coords = 126;
  } else {
    FA[figureFind(SelCoords + 1)].coords = 126;
  }
  playSound(eat_snd, EAT_SND_LEN);
  FA[SelFigure].coords = SelCoords;
  CurrMove++;
  FA[SelFigure].last_time_moved = CurrMove;

  FA[SelFigure].moved++;
  team = !team;
  SelectedCoords = -1;

  DrawMovesCounter = 0;

}


int8_t mod(int8_t num) {
  if (num < 0) { num = num * -1; }
  return num;
}

int8_t choiceFigure;

bool CheckAttackable(int8_t coordsAC, bool teamAC) {

  bool Found = false;

  if(coordsAC / 10 >= 0 && coordsAC / 10 <= 7 && coordsAC % 10 >= 0 && coordsAC % 10 <= 7){
    //right check
    for (int8_t i = coordsAC / 10 + 1; i <= 7; i++) {
      if ( i / 10 > 7 || i % 10 > 7 || i < 0 ) { break; }
      if (FigureHere(i * 10 + coordsAC % 10)) {
        if (FA[figureFind(i * 10 + coordsAC % 10)].team != teamAC){
          if (FA[figureFind(i * 10 + coordsAC % 10)].type == 'Q' || FA[figureFind(i * 10 + coordsAC % 10)].type == 'R')
          {
            //drawChar(30,30,FA[figureFind(i * 10 + coordsAC % 10)].type);
            Found = true;
            break;
          }
          else{
            break;
          }
        }
        else{
          break;
        }
      }
    }

    //left check
    for (int8_t i = coordsAC / 10 - 1; i >= 0; i--) {
      if ( i / 10 > 7 || i % 10 > 7 || i < 0 ) { break; }
      if (FigureHere(i * 10 + coordsAC % 10)) {
        if (FA[figureFind(i * 10 + coordsAC % 10)].team != teamAC){
          if (FA[figureFind(i * 10 + coordsAC % 10)].type == 'Q' || FA[figureFind(i * 10 + coordsAC % 10)].type == 'R')
          {
            //drawChar(30,30,FA[figureFind(i * 10 + coordsAC % 10)].type);
            Found = true;
            break;
          }
          else{
            break;
          }
        }
        else{
          break;
        }
      }
    }

    //down check
    for (int8_t i = coordsAC % 10 + 1; i <= 7; i++) {
      if ( i / 10 > 7 || i % 10 > 7 || i < 0 ) { break; }
      if (FigureHere(i + coordsAC / 10 * 10)) {
        if (FA[figureFind(i + coordsAC / 10 * 10)].team != teamAC){
          if (FA[figureFind(i + coordsAC / 10 * 10)].type == 'Q' || FA[figureFind(i + coordsAC / 10 * 10)].type == 'R')
          {
            //drawChar(30,30,FA[figureFind(i + coordsAC / 10 * 10)].type);
            Found = true;
            break;
          }
          else{
            break;
          }
        }
        else{
          break;
        }
      }
    }

    //up check
    for (int8_t i = coordsAC % 10 - 1; i >= 0; i--) {
      if ( i / 10 > 7 || i % 10 > 7 || i < 0 ) { break; }
      if (FigureHere(i + coordsAC / 10 * 10)) {
        if (FA[figureFind(i + coordsAC / 10 * 10)].team != teamAC){
          if (FA[figureFind(i + coordsAC / 10 * 10)].type == 'Q' || FA[figureFind(i + coordsAC / 10 * 10)].type == 'R')
          {
            //drawChar(30,30,FA[figureFind(i + coordsAC / 10 * 10)].type);
            Found = true;
            break;
          }
          else{
            break;
          }
        }
        else{
          break;
        }
      }
    }


    //up-right check
    for (int8_t i = coordsAC + 9; i / 10 <= 7 || i % 10 >= 0; i+=9) {
      if ( i / 10 > 7 || i % 10 > 7 || i < 0 ) { break; }
      if (i / 10 > 7 || i % 10 < 0){
        break;
      }
      if (FigureHere(i)) {
        if (FA[figureFind(i)].team != teamAC){
          if (FA[figureFind(i)].type == 'Q' || FA[figureFind(i)].type == 'B')
          {
            //drawChar(30,30,FA[figureFind(i)].type);
            Found = true;
            break;
          }
          else{
            break;
          }
        }
        else{
          break;
        }
      }
    }

    //up-left check
    for (int8_t i = coordsAC - 11; i / 10 >= 0 || i % 10 >= 0; i -= 11) {
      if ( i / 10 > 7 || i % 10 > 7 || i < 0 ) { break; }
      if(i / 10 < 0 || i % 10 < 0){
        break;
      }
      if (FigureHere(i)) {
        if (FA[figureFind(i)].team != teamAC){
          if (FA[figureFind(i)].type == 'Q' || FA[figureFind(i)].type == 'B')
          {
            //drawChar(30,30,FA[figureFind(i)].type);
            Found = true;
            break;
          }
          else{
            break;
          }
        }
        else{
          break;
        }
      }
    }

    //down-left check
    for (int8_t i = coordsAC - 9; i / 10 >= 0 || i % 10 <= 7; i -= 9) {
      if ( i / 10 > 7 || i % 10 > 7 || i < 0 ) { break; }
      if(i / 10 < 0 || i % 10 > 7){
        break;
      }
      if (FigureHere(i)) {
        if (FA[figureFind(i)].team != teamAC){
          if (FA[figureFind(i)].type == 'Q' || FA[figureFind(i)].type == 'B')
          {
            //drawChar(30,30,FA[figureFind(i)].type);
            Found = true;
            break;
          }
          else{
            break;
          }
        }
        else{
          break;
        }
      }
    }

    //down-right check
    for (int8_t i = coordsAC + 11; i / 10 <= 7 || i % 10 <= 7; i += 11) {
      if ( i / 10 > 7 || i % 10 > 7 || i < 0 ) { break; }
      if(i / 10 > 7 || i % 10 > 7){
        break;
      }
      if (FigureHere(i)) {
        if (FA[figureFind(i)].team != teamAC){
          if (FA[figureFind(i)].type == 'Q' || FA[figureFind(i)].type == 'B')
          {
            //drawChar(30,30,FA[figureFind(i)].type);
            Found = true;
            break;
          }
          else{
            break;
          }
        }
        else{
          break;
        }
      }
    }

    //knight check
    if(FigureHere(coordsAC + 12) && FA[figureFind(coordsAC + 12)].team != teamAC && FA[figureFind(coordsAC + 12)].type == 'N'){
      Found = true;
    }
    else if(FigureHere(coordsAC + 8) && FA[figureFind(coordsAC + 8)].team != teamAC && FA[figureFind(coordsAC + 8)].type == 'N'){
      Found = true;
    }
    else if(FigureHere(coordsAC + 19) && FA[figureFind(coordsAC + 19)].team != teamAC && FA[figureFind(coordsAC + 19)].type == 'N'){
      Found = true;
    }
    else if(FigureHere(coordsAC + 21) && FA[figureFind(coordsAC + 21)].team != teamAC && FA[figureFind(coordsAC + 21)].type == 'N'){
      Found = true;
    }
    else if(FigureHere(coordsAC - 8) && FA[figureFind(coordsAC  - 8)].team != teamAC && FA[figureFind(coordsAC - 8)].type == 'N'){
      Found = true;
    }
    else if(FigureHere(coordsAC - 19) && FA[figureFind(coordsAC  - 19)].team != teamAC && FA[figureFind(coordsAC - 19)].type == 'N'){
      Found = true;
    }
    else if(FigureHere(coordsAC - 12) && FA[figureFind(coordsAC  - 12)].team != teamAC && FA[figureFind(coordsAC - 12)].type == 'N'){
      Found = true;
    }
    else if(FigureHere(coordsAC - 21) && FA[figureFind(coordsAC  - 21)].team != teamAC && FA[figureFind(coordsAC - 21)].type == 'N'){
      Found = true;
    }

    //pawn check
    if(teamAC){
      if(FigureHere(coordsAC + 11) && FA[figureFind(coordsAC + 11)].team != teamAC && FA[figureFind(coordsAC + 11)].type == 'P'){
        Found = true;
      }
      if(FigureHere(coordsAC - 9) && FA[figureFind(coordsAC - 9)].team != teamAC && FA[figureFind(coordsAC - 9)].type == 'P'){
        Found = true;
      }
    }
    if(!teamAC){
      if(FigureHere(coordsAC - 11) && FA[figureFind(coordsAC - 11)].team != teamAC && FA[figureFind(coordsAC - 11)].type == 'P'){
        Found = true;
      }
      if(FigureHere(coordsAC + 9) && FA[figureFind(coordsAC + 9)].team != teamAC && FA[figureFind(coordsAC + 9)].type == 'P'){
        Found = true;
      }
    }
  }

  return Found;
}

bool CheckAttackablePat(int8_t CAC, bool TAC){
  bool Pat = false;
  if(CheckAttackable(CAC+10,TAC) && CheckAttackable(CAC+10+1,TAC) && CheckAttackable(CAC+1,TAC) && CheckAttackable(CAC-10+1,TAC) && CheckAttackable(CAC-10,TAC) && CheckAttackable(CAC-10-1,TAC) && CheckAttackable(CAC-1,TAC) && CheckAttackable(CAC+10-1,TAC) && !CheckAttackable(CAC,TAC)){
    Pat = true;
  }
  return Pat;
}


void loopTest() {
#ifdef SNES_CONTROLLER
  getControllerData();
#endif
  display.clearDisplay();


  if (CheckAttackable(FA[FFBTAT('K', 0)].coords, 0)) {
    Check = 2;
  }
  else if (CheckAttackable(FA[FFBTAT('K', 1)].coords, 1)) {
    Check = 1;
  } 
  else {
    Check = 0;
  }

  if(CheckAttackablePat(FA[FFBTAT('K', 0)].coords, 0) || CheckAttackablePat(FA[FFBTAT('K', 1)].coords, 1)){
    Win = 3;
  }

  DeltaTime = float(millis() - timing) / 1000;
  timing = millis();

  oldTime += DeltaTime;
  if (oldTime >= 0.2f) {
    oldTime = 0;
    second = !second;
  }

  display.fillRect(75 - k, 9, 7, 7, 1);
  if (team) {
    display.fillRect(76 - k, 10, 5, 5, 2);
  }
  
  if(TimerEnabeled){

    if(timer_mode == 1 || timer_mode == 3){
      if(TimeMinWhite < 0){
        Win = 1;
      }
      if(TimeMinBlack < 0){
        Win = 2;
      }
    }

    int8_t TeamTimeK;
    if(Win == 0){
      if (!team) {
        if(timer_mode == 2){
          TimeSecWhite += DeltaTime;
        }
        else{
          TimeSecWhite -= DeltaTime;
        }
        TeamTimeK = 0;
      } else {
        if(timer_mode == 2){
          TimeSecBlack += DeltaTime;
        }
        else{
          TimeSecBlack -= DeltaTime;
        }
        TeamTimeK = 26;
      }
    }

    if (int8_t(TimeSecWhite) >= 60) {
      TimeSecWhite = TimeSecWhite - 60;
      TimeMinWhite++;
    } else if (TimeSecWhite < 0) {
      TimeSecWhite = 60 - TimeSecWhite*-1;
      TimeMinWhite--;
    }

    if (int8_t(TimeSecBlack) >= 60) {
      TimeSecBlack = TimeSecBlack - 60;
      TimeMinBlack++;
    } else if (TimeSecBlack < 0) {
      TimeSecBlack = 60 - TimeSecBlack*-1;
      TimeMinBlack--;
    }

    if(TimeMinBlack > 99){
      TimeMinBlack = 99;
    }
    if(TimeMinWhite > 99){
      TimeMinWhite = 99;
    }

    display.drawPixel(86 - k + TeamTimeK - 2, 12 - 3, 1);
    display.drawPixel(86 - k + TeamTimeK - 2, 12 + 2 - 3, 1);
    display.drawPixel(86 - k + TeamTimeK - 2, 12 + 4 - 3, 1);
    display.drawPixel(86 - k + TeamTimeK - 2, 12 + 6 - 3, 1);
    display.drawPixel(86 - k + 24 + TeamTimeK - 2, 12 - 3, 1);
    display.drawPixel(86 - k + 24 + TeamTimeK - 2, 12 + 2 - 3, 1);
    display.drawPixel(86 - k + 24 + TeamTimeK - 2, 12 + 4 - 3, 1);
    display.drawPixel(86 - k + 24 + TeamTimeK - 2, 12 + 6 - 3, 1);

    drawNum(86 - k, 12 - 3, int8_t(TimeMinWhite) / 10);
    drawNum(91 - k, 12 - 3, int8_t(TimeMinWhite) % 10);
    drawChar(95 - k, 12 - 3, '.');
    if(int8_t(TimeSecWhite) / 10 == 6){
      drawNum(98 - k, 12 - 3, 5);
      drawNum(103 - k, 12 - 3, 9);
    }
    else{
      drawNum(98 - k, 12 - 3, int8_t(TimeSecWhite) / 10);
      drawNum(103 - k, 12 - 3, int8_t(TimeSecWhite) % 10);
    }
    display.fillRect(86 - k - 2, 12 - 3, 25, 7, 2);

    drawNum(86 - k + 26, 12 - 3, int8_t(TimeMinBlack) / 10);
    drawNum(91 - k + 26, 12 - 3, int8_t(TimeMinBlack) % 10);
    drawChar(95 - k + 26, 12 - 3, '.');
    if(int8_t(TimeSecBlack) / 10 == 6){
      drawNum(98 - k + 26, 12 - 3, 5);
      drawNum(103 - k + 26, 12 - 3, 9);
    }
    else{
      drawNum(98 - k + 26, 12 - 3, int8_t(TimeSecBlack) / 10);
      drawNum(103 - k + 26, 12 - 3, int8_t(TimeSecBlack) % 10);
    }
  }

  if (Choice) {
    CallChoice(choiceFigure);
  }

  if (input_down_once() && SelCoords % 10 < 7 && Win == 0 && !Choice) {
    SelCoords++;
    second = true;
  }
  if (input_up_once() && SelCoords % 10 > 0 && Win == 0 && !Choice) {
    SelCoords--;
    second = true;
  }
  if (input_right_once() && SelCoords / 10 < 7 && Win == 0 && !Choice) {
    SelCoords += 10;
    second = true;
  }
  if (input_left_once() && SelCoords / 10 > 0 && Win == 0 && !Choice) {
    SelCoords -= 10;
    second = true;
  }

  for (int8_t CheckPB = 0; CheckPB <= 70; CheckPB += 10) {
    if (FigureHere(CheckPB + 7) && FA[figureFind(CheckPB + 7)].type == 'P' && FA[figureFind(CheckPB + 7)].team) {
      Choice = true;

      choiceFigure = figureFind(CheckPB + 7);
      break;
    }
  }


  for (int8_t CheckPW = 0; CheckPW <= 70; CheckPW += 10) {
    if (FigureHere(CheckPW) && FA[figureFind(CheckPW)].type == 'P' && !FA[figureFind(CheckPW)].team) {
      Choice = true;
      choiceFigure = figureFind(CheckPW);
      break;
    }
  }

  drawText(80 - k, 17, F("TURN"));
  drawNum(105 - k, 17, CurrMove);

  if (!Choice) {
    if (SelectedCoords != -1) {
      drawText(75 - k, 41 + 3, F("SELECTED"));

      switch (SelectedCoords / 10) {
        case 0:
          {
            drawChar(80 - k, 48 + 3, 'A');
            break;
          }
        case 1:
          {
            drawChar(80 - k, 48 + 3, 'B');
            break;
          }
        case 2:
          {
            drawChar(80 - k, 48 + 3, 'C');
            break;
          }
        case 3:
          {
            drawChar(80 - k, 48 + 3, 'D');
            break;
          }
        case 4:
          {
            drawChar(80 - k, 48 + 3, 'E');
            break;
          }
        case 5:
          {
            drawChar(80 - k, 48 + 3, 'F');
            break;
          }
        case 6:
          {
            drawChar(80 - k, 48 + 3, 'G');
            break;
          }
        case 7:
          {
            drawChar(80 - k, 48 + 3, 'H');
            break;
          }
      }
      drawNum(85 - k, 48 + 3, mod(SelectedCoords % 10 - 8));
      if (team) {
        drawText(90 - k, 48 + 3, F(", BLACK"));
      } else {
        drawText(90 - k, 48 + 3, F(", WHITE"));
      }
      switch (FA[figureFind(SelectedCoords)].type) {
        case 'P':
          {
            drawText(80 - k, 54 + 3, F("PAWN (P)"));
            break;
          }
        case 'R':
          {
            drawText(80 - k, 54 + 3, F("ROOK (R)"));
            break;
          }
        case 'N':
          {
            drawText(80 - k, 54 + 3, F("KNIGHT (N)"));
            break;
          }
        case 'B':
          {
            drawText(80 - k, 54 + 3, F("BISHOP (B)"));
            break;
          }
        case 'Q':
          {
            drawText(80 - k, 54 + 3, F("QUEEN (Q)"));
            break;
          }
        case 'K':
          {
            drawText(80 - k, 54 + 3, F("KING (K)"));
            break;
          }
      }
    }
    drawText(75 - k, 21 + 3, F("CURSOR ON"));
    switch (SelCoords / 10) {
      case 0:
        {
          drawChar(80 - k, 28 + 3, 'A');
          break;
        }
      case 1:
        {
          drawChar(80 - k, 28 + 3, 'B');
          break;
        }
      case 2:
        {
          drawChar(80 - k, 28 + 3, 'C');
          break;
        }
      case 3:
        {
          drawChar(80 - k, 28 + 3, 'D');
          break;
        }
      case 4:
        {
          drawChar(80 - k, 28 + 3, 'E');
          break;
        }
      case 5:
        {
          drawChar(80 - k, 28 + 3, 'F');
          break;
        }
      case 6:
        {
          drawChar(80 - k, 28 + 3, 'G');
          break;
        }
      case 7:
        {
          drawChar(80 - k, 28 + 3, 'H');
          break;
        }
    }
    drawNum(85 - k, 28 + 3, mod(SelCoords % 10 - 8));

    if (FigureHere(SelCoords)) {
      if (FA[figureFind(SelCoords)].team) {
        drawText(90 - k, 28 + 3, F(", BLACK"));
      } else {
        drawText(90 - k, 28 + 3, F(", WHITE"));
      }
      switch (FA[figureFind(SelCoords)].type) {
        case 'P':
          {
            drawText(80 - k, 34 + 3, F("PAWN (P)"));
            break;
          }
        case 'R':
          {
            drawText(80 - k, 34 + 3, F("ROOK (R)"));
            break;
          }
        case 'N':
          {
            drawText(80 - k, 34 + 3, F("KNIGHT (N)"));
            break;
          }
        case 'B':
          {
            drawText(80 - k, 34 + 3, F("BISHOP (B)"));
            break;
          }
        case 'Q':
          {
            drawText(80 - k, 34 + 3, F("QUEEN (Q)"));
            break;
          }
        case 'K':
          {
            drawText(80 - k, 34 + 3, F("KING (K)"));
            break;
          }
      }
    }
  }

  if (FA[FFBTAT('K', false)].coords > 100) {
    Win = 1;
  }
  if (FA[FFBTAT('K', true)].coords > 100) {
    Win = 2;
  }


  CurrFigure = figureFind(SelCoords);
  //display.fillRect(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t)
  drawChar(16 - k, 57, 'A');
  drawChar(23 - k, 57, 'B');
  drawChar(30 - k, 57, 'C');
  drawChar(37 - k, 57, 'D');
  drawChar(44 - k, 57, 'E');
  drawChar(51 - k, 57, 'F');
  drawChar(58 - k, 57, 'G');
  drawChar(65 - k, 57, 'H');

  drawChar(10 - k, 1, '8');
  drawChar(10 - k, 8, '7');
  drawChar(10 - k, 15, '6');
  drawChar(10 - k, 22, '5');
  drawChar(10 - k, 29, '4');
  drawChar(10 - k, 36, '3');
  drawChar(10 - k, 43, '2');
  drawChar(10 - k, 50, '1');

  for (int8_t FL = 0; FL < sizeof(FA) / sizeof(Figure); FL++) {
    drawFigure(FA[FL].type, FA[FL].coords / 10, FA[FL].coords % 10, FA[FL].team);
  }

  if (Win != 0) {
    if (Win == 1) {
      drawText(80 - k, 1, F("BLACK WON"));
    } else if (Win == 2) {
      drawText(80 - k, 1, F("WHITE WON"));
    } else if (Win == 3) {
      drawText(80 - k, 1, F("DRAW"));
    }

    if (input_fire_up()) {
      if(FA[FFBTAT('K', false)].coords > 100 || FA[FFBTAT('K', true)].coords > 100){}
      else{
        Endable = true;
      }
      if (!Endable) {
        Endable = true;
      } else {
        softReset();
      }
    }
  }



  if (Check == 1 && Win == 0) {
    drawFigureHUD('K', 80 - k, 1, 1);
    drawText(90 - k, 1, F("IN CHECK"));
  } else if (Check == 2 && Win == 0) {
    drawFigureHUD('K', 80 - k, 1, 0);
    drawText(90 - k, 1, F("IN CHECK"));
  }

  drawSelector(SelCoords, second);


  if (!Win && !Choice && input_fire() && FigureHere(SelCoords) && team == FA[CurrFigure].team) {
    SelectedCoords = SelCoords;
  }
  drawSelected(SelectedCoords);
  SelFigure = figureFind(SelectedCoords);
  bool MoveAble = true;
  bool DiagMoveAble = true;
  bool KingMoveAble = true;
  bool AntiMoveAble = false;
  if (!Choice && !Win && input_fire_once() && SelCoords != FA[SelFigure].coords) {
    if (FA[SelFigure].type == 'P') {
      bool PawnSoundCant = true;
      if (!FA[SelFigure].team) {
        if (SelCoords / 10 == FA[SelFigure].coords / 10) {
          if ((FA[SelFigure].coords % 10 - SelCoords % 10 == 1) && !FigureHere(SelCoords)) {
            FigureMove();
            PawnSoundCant = false;
          }
          if(FA[SelFigure].coords % 10 - SelCoords % 10 == 2 && !FA[SelFigure].moved && !FigureHere(SelCoords) && !FigureHere(SelCoords+1)){
            FigureMove();
            PawnSoundCant = false;
          }
        }
        if (FigureHere(SelCoords) && FA[figureFind(SelCoords)].team && FA[SelFigure].coords % 10 - FA[figureFind(SelCoords)].coords % 10 == 1 && (FA[figureFind(SelCoords)].coords / 10 - FA[SelFigure].coords / 10 == 1 || FA[SelFigure].coords / 10 - FA[figureFind(SelCoords)].coords / 10 == 1)) {
          FigureMove();
          PawnSoundCant = false;
        }
        //en passant
        if (!FigureHere(SelCoords) && (SelCoords == FA[SelFigure].coords - 1 - 10 || SelCoords == FA[SelFigure].coords - 1 + 10) && FigureHere(SelCoords + 1) && FA[figureFind(SelCoords + 1)].type == 'P' && FA[figureFind(SelCoords + 1)].team && FA[figureFind(SelCoords + 1)].moved == 1 && FA[figureFind(SelCoords + 1)].coords % 10 == 3 && FA[figureFind(SelCoords + 1)].last_time_moved == CurrMove) {
          PawnMove();
          PawnSoundCant = false;
        }

      } else {
        if (SelCoords / 10 == FA[SelFigure].coords / 10) {
          if ((FA[SelFigure].coords % 10 - SelCoords % 10 == -1) && !FigureHere(SelCoords)) {
            FigureMove();
            PawnSoundCant = false;
          }
          if(FA[SelFigure].coords % 10 - SelCoords % 10 == -2 && !FA[SelFigure].moved && !FigureHere(SelCoords) && !FigureHere(SelCoords-1)){
            FigureMove();
            PawnSoundCant = false;
          }
        }
        if (FigureHere(SelCoords) && !FA[figureFind(SelCoords)].team && FA[SelFigure].coords % 10 - FA[figureFind(SelCoords)].coords % 10 == -1 && (FA[figureFind(SelCoords)].coords / 10 - FA[SelFigure].coords / 10 == 1 || FA[SelFigure].coords / 10 - FA[figureFind(SelCoords)].coords / 10 == 1)) {
          FigureMove();
          PawnSoundCant = false;
        }

        //en passant
        if (!FigureHere(SelCoords) && (SelCoords == FA[SelFigure].coords + 1 - 10 || SelCoords == FA[SelFigure].coords + 1 + 10) && FigureHere(SelCoords - 1) && FA[figureFind(SelCoords - 1)].type == 'P' && !FA[figureFind(SelCoords - 1)].team && FA[figureFind(SelCoords - 1)].moved == 1 && FA[figureFind(SelCoords - 1)].coords % 10 == 4 && FA[figureFind(SelCoords - 1)].last_time_moved == CurrMove) {
          PawnMove();
          PawnSoundCant = false;
        }
      }
      if (PawnSoundCant) {
        playSound(cant_snd, CANT_SND_LEN);
      }
    } else if (FA[SelFigure].type == 'R') {
      if (SelCoords / 10 == FA[SelFigure].coords / 10) {

        if (SelCoords % 10 > FA[SelFigure].coords % 10) {
          for (int8_t checkP = SelCoords % 10; checkP > FA[SelFigure].coords % 10; checkP--) {
            if (FigureHere(checkP + (SelCoords / 10) * 10) == true) {
              if (CurrFigure != figureFind(checkP + (SelCoords / 10) * 10)) {
                MoveAble = false;
              } else {
                if (FA[CurrFigure].team == FA[SelFigure].team) {
                  MoveAble = false;
                }
              }
            }
          }
        } else {
          for (int8_t checkP = SelCoords % 10; checkP < FA[SelFigure].coords % 10; checkP++) {
            if (FigureHere(checkP + (SelCoords / 10) * 10) == true) {
              if (CurrFigure != figureFind(checkP + (SelCoords / 10) * 10)) {
                MoveAble = false;
              } else {
                if (FA[CurrFigure].team == FA[SelFigure].team) {
                  MoveAble = false;
                }
              }
            }
          }
        }
      } else if (SelCoords % 10 == FA[SelFigure].coords % 10) {

        if (SelCoords / 10 > FA[SelFigure].coords / 10) {
          for (int8_t checkP = (SelCoords / 10) * 10; checkP > (FA[SelFigure].coords / 10) * 10; checkP = checkP - 10) {
            if (FigureHere(checkP + (SelCoords % 10)) == true) {
              if (CurrFigure != figureFind(checkP + (SelCoords % 10))) {
                MoveAble = false;
              } else {
                if (FA[CurrFigure].team == FA[SelFigure].team) {
                  MoveAble = false;
                }
              }
            }
          }
        } else {
          for (int8_t checkP = (SelCoords / 10) * 10; checkP < (FA[SelFigure].coords / 10) * 10; checkP = checkP + 10) {
            if (FigureHere(checkP + (SelCoords % 10)) == true) {
              if (CurrFigure != figureFind(checkP + (SelCoords % 10))) {
                MoveAble = false;
              } else {
                if (FA[CurrFigure].team == FA[SelFigure].team) {
                  MoveAble = false;
                }
              }
            }
          }
        }

      } else {
        MoveAble = false;
      }
      if (MoveAble == true) {
        FigureMove();
      } else {
        playSound(cant_snd, CANT_SND_LEN);
      }
    }
    if (FA[SelFigure].type == 'N') {
      if (SelCoords == FA[SelFigure].coords - 2 + 10 || SelCoords == FA[SelFigure].coords - 1 + 20 || SelCoords == FA[SelFigure].coords + 1 + 20 || SelCoords == FA[SelFigure].coords + 2 + 10 || SelCoords == FA[SelFigure].coords + 2 - 10 || SelCoords == FA[SelFigure].coords + 1 - 20 || SelCoords == FA[SelFigure].coords - 2 - 10 || SelCoords == FA[SelFigure].coords - 1 - 20) {
        if (FigureHere(SelCoords) && FA[figureFind(SelCoords)].team != FA[SelFigure].team) {
          AntiMoveAble = true;
        } else if (!FigureHere(SelCoords)) {
          AntiMoveAble = true;
        }
      }
      if (AntiMoveAble == true) {
        FigureMove();
      } else {
        playSound(cant_snd, CANT_SND_LEN);
      }
    }
    if (FA[SelFigure].type == 'B') {
      if (mod(SelCoords % 10 - FA[SelFigure].coords % 10) == mod(SelCoords / 10 - FA[SelFigure].coords / 10)) {
        int8_t diagCoords = FA[SelFigure].coords;
        while (diagCoords != SelCoords) {
          if (SelCoords / 10 - FA[SelFigure].coords / 10 < 0) {
            diagCoords -= 10;
          } else {
            diagCoords += 10;
          }
          if (SelCoords % 10 - FA[SelFigure].coords % 10 < 0) {
            diagCoords--;
          } else {
            diagCoords++;
          }
          if (FigureHere(diagCoords)) {
            if (FA[figureFind(diagCoords)].team != FA[SelFigure].team) {
              if (diagCoords != SelCoords) {
                DiagMoveAble = false;
              }
            } else {
              DiagMoveAble = false;
            }
          }
        }
      } else {
        DiagMoveAble = false;
      }
      if (DiagMoveAble) {
        FigureMove();
      } else {
        playSound(cant_snd, CANT_SND_LEN);
      }
    }
    if (FA[SelFigure].type == 'Q') {
      //Rook code
      if (SelCoords / 10 == FA[SelFigure].coords / 10) {

        if (SelCoords % 10 > FA[SelFigure].coords % 10) {
          for (int8_t checkP = SelCoords % 10; checkP > FA[SelFigure].coords % 10; checkP--) {
            if (FigureHere(checkP + (SelCoords / 10) * 10) == true) {
              if (CurrFigure != figureFind(checkP + (SelCoords / 10) * 10)) {
                MoveAble = false;
              } else {
                if (FA[CurrFigure].team == FA[SelFigure].team) {
                  MoveAble = false;
                }
              }
            }
          }
        } else {
          for (int8_t checkP = SelCoords % 10; checkP < FA[SelFigure].coords % 10; checkP++) {
            if (FigureHere(checkP + (SelCoords / 10) * 10) == true) {
              if (CurrFigure != figureFind(checkP + (SelCoords / 10) * 10)) {
                MoveAble = false;
              } else {
                if (FA[CurrFigure].team == FA[SelFigure].team) {
                  MoveAble = false;
                }
              }
            }
          }
        }
      } else if (SelCoords % 10 == FA[SelFigure].coords % 10) {

        if (SelCoords / 10 > FA[SelFigure].coords / 10) {
          for (int8_t checkP = (SelCoords / 10) * 10; checkP > (FA[SelFigure].coords / 10) * 10; checkP = checkP - 10) {
            if (FigureHere(checkP + (SelCoords % 10)) == true) {
              if (CurrFigure != figureFind(checkP + (SelCoords % 10))) {
                MoveAble = false;
              } else {
                if (FA[CurrFigure].team == FA[SelFigure].team) {
                  MoveAble = false;
                }
              }
            }
          }
        } else {
          for (int8_t checkP = (SelCoords / 10) * 10; checkP < (FA[SelFigure].coords / 10) * 10; checkP = checkP + 10) {
            if (FigureHere(checkP + (SelCoords % 10)) == true) {
              if (CurrFigure != figureFind(checkP + (SelCoords % 10))) {
                MoveAble = false;
              } else {
                if (FA[CurrFigure].team == FA[SelFigure].team) {
                  MoveAble = false;
                }
              }
            }
          }
        }

      } else {
        MoveAble = false;
      }
      //Bishop code
      if (mod(SelCoords % 10 - FA[SelFigure].coords % 10) == mod(SelCoords / 10 - FA[SelFigure].coords / 10)) {
        int8_t diagCoords = FA[SelFigure].coords;
        while (diagCoords != SelCoords) {
          if (SelCoords / 10 - FA[SelFigure].coords / 10 < 0) {
            diagCoords -= 10;
          } else {
            diagCoords += 10;
          }
          if (SelCoords % 10 - FA[SelFigure].coords % 10 < 0) {
            diagCoords--;
          } else {
            diagCoords++;
          }
          if (FigureHere(diagCoords)) {
            if (FA[figureFind(diagCoords)].team != FA[SelFigure].team) {
              if (diagCoords != SelCoords) {
                DiagMoveAble = false;
              }
            } else {
              DiagMoveAble = false;
            }
          }
        }
      } else {
        DiagMoveAble = false;
      }

      if (DiagMoveAble || MoveAble) {
        FigureMove();
      } else {
        playSound(cant_snd, CANT_SND_LEN);
      }
    }

    if (FA[SelFigure].type == 'K' && FA[SelFigure].team == team) {
      int8_t RookMove = 0;
      if (FA[SelFigure].coords / 10 - SelCoords / 10 >= -1 && FA[SelFigure].coords / 10 - SelCoords / 10 <= 1 && FA[SelFigure].coords % 10 - SelCoords % 10 >= -1 && FA[SelFigure].coords % 10 - SelCoords % 10 <= 1) {
        if (FigureHere[SelCoords] == true && FA[figureFind(SelCoords)].team == FA[SelFigure].team) {
          KingMoveAble = false;
        }
      } else if ((FA[SelFigure].coords / 10 - SelCoords / 10 == 2 || FA[SelFigure].coords / 10 - SelCoords / 10 == -2) && !FA[SelFigure].moved) {

        if (SelCoords / 10 > FA[SelFigure].coords / 10 && FHU('R', FA[SelFigure].team, 70 + FA[SelFigure].coords % 10, false)) {
          if (FigureHere(50 + FA[SelFigure].coords % 10) || FigureHere(60 + FA[SelFigure].coords % 10)) {
            KingMoveAble = false;
          } else {
            RookMove = 1;
          }

          //RookMove = 1; // aboba

        }
        //else{KingMoveAble = false;} // aboba
        else if (SelCoords / 10 < FA[SelFigure].coords / 10 && FHU('R', FA[SelFigure].team, FA[SelFigure].coords % 10, false)) {
          if (FigureHere(10 + FA[SelFigure].coords % 10) || FigureHere(20 + FA[SelFigure].coords % 10) || FigureHere(30 + FA[SelFigure].coords % 10)) {
            KingMoveAble = false;
          } else {
            RookMove = 2;
          }
        } else {
          KingMoveAble = false;
        }
      } else {
        KingMoveAble = false;
      }

      if(CheckAttackable(SelCoords, FA[SelFigure].team)){
        KingMoveAble = false;
      }

      if (KingMoveAble) {
        if (RookMove == 1) {
          FA[figureFind(70 + FA[SelFigure].coords % 10)].coords = 50 + FA[SelFigure].coords % 10;
        } else if (RookMove == 2) {
          FA[figureFind(FA[SelFigure].coords % 10)].coords = 30 + FA[SelFigure].coords % 10;
        }
        FigureMove();
      } else {
        playSound(cant_snd, CANT_SND_LEN);
      }
    }
  }

  for (i = 0; i < 8; i++) {
    display.fillRect(15 + (i * 7) - k, 50 - (i * 7), 7, 7, 2);
  }
  for (i = 0; i < 6; i++) {
    display.fillRect(15 + (i * 7) - k, 36 - (i * 7), 7, 7, 2);
  }
  for (i = 0; i < 4; i++) {
    display.fillRect(15 + (i * 7) - k, 22 - (i * 7), 7, 7, 2);
  }
  for (i = 0; i < 2; i++) {
    display.fillRect(15 + (i * 7) - k, 8 - (i * 7), 7, 7, 2);
  }

  for (i = 0; i < 6; i++) {
    display.fillRect(29 + (i * 7) - k, 50 - (i * 7), 7, 7, 2);
  }
  for (i = 0; i < 4; i++) {
    display.fillRect(43 + (i * 7) - k, 50 - (i * 7), 7, 7, 2);
  }
  for (i = 0; i < 2; i++) {
    display.fillRect(57 + (i * 7) - k, 50 - (i * 7), 7, 7, 2);
  }
  display.fillRect(15 - k, 1, 56, 56, 2);
  display.display();

  if (DrawMovesCounter == 50) {
    Win = 3;
  }
}

int8_t selectorPos = 1, selectorSubPos;
bool selectorMode;
void loopSettings() {
  display.clearDisplay();
  #ifdef SNES_CONTROLLER
  getControllerData();
  #endif

  if(selectorPos == 1){
    drawChar(5,14,'#');
    if(input_fire_once() && !selectorMode){
      timer_mode++;
      if(timer_mode == 4){
        timer_mode = 1;
      }
      if(timer_mode == 1){
        TimeMinWhite = 0;
        TimeSecWhite = 0;

        TimeMinBlack = 0;
        TimeSecBlack = 0;

        selectorSubPos = 0;

        TimerEnabeled = true;
      }
      else if(timer_mode == 2){
        TimeMinWhite = 0;
        TimeSecWhite = 0;

        TimeMinBlack = 0;
        TimeSecBlack = 0;

        selectorSubPos = 0;

        TimerEnabeled = true;
      }
      else if(timer_mode == 3){
        TimeMinWhite = 0;
        TimeSecWhite = 0;

        TimeMinBlack = 0;
        TimeSecBlack = 0;

        selectorSubPos = 0;

        TimerEnabeled = true;
      }
    }
  }
  if(selectorPos == 3){
    drawChar(47,54,'#');
  }




  drawText(34, 1, F("TIME SETTINGS"));
  drawText(12, 14, F("MODE-"));
  if (timer_mode == 1) {
    drawText(37, 14, F("TIMED"));
    
    if(selectorPos == 2){
      if(!selectorMode){
        drawChar(86 - 74 - 7,11 + 26,'#');
      }
      else{
        if(input_up_once()){
          if(selectorSubPos == 0 && int8_t(TimeMinWhite) / 10 < 9){
            TimeMinWhite+=10;
          }
          else if(selectorSubPos == 1 && int8_t(TimeMinWhite) % 10 < 9){
            TimeMinWhite++;
          }
          else if(selectorSubPos == 2 && int8_t(TimeSecWhite) / 10 < 5){
            TimeSecWhite+=10;
          }
          else if(selectorSubPos == 3 && int8_t(TimeSecWhite) % 10 < 9){
            TimeSecWhite++;
          }

          else if(selectorSubPos == 4 && int8_t(TimeMinBlack) / 10 < 9){
            TimeMinBlack+=10;
          }
          else if(selectorSubPos == 5 && int8_t(TimeMinBlack) % 10 < 9){
            TimeMinBlack++;
          }
          else if(selectorSubPos == 6 && int8_t(TimeSecBlack) / 10 < 5){
            TimeSecBlack+=10;
          }
          else if(selectorSubPos == 7 && int8_t(TimeSecBlack) % 10 < 9){
            TimeSecBlack++;
          }
        }

        else if(input_down_once()){
          if(selectorSubPos == 0 && int8_t(TimeMinWhite) / 10 > 0){
            TimeMinWhite-=10;
          }
          else if(selectorSubPos == 1 && int8_t(TimeMinWhite) % 10 > 0){
            TimeMinWhite--;
          }
          else if(selectorSubPos == 2 && int8_t(TimeSecWhite) / 10 > 0){
            TimeSecWhite-=10;
          }
          else if(selectorSubPos == 3 && int8_t(TimeSecWhite) % 10 > 0){
            TimeSecWhite--;
          }

          else if(selectorSubPos == 4 && int8_t(TimeMinBlack) / 10 > 0){
            TimeMinBlack-=10;
          }
          else if(selectorSubPos == 5 && int8_t(TimeMinBlack) % 10 > 0){
            TimeMinBlack--;
          }
          else if(selectorSubPos == 6 && int8_t(TimeSecBlack) / 10 > 0){
            TimeSecBlack-=10;
          }
          else if(selectorSubPos == 7 && int8_t(TimeSecBlack) % 10 > 0){
            TimeSecBlack--;
          }
        }

        if(input_right_once() && selectorSubPos<7){
          selectorSubPos++;
        }
        if(input_left_once() && selectorSubPos>0){
          selectorSubPos--;
        }

        if(selectorSubPos<2){
          drawArrow(12+selectorSubPos*5, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5, 44, 3, 'd');
        }
        else if(selectorSubPos>1 && selectorSubPos<4){
          drawArrow(12+selectorSubPos*5+2, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5+2, 44, 3, 'd');
        }

        else if(selectorSubPos>5 && selectorSubPos<8){
          drawArrow(12+selectorSubPos*5+19+2, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5+19+2, 44, 3, 'd');
        }
        else{
          drawArrow(12+selectorSubPos*5+19, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5+19, 44, 3, 'd');
        }
      }
      if(input_fire_once()){
        selectorMode = !selectorMode;
      }
    }

    if(input_down_once() && selectorPos<3 && !selectorMode){
      selectorPos++;
    }
    if(input_up_once() && selectorPos>1 && !selectorMode){
      selectorPos--;
    }

  }

  else if(timer_mode == 2) {
    drawText(37, 14, F("CLASSIC"));
    if(input_down_once() && selectorPos<3 && !selectorMode){
      selectorPos++;
    }
    if(input_up_once() && selectorPos>1 && !selectorMode){
      selectorPos--;
    }
    if(selectorPos == 2){
      drawChar(86 - 74 - 7,11 + 26,'#');
      if(input_fire_once()){
        TimerEnabeled = !TimerEnabeled;
      }
    }
    drawText(86 - 74 ,11 + 26,F("TIMER"));
    if(TimerEnabeled){
      drawText(86 - 74+30 ,11 + 26,F("ENABELED"));
    }
    else{
      drawText(86 - 74 +30,11 + 26,F("DISABELED"));
    }
  }

  else if (timer_mode == 3) {
    drawText(37, 14, F("INCRIMENTAL"));

    drawText(90-6, 26, F("ADDITION"));

    drawNum(86, 11+ 26, int8_t(TimeMinInc) / 10);
    drawNum(91, 11+ 26, int8_t(TimeMinInc) % 10);
    drawChar(95, 11+ 26, '.');
    drawNum(98, 11+ 26, int8_t(TimeSecInc) / 10);
    drawNum(103, 11+ 26, int8_t(TimeSecInc) % 10);
    
    if(selectorPos == 2){
      if(!selectorMode){
        drawChar(86 - 74 - 7,11 + 26,'#');
      }
      else{
        if(input_up_once()){
          if(selectorSubPos == 0 && int8_t(TimeMinWhite) / 10 < 9){
            TimeMinWhite+=10;
          }
          else if(selectorSubPos == 1 && int8_t(TimeMinWhite) % 10 < 9){
            TimeMinWhite++;
          }
          else if(selectorSubPos == 2 && int8_t(TimeSecWhite) / 10 < 5){
            TimeSecWhite+=10;
          }
          else if(selectorSubPos == 3 && int8_t(TimeSecWhite) % 10 < 9){
            TimeSecWhite++;
          }

          else if(selectorSubPos == 4 && int8_t(TimeMinBlack) / 10 < 9){
            TimeMinBlack+=10;
          }
          else if(selectorSubPos == 5 && int8_t(TimeMinBlack) % 10 < 9){
            TimeMinBlack++;
          }
          else if(selectorSubPos == 6 && int8_t(TimeSecBlack) / 10 < 5){
            TimeSecBlack+=10;
          }
          else if(selectorSubPos == 7 && int8_t(TimeSecBlack) % 10 < 9){
            TimeSecBlack++;
          }

          else if(selectorSubPos == 8 && int8_t(TimeMinInc) / 10 < 9){
            TimeMinInc+=10;
          }
          else if(selectorSubPos == 9 && int8_t(TimeMinInc) % 10 < 9){
            TimeMinInc++;
          }
          else if(selectorSubPos == 10 && int8_t(TimeSecInc) / 10 < 5){
            TimeSecInc+=10;
          }
          else if(selectorSubPos == 11 && int8_t(TimeSecInc) % 10 < 9){
            TimeSecInc++;
          }
        }

        else if(input_down_once()){
          if(selectorSubPos == 0 && int8_t(TimeMinWhite) / 10 > 0){
            TimeMinWhite-=10;
          }
          else if(selectorSubPos == 1 && int8_t(TimeMinWhite) % 10 > 0){
            TimeMinWhite--;
          }
          else if(selectorSubPos == 2 && int8_t(TimeSecWhite) / 10 > 0){
            TimeSecWhite-=10;
          }
          else if(selectorSubPos == 3 && int8_t(TimeSecWhite) % 10 > 0){
            TimeSecWhite--;
          }

          else if(selectorSubPos == 4 && int8_t(TimeMinBlack) / 10 > 0){
            TimeMinBlack-=10;
          }
          else if(selectorSubPos == 5 && int8_t(TimeMinBlack) % 10 > 0){
            TimeMinBlack--;
          }
          else if(selectorSubPos == 6 && int8_t(TimeSecBlack) / 10 > 0){
            TimeSecBlack-=10;
          }
          else if(selectorSubPos == 7 && int8_t(TimeSecBlack) % 10 > 0){
            TimeSecBlack--;
          }

          else if(selectorSubPos == 8 && int8_t(TimeMinInc) / 10 > 0){
            TimeMinInc-=10;
          }
          else if(selectorSubPos == 9 && int8_t(TimeMinInc) % 10 > 0){
            TimeMinInc--;
          }
          else if(selectorSubPos == 10 && int8_t(TimeSecInc) / 10 > 0){
            TimeSecInc-=10;
          }
          else if(selectorSubPos == 11 && int8_t(TimeSecInc) % 10 > 0){
            TimeSecInc--;
          }
        }

        if(input_right_once() && selectorSubPos<11){
          selectorSubPos++;
        }
        if(input_left_once() && selectorSubPos>0){
          selectorSubPos--;
        }

        if(selectorSubPos<2){
          drawArrow(12+selectorSubPos*5, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5, 44, 3, 'd');
        }
        else if(selectorSubPos>1 && selectorSubPos<4){
          drawArrow(12+selectorSubPos*5+2, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5+2, 44, 3, 'd');
        }

        else if(selectorSubPos>5 && selectorSubPos<8){
          drawArrow(12+selectorSubPos*5+19+2, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5+19+2, 44, 3, 'd');
        }
        else if (selectorSubPos == 4 || selectorSubPos == 5){
          drawArrow(12+selectorSubPos*5+19, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5+19, 44, 3, 'd');
        }

        else if(selectorSubPos>9 && selectorSubPos<12){
          drawArrow(12+selectorSubPos*5+34+2, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5+34+2, 44, 3, 'd');
        }
        else if (selectorSubPos == 8 || selectorSubPos == 9){
          drawArrow(12+selectorSubPos*5+34, 35, 3, 'u');
          drawArrow(12+selectorSubPos*5+34, 44, 3, 'd');
        }
      }
      if(input_fire_once()){
        selectorMode = !selectorMode;
      }
    }

    if(input_down_once() && selectorPos<3 && !selectorMode){
      selectorPos++;
    }
    if(input_up_once() && selectorPos>1 && !selectorMode){
      selectorPos--;
    }

  }

  if(timer_mode == 1 || timer_mode == 3){

    drawNum(86 - 74, 11 + 26, int8_t(TimeMinWhite) / 10);
    drawNum(91 - 74, 11 + 26, int8_t(TimeMinWhite) % 10);
    drawChar(95 - 74, 11 + 26, '.');
    drawNum(98 - 74, 11 + 26, int8_t(TimeSecWhite) / 10);
    drawNum(103 - 74, 11 + 26, int8_t(TimeSecWhite) % 10);

    drawNum(86 - 35, 11+ 26, int8_t(TimeMinBlack) / 10);
    drawNum(91 - 35 , 11+ 26, int8_t(TimeMinBlack) % 10);
    drawChar(95 - 35 , 11+ 26, '.');
    drawNum(98 - 35 , 11+ 26, int8_t(TimeSecBlack) / 10);
    drawNum(103 - 35 , 11+ 26, int8_t(TimeSecBlack) % 10);

    drawText(12, 26, F("WHITE"));
    drawText(51, 26, F("BLACK"));
  }

  drawText(54,54,F("PLAY"));

  //playSound(mus_s1_snd, MUS_S1_SND_LEN);
  display.display();
  if(selectorPos == 3){
    if(input_fire_once() && !selectorMode){
      if((timer_mode == 1||timer_mode == 3) && ((TimeSecBlack == 0 && TimeMinBlack == 0) || (TimeSecWhite == 0 && TimeMinWhite == 0))){
        playSound(cant_snd, CANT_SND_LEN);
      }
      else{
        delay(200);
        jumpTo(TEST);
      }
    }
  }
}

unsigned long last_time;
void loop(void) {


  switch (scene) {
    case INTRO:
      {
        loopIntro();
        break;
      }
    case TEST:
      {
        loopTest();
        break;
      }
    case SETTINGS:
      {
        loopSettings();
        break;
      }
  }

  if (B_READ(Globals, B_FADE_SCREEN)) {  // fade out effect
    for (uint8_t i = 0; i < GRADIENT_COUNT; i++) {
      fadeScreen(i, 0);
      display.display();
      delay(40);
    }
  }
  B_FALSE(Globals, B_EXIT_SCENE);
}