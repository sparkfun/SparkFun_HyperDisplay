#include "hyperdisplay.h"

class SomeDisplay : public hyperdisplay{
  private:
  protected:
  public:

  SomeDisplay(); // Constructor

  // Bare minimum implementation for classes derived from hyperdisplay, if they are to be instantiated
  void pixel(uint16_t x0, uint16_t y0, color_t color);
  size_t write(uint8_t b);
  
  
};

SomeDisplay::SomeDisplay()
{
  // A constructor for the class
}

void SomeDisplay::pixel(uint16_t x0, uint16_t y0, color_t color)
{
  // Here you include the code necessary to write a single pixel to the given location with the given color
  // This should be enough to get your display working, but it is not very efficient. If possible you should
  // implement some of the other primitive drawing types such as xline, yline, rectangle, and fillFromArray
  // That way you reduce overhead.
}

size_t SomeDisplay::write(uint8_t b)
{
  return 0;
}

SomeDisplay myDisp;


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
