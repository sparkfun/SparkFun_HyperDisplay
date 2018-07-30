#include "display.h"


typedef enum{
  R = 0,
  B,
  W
}EInk_colorTyeDef;

class EInk : public display{
  public:

  EInk();
  
  void printColor(color_t color);
  void printEInkColor(EInk_colorTyeDef Ecolor);
  EInk_colorTyeDef getEInkColor(color_t color);

  size_t write(uint8_t);
};

void EInk::printColor(color_t color){
  EInk_colorTyeDef theEInkColor = getEInkColor(color);
  printEInkColor(theEInkColor);
}

void EInk::printEInkColor(EInk_colorTyeDef Ecolor)
{
  Serial.println(Ecolor);
}

EInk_colorTyeDef EInk::getEInkColor(color_t color)
{
  EInk_colorTyeDef * pEInkColor = (EInk_colorTyeDef *) color;
  EInk_colorTyeDef retval = *(pEInkColor);
  return retval;
}

size_t EInk::write(uint8_t)
{
  return 0; // Do nothing hahahahahaha
}

EInk::EInk()
{
  
}













typedef struct{
  byte R, G, B;
}OLED_colorTyeDef;

class OLED : public display{
  public:

  OLED();
  
  void printColor(color_t color);
  
  size_t write(uint8_t);
};

void OLED::printColor(color_t color){
  OLED_colorTyeDef theOLEDColor = *((OLED_colorTyeDef *) color);

  Serial.print(theOLEDColor.R);
  Serial.print(", ");
  Serial.print(theOLEDColor.G);
  Serial.print(", ");
  Serial.println(theOLEDColor.B);
}

size_t OLED::write(uint8_t)
{
  return 0; // Do nothing hahahahahaha
}


OLED::OLED()
{
  
}
















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
