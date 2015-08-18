#include <stdlib.h>
#include <Cube.h>

Cube::Cube(OctoWS2811 ledsIn, char x, char y, char z) {
  leds = &ledsIn;

  xSize = x;
  ySize = y;
  zSize = z;
}

void Cube::show() {
  leds->show();
}

void Cube::setPixel(int x, int y, int z, Point *c) {
  setPixel(x, y, z, c->x, c->y, c->z);
}

void Cube::setPixel(int x, int y, int z, int color) {
  if (inCube(x, y, z)) {
    leds->setPixel(cube[x][y][z], color);
  }
}

void Cube::setPixel(const Point *p, Point *c) {
  setPixel(p->x, p->y, p->z, c->x, c->y, c->z);
}

void Cube::setPixel(Point *p, int r, int g, int b) {
  setPixel(p->x, p->y, p->z, r, g, b);
}

bool Cube::inCube(int x, int y, int z) {
  return x < xSize && y < ySize && z < zSize && x >= 0 && y >= 0 && z >= 0;
}

bool Cube::inCube(Point *p) {
  return inCube(p->x, p->y, p->z);
}

void Cube::setPixel(int x, int y, int z, int r, int g, int b) {
  if (inCube(x, y, z)) {
    leds->setPixel(cube[x][y][z], r, g, b);
  }
}

int Cube::getPixel(int x, int y, int z) {
  if (inCube(x, y, z)) {
    return leds->getPixel(cube[x][y][z]);
  }
  return 0;
}

// TODO: make more efficient. memset? all elements would need to be all adjacent
void Cube::resetPixels() {
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        leds->setPixel(cube[x][y][z], 0, 0, 0);
      }
    }
  }
}

int Cube::updateX(int x, int panel) {
  if (panel % 2 == 0)
    return x + 1;
  else
    return x - 1;
}

// TODO: use new instead of malloc. use int[][][] instead of int***??
unsigned short *** Cube::build_cube(int xSize, int ySize, int zSize) {
  unsigned short ***cube;
  size_t xMalloc = sizeof(unsigned short *) * xSize;
  size_t yMalloc = sizeof(unsigned short *) * ySize;
  size_t zMalloc = sizeof(unsigned short *) * zSize;
  cube = (unsigned short ***) malloc(xMalloc);
  for(int x = 0; x < xSize; x++) {
    cube[x] = (unsigned short **) malloc(yMalloc);
    for(int y = 0; y < ySize; y++) {
      cube[x][y] = (unsigned short *) malloc(zMalloc);
    }
  }
  return cube;
}

void Cube::setUp(int strandsPerPanel, int startBurn, int bottomBurn, int endBurn) {
  leds->begin();
  cube = build_cube(xSize, ySize, zSize);
  int index = 0;
  int x = 0;
  for(int panel = 0; panel < ySize; panel++) { // panels per cube
    for(int strand = 0; strand < strandsPerPanel; strand++) {
      index += startBurn; // burn begining of strand
      for(int upDown = 0; upDown < (xSize / 2); upDown++) {  // up/down column pairs per cube
        Serial.println("up");
        if (upDown != 0) {
          x = updateX(x, panel); // bottom of column, translate 1 in x dir if not on the first column-set
        }
        for(int colIndex = 0; colIndex < zSize; colIndex++) { // assign column upwards
          cube[x][panel][colIndex] = index;
          index++;
        }
        Serial.println("down");
        x = updateX(x, panel); // top of column, translate 1 in x direction
        for(int colIndex = zSize - 1; colIndex >= 0 ; colIndex--) { // assign column downwards
          cube[x][panel][colIndex] = index;
          index++;
        }
        index += bottomBurn;
      }
      index += endBurn - bottomBurn;
    }
  }
}
