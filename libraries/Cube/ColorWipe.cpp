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
#include <ColorWipe.h>
#include <rainbow.h>

ColorWipe::ColorWipe(Cube *cubeIn) {
  cube = cubeIn;

  xSize = cube->xSize;
  ySize = cube->ySize;
  zSize = cube->zSize;

  colorWipeIndex = rand() % 180;
  setupRainbow();
}

void ColorWipe::setYZPlane(int x, Point* color) {
  for(int y = 0; y < ySize; y++) {
    for(int z = 0; z < zSize; z++) {
      cube->setPixel(x, y, z, color);
    }
  }
  cube->show();
}

void ColorWipe::setXYPlane(int z, Point* color) {
  for(int y = 0; y < ySize; y++) {
    for(int x = 0; x < xSize; x++) {
      cube->setPixel(x, y, z, color);
    }
  }
  cube->show();
}

void ColorWipe::setXZPlane(int y, Point* color) {
  for(int z = 0; z < zSize; z++) {
    for(int x = 0; x < xSize; x++) {
      cube->setPixel(x, y, z, color);
    }
  }
  cube->show();
}

void ColorWipe::xWipe(Point* color) {
  for(int x = 0; x < xSize; x++) {
    setYZPlane(x, color);
    delay(100);
  }
}

void ColorWipe::negxWipe(Point* color) {
  for(int x = xSize - 1; x >= 0; x--) {
    setYZPlane(x, color);
    delay(100);
  }
}

void ColorWipe::yWipe(Point* color) {
  for(int y = 0; y < ySize; y++) {
    setXZPlane(y, color);
    delay(100);
  }
}

void ColorWipe::negyWipe(Point* color) {
  for(int y = ySize - 1; y >= 0; y--) {
    setXZPlane(y, color);
    delay(100);
  }
}

void ColorWipe::zWipe(Point* color) {
  for(int z = 0; z < zSize; z++) {
    setXYPlane(z, color);
    delay(100);
  }
}

void ColorWipe::negzWipe(Point* color) {
  for(int z = zSize - 1; z >= 0; z--) {
    setXYPlane(z, color);
    delay(100);
  }
}

int prevDir = 0;

int ColorWipe::newDir() {
  int newDir = rand() % 6;
  while(newDir == prevDir) {
    newDir = rand() % 6;
  }
  prevDir = newDir;
  return newDir;
}

void ColorWipe::colorWipe() {
  colorWipeIndex = (colorWipeIndex + 60 + (rand() % 60)) % 180;
  Point* p = new Point(dimInt(rainbow[colorWipeIndex], 0.4));
  int nDir = newDir();
  switch(nDir) {
    case 0:
      xWipe(p);
      break;
    case 1:
      negxWipe(p);
      break;
    case 2:
      yWipe(p);
      break;
    case 3:
      negyWipe(p);
      break;
    case 4:
      zWipe(p);
      break;
    case 5:
      negzWipe(p);
      break;
    default:
      break;
  }

  delete p;
  delay(20);
}
