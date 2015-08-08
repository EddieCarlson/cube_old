#include <OctoWS2811.h>

// TODO: shorts should be (unsigned?) chars (no way is a cube gonna be > 255 on a side)
class Point {
  public:
  short x;
  short y;
  short z;

  Point(short _x, short _y, short _z) {
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
public:
	Cube(OctoWS2811 _leds, int x, int y, int z);
	int *** build_cube(int xSize, int ySize, int zSize); // should be private probably
	void setPixel(int x, int y, int z, int r, int g, int b);
	void setPixel(int x, int y, int z, Point *c);
	void setPixel(Point *p, int r, int g, int b);
	// TODO: alias point as Color? (char x,y,z could easily be char r,g,b)
	void setPixel(Point *p, Point *c);
	void setPixel(int x, int y, int z, int color);
	// TODO: begin should be private and called in constructor
	void begin(int xSize, int ySize, int zSize, int strandsPerPanel, int startBurn, int bottomBurn, int endBurn);
	void resetPixels();
	int getPixel(int x, int y, int z);
	bool inCube(int x, int y, int z);
	bool inCube(Point *p);
	int update_xx(int x, int panel);
	int xSize;
	int ySize;
	int zSize;
	// TODO: could be unsigned shorts, not gonna have more than 65k lights in the cube
	int ***cube;
};
