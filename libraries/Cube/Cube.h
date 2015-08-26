#ifndef cube_h
#define cube_h


#include <OctoWS2811.h>
#include <Point.h>


// should internal cube representation really be int***? or int[][][]?
class Cube {
private:
	// seems like maybe better style for this to be the actual object, not pointer. no reason to
	// use this outside of the cube. Maybe have the cube construct it instead.
	OctoWS2811 *leds;
	int ***cube;
	int updateX(int x, int panel);
public:
	Cube(OctoWS2811 _leds, int x, int y, int z);
	int *** build_cube(int xSize, int ySize, int zSize); // should be private probably
	void setPixel(int x, int y, int z, int r, int g, int b);
	// TODO: alias point as Color? (char x,y,z === char r,g,b)
	void setPixel(int x, int y, int z, Point *c);
	void setPixel(Point *p, int r, int g, int b);
	void setPixel(int x, int y, int z, int color);
	void setPixel(const Point *p, Point *c);
	void setUp(int strandsPerPanel, int startBurn, int bottomBurn, int endBurn);
	void resetPixels();
  	void show();
	int getPixel(int x, int y, int z);
	bool inCube(int x, int y, int z);
	bool inCube(Point *p);
	int xSize;
	int ySize;
	int zSize;
};

#endif