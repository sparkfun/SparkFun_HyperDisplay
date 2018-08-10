/*
hyperdisplay.cpp

header file: hyperdisplay.h
*/




#include "hyperdisplay.h"

wind_info_t hyperdisplayDefaultWindow;		// This window is used by default so that the user does not have to worry about windows if they don't want to
char_info_t hyperdisplayDefaultCharacter;

// void hyperdisplay::pixel(int32_t x0, int32_t y0, color_t color)
// {
	// TIP: When you implement pixel for a derived class be aware that x0 and y0 are in terms of the current window.
	// For example if pixel() calls a hardware-specific function hardwarePixel() that draws to the display RAM 
	// coordinate (hX, hY) then it should look smething like this:
	/*

	uint16_t hX = pCurrentWindow->xMin + x0;
	uint16_t hY = pCurrentWindow->xMin + y0;
	hardwarePixel(hX, hY, color);
	
	*/
	// Furthermore this principal applies any time that you are overwriting one of the 'primitive' drawing functions.
// }

void hyperdisplay::xline(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool goLeft)
{
	// Note: color_t data is always a void pointer. You need to make sure that it points at the correct color type with enough elements.
	// In this case the correct number of elements is colorCycleLength
	if(colorCycleLength == 0)
	{
		colorCycleLength = 1;
	}
	// if(startColorOffset >= colorCycleLength)
	// {
	// 	startColorOffset = colorCycleLength - 1;
	// }

	color_t value = getOffsetColor(data, startColorOffset);

	if(goLeft)
	{
		for(uint16_t indi = 0; indi < len; indi++)
		{
			pixel(x0 - indi, y0, value);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	else
	{
		// Serial.println("Here3?");
		for(uint16_t indi = 0; indi < len; indi++)
		{
			// Serial.println("Here?4");

			Serial.println((uint32_t)value, HEX);
			// Serial.println("Here?");

			pixel(x0 + indi, y0, value);

			

			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);

			// Serial.println("Here2?");
			Serial.print("Offset: "); Serial.println(startColorOffset);

			value = getOffsetColor(data, startColorOffset);
		}
	}
	hyperdisplayXLineCallback(x0, y0, len, data, colorCycleLength, startColorOffset, goLeft);
}

void hyperdisplay::yline(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool goUp)
{
	// Note: color_t data is always a void pointer. You need to make sure that it points at the correct color type with enough elements.
	// In this case the correct number of elements is colorCycleLength
	if(colorCycleLength == 0)
	{
		colorCycleLength = 1;
	}
	// if(startColorOffset >= colorCycleLength)
	// {
	// 	startColorOffset = colorCycleLength - 1;
	// }

	color_t value = getOffsetColor(data, startColorOffset);

	for(uint16_t indi = 0; indi < len; indi++)
	{
		if(goUp)
		{
			pixel(x0, y0 - indi, value);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
		else
		{
			pixel(x0, y0 + indi, value);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	hyperdisplayYLineCallback(x0, y0, len, data, colorCycleLength, startColorOffset, goUp);
}

void hyperdisplay::rectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, bool filled, uint16_t colorCycleLength, uint16_t startColorOffset, bool gradientVertical, bool reverseGradient)
{
	// xline(x0, y0, x1-x0+1, data, colorCycleLength, startColorOffset, reverseGradient);
	if(x0 > x1)
	{ 
		SWAP_COORDS(x0, x1);
	}
	
	if(y0 > y1)
	{ 
		SWAP_COORDS(y0, y1);
	}

	uint16_t xlen = x1 - x0 + 1;
	if(filled)
	{
		if(gradientVertical)
		{
			for(uint16_t y = y0; y <= y1; y++)
			{
				color_t value = getOffsetColor(data, startColorOffset);
				xline(x0, y, xlen, value, 1, reverseGradient);
				startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			}
		}
		else
		{
			for(uint16_t y = y0; y <= y1; y++)
			{
				xline(x0, y, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
			}
		}
	}
	else
	{
		uint16_t ylen = y1 - y0 + 1;
		color_t value = getOffsetColor(data, startColorOffset);

		Serial.print("StartOffset: "); Serial.println(startColorOffset);

		xline(x0, y0, xlen, value, colorCycleLength, startColorOffset, reverseGradient);
		startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
		value = getOffsetColor(data, startColorOffset);

		Serial.print("StartOffset: "); Serial.println(startColorOffset);

		yline(x1, y0, ylen, value, colorCycleLength, startColorOffset, reverseGradient);
		startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen);
		value = getOffsetColor(data, startColorOffset);

		Serial.print("StartOffset: "); Serial.println(startColorOffset);

		xline(x1, y1, xlen, value, colorCycleLength, startColorOffset, !reverseGradient);
		startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
		value = getOffsetColor(data, startColorOffset);

		Serial.print("StartOffset: "); Serial.println(startColorOffset);

		yline(x0, y1, ylen, value, colorCycleLength, startColorOffset, !reverseGradient);
		startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen);
		value = getOffsetColor(data, startColorOffset);

		Serial.print("StartOffset: "); Serial.println(startColorOffset);
		
	}
	hyperdisplayRectangleCallback(x0, y0, x1, y1, data, filled, colorCycleLength, startColorOffset, gradientVertical, reverseGradient);
}

void hyperdisplay::fillFromArray(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t numPixels, color_t data)
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
			pixel(x, y, getOffsetColor(data, pixel_count++));
			if(pixel_count >= numPixels)
			{
				pixel_count = 0;
			}
		}
	}
	hyperdisplayFillFromArrayCallback(x0, y0, x1, y1, numPixels, data);
}


