#include "hyperdisplay.h"

#include "EInk.h"
#include "OLED.h"

EInk  myEInk;
OLED  myOLED;

void setup() {
  // put your setup code here, to run once:

}

color_t makeColorTypeFromEInkColor(EInk_colorTyeDef Ecolor)
{
  EInk_colorTyeDef * EcolorPointer = &Ecolor;
  return (color_t)EcolorPointer;
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

  color_t myColorType = makeColorTypeFromEInkColor(W);
   myEInk.printColor(myColorType);

  OLED_colorTyeDef oColor;
  oColor.R = 255;
  oColor.G = 127;
  oColor.B = 3;

  myOLED.printColor((color_t)(&oColor));
}
