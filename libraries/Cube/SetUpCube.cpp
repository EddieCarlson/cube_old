
#include <stdlib.h>
#include <OctoWS2811.h>
#include <SetUpCube.h>

Cube::Cube(OctoWS2811 leds_in, int x, int y, int z) {
  leds = &leds_in;
  xSize = x;
  ySize = y;
  zSize = z;
  // TODO: how is this commented out? wat?
  //cube = build_cube(xSize, ySize, zSize);
}

void Cube::setPixel(int x, int y, int z, Point *c) {
  setPixel(x, y, z, c->x, c->y, c->z);
}

void Cube::setPixel(int x, int y, int z, int color) {
  if (inCube(x, y, z)) {
    leds->setPixel(cube[x][y][z], color);
  }
}

void Cube::setPixel(Point *p, Point *c) {
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

// TODO: make more efficient. memset? all ints would need to be all adjacent
void Cube::resetPixels() {
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        leds->setPixel(cube[x][y][z], 0, 0, 0);
      }
    }
  }
}

int Cube::update_xx(int x, int panel) {
  if (panel % 2 == 0)
    return x + 1;
  else
    return x - 1;
}

// TODO: use new instead of malloc. use int[][][] instead of int***??
int *** Cube::build_cube(int xSize, int ySize, int zSize) {
  int ***cube;
  size_t xMalloc = sizeof(int *) * xSize;
  size_t yMalloc = sizeof(int *) * ySize;
  size_t zMalloc = sizeof(int *) * zSize;
  cube = (int ***) malloc(xMalloc);
  for(int x = 0; x < xSize; x++) {
    cube[x] = (int **) malloc(yMalloc);
    for(int y = 0; y < ySize; y++) {
      cube[x][y] = (int *) malloc(zMalloc);
    }
  }
  return cube;
}

void Cube::begin(int xSize, int ySize, int zSize, int strandsPerPanel, int startBurn, int bottomBurn, int endBurn) {
  cube = build_cube(xSize, ySize, zSize);
  int index = 0;
  int x = 0;
  for(int panel = 0; panel < ySize; panel++) { // panels per cube
    for(int strand = 0; strand < strandsPerPanel; strand++) {
      index += startBurn; // burn begining of strand
      for(int upDown = 0; upDown < (xSize / 2); upDown++) {  // up/down column pairs per cube
        Serial.println("up");
        if (upDown != 0) {
          x = update_xx(x, panel); // bottom of column, translate 1 in x dir if not on the first column-set
        }
        for(int colIndex = 0; colIndex < zSize; colIndex++) { // assign column upwards
          cube[x][panel][colIndex] = index;
          index++;
        }
        Serial.println("down");
        x = update_xx(x, panel); // top of column, translate 1 in x direction
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