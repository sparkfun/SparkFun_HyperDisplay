/*
hyperdisplay.cpp

header file: hyperdisplay.h
*/






#include "hyperdisplay.h"







void hyperdisplay::xline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width)
{
	for(uint16_t indi = 0; indi < len; indi++)
	{
		pixel(x0 + indi, y0, *(color + startColorOffset++));
		if(startColorOffset >= colorCycleLength)
		{
			startColorOffset = 0;
		}
	}
	hyperdisplayXLineCallback( x0, y0, len, color, colorCycleLength, startColorOffset, width);
}

void hyperdisplay::yline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width)
{
	for(uint16_t indi = 0; indi < len; indi++)
	{
		pixel(x0 , y0 + indi, *(color + startColorOffset));
		if(startColorOffset >= colorCycleLength)
		{
			startColorOffset = 0;
		}
	}
	hyperdisplayYLineCallback( x0, y0, len, color, colorCycleLength, startColorOffset, width);
}

void hyperdisplay::rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color, bool filled)
{
	if(x0 > x1)
	{ 
		SWAP_COORDS(x0, x1);
	}
	
	if(y0 > y1)
	{ 
		SWAP_COORDS(y0, y1);
	}

	uint16_t xlen = x1 - x0;
	if(filled)
	{
		for(uint16_t y = y0; y < y1; y++)
		{
			xline(x0, y, xlen, &color, 1, 0, 1);
		}
	}
	else
	{
		uint16_t ylen = y1 - y0;

		xline(x0, y0, xlen, &color, 1, 0, 1);
		xline(x0, y1, xlen, &color, 1, 0, 1);
		yline(x0, y0, ylen, &color, 1, 0, 1);
		yline(x1, y0, ylen, &color, 1, 0, 1);
	}
	hyperdisplayRectangleCallback( x0, y0, x1, y1, color, filled);
}

void hyperdisplay::fillFromArray(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t size, color_t data[])
{
	if(x0 > x1)
	{ 
		SWAP_COORDS(x0, x1);
	}
	
	if(y0 > y1)
	{ 
		SWAP_COORDS(y0, y1);
	}

	uint32_t pixel_count = 0;
	for(uint16_t x = x0; x < x1; x++)
	{
		for(uint16_t y = y0; y < y1; y++)
		{
			pixel(x, y, *(data + pixel_count++));
			if(pixel_count >= size)
			{
				pixel_count = 0;
			}
		}
	}
	hyperdisplayFillFromArrayCallback( x0, y0, x1, y1, size, data);
}



// FYI this virtual function can be overwritten. It is just the most basic default version
size_t hyperdisplay::write(uint8_t val)
{
	size_t numWritten = 0;

	char_info_t * pCharacter = getCharInfo(val);
	if(pCharacter->show)
	{
		// Code to show the character
		
		if(pCurrentWindow != NULL)		// Make sure there is a valid window object
		{
			if(pCharacter->causedNewline)		// Check if the character is meant to cause a new line
			{
				pCurrentWindow->cursorX = pCurrentWindow->xMin;		// Reset the x cursor to the beginning
				if((pCurrentWindow->yMax - pCharacter->yDim) < pCurrentWindow->cursorY)			// Check if the y cursor will runn off the screen
				{
					pCharacter->show = false;						// If so then don't show the character
					pCurrentWindow->cursorY += pCharacter->yDim;	// And also only increment the cursor if within the proper bounds to avoid a long-term wraparound condition
				}
			}
			else if((pCurrentWindow->xMax - pCharacter->xDim) < pCurrentWindow->cursorX)	// If the character does not trigger newlines then make sure there is still room for it left-to-right
			{
				pCharacter->causedNewline = true;					// If there is not then it needs to cause a newline
				pCurrentWindow->cursorX = pCurrentWindow->xMin;		// Go through the same process as above to check bounds
				if((pCurrentWindow->yMax - pCharacter->yDim) < pCurrentWindow->cursorY)
				{
					pCharacter->show = false;
					pCurrentWindow->cursorY += pCharacter->yDim;
				}
			}
			else	// If will be entirely within bounds then simply increment the x cursor
			{
				pCurrentWindow->cursorX += pCharacter->xDim;
			}
			// Write data to display, but only if it is still meant to be shown
			if(pCharacter->show)
			{
				fillFromArray(pCurrentWindow->cursorX, pCurrentWindow->cursorY, pCurrentWindow->cursorX+pCharacter->xDim, pCurrentWindow->cursorY+pCharacter->yDim, pCharacter->dataSize, pCharacter->pdata);
				numWritten = 1;
			}
		}
	}

	pLastCharacter = pCharacter;	// Set this character as the previous character 
	return numWritten;				
}












void hyperdisplay::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color , uint16_t width)
{
	uint16_t absY, absX;

	if(y1 > y0){ absY = y1 - y0; }
	else{ absY = y0 - y1; }

	if(x1 > x0){ absX = x1 - x0; }
	else{ absX = x0 - x1; }

  	if( absY < absX )
  	{
	    if( x0 > x1 )
	    {
	      	lineLow(x1, y1, x0, y0, &color, 1, 0, width);
	    }
	    else
	    {
	      	lineLow(x0, y0, x1, y1, &color, 1, 0, width);
	    }
	}
  	else
	{
    	if( y0 > y1 )
      	{
      		lineHigh(x1, y1, x0, y0, &color, 1, 0, width);
      	}
    	else
    	{
      		lineHigh(x0, y0, x1, y1, &color, 1, 0, width);
  		}
	}
}

