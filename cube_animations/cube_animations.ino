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

int top_burn = 0;
int bottomBurn = 3;
int startBurn = 2;
int colsPerStrand = 6;
int endBurn = 0;
int strandsPerPanel = 1;

const int xSize = 6;
const int ySize = 7;
const int zSize = 7;

//int top_burn = 0;
//int bottomBurn = 1;
//int startBurn = 3;
//int colsPerStrand = 6;
//int endBurn = 2;
//int strandsPerPanel = 1;
//
//const int xSize = 6;
//const int ySize = 1;
//const int zSize = 7;

//int ***cube;
Cube cube(leds, xSize, ySize, zSize);


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
  cube.setUp(strandsPerPanel, startBurn, bottomBurn, endBurn);
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
           int curColor = cube.getPixel(x, y, z);
           Point colorToSet = *color;
           if (curColor != 0) {
//             Point curColorP = intToColor(curColor);
             colorToSet = Point(20, 150, 20);//curColorP.average(color);
           }
           cube.setPixel(x, y, z, colorToSet.x, colorToSet.y, colorToSet.z);
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
    int curColor = 0;//cube.getPixel(x, 0, start->z);
    Point colorToSet = *color;
    if (curColor != 0) {
//      Point curColorP = intToColor(curColor);
      colorToSet = Point(20, 150, 20);//curColorP.average(color);
    }
    cube.setPixel(x, 0, start->z - t, &colorToSet);
    cube.setPixel(x, 0, start->z + t, &colorToSet);
  }
  for(int z = start->z - t; z <= start->z + t; z ++) {
    int curColor = 0;//cube.getPixel(start->x, 0, z);
    if (z == start->z - t || z == start->z + t) curColor = 0;
    Point colorToSet = *color;
    if (curColor != 0) {
//      Point curColorP = intToColor(curColor);
      colorToSet = Point(20, 150, 20);//curColorP.average(color);
    }
    cube.setPixel(start->x - t, 0, z, &colorToSet);
    cube.setPixel(start->x + t, 0, z, &colorToSet);
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
            cube.setPixel(&p, &color);
          }
        }
      }
    }
    cube.show();
    delay(100);
    cube.resetPixels();
  }
}

float near = 0.52;



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
    cube.show();
    delay(200);
    cube.resetPixels();
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
    cube.show();
    delay(22);
    cube.resetPixels();
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
  
  float t = tt / 5;
  float maxDist = 4;
  float dimness = 0.2;
  
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        int dist = abs(abs((x - start->x)) + abs((y - start->y)) + abs((z - start->z)) - t);
        if (dist < maxDist) {
          float factor = (abs(maxDist - dist) / maxDist);
          factor = factor * factor * factor * factor * factor;
          cube.setPixel(x, y, z, dimInt(rainbow[colorI], factor * dimness));
        }
//         if (abs(dist - t) < 0.05) { // unnecessary if
//           cube.setPixel(x, y, z, rainbow[colorI]);
//         } else if (abs(dist - t) < 2) {
//           // TODO: this factor is wrong, I think it should be: (2 - abs(t - dist)) / 2
//          cube.setPixel(x, y, z, dimInt(rainbow[colorI], ((float) t - dist) / t));  
//         }
      }
    }
  }
}

int colorI = 0;

void manhattanSphere() {
  Point start = Point (rand() % xSize, rand() % ySize, rand() % zSize);
  
  for (int t = 0; t < 100; t++) {
      manhattanSphereRad(&start, &start, t, colorI);
      //manhattanSphereRad(&start, &start, t - 50, colorI);
      cube.show();
      delay(9);
      colorI = (colorI + 2) % 180;
      cube.resetPixels();
    }
  //delay(1000);
}

void dimTest() {
  //Point color = Point(200, 100, 50);
  int color = rainbow[50];
  float fade = 0.5;
  for(int x = 0; x < xSize; x++) {
    for(int z = 0; z < zSize; z++) {
      cube.setPixel(x, 0, z, color);
    }
    color = dimInt(color, fade);
    //Point(color.x * fade, color.y * fade, color.z * fade);
  } 
  cube.show();
  delay(1000);
  cube.resetPixels();
}

