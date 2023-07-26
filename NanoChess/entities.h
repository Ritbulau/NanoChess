#ifndef _entities_h
#define _entities_h

#include "types.h"

// Shortcuts
#define create_player(x, y)   { \
    create_coords((double) x + 0.5, (double) y + 0.5), \
    create_coords(1, 0), \
    create_coords(0, -0.66), \
    0, \
    100,  \
    10, \
    0, \
    0, \
    0, \
    0, \
    false, \
  }

#define create_key(x, y)              create_entity(E_KEY, x, y, S_STAND, 0)

// entity statuses



struct Player { 
  Coords pos;
  Coords dir;
  Coords plane;
  double velocity;
  uint8_t health;
  uint8_t keys;
  int16_t score;
  bool cheats;
};

struct Entity {
  UID uid;
  Coords pos;
  uint8_t state;
  uint8_t health;     // angle for fireballs
  uint8_t distance;
  uint8_t timer;
  bool a;
  bool b;
};

struct StaticEntity  { 
  UID uid;
  uint8_t x;
  uint8_t y;
  bool active;
};

class Figure {
public:
  char type;
  bool team;
  int8_t coords;
  uint8_t moved;
  uint8_t last_time_moved;
  Figure(char _type, bool _team, int8_t _coords, uint8_t _moved, uint8_t _last_time_moved){
    type = _type;
    team = _team;
    coords = _coords;
    moved = _moved;
    last_time_moved = _last_time_moved;
  }
};

Entity create_entity(uint8_t type, uint8_t x,  uint8_t y, uint8_t initialState, uint8_t initialHealth);
StaticEntity create_static_entity(UID uid, uint8_t x,  uint8_t y, bool active);

#endif

