
#ifndef OLED_H
#define OLED_H

#include "hyperdisplay.h"

typedef struct{
  byte R, G, B;
}OLED_colorTyeDef;


class OLED : public hyperdisplay{
  public:

  OLED();
  
  void printColor(color_t color);
  
  size_t write(uint8_t);
  void pixel(uint16_t x0, uint16_t y0, color_t color);
};

#endif /* OLED_H */