#if HYPERDISPLAY_USE_PRINT
	// FYI this virtual function can be overwritten. It is just the most basic default version
	size_t hyperdisplay::write(uint8_t val)
	{
		size_t numWritten = 0;


		getCharInfo(val, &hyperdisplayDefaultCharacter);
		if(hyperdisplayDefaultCharacter.show)
		{
			// Code to show the character
			
			if(pCurrentWindow != NULL)		// Make sure there is a valid window object
			{
				if(hyperdisplayDefaultCharacter.causedNewline)		// Check if the character is meant to cause a new line
				{
					pCurrentWindow->cursorX = pCurrentWindow->xMin;		// Reset the x cursor to the beginning
					if((pCurrentWindow->yMax - hyperdisplayDefaultCharacter.yDim) < pCurrentWindow->cursorY)			// Check if the y cursor will runn off the screen
					{
						hyperdisplayDefaultCharacter.show = false;						// If so then don't show the character
						pCurrentWindow->cursorY += hyperdisplayDefaultCharacter.yDim;	// And also only increment the cursor if within the proper bounds to avoid a long-term wraparound condition
					}
				}
				else if((pCurrentWindow->xMax - hyperdisplayDefaultCharacter.xDim) < pCurrentWindow->cursorX)	// If the character does not trigger newlines then make sure there is still room for it left-to-right
				{
					hyperdisplayDefaultCharacter.causedNewline = true;					// If there is not then it needs to cause a newline
					pCurrentWindow->cursorX = pCurrentWindow->xMin;		// Go through the same process as above to check bounds
					if((pCurrentWindow->yMax - hyperdisplayDefaultCharacter.yDim) < pCurrentWindow->cursorY)
					{
						hyperdisplayDefaultCharacter.show = false;
						pCurrentWindow->cursorY += hyperdisplayDefaultCharacter.yDim;
					}
				}
				else	// If will be entirely within bounds then simply increment the x cursor
				{
					pCurrentWindow->cursorX += hyperdisplayDefaultCharacter.xDim;
				}
				// Write data to display, but only if it is still meant to be shown
				if(hyperdisplayDefaultCharacter.show)
				{
					fillFromArray(pCurrentWindow->cursorX, pCurrentWindow->cursorY, pCurrentWindow->cursorX+hyperdisplayDefaultCharacter.xDim, pCurrentWindow->cursorY+hyperdisplayDefaultCharacter.yDim, hyperdisplayDefaultCharacter.numPixels, hyperdisplayDefaultCharacter.data);
					numWritten = 1;
				}
			}
		}

		pCurrentWindow->lastCharacter = hyperdisplayDefaultCharacter;	// Set this character as the previous character 
		return numWritten;				
	}
#else /* HYPERDISPLAY_USE_PRINT */
	// This is here in case you choose not to implement printing functions
	size_t hyperdisplay::write(uint8_t val)
	{
		Serial.write(val);
	}
