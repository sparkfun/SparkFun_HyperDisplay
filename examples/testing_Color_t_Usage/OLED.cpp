
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

size_t OLED::write(uint8_t)		
{
  return 0;
}

// void OLED::pixel(uint16_t x0, uint16_t y0, color_t color)
// {

// }

// void OLED::xline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t width = 1)
// {

// }

// void OLED::yline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t width = 1)
// {

// }

// void OLED::rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color, bool filled = false)
// {

// }

// void OLED::fillFromArray(uint16_t x, uint16_t y, uint16_t size, color_t data[])
// {

// }


