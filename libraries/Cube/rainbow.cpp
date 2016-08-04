#include <rainbow.h>

int rainbow[180];

extern unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
	if (hue < 60) return (v1 * 60 + (v2 - v1) * hue);
	if (hue < 180) return (v2 * 60);
	if (hue < 240) return (v1 * 60 + (v2 - v1) * (240 - hue));
	return v1 * 60;
}


extern int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
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

extern void setupRainbow() {
  int saturation = 100;
  int lightness = 50;
  for (int i=0; i<180; i++) {
    int hue = i * 2;
    rainbow[i] = makeColor(hue, saturation, lightness);
  }
}

extern int dimInt(int i, float f) {
  int red = (i >> 16) & 0xFF;
  int green = (i >> 8) & 0xFF;
  int blue = i & 0xFF;
  red *= f;
  green *= f;
  blue *= f;
  return ((red << 16) & 0xFF0000) | ((green << 8) & 0x00FF00) | (blue & 0xFF);
}
