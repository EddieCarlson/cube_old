#include <Cube.h>

#include <OctoWS2811.h>
#include <array>
#include <math.h>


int ledPin = 13;

const int ledsPerStrip = 350;
const int numStrips = 2;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

//int top_burn = 0;
//int bottomBurn = 3;
//int startBurn = 2;
//int colsPerStrand = 6;
//int endBurn = 0;
//int strandsPerPanel = 1;
//
//const int xSize = 6;
//const int ySize = 7;
//const int zSize = 7;

int top_burn = 0;
int bottomBurn = 1;
int startBurn = 3;
int colsPerStrand = 6;
int endBurn = 2;
int strandsPerPanel = 1;

const int xSize = 6;
const int ySize = 1;
const int zSize = 7;

int ***cube;
Cube kewb(leds, xSize, ySize, zSize);


int rainbow[180];

void setupRainbow() {
  for (int i=0; i<180; i++) {
    int hue = i * 2;
    int saturation = 100;
    int lightness = 50;
    
    rainbow[i] = makeColor(hue, saturation, lightness);
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  setupRainbow();
  kewb.setUp(strandsPerPanel, startBurn, bottomBurn, endBurn);
}


void solidCircle(Point *start, Point *color, float t) {
  if (t < 0) {
    return;
  }
  float radius = sqrt(t);
  float near = min(2, radius / 1.5);
  float r2 = radius * radius;
  
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
         if (abs((x - start->x) * (x - start->x) + (y - start->y) * (y - start->y) + (z - start->z) * (z - start->z) - r2) < near * near) {
           int curColor = kewb.getPixel(x, y, z);
           Point colorToSet = *color;
           if (curColor != 0) {
//             Point curColorP = intToColor(curColor);
             colorToSet = Point(20, 150, 20);//curColorP.average(color);
           }
           kewb.setPixel(x, y, z, colorToSet.x, colorToSet.y, colorToSet.z);
         }
      }
    }
  }
}

// TODO: why do i have to define these? Shouldn't they come in from Math.h?
int min(int a, int b) {
 if (a <= b)
   return a; 
 else
   return b;
}

int max(int a, int b) {
 if (a > b)
   return a; 
 else
   return b;
}

void square(Point *start, Point *color, float tt) {
  int t = tt;
  for(int x = start->x - t; x <= start->x + t; x ++) {
    int curColor = 0;//kewb.getPixel(x, 0, start->z);
    Point colorToSet = *color;
    if (curColor != 0) {
//      Point curColorP = intToColor(curColor);
      colorToSet = Point(20, 150, 20);//curColorP.average(color);
    }
    kewb.setPixel(x, 0, start->z - t, &colorToSet);
    kewb.setPixel(x, 0, start->z + t, &colorToSet);
  }
  for(int z = start->z - t; z <= start->z + t; z ++) {
    int curColor = 0;//kewb.getPixel(start->x, 0, z);
    if (z == start->z - t || z == start->z + t) curColor = 0;
    Point colorToSet = *color;
    if (curColor != 0) {
//      Point curColorP = intToColor(curColor);
      colorToSet = Point(20, 150, 20);//curColorP.average(color);
    }
    kewb.setPixel(start->x - t, 0, z, &colorToSet);
    kewb.setPixel(start->x + t, 0, z, &colorToSet);
  }
}


bool isOnCube(Point center, int cubeRad, Point p) {
  return (abs(p.x - center.x) == cubeRad && abs(p.y - center.y) <= cubeRad && abs(p.z - center.z) <= cubeRad) ||
    (abs(p.y - center.y) == cubeRad && abs(p.x - center.x) <= cubeRad && abs(p.z - center.z) <= cubeRad) ||
    (abs(p.z - center.z) == cubeRad && abs(p.x - center.x) <= cubeRad && abs(p.y - center.y) <= cubeRad);

}

void explodeCube() {
  Point start = Point (rand() % (xSize - 2) + 1, ySize / 2, rand() % (zSize - 2) + 1);
  Point color = Point(190, 70, 40);
  for(int t = 0; t < 10; t++) {
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        for(int z = 0; z < zSize; z++) {
          Point p = Point(x, y, z);
          if (isOnCube(start, t, p)) {
            kewb.setPixel(&p, &color);
          }
        }
      }
    }
    kewb.show();
    delay(100);
    kewb.resetPixels();
  }
}

float near = 0.52;

void circle(int r, float tStep) {
  float t = 0;

  while(true) {
    float x = r * cos(t) + 2;
    float y = r * sin(t) + 2;
//    float x = r * (cos(t) - cos(2 * t)) + 2;
//    float y = r * (sin(t) - sin(2 * t)) + 2;
    Serial.println(x);
    Serial.println(y);
    float topX = min(max(0, floor(x + near)), xSize - 1);
    float botX = min(max(0, ceil(x - near)), xSize - 1);
    float topY = min(max(0, floor(y + near)), zSize - 1);
    float botY = min(max(0, ceil(y - near)), zSize - 1);
    Serial.println(topX);
    Serial.println(botX);
    Serial.println(topY);
    Serial.println(botY);

    for(int xi = botX; xi <= topX; xi++) {
      for(int yi = botY; yi <= topY; yi++) {
        if (abs(xi - x) < near)
          leds.setPixel(cube[xi][0][yi], 250, 0, 126);
      }
    }
    leds.show();
    delay(5);
    for(int xi = botX; xi <= topX; xi++) {
      for(int yi = botY; yi <= topY; yi++) {
        leds.setPixel(cube[xi][0][yi], 0, 0, 0);
      }
    }
    leds.show();
    t = t + tStep;
  }
}

