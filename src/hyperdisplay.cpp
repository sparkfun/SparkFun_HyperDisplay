/*
hyperdisplay.cpp

header file: hyperdisplay.h
*/






#include "hyperdisplay.h"







void hyperdisplay::xline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width)
{

	hyperdisplayXLineCallback( x0, y0, len, color, colorCycleLength, width);
}

void hyperdisplay::yline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width)
{

	hyperdisplayYLineCallback( x0, y0, len, color, colorCycleLength, width);
}

void hyperdisplay::rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color, bool filled)
{

	hyperdisplayRectangleCallback( x0, y0, x1, y1, color, filled);
}

void hyperdisplay::fillFromArray(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t size, color_t data[])
{

	hyperdisplayFillFromArrayCallback( x0, y0, x1, y1, size, data);
}








void hyperdisplay::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color , uint16_t width)
{

}

void hyperdisplay::polygon(uint16_t x[], uint16_t y[], uint8_t numSides, uint16_t width)
{

}

void hyperdisplay::circle(uint16_t x0, uint16_t y0, uint16_t radius, color_t color, bool filled)
{

}

void hyperdisplay::fillScreen(color_t color)
{

}













// Protected drawing functions
void hyperdisplay::lineHigh(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color[], uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width)
{
	uint16_t dy = y1 - y0;	// Guaranteed positive
  	int32_t dx = x1 - x0;
	int8_t xi = 1;

	if( dx < 0 )
	{
		xi = -1;
		dx = -dx;
	}

	int32_t D = 2*dx - dy;
	uint16_t x = x0;
	uint16_t consecutive = 0;

	for(uint8_t y = y0; y < y1; y++)
	{
		if( D > 0 )
		{
			yline(x, y-consecutive, consecutive, color, colorCycleLength, currentColorOffset, width);
			currentColorOffset = getNewColorOffset(colorCycleLength, currentColorOffset, consecutive);

		   	x = x + xi;
		   	D = D - 2*dy;
		}
		D = D + 2*dx;
	}
}
    	
void hyperdisplay::lineLow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color[], uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width)
{
	uint16_t dx = x1 - x0;	// Guaranteed positive
	int32_t dy = y1 - y0;
	int8_t yi = 1;

	if( dy < 0 )
	{
		yi = -1;
		dy = -dy;
	}
	int32_t D = 2*dy - dx;
	uint16_t y = y0;

	for(uint8_t x = x0; x < x1; x++)
	{
		if( D > 0 )
		{
			xline(x-consecutive, y, consecutive, color, colorCycleLength, currentColorOffset, width);
			currentColorOffset = getNewColorOffset(colorCycleLength, currentColorOffset, consecutive);

		   	y = y + yi;
		   	D = D - 2*dx;
		}
		D = D + 2*dy;
	}
}

uint16_t getNewColorOffset(uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t numWritten)
{
	uint16_t remainder = (numWritten % colorCycleLength);
	if(remainder < startColorOffset)
	{
		return = (remainder + colorCycleLength) - startColorOffset; 
	}
	else
	{
		return = (remainder) - startColorOffset; 
	}
}








// Callbacks default (blank) implementation
void hyperdisplayXLineCallback(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t width){}
void hyperdisplayYLineCallback(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t width){}
void hyperdisplayRectangleCallback(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color, bool filled){}
void hyperdisplayFillFromArrayCallback(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t size, color_t data[]){}
