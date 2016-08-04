#ifndef rainbow_h
#define rainbow_h

extern int rainbow[180];

extern unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue);

extern int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness);

extern void setupRainbow();

extern int dimInt(int i, float f);

#endif
