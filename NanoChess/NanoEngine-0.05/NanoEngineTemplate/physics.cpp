#include <stdint.h>
#include "constants.h"

#include "physics.h"

/*

There is easy way to calculate physics detection.
 its simply:

    pos*[x] can move only left and right, so i do 
    pos1[x] < 1 to check left and pos4[x] > SCREEN_WIDTH to check right

    pos*[y] can move only up and down, so i do 
    pos1[1] < 1 to check up and pos4[1] > SCREEN_HEIGHT to check down

    -celisej567
 
(pos1) > .____________. < (pos3)
         |           |
         |           |                     
         |           |
         |           |
         |           |
         |           |
         |___________|
         '           '
         ^           ^
       (pos2)      (pos4)  

*/

bool PhysBoxCollider::ScreenCollideXleft()
{
  return pos1[0] < 1;
}
bool PhysBoxCollider::ScreenCollideXright()
{
  return pos4[0] > SCREEN_WIDTH;
}

bool PhysBoxCollider::ScreenCollideYup()
{
  return pos1[1] < 1;
}
bool PhysBoxCollider::ScreenCollideYdown()
{
  return pos4[1] > SCREEN_HEIGHT;
}

bool PhysBoxCollider::TestCollide()
  {
    if(pos1[0] < 1 || pos1[1] < 1 || pos4[0] > SCREEN_WIDTH || pos4[1] > SCREEN_HEIGHT)
    {
      return true;
    }

    return false;
  }