#endif /* HYPERDISPLAY_USE_PRINT */











void hyperdisplay::line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width, bool reverseGradient)
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
	      	lineLow(x1, y1, x0, y0, data, 1, 0, width, reverseGradient);
	    }
	    else
	    {
	      	lineLow(x0, y0, x1, y1, data, 1, 0, width, reverseGradient);
	    }
	}
  	else
	{
    	if( y0 > y1 )
      	{
      		lineHigh(x1, y1, x0, y0, data, 1, 0, width, reverseGradient);
      	}
    	else
    	{
      		lineHigh(x0, y0, x1, y1, data, 1, 0, width, reverseGradient);
  		}
	}
}

void hyperdisplay::polygon(int32_t x[], int32_t y[], uint8_t numSides, color_t color, uint16_t width)
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

void hyperdisplay::circle(int32_t x0, int32_t y0, uint16_t radius, color_t color, bool filled)
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

void hyperdisplay::fillWindow(color_t color)
{
	rectangle(0, 0, pCurrentWindow->xMax-pCurrentWindow->xMin, pCurrentWindow->yMax-pCurrentWindow->yMin, color, true, 1, 0, false, false);
}





#if HYPERDISPLAY_USE_MATH
	// Write math functions here
#endif	










// Protected drawing functions
void hyperdisplay::lineHigh(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width, bool reverseGradient)
{
	// Note: color_t color is always a void pointer. You need to make sure that it points at the correct color type with enough elements.
	// In this case the correct number of elements is colorCycleLength
	uint16_t halfWidth = width/2;
	uint8_t shift = 0;
	if(!(width % 2))
	{
		shift = 1;
	}


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
			rectangle(x-shift-halfWidth, y-consecutive, x+halfWidth, y, data, true, colorCycleLength, startColorOffset, true, reverseGradient); 
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, consecutive);

		   	x = x + xi;
		   	D = D - 2*dy;
		}
		D = D + 2*dx;
	}
}
    	
void hyperdisplay::lineLow(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width, bool reverseGradient)
{
	// Note: color_t color is always a void pointer. You need to make sure that it points at the correct color type with enough elements.
	// In this case the correct number of elements is colorCycleLength
	uint16_t halfWidth = width/2;
	uint8_t shift = 0;
	if(!(width % 2))
	{
		shift = 1;
	}

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

	for(uint8_t x = x0; x < x1; x++)
	{
		if( D > 0 )
		{
			rectangle(x-consecutive, y-shift-halfWidth, x, y+halfWidth, data, true, colorCycleLength, startColorOffset, true, reverseGradient); 
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, consecutive);

		   	y = y + yi;
		   	D = D - 2*dx;
		}
		D = D + 2*dy;
	}
}

void hyperdisplay::circle_Bresenham(int32_t x0, int32_t y0, uint16_t radius, color_t color, bool fill)
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

uint16_t hyperdisplay::getNewColorOffset(uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t numWritten)
{
	return ((numWritten+startColorOffset) % colorCycleLength);
}

void hyperdisplay::setupDefaultWindow( void )
{
	pCurrentWindow = &hyperdisplayDefaultWindow;
	pCurrentWindow->xMin = 0;
	pCurrentWindow->yMin = 0;
	pCurrentWindow->xMax = xExt;
	pCurrentWindow->yMax = yExt;
	pCurrentWindow->cursorX = 0;
	pCurrentWindow->cursorY = 0;
	pCurrentWindow->xReset = 0;
	pCurrentWindow->yReset = 0;
	// pCurrentWindow->lastCharacter = NULL;	// 
	pCurrentWindow->data = NULL;
}

void hyperdisplay::setupHyperDisplay(uint16_t xSize, uint16_t ySize)
{
	xExt = xSize;
	yExt = ySize;
	setupDefaultWindow();
}








// Callbacks default (blank) implementation
void hyperdisplayXLineCallback(int32_t x0, int32_t y0, uint16_t len, color_t color, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width){}
void hyperdisplayYLineCallback(int32_t x0, int32_t y0, uint16_t len, color_t color, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width){}
void hyperdisplayRectangleCallback(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, bool filled, uint16_t colorCycleLength, uint16_t startColorOffset, bool gradientVertical, bool reverseGradient){}
void hyperdisplayFillFromArrayCallback(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t size, color_t data){}
