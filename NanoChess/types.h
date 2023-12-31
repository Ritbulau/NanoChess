#ifndef _types_h
#define _types_h

#define UID_null  0

// Entity types (legend applies to level.h)
#define E_FLOOR             0x0   // . (also null)
#define E_WALL              0xF   // #
#define E_DOOR              0xD   
#define E_DOOR2             0xA   
#define E_DOOR3             0xB   
#define E_COLL              0xC   
#define E_PLAYER            0x1   // P
#define E_ENEMY             0x2   // E
#define E_EXIT              0x7   // X
// collectable entities >= 0x8
#define E_MEDIKIT           0x8   // M
#define E_KEY               0x9   // K


typedef uint16_t UID;
typedef uint8_t  EType;

struct Coords {
  double x;
  double y;
};

UID create_uid(EType type, uint8_t x, uint8_t y);
EType uid_get_type(UID uid);

Coords create_coords(double x, double y);
uint8_t coords_distance(Coords* a, Coords* b);

#endif
