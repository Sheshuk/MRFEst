#ifndef __RAYTRACE__H
#define __RAYTRACE__H
#include "Terrain.h"
class Terrain;
class vect3;
///----------------------- ray tracing ---------------
class RayTrace{
public:
  RayTrace(vect3 pos, vect3 dir,Terrain* Land=0):fPos(pos),fDir(dir),fLand(Land){};
  double find_crossing(vect3, vect3, double, double);
  double GetThickness();
  double GetDistance();
public:
  vect3 fPos;
  vect3 fDir;
  Terrain* fLand;
};
#endif
