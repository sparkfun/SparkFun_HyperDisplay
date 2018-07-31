
#include "EInk.h"

// Function of interst - need to use the EInk_colorTyeDef variable type in a function specific to the EInk class using a function that takes color_t
void EInk::printColor(color_t color){
  EInk_colorTyeDef theEInkColor = getEInkColor(color);
  printEInkColor(theEInkColor);
}

// This is the function that needs to take an EInk_colorTyeDef type
void EInk::printEInkColor(EInk_colorTyeDef Ecolor)
{
	Serial.print("EInk color: ");
	switch(Ecolor)
	{
		case R : Serial.println("R"); break;
		case W : Serial.println("W"); break;
		default :
		case B : Serial.println("B"); break;
	}
}

// This function is just a complicated way to cast the color_t type to the EInk_colorTyeDef
EInk_colorTyeDef EInk::getEInkColor(color_t color)
{
  EInk_colorTyeDef * pEInkColor = (EInk_colorTyeDef *) color;
  EInk_colorTyeDef retval = *(pEInkColor);
  return retval;
}




// And here are required functions so that we can make an instance of the class (pure virtual functions from parents)
EInk::EInk()
{
  
}

char_info_t * EInk::getCharInfo(uint8_t val)
{
  return NULL;
}

void EInk::pixel(uint16_t x0, uint16_t y0, color_t color)
{

}


