
#ifndef EINK_H
#define EINK_H

#include "hyperdisplay.h"

typedef enum{
  R = 0,
  B,
  W
}EInk_colorTyeDef;

class EInk : public hyperdisplay{
  public:

  EInk();
  
  void printColor(color_t color);
  void printEInkColor(EInk_colorTyeDef Ecolor);
  EInk_colorTyeDef getEInkColor(color_t color);

  size_t write(uint8_t);
  void pixel(uint16_t x0, uint16_t y0, color_t color);
};

#endif /* EINK_H */
