/*

Display Test Template:
This is not expected to work without some configiration by the user, namely to implement a derived display class and choose some colors

*/
#include "hyperdisplay.h"   // Replace this line with the header file for your particular implementation
                            // Click here to get HyperDisplay (top level): http://librarymanager/All#SparkFun_HyperDisplay 
                                           
#define SERIAL_PORT Serial

//myDisplayClass myDisplay; // Create an object of your display type

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
//    myDisplay.println("Hello world!");   // Prints using the default font at the cursor location
}

void loop() {
  
  lineTest();
  delay(500);

  rectTest();
  delay(500);

  circleTest();
  delay(500);
}

void lineTest( void )
{
//  myDisplay.clearDisplay();
//
//  for(hd_hw_extent_t indi = 0; indi < myDisplay.xExt; indi++)
//  {
////    myDisplay.line(0,0,indi,myDisplay.yExt-1,1,(color_t)&color);
//  }
//  
//  for(hd_hw_extent_t indi = 0; indi < myDisplay.yExt; indi++)
//  {
////    myDisplay.line(0,myDisplay.yExt-1,myDisplay.xExt-1,myDisplay.yExt-indi-1,1,(color_t)&color);
//  }
//
//  for(hd_hw_extent_t indi = 0; indi < myDisplay.xExt; indi++)
//  {
////    myDisplay.line(myDisplay.xExt-1,myDisplay.yExt-1,myDisplay.xExt-indi-1,0,1,(color_t)&color);
//  }
//
//  for(hd_hw_extent_t indi = 0; indi < myDisplay.yExt; indi++)
//  {
////    myDisplay.line(myDisplay.xExt-1,0,0,indi,1,(color_t)&color);
//  }
}

void rectTest( void )
{
////  myDisplay.clearDisplay();
//
//  for(uint8_t indi = 0; indi < myDisplay.xExt/2; indi+=1)
//  {
////    myDisplay.rectangle(myDisplay.xExt/2-1-indi, myDisplay.yExt/2-1-indi, myDisplay.xExt/2+1+indi, myDisplay.yExt/2+1+indi, false, (color_t)&color);
//    delay(50);
//  }
//
//  for(uint8_t indi = 0; indi < myDisplay.xExt/2; indi+=1)
//  {
////    myDisplay.rectangle(myDisplay.xExt/2-1-indi, myDisplay.yExt/2-1-indi, myDisplay.xExt/2+1+indi, myDisplay.yExt/2+1+indi, false, (color_t)&color);
//    delay(50);
//  }
//
//  for(uint8_t indi = 0; indi < myDisplay.xExt/2; indi+=1)
//  {
////    myDisplay.rectangle(myDisplay.xExt/2-1-indi, myDisplay.yExt/2-1-indi, myDisplay.xExt/2+1+indi, myDisplay.yExt/2+1+indi, false, (color_t)&color);
//    delay(50);
//  }
}

void circleTest( void )
{
//  myDisplay.clearDisplay();
  
//  for(uint8_t indi = 0; indi < (myDisplay.xExt/2 - 1); indi++)
//  {
////    myDisplay.circle((myDisplay.xExt/2 - 1),(myDisplay.yExt/2 - 1),(myDisplay.xExt/2 - 1)-indi, true, (color_t)&color);
//    delay(100);
//  }
}
