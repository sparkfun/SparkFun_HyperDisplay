
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

size_t EInk::write(uint8_t)
{
  return 0; // Do nothing hahahahahaha
}

// void EInk::pixel(uint16_t x0, uint16_t y0, color_t color)
// {

// }

// void EInk::xline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t width = 1)
// {

// }

// void EInk::yline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t width = 1)
// {

// }

// void EInk::rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color, bool filled = false)
// {

// }

// void EInk::fillFromArray(uint16_t x, uint16_t y, uint16_t size, color_t data[])
// {

// }
