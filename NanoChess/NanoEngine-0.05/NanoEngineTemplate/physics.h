
#include <stdint.h>
#include "constants.h"

//basic Collider with the shape of box
class PhysBoxCollider
{
public:
  //up-left
  uint8_t pos1[2];

  //down-left
  uint8_t pos2[2];

  //up-right
  uint8_t pos3[2];

  //down-right
  uint8_t pos4[2];
  
  bool ScreenCollideXleft();
  bool ScreenCollideXright();

  bool ScreenCollideYup();
  bool ScreenCollideYdown();

  bool TestCollide();

  void MoveCollider(int8_t x, int8_t y)
  {
    pos1[0] += x;
    pos1[1] += y;

    pos2[0] += x;
    pos2[1] += y;

    pos3[0] += x;
    pos3[1] += y;

    pos4[0] += x;
    pos4[1] += y;
  }
};