#ifndef _constants_h
#define _constants_h

// Key pinout
#define USE_INPUT_PULLUP
#define K_LEFT              6
#define K_RIGHT             7
#define K_UP                8
#define K_DOWN              3
#define K_FIRE              10

// functions for bit-read and bit-write
#define B_TRUE(bp,bb)    bp |= bb
#define B_FALSE(bp,bb)   bp &= ~(bb)
#define B_READ(bp,bb)    bool(bp & bb)

// SNES Controller
// uncomment following line to enable snes controller support
// #define SNES_CONTROLLER
constexpr uint8_t DATA_CLOCK   = 11;
constexpr uint8_t DATA_LATCH   = 12;
constexpr uint8_t DATA_SERIAL  = 13;

// Sound
constexpr uint8_t SOUND_PIN   = 9; // do not change, belongs to used timer

// GFX settings
#define OPTIMIZE_SSD1306                // Optimizations for SSD1366 displays

#define FRAME_TIME          66.666666   // Desired time per frame in ms (66.666666 is ~15 fps)
#define RES_DIVIDER         2           // Higher values will result in lower horizontal resolution when rasterize and lower process and memory usage

// Level 
#define LEVEL_WIDTH_BASE    6
#define LEVEL_WIDTH         (1 << LEVEL_WIDTH_BASE)
#define LEVEL_HEIGHT        57
#define LEVEL_SIZE          LEVEL_WIDTH / 2 * LEVEL_HEIGHT

// scenes
#define INTRO                 0
#define TEST                  1
#define SETTINGS              2

// Game

#define MAX_ENTITIES          12           // Max num of active entities
#define MAX_STATIC_ENTITIES   24          // Max num of entities in sleep mode


constexpr uint8_t SCREEN_WIDTH     =  128;
constexpr uint8_t SCREEN_HEIGHT    =  64;
constexpr uint8_t HALF_WIDTH       =  SCREEN_WIDTH/2;
//constexpr uint8_t RENDER_HEIGHT    =  56;         // raycaster working height (the rest is for the hud)
constexpr uint8_t HALF_HEIGHT      =  SCREEN_HEIGHT/2;

#endif
