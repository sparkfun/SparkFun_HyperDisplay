#include "bareNecessities.h"

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
