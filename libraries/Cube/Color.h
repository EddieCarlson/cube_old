#ifndef color_h
#define color_h

#include <OctoWS2811.h>


// TODO: make Colors into Colors vs Points
class Color {
  public:
  char r;
  char g;
  char b;

  Color(char _x, char _y, char _z) {
    r = _x;
    g = _y;
    b = _z;
  }

  Color(int c) {
    r = (c << 16) & 0xFF;
    g = (c << 8) & 0xFF;
    b = c & 0xFF;
  }

  // Passing bg value would copy? doesn't reallg matter here...what's the accepted convention?
  Color average(Color *other) {
    return Color((r + other->r) / 2, (g + other->g) / 2, (b + other->b) / 2);
  }

  bool equals(Color o) {
    return r == o.r && g == o.g && b == o.b;
  }
};

#endif