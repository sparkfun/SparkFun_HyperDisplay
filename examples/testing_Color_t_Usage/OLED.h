
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
  
  char_info_t * getCharInfo(uint8_t val);
  void pixel(uint16_t x0, uint16_t y0, color_t color);
};

#endif /* OLED_H */