Point intToColor(int c) {
  short red = (c << 16) & 0xFF;
  short green = (c << 8) & 0xFF;
  short blue = c & 0xFF;
  return Point(red, green, blue);
}

void doubleSquare() {
  float t = 0;
  Point start1 = Point (rand() % 3 + 2, ySize / 2, rand() % 3 + 1);
  Point start2 = Point (rand() % 3 + 1, ySize / 2, rand() % 3 + 3);
  
  Point color1 = Point (20, 90, 160);
  Point color2 = Point (190, 20, 40);
  while(t < 7) {
    square(&start1, &color1, t);
    square(&start2, &color2, t);
    kewb.show();
    delay(200);
    kewb.resetPixels();
    t++;
  }
}

void doubleStarburst() {
  float t = 0;
  int diff = 0;//rand() % 10 + 20; 
  //Point start1 = Point (rand() % (xSize - 2) + 2, ySize / 2, rand() % (zSize - 2) + 2);
  //Point start2 = Point (rand() % (xSize - 2), ySize / 2, rand() % (zSize - 2));
  Point start1 = Point (rand() % 2 + 3, ySize / 2, rand() % 2 + 1);
  Point start2 = Point (rand() % 2 + 1, ySize / 2, rand() % 2 + 4);

  Point color1 = Point (20, 90, 170);
  Point color2 = Point (170, 90, 20);
  while(t < 50) {
    solidCircle(&start1, &color1, t);
    solidCircle(&start2, &color2, t - diff);
    kewb.show();
    delay(22);
    kewb.resetPixels();
    t++;
  }
}

void test() {
  Point color = Point(rand() % 256, rand() % 256, rand() % 256);
  for (int i = 0; i < 50; i++) {
    leds.setPixel(i, color.x, color.y, color.z);
    leds.show();
    delay(50);
  }
  for (int i = 0; i < 50; i++) {
    leds.setPixel(i, 0, 0, 0);
  }
}

//void fillSpace(Point one, Point two) {
//  for(int x = min(one.x, two.x); x <= max(one.x, two.x); x++) {
//    for(int y = min(one.y, two.y); y <= max(one.y, two.y); y++) {
//      for(int z = min(one.z, two.z); y <= max(one.z, two.z); z++) {
//        kewb.setPixel(x, y, z);
//      }
//    }
//  }

int dimInt(int i, float f) {
  int red = (i >> 16) & 0xFF;
  int green = (i >> 8) & 0xFF;
  int blue = i & 0xFF;
  red *= f;
  green *= f;
  blue *= f;
  return ((red << 16) & 0xFF0000) | ((green << 8) & 0x00FF00) | (blue & 0xFF);
}

void manhattanSphereRad(Point *start, Point *color, float tt, int colorI) {
  
  if (tt < 0) {
    return;
  }
  
  float t = tt / 3;
  
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        int dist = abs((x - start->x)) + abs((y - start->y)) + abs((z - start->z));
         if (abs(dist - t) < 0.05) { // unnecessary if
           kewb.setPixel(x, y, z, rainbow[colorI]);
         } else if (abs(dist - t) < 2) {
           // TODO: this factor is wrong, I think it should be: (2 - abs(t - dist)) / 2
          kewb.setPixel(x, y, z, dimInt(rainbow[colorI], ((float) t - dist) / t));  
         }
      }
    }
  }
}



int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
	unsigned int red, green, blue;
	unsigned int var1, var2;

	if (hue > 359) hue = hue % 360;
	if (saturation > 100) saturation = 100;
	if (lightness > 100) lightness = 100;

	// algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
	if (saturation == 0) {
		red = green = blue = lightness * 255 / 100;
	} else {
		if (lightness < 50) {
			var2 = lightness * (100 + saturation);
		} else {
			var2 = ((lightness + saturation) * 100) - (saturation * lightness);
		}
		var1 = lightness * 200 - var2;
		red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
		green = h2rgb(var1, var2, hue) * 255 / 600000;
		blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
	}
	return (red << 16) | (green << 8) | blue;
}

float colorFactor = 1.0;

unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
	if (hue < 60) return (v1 * 60 + (v2 - v1) * hue) * colorFactor;
	if (hue < 180) return (v2 * 60) * colorFactor;
	if (hue < 240) return (v1 * 60 + (v2 - v1) * (240 - hue)) * colorFactor;
	return v1 * 60;
}

int colorI = 0;

void manhattanSphere() {
  Point start = Point (rand() % (xSize / 2) + (xSize / 3), 1, rand() % (zSize / 2) + (zSize / 3));
  
  for (int t = 0; t < 80; t++) {
      manhattanSphereRad(&start, &start, t, colorI);
      //manhattanSphereRad(&start, &start, t - 50, colorI);
      kewb.show();
      delay(20);
      colorI = (colorI + 1) % 180;
      kewb.resetPixels();
    }
  delay(1000);
}
    
void loop() {
  //Serial.println("loop");
  //circle(2, 0.07);
  //rain2();
  //explodeCube();
  ///doubleStarburst();
  //test();
  //doubleSquare();
//  Point c = Point(255, 0, 0);
//  for(int i = 0; i < xSize; i++) {
//    Point y0 = Point(i, 0, 2);
//    Point y1 = Point(i, 1, 2);
//    
//    kewb.setPixel(&y0, &c);
//    kewb.setPixel(&y1, &c);
//    leds.show();
//    delay(1000);
//    kewb.resetPixels();
//  }
  manhattanSphere();
//  Point start = Point(2, 0, 2);
//  Point occupied[300] = Point(0, 0, 0);
//  occupied[0] = start;
//  occupied[1] = start.move(0);
//  short start_dir = 0;
//  wander(occupied, 2, start_dir);
}

