/*
  HyperDisplay Example 1:  simpleBareNecessities 
  By: Owen Lye
  SparkFun Electronics
  Date: August 17, 2018
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Don't expect too much from this code: it just prints a nice message to the serial terminal...
  However it demonstrates instantiation of a class derived from the HyperDisplay library. Once
  you implement the functions:
  - getOffsetColor(color_t base, uint32_t numPixels);
  - hwpixel(uint16_t x0, uint16_t y0, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset);

  Then you will be able to use all these standardized functions (and more to come in the near future!)
  - xline
  - yline
  - rectangle
  - fillFromArray
  - fillWindow
  - line
  - polygon
  - circle
    
*/

#include "bareNecessities.h"  // Click here to get the library: http://librarymanager/SparkFun_HyperDisplay

#define NUM_PIX_X 64
#define NUM_PIX_Y 64

// Note: this won't make any displays work because the functions are not implemented, however it shows that this implementation is instantiable
bareMinDerived myMinimalDisplay(NUM_PIX_X, NUM_PIX_Y);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Example1: Simple Bare Necessities");
  Serial.println("Well, this is a good launch point in case you want to make your own class of displays underneath the hyperdisplay library. Have fun! :D");
}

void loop() {
  // put your main code here, to run repeatedly:

}
