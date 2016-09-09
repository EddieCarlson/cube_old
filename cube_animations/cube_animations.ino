#include <Cube.h>
#include <Dumb.h>
#include <Point.h>
#include <ColorWipe.h>

#include <OctoWS2811.h>
#include <rainbow.h>

#include <array>
#include <math.h>
#include <iterator>
#include <list>
#include <vector>
#include <deque>

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

ColorWipe colorWipe(&cube);

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
  float near = std::min(2.0, radius / 1.5);
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
  int t = tt / 5;
  
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        
      }
    }
  }  
//  for(int x = start->x - t / 2; x <= start->x + t / 2; x++) {
//    int curColor = 0;//cube.getPixel(x, 0, start->z);
//    Point colorToSet = *color;
//    if (curColor != 0) {
////      Point curColorP = intToColor(curColor);
//      colorToSet = Point(20, 150, 20);//curColorP.average(color);
//    }
//    cube.setPixel(x, t, start->z - t / 2, &colorToSet);
//    cube.setPixel(x, t, start->z + t / 2, &colorToSet);
//  }
//  for(int z = start->z - t / 2; z <= start->z + t / 2; z++) {
//    int curColor = 0;//cube.getPixel(start->x, 0, z);
//    if (z == start->z - t || z == start->z + t) curColor = 0;
//    Point colorToSet = *color;
//    if (curColor != 0) {
////      Point curColorP = intToColor(curColor);
//      colorToSet = Point(20, 150, 20);//curColorP.average(color);
//    }
//    cube.setPixel(start->x - t / 2, t, z, &colorToSet);
//    cube.setPixel(start->x + t / 2, t, z, &colorToSet);
//  }
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