void hyperdisplay::polygon(uint16_t x[], uint16_t y[], uint8_t numSides, color_t color, uint16_t width)
{
	uint8_t indi = 0;
	for(indi = 0; indi < numSides-1; indi++)
	{
		line(*(x+indi), *(y+indi), *(x+indi+1), *(y+indi+1), &color , width);
	}
	if(numSides > 1)
	{
		line(*(x+indi), *(y+indi), *(x), *(y), &color , width);
	}
}

void hyperdisplay::circle(uint16_t x0, uint16_t y0, uint16_t radius, color_t color, bool filled)
{
	if(radius < 2)
	{
		circle_Bresenham(x0, y0, radius, color, false);
	}
	else
	{
		circle_midpoint(x0, y0, radius, color, false);
	}
}

void hyperdisplay::fillScreen(color_t color)
{
	rectangle(0, 0, xExt, yExt, color, true);
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
	uint16_t currentColorOffset = startColorOffset;

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
	uint16_t consecutive = 0;
	uint16_t currentColorOffset = startColorOffset;

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

void hyperdisplay::circle_Bresenham(uint16_t x0, uint16_t y0, uint16_t radius, color_t color, bool fill)
{
	// Thanks to the tutorial here: https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
	uint8_t dx = 0;
	uint8_t dy = radius;
	uint8_t D = 3 - 2*radius;

	if(fill)
	{
		pixel(x0, y0, color);
		if(radius == 0)
		{
			return;
		}
	}

	if(radius == 0)
	{
		pixel(x0, y0, color);
		return;
	}

	while(dy >= dx)
	{
		circle_eight(x0, y0, dx, dy, color, fill);
		dx++;
		if(D > 0)
		{
			dy--; 
		    D = D + 4 * (dx - dy) + 10;
		}
		else
		{
			D = D + 4 * dx + 6;
		}
	}
}

void hyperdisplay::circle_midpoint(uint8_t x0, uint8_t y0, uint8_t radius, color_t color, bool fill)
{
	// Thanks to the tutorial here: https://www.geeksforgeeks.org/mid-point-circle-drawing-algorithm/
    uint8_t dx = radius;
    uint8_t dy = 0;

    // Set first or center pixel
    pixel(x0+dx, y0+dy, color);
    if (radius > 0)
    {
       	pixel(x0-dx, y0+dy, color);
        pixel(x0+dy, y0-dx, color);
        pixel(x0+dy, y0+dx, color);

        if(fill)
        {
        	rectangle(x0-dx, y0, x0+dx, y0, color, true);
        	rectangle(x0, y0-dx, x0, y0+dx, color, true);
        }
    }

    // Initializing the value of P
    int16_t P = 1 - radius;
    while (dx > dy)
    { 
        dy++;
        
        if (P <= 0)
        {
        	// Mid-point is inside or on the perimeter
            P = P + 2*dy + 1;
        }
        else
        {
        	// Mid-point is outside the perimeter
            dx--;
            P = P + 2*dy - 2*dx + 1;
        }
         
        // All the perimeter points have already been printed
        if (dx < dy)
        {
            break;
        }

        pixel(x0+dx, y0+dy, color);
        pixel(x0-dx, y0+dy, color);
        pixel(x0+dx, y0-dy, color);
        pixel(x0-dx, y0-dy, color);

        if(fill)
        {
        	rectangle(x0-dx, y0+dy, x0+dx, y0+dy, color, true);
        	rectangle(x0-dx, y0-dy, x0+dx, y0-dy, color, true);
        }
         
        // If the generated point is on the line x = y then 
        // the perimeter points have already been printed
        if (dx != dy)
        {
            pixel(x0+dy, y0+dx, color);
        	pixel(x0-dy, y0+dx, color);
        	pixel(x0+dy, y0-dx, color);
        	pixel(x0-dy, y0-dx, color);

        	if(fill)
	        {
	        	rectangle(x0-dy, y0+dx, x0+dy, y0+dx, color, true);
	        	rectangle(x0-dy, y0-dx, x0+dy, y0-dx, color, true);
	        }
        }
    } 
}

void hyperdisplay::circle_eight(uint8_t x0, uint8_t y0, int16_t dx, int16_t dy, color_t color, bool fill)
{
	pixel(x0+dx, y0+dy, color);
	pixel(x0-dx, y0+dy, color);
	pixel(x0+dx, y0-dy, color);
	pixel(x0-dx, y0-dy, color);
	pixel(x0+dy, y0+dx, color);
	pixel(x0-dy, y0+dx, color);
	pixel(x0+dy, y0-dx, color);
	pixel(x0-dy, y0-dx, color);

	if(fill)
	{
		rectangle(x0-dx, y0+dy, x0+dx, y0+dy, color, true);
    	rectangle(x0-dx, y0-dx, x0+dx, y0-dx, color, true);
    	rectangle(x0-dy, y0+dx, x0+dy, y0+dx, color, true);
    	rectangle(x0-dy, y0-dx, x0+dy, y0-dx, color, true);
	}
}

uint16_t getNewColorOffset(uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t numWritten)
{
	uint16_t remainder = (numWritten % colorCycleLength);
	if(remainder < startColorOffset)
	{
		return (remainder + colorCycleLength) - startColorOffset; 
	}
	else
	{
		return (remainder) - startColorOffset; 
	}
}








// Callbacks default (blank) implementation
void hyperdisplayXLineCallback(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width){}
void hyperdisplayYLineCallback(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width){}
void hyperdisplayRectangleCallback(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color, bool filled){}
void hyperdisplayFillFromArrayCallback(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t size, color_t data[]){}
