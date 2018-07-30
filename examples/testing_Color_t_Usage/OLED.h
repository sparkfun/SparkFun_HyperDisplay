
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
};

#endif /* OLED_H */