void flyingSquare() {
  float t = 0;
  Point start1 = Point (rand() % 3 + 2, 0, rand() % 3 + 1);
  
  Point color1 = Point (20, 90, 160);
  while(t < 7) {
    square(&start1, &color1, t);
    cube.show();
    delay(50);
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


void manhattanSphereRad(Point *start, Point *color, float tt, int colorI) {
  
  if (tt < 0) {
    return;
  }
  
  float t = tt / 5;
  float maxDist = 4;
  float dimness = 0.4;
  
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
  delay(800);
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
        cube.setPixel(x, y, z, dimInt(rainbow[(colorIndex + ((int)((x + y + z) * 3 * breathingFactor))) % 180], 0.2));
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

dirs_t dirs[4] = {PX, PY, NX, NY};
int dirsIndex = 0;

dirs_t nonX[4] = {PY, NY, PZ, NZ};
dirs_t nonY[4] = {PX, NX, PZ, NZ};
dirs_t nonZ[4] = {PY, NY, PX, NX};

dirs_t newDir(dirs_t cur) {
  switch(cur) {
    case PX:
    case NX: nonX[rand() % 4]; break;
    case PY:
    case NY: nonY[rand() % 4]; break;
    case PZ:
    case NZ: nonZ[rand() % 4]; break;
    default: NX;
  }
}

class Snake {
public:
  std::deque<Point *> points;
  dirs_t dir;
  int age;
  Snake(Point *p, dirs_t dir) {
    points.push_front(p);
    dir = dir;
    age = 0;
  }
  void move(bool grow = false) {
    Point* newHead = points.front()->move(dir);
    points.push_front(newHead);
    if (!grow) {
      Point* last = points.back();
      delete last;
    }
  }
  int size() {
    points.size();
  }
  void setPixels(Cube c) {
    Serial.println("snake set pixel");
    Point color = Point(200, 10, 50);
    int s = points.size();
    Serial.println(s);
    for (int i = 0; i < s; i++) {
      Serial.println(points.at(i)->z);
      cube.setPixel(points.at(i), &color);
    }
  }
};

void std::__throw_out_of_range(char const*) {
  Serial.println("out of range");
}

void splitter(Point p) {
  delay(2000);
  int t = 0;
  Point color = Point(10, 200, 50);
  Point fadedColor = Point(color.x * 0.2, color.y * 0.2, color.z * 0.2);
  std::deque<Snake *> snakes;
  Point* ps = new Point(2, 2, 2);
  Snake* s = new Snake(ps, PZ);
  snakes.push_back(s);
  while(true) {
    Serial.println("while");
    int size = snakes.size();
    for(int i = 0; i < size; i++) {
      Serial.println("for");
      Snake* snake = snakes.front();
      snakes.pop_front();
      if (snake->age % 3 == 2) {
        snake->dir = newDir(snake->dir);
      }
      snake->setPixels(cube);
      snake->age = snake->age + 1;
      snake->move();
      snakes.push_back(snake);
    }
    cube.show();
    delay(250);
    cube.resetPixels();
  }
}

void std::__throw_length_error(char const*) {
  Serial.println("throwing length error");
}

void std::__detail::_List_node_base::_M_hook(std::__detail::_List_node_base*) {
  Serial.println("ok");
}

Point *randomTopPixel() {
  new Point(rand() % xSize, rand() % ySize, zSize - 1);
}

void std::__throw_bad_alloc() {
  Serial.println("hi");
}

void rain() {
  Point color = Point(250, 50, 0);
  Point fadedColor = Point(color.x * 0.1, color.y * 0.1, color.z * 0.1);

  std::deque<Point *> points;
  Point *p = randomTopPixel();
  points.push_back(p);
  while(true) {
    int size = points.size();
    Serial.println(size);
    for(int i = 0; i < size; i++) {
      Point* cur = points.front();
      points.pop_front();
      cube.setPixel(cur, &color);
      Point* a = cur->move(PZ);
      cube.setPixel(a, &fadedColor);
      delete a;
      cur->move_in_place(NZ);
      if (cube.inCube(cur)) {
        points.push_back(cur);
      } else {
        delete cur;
      }
    }
    cube.show();
    points.push_back(randomTopPixel());
    delay(100);
    cube.resetPixels();
  }
}

void snake() {
  Point color = Point(10, 200, 50);
  Point fadedColor = Point(color.x * 0.1, color.y * 0.1, color.z * 0.1);

  std::deque<Point *> points;
  Point *start = new Point(2, 2, 2);
  points.push_back(start);
}

void setXY(int z) {
  Point color = Point(30, 0, 255);
  Point intersect = Point(0, 255, 0);
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      int cur = cube.getPixel(x, y, z);
      if (cur != 0) {
        cube.setPixel(x, y, z, &intersect);
      } else {
        cube.setPixel(x, y, z, &color);
      }
    }
  }
}

void setYZ(int x) {
  Point color = Point(30, 0, 255);
  Point intersect = Point(0, 255, 0);
  for(int z = 0; z < zSize; z++) {
    for(int y = 0; y < ySize; y++) {
      int cur = cube.getPixel(x, y, z);
      if (cur != 0) {
        cube.setPixel(x, y, z, &intersect);
      } else {
        cube.setPixel(x, y, z, &color);
      }
    }
  }
}

void dualSweep() {
  for(int i = 0; i < zSize; i++) {
    setXY(i);
    setYZ(i);
    cube.show();
    delay(100);
    cube.resetPixels();
  }
}

void drawXZLine(int y, int z) {
  Serial.println("drawXZLine");
  Serial.println(y);
  Serial.println(z);
  Point color = Point(30, 0, 255);
  for (int x = 0; x < xSize; x++) {
    //for (int zi = 0; zi <= z; zi++) {
      cube.setPixel(x, y, z, &color);
    //}
  }
}

int zeez[7] = {3, 4, 5, 6, 5, 4, 3};

void wave() {
  delay(2000);
  float t = 0;
  float pi = 3.14159;
  Serial.println(sin(pi));
  Serial.println(sin(1));
  Serial.println(sin(180));
  while(true) {
    for(float y = 0; y < ySize; y++) {
      int z = (zSize / 2) + (3 * sin((y * 1.25 / pi) + t));
      drawXZLine((int) y, (int) z);
    }
    cube.show();
    delay(100);
    cube.resetPixels();
    t += 0.5;
  }
}



RainDrop* randomTopDrop() {
  Point *p = randomTopPixel();
  Point *c = new Point(220 + (rand() % 25), rand() % 20, rand() % 20);
  return new RainDrop(p, c, (rand() % 4) + 2);
}

RainDrop* randomTopDrop(Point* color) {
  Point *p = randomTopPixel();
  return new RainDrop(p, color, (rand() % 4) + 2);
}

void theMatrix(long maxTime) {
  Point color = Point(250, 50, 0);
  Point fadedColor = Point(color.x * 0.1, color.y * 0.1, color.z * 0.1);

  std::deque<RainDrop *> drops;
  RainDrop* d = randomTopDrop();
  drops.push_back(d);
  int colorI = 0;
  while(millis() < maxTime) {
    int size = drops.size();
    for(int i = 0; i < size; i++) {
      RainDrop* cur = drops.front();
      drops.pop_front();
      Point* curPix = cur->point;
      cube.setPixel(curPix, cur->color);
      Point* a = curPix->move(PZ);
      cube.setPixel(a, cur->fadedColor);
      a->move_in_place(PZ);
      cube.setPixel(a, cur->moreFadedColor);
      a->move_in_place(PZ);
      cube.setPixel(a, cur->mostFadedColor);
      cur->move();
      if (cube.inCube(cur->point) || cube.inCube(a)) {
        drops.push_back(cur);
      } else {
        delete cur;
      }
      delete a;
    }
    cube.show();
    if (rand() % 4 > 1) {
      int colorIrand = (colorI + rand() % 14) % 180;
      drops.push_back(randomTopDrop(new Point(rainbow[colorIrand])));
      if (rand() % 4 == 0) {
        colorI = (colorI + 1) % 180;
      }
    }
    delay(32);
    cube.resetPixels();
  }


  while (drops.size() > 0) {
    RainDrop* cur = drops.front();
    drops.pop_front();
    delete cur;
  }
}

long animationMillis = 0;
long animationDuration = 180000; // 3 minutes
long twoMinutes = 120000; // 2 minutes

void rainbowFadeWrapper() {
  animationMillis = millis();
  long rainbowDur = animationDuration;
  while(millis() - animationMillis < rainbowDur) {
    rainbowFade(colorI);
    colorI = (colorI + 1) % 180;
  }
}

void sphereWrapper() {
  animationMillis = millis();
  while(millis() - animationMillis < animationDuration) {
    manhattanSphere();
  }
}

void matrixWrapper() {
  theMatrix(millis() + animationDuration);
}

void colorWipeWrapper() {
  animationMillis = millis();
  while(millis() - animationMillis < twoMinutes) {
    colorWipe.colorWipe();
  }
}

bool redSet[xSize][ySize][zSize];

int sortIndexRed;
int sortIndexGreen;

void sortOne() {
  int rz;
  int ry;
  int rx;
  do {
    rz = sortIndexRed % zSize;
    ry = (sortIndexRed / zSize) % ySize;
    rx = (sortIndexRed / (zSize * ySize)) % zSize;
    sortIndexRed++;
  } while(redSet[rx][ry][rz]);

  int gz;
  int gy;
  int gx;
  do {
    gz = sortIndexGreen % zSize;
    gy = (sortIndexGreen / zSize) % ySize;
    gx = (sortIndexGreen / (zSize * ySize)) % xSize;
    sortIndexGreen--;
  } while(!redSet[gx][gy][gz]);

  cube.setPixel(gx, gy, gz, 0, 255, 0);
  cube.setPixel(rx, ry, rz, 255, 0, 0);
  Serial.print("sortIndexRed: ");
  Serial.println(sortIndexRed);
  Serial.print("sortIndexGreen: ");
  Serial.println(sortIndexGreen);
  Serial.println();
  cube.show();
}

void cubeSort() {
  sortIndexRed = 0;
  sortIndexGreen = xSize * ySize * zSize - 1;
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        redSet[x][y][z] = rand() % 2 == 1;
      }
    }
  }

  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        if(redSet[x][y][z]) {
          cube.setPixel(x, y, z, 255, 0, 0);
        } else {
          cube.setPixel(x, y, z, 0, 255, 0);
        }
      }
    }
  }
  cube.show();

  while(sortIndexRed < sortIndexGreen) {
    sortOne();
    delay(25);
  }
  Serial.println("DONE!");
  delay(1000);
}


void loop() {
  rainbowFadeWrapper();
  sphereWrapper();
  matrixWrapper();
  colorWipeWrapper();
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