float breathingFactor = 1;
bool up = true;

void updateBreathingFactor() {
  if (breathingFactor >= 1.75) {
    up = false;
    Serial.println("top");
  } else if (breathingFactor <= 0.5) {
    Serial.println("bottom");
    up = true; 
  }
  if (up) {
    breathingFactor += 0.003;
  } else {
    breathingFactor -= 0.003;
  }
}

void rainbowFade(int colorIndex) {
  updateBreathingFactor();
  
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        cube.setPixel(x, y, z, dimInt(rainbow[(colorIndex + ((int)((x + y + z) * 3 * breathingFactor))) % 180], 0.15));
        //cube.setPixel(x, y, z, 10, 255, 255);
      }
    }
  }
  cube.show();
  delay(20);
}


void farFadeTest(int colorIndex) {
  float maxDist = 3;
  Point start = Point(2, 0, 3);
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        int dist = abs(x - start.x) + abs(y - start.y) + abs(z - start.z);
        if (dist < maxDist) {
          float factor = (abs(maxDist - dist) / maxDist);
          factor = factor * factor * factor;
          cube.setPixel(x, y, z, dimInt(rainbow[50], factor));
        }
      }
    }
  }
  cube.show();
  delay(20);
}

char dirs[4] = {0, 4, 1, 5};
int dirsIndex = 0;

void splitter(Point p) {
  int t = 0;
  while(true) {
    Point color = Point(10, 200, 50);
    Point fadedColor = Point(color.x * 0.2, color.y * 0.2, color.z * 0.2);
    cube.setPixel(&p, &fadedColor);
    if (t % 3 == 2) {
      dirsIndex = (dirsIndex + 1) % 4;
    }
    Point newP = p.move(dirs[dirsIndex]);
    cube.setPixel(&newP, &color);
    cube.show();
    delay(200);
    cube.resetPixels();
    t++;
    p = newP;
  }
}
    
void loop() {
  //Serial.println("loop");
  //circle(2, 0.07);
  //rain2();
  //explodeCube();
  ///doubleStarburst();
  //test();
  //doubleSquare();
  //dimTest();
  //rainbowFade(colorI);
  //farFadeTest(colorI);
  //colorI = (colorI + 1) % 180;
  manhattanSphere();
  //splitter(Point(2, 0, 2));
}


//void circle(int r, float tStep) {
//  float t = 0;
//
//  while(true) {
//    float x = r * cos(t) + 2;
//    float y = r * sin(t) + 2;
////    float x = r * (cos(t) - cos(2 * t)) + 2;
////    float y = r * (sin(t) - sin(2 * t)) + 2;
//    Serial.println(x);
//    Serial.println(y);
//    float topX = min(max(0, floor(x + near)), xSize - 1);
//    float botX = min(max(0, ceil(x - near)), xSize - 1);
//    float topY = min(max(0, floor(y + near)), zSize - 1);
//    float botY = min(max(0, ceil(y - near)), zSize - 1);
//    Serial.println(topX);
//    Serial.println(botX);
//    Serial.println(topY);
//    Serial.println(botY);
//
//    for(int xi = botX; xi <= topX; xi++) {
//      for(int yi = botY; yi <= topY; yi++) {
//        if (abs(xi - x) < near)
//          leds.setPixel(cube[xi][0][yi], 250, 0, 126);
//      }
//    }
//    leds.show();
//    delay(5);
//    for(int xi = botX; xi <= topX; xi++) {
//      for(int yi = botY; yi <= topY; yi++) {
//        leds.setPixel(cube[xi][0][yi], 0, 0, 0);
//      }
//    }
//    leds.show();
//    t = t + tStep;
//  }
//}


//void fillSpace(Point one, Point two) {
//  for(int x = min(one.x, two.x); x <= max(one.x, two.x); x++) {
//    for(int y = min(one.y, two.y); y <= max(one.y, two.y); y++) {
//      for(int z = min(one.z, two.z); y <= max(one.z, two.z); z++) {
//        cube.setPixel(x, y, z);
//      }
//    }
//  }



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
