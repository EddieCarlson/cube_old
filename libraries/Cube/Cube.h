#include <OctoWS2811.h>

class Point {
  public:
  char x;
  char y;
  char z;

  Point(char _x, char _y, char _z) {
    x = _x;
    y = _y;
    z = _z;
  }

  // Passing by value would copy? doesn't really matter here...what's the accepted convention?
  Point average(Point *other) {
  	return Point((x + other->x) / 2, (y + other->y) / 2, (z + other->z) / 2);
  }

  // TODO: dir should be enum
  // TODO: should new the returned points. (ugh but someone has to free them later. so lame. any way around that?)
  // 	   could return int and bitshift, but that also sucks
  Point move(short dir) {
  	switch(dir) {
      case 0: return Point(x + 1, y, z); break;
      case 1: return Point(x - 1, y, z); break;
      case 2: return Point(x, y + 1, z); break;
      case 3: return Point(x, y - 1, z); break;
      case 4: return Point(x, y, z + 1); break;
      case 5: return Point(x, y, z - 1); break;
      default: return Point(-1, -1, -1);
  	}
  }

  bool equals(Point o) {
  	return x == o.x && y == o.y && z == o.z;
  }
};

// should internal cube representation really be int***? or int[][][]?
class Cube {
private:
	// seems like maybe better style for this to be the actual object, not pointer. no reason to
	// use this outside of the cube. Maybe have the cube construct it instead.
	OctoWS2811 *leds;
	unsigned short ***cube;
	int updateX(int x, int panel);
public:
	Cube(OctoWS2811 _leds, char x, char y, char z);
	unsigned short *** build_cube(int xSize, int ySize, int zSize); // should be private probably
	void setPixel(int x, int y, int z, int r, int g, int b);
	// TODO: alias point as Color? (char x,y,z === char r,g,b)
	void setPixel(int x, int y, int z, Point *c);
	void setPixel(Point *p, int r, int g, int b);
	void setPixel(int x, int y, int z, int color);
	void setPixel(Point *p, Point *c);
	void setUp(int strandsPerPanel, int startBurn, int bottomBurn, int endBurn);
	void resetPixels();
  void show();
	int getPixel(int x, int y, int z);
	bool inCube(int x, int y, int z);
	bool inCube(Point *p);
	char xSize;
	char ySize;
	char zSize;
};
