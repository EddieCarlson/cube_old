#ifndef colorwipe_cpp
#define colorwipe_cpp

#include <OctoWS2811.h>

#include <Cube.h>
#include <Dumb.h>
#include <Point.h>

#include <array>
#include <math.h>
#include <iterator>
#include <list>
#include <vector>
#include <deque>
#include <rainbow.h>

class ColorWipe {
private:
  int prevDir;
  int newDir();
  int colorWipeIndex;

public:
  Cube *cube;

  char xSize;
  char ySize;
  char zSize;

  ColorWipe(Cube *cubeIn);

  void setYZPlane(int x, Point* color);

  void setXYPlane(int z, Point* color);

  void setXZPlane(int y, Point* color);

  void xWipe(Point* color);

  void negxWipe(Point* color);

  void yWipe(Point* color);

  void negyWipe(Point* color);

  void zWipe(Point* color);

  void negzWipe(Point* color);

  void colorWipe();
};

#endif
