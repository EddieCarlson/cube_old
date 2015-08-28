#include <stdlib.h>
#include <MovingCube.h>
#include <Dumb.h>

//// Functional derived constructor example
//MovingCube::MovingCube(Cube* theCube)  : Animation(theCube) {
//  Serial.println("This thing on?");
//}
//
//
//
//void MovingCube::explodeCube() {
//  int xSize = 6;
//  int ySize = 7;//theCube->ySize;
//  int zSize = 7;//theCube->zSize;
//  Serial.println(xSize);
//  Serial.println(ySize);
//  Serial.println(zSize);
//
//  Point start = Point (rand() % (xSize - 2) + 1, ySize / 2, rand() % (zSize - 2) + 1);
//  Point color = Point(190, 70, 40);
//  for(int t = 0; t < 10; t++) {
//    for(int x = 0; x < xSize; x++) {
//      for(int y = 0; y < ySize; y++) {
//        for(int z = 0; z < zSize; z++) {
//          Point p = Point(x, y, z);
//          if (isOnCube(start, t, p)) {
//            Serial.println("On the cube");
//            theCube->setPixel(&p, &color);
//          }
//        }
//      }
//    }
//    Serial.println("Bout to go black");
//    theCube->show();
//    Serial.println("I never cam back");
//    delay(100);
//    theCube->resetPixels();
//  }
//}