/*

Font Handling:
This is an example of how to add font handling to a HyperDisplay driver.

*/
#include "fontDriver.h"   // This is just an example - it doesn't control any real displays. Read the source code
                          // Click here to get HyperDisplay (top level): http://librarymanager/All#SparkFun_HyperDisplay 
                                           
#define SERIAL_PORT Serial

fontDriver myDisplay; // Create an object of your display type

struct{
  
}color;   // You'll need to handle colors yourself

void setup() {
  SERIAL_PORT.begin(9600);
  SERIAL_PORT.println("Example1 Display Test Template");

  //  myDisplay.begin();  // If necessary
//  myDisplay.clearDisplay();

//  myDisplay.setTextCursor(0,0);            // Sets the cursor relative to the current window, however the current (default) window is equivalent to the whole display. (0,0) is the upper left corner and (myDisplay.xExt-1, myDisplay.yExt-1) is the lower right
//  myDisplay.setCurrentWindowColorSequence((color_t)&color);
  
//    myDisplay.setTextCursor(0,0);
//    myDisplay.println("Hello world!");   // Prints using the font implementation that we created in 'fontHandler.h/cpp'
}

void loop() {
  
}