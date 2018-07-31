
#include "OLED.h"

// Here is the function of interest - Need to show an OLED color typedef that was passed using color_t
void OLED::printColor(color_t color){
  OLED_colorTyeDef theOLEDColor = *((OLED_colorTyeDef *) color);

  Serial.print("OLED Color Typedef: R = ");
  Serial.print(theOLEDColor.R);
  Serial.print(", G = ");
  Serial.print(theOLEDColor.G);
  Serial.print(", B = ");
  Serial.println(theOLEDColor.B);
}


// And there are a lot of virtual functions that need to be implemented to make it possible to construct an object of the OLED class...
OLED::OLED()
{

}

char_info_t * OLED::getCharInfo(uint8_t val)
{
  return NULL;
}

void OLED::pixel(uint16_t x0, uint16_t y0, color_t color)
{

}


