/*
hyperdisplay.cpp

header file: hyperdisplay.h
*/




#include "hyperdisplay.h"

wind_info_t hyperdisplayDefaultWindow;		// This window is used by default so that the user does not have to worry about windows if they don't want to
char_info_t hyperdisplayDefaultCharacter;

#if HYPERDISPLAY_USE_PRINT                  // 
    #if HYPERDISPLAY_INCLUDE_DEFAULT_FONT   
		uint16_t hyperdisplayDefaultXloc[HYPERDISPLAY_DEFAULT_FONT_WIDTH*HYPERDISPLAY_DEFAULT_FONT_HEIGHT];
		uint16_t hyperdisplayDefaultYloc[HYPERDISPLAY_DEFAULT_FONT_WIDTH*HYPERDISPLAY_DEFAULT_FONT_HEIGHT];
	#endif
#endif
// void hyperdisplay::hwpixel(int32_t x0, int32_t y0, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
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

void hyperdisplay::hwxline(uint16_t x0, uint16_t y0, uint16_t len, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool goLeft)
{
	// Note: color_t data is always a void pointer. You need to make sure that it points at the correct color type with enough elements.
	// In this case the correct number of elements is colorCycleLength
	if(colorCycleLength == 0){ colorCycleLength = 1; } // This is just a simple guard

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset
	color_t value = getOffsetColor(data, startColorOffset);

	if(goLeft)
	{
		for(uint16_t indi = 0; indi < len; indi++)
		{
			hwpixel(x0 - indi, y0, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	else
	{
		for(uint16_t indi = 0; indi < len; indi++)
		{
			hwpixel(x0 + indi, y0, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	hyperdisplayXLineCallback(x0, y0, len, data, colorCycleLength, startColorOffset, goLeft);
}

void hyperdisplay::hwyline(uint16_t x0, uint16_t y0, uint16_t len, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool goUp)
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

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset
	color_t value = getOffsetColor(data, startColorOffset);

	for(uint16_t indi = 0; indi < len; indi++)
	{
		if(goUp)
		{
			hwpixel(x0, y0 - indi, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
		else
		{
			hwpixel(x0, y0 + indi, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	hyperdisplayYLineCallback(x0, y0, len, data, colorCycleLength, startColorOffset, goUp);
}

void hyperdisplay::hwrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t data, bool filled, uint16_t colorCycleLength, uint16_t startColorOffset, bool reverseGradient, bool gradientVertical)
{
	if(x0 > x1){ SWAP_COORDS(x0, x1); }
	if(y0 > y1){ SWAP_COORDS(y0, y1); }

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset

	uint16_t xlen = x1 - x0 + 1;
	if(filled)
	{
		color_t value = getOffsetColor(data, startColorOffset);
		if(gradientVertical)
		{
			if(reverseGradient)
			{
				for(uint16_t y = y1; y >= y0; y--)
				{
					hwxline(x1, y, xlen, value, 1, startColorOffset, reverseGradient);
					startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
					value = getOffsetColor(data, startColorOffset);
				}
			}
			else
			{
				for(uint16_t y = y0; y <= y1; y++)
				{
					hwxline(x0, y, xlen, value, 1, startColorOffset, reverseGradient);
					startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
					value = getOffsetColor(data, startColorOffset);
				}
			}
		}
		else
		{
			if(reverseGradient)
			{
				for(uint16_t y = y1; y >= y0; y--)
				{
					hwxline(x1, y, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
				}
			}
			else
			{
				for(uint16_t y = y0; y <= y1; y++)
				{
					hwxline(x0, y, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
				}
			}
		}
	}
	else
	{
		uint16_t ylen = y1 - y0 + 1;

		if(reverseGradient)
		{
			hwxline(x1, y0, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
			hwyline(x0, y0+1, ylen-2, data, colorCycleLength, startColorOffset, !reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen-2);
			hwxline(x0, y1, xlen, data, colorCycleLength, startColorOffset, !reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
			hwyline(x1, y1-1, ylen-2, data, colorCycleLength, startColorOffset, reverseGradient);
		}
		else
		{
			hwxline(x0, y0, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
			hwyline(x1, y0+1, ylen-2, data, colorCycleLength, startColorOffset, reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen-2);
			hwxline(x1, y1, xlen, data, colorCycleLength, startColorOffset, !reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
			hwyline(x0, y1-1, ylen-2, data, colorCycleLength, startColorOffset, !reverseGradient);
		}
	}
	hyperdisplayRectangleCallback(x0, y0, x1, y1, data, filled, colorCycleLength, startColorOffset, gradientVertical, reverseGradient);
}

void hyperdisplay::hwfillFromArray(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t numPixels, color_t data)
{
	if(x0 > x1){ SWAP_COORDS(x0, x1); }
	if(y0 > y1){ SWAP_COORDS(y0, y1); }

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










void hyperdisplay::pixel(int32_t x0, int32_t y0, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
{
	hyperdisplay_dim_check_t x0c = enforceLimits(&x0, false);				// if the dimension was off-window then it will now be on the edge. 
	hyperdisplay_dim_check_t y0c = enforceLimits(&y0, true); 
	if((x0c != hyperdisplay_dim_ok) || (y0c != hyperdisplay_dim_ok))	// Next use 'enforceLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwpixel()
	{
		Serial.println("failed pixel check");
		return;	// Do not print a single pixel at the wrong location ever
	}
	hwpixel(x0, y0, data, colorCycleLength, startColorOffset);
}

void hyperdisplay::xline(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool goLeft)
{
	int32_t x1;
	if(goLeft){ x1 = x0 - len; }else{ x1 = x0 + len; }						// Correct for goLeft
	hyperdisplay_dim_check_t x0c = enforceLimits(&x0, false);				// if the dimension was off-window then it will now be on the edge. 
	hyperdisplay_dim_check_t x1c = enforceLimits(&x1, false);
	hyperdisplay_dim_check_t y0c = enforceLimits(&y0, true); 						// Next use 'enforceLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwxline()
	if(y0c != hyperdisplay_dim_ok){ return; }											// Don't do it if y was wrong
	if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom dot at xMin)
	if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom dot at xMax)
	hwxline(x0, y0, len, data, colorCycleLength, startColorOffset, goLeft);
}

void hyperdisplay::yline(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool goUp)
{
	int32_t y1;
	if(goUp){ y1 = y0 - len; }else{ y1 = y0 + len; }						// Correct for goUp
	hyperdisplay_dim_check_t y0c = enforceLimits(&y0, true);				// if the dimension was off-window then it will now be on the edge. 
	hyperdisplay_dim_check_t y1c = enforceLimits(&y1, true);
	hyperdisplay_dim_check_t x0c = enforceLimits(&x0, false); 						// Next use 'enforceLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwyline()
	if(x0c != hyperdisplay_dim_ok){ return; }											// Don't do it if x was wrong
	if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom dot at yMin)
	if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom dot at yMax)
	hwyline(x0, y0, len, data, colorCycleLength, startColorOffset, goUp);
}

void hyperdisplay::rectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, bool filled, uint16_t colorCycleLength, uint16_t startColorOffset, bool gradientVertical, bool reverseGradient)
{
	hyperdisplay_dim_check_t y0c = enforceLimits(&y0, true);				// if the dimension was off-window then it will now be on the edge. enforceLimits also applies the transformation into hw coordinates
	hyperdisplay_dim_check_t y1c = enforceLimits(&y1, true);
	hyperdisplay_dim_check_t x0c = enforceLimits(&x0, false); 
	hyperdisplay_dim_check_t x1c = enforceLimits(&x1, false);						// Next use 'enforceLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwrectangle()
	if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom line at yMin)
	if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom line at yMax)
	if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom line at xMin)
	if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom line at xMax)
	hwrectangle(x0, y0, x1, y1, data, filled, colorCycleLength, startColorOffset, gradientVertical, reverseGradient);
}

void hyperdisplay::fillFromArray(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t numPixels, color_t data)
{
	// Check window coordinate validity
	hyperdisplay_dim_check_t y0c = enforceLimits(&y0, true);
	hyperdisplay_dim_check_t y1c = enforceLimits(&y1, true);
	hyperdisplay_dim_check_t x0c = enforceLimits(&x0, false); 
	hyperdisplay_dim_check_t x1c = enforceLimits(&x1, false);	// Next use 'enforceLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwfillFromArray()
	if(x0c != hyperdisplay_dim_ok){ return; } 						// Return if any dimension was not ok because changing the dimensions could have adverse effects on the array (potential overflow or underflow)
	if(x1c != hyperdisplay_dim_ok){ return; } 
	if(y0c != hyperdisplay_dim_ok){ return; } 
	if(y1c != hyperdisplay_dim_ok){ return; } 
	hwfillFromArray(x0, y0, x1, y1, numPixels, data);
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
			
			// if(pCurrentWindow != NULL)		// Make sure there is a valid window object
			// {
			// 	if(hyperdisplayDefaultCharacter.causedNewline)		// Check if the character is meant to cause a new line
			// 	{
			// 		pCurrentWindow->cursorX = pCurrentWindow->xMin;		// Reset the x cursor to the beginning
			// 		if((pCurrentWindow->yMax - hyperdisplayDefaultCharacter.yDim) < pCurrentWindow->cursorY)			// Check if the y cursor will runn off the screen
			// 		{
			// 			hyperdisplayDefaultCharacter.show = false;						// If so then don't show the character
			// 			pCurrentWindow->cursorY += hyperdisplayDefaultCharacter.yDim;	// And also only increment the cursor if within the proper bounds to avoid a long-term wraparound condition
			// 		}
			// 	}
			// 	else if((pCurrentWindow->xMax - hyperdisplayDefaultCharacter.xDim) < pCurrentWindow->cursorX)	// If the character does not trigger newlines then make sure there is still room for it left-to-right
			// 	{
			// 		hyperdisplayDefaultCharacter.causedNewline = true;					// If there is not then it needs to cause a newline
			// 		pCurrentWindow->cursorX = pCurrentWindow->xMin;		// Go through the same process as above to check bounds
			// 		if((pCurrentWindow->yMax - hyperdisplayDefaultCharacter.yDim) < pCurrentWindow->cursorY)
			// 		{
			// 			hyperdisplayDefaultCharacter.show = false;
			// 			pCurrentWindow->cursorY += hyperdisplayDefaultCharacter.yDim;
			// 		}
			// 	}
			// 	else	// If will be entirely within bounds then simply increment the x cursor
			// 	{
			// 		pCurrentWindow->cursorX += hyperdisplayDefaultCharacter.xDim;
			// 	}
			// 	// Write data to display, but only if it is still meant to be shown
			// 	if(hyperdisplayDefaultCharacter.show)
			// 	{
			// 		fillFromArray(pCurrentWindow->cursorX, pCurrentWindow->cursorY, pCurrentWindow->cursorX+hyperdisplayDefaultCharacter.xDim, pCurrentWindow->cursorY+hyperdisplayDefaultCharacter.yDim, hyperdisplayDefaultCharacter.numPixels, hyperdisplayDefaultCharacter.data);
			// 		numWritten = 1;
			// 	}
			// }
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











void hyperdisplay::line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool reverseGradient)
{
	uint16_t absY, absX;

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset

	if(y1 > y0){ absY = y1 - y0; }
	else{ absY = y0 - y1; }

	if(x1 > x0){ absX = x1 - x0; }
	else{ absX = x0 - x1; }

  	if( absY < absX )
  	{
	    if( x0 > x1 )
	    {
	      	lineLow(x1, y1, x0, y0, width, data, colorCycleLength, startColorOffset, reverseGradient);
	    }
	    else
	    {
	      	lineLow(x0, y0, x1, y1, width, data, colorCycleLength, startColorOffset, reverseGradient);
	    }
	}
  	else
	{
    	if( y0 > y1 )
      	{
      		lineHigh(x1, y1, x0, y0, width, data, colorCycleLength, startColorOffset, reverseGradient);
      	}
    	else
    	{
      		lineHigh(x0, y0, x1, y1, width, data, colorCycleLength, startColorOffset, reverseGradient);
  		}
	}
}

void hyperdisplay::polygon(int32_t x[], int32_t y[], uint8_t numSides, uint16_t width, color_t color)
{
	// uint8_t indi = 0;
	// for(indi = 0; indi < numSides-1; indi++)
	// {
	// 	line(*(x+indi), *(y+indi), *(x+indi+1), *(y+indi+1), &color , width);
	// }
	// if(numSides > 1)
	// {
	// 	line(*(x+indi), *(y+indi), *(x), *(y), &color , width);
	// }
}

void hyperdisplay::circle(int32_t x0, int32_t y0, uint16_t radius, color_t color, bool filled)
{
	if(radius < 2)
	{
		circle_Bresenham(x0, y0, radius, color, filled);
	}
	else
	{
		circle_midpoint(x0, y0, radius, color, filled);
	}
}

void hyperdisplay::fillWindow(color_t color)
{
	rectangle(0, 0, pCurrentWindow->xMax-pCurrentWindow->xMin, pCurrentWindow->yMax-pCurrentWindow->yMin, color, true, 1, 0, false, false);
}


#if HYPERDISPLAY_USE_PRINT
	#if HYPERDISPLAY_INCLUDE_DEFAULT_FONT 					        
	    void getCharInfo(uint8_t val, char_info_t * pchar)		// The defualt implementation 
	    {

	    }	
	#endif
#endif





#if HYPERDISPLAY_USE_MATH
	// Write math functions here
#endif	










// Protected drawing functions
void hyperdisplay::lineHigh(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool reverseGradient)
{
	// Note: color_t color is always a void pointer. You need to make sure that it points at the correct color type with enough elements.
	// In this case the correct number of elements is colorCycleLength
	uint16_t halfWidth = width/2;
	uint8_t shift = 0;
	if(!(width % 2))
	{
		shift = 1;
	}

	int32_t dy = y1 - y0;	// Guaranteed positive - but we might want it to be negative
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
		consecutive++;
		if( D > 0 )
		{

			if(width == 1)
			{
				yline(x, y-consecutive+1, consecutive, data, colorCycleLength, startColorOffset, false);
			}
			else
			{
				rectangle(x+shift-halfWidth, y-consecutive+1, x+halfWidth, y, data, true, colorCycleLength, startColorOffset, reverseGradient, true); 
			}
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, consecutive);

			consecutive = 0;

		   	x = x + xi;
		   	D = D - 2*dy;		
		}
		D = D + 2*dx;
	}
}
    	
void hyperdisplay::lineLow(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool reverseGradient)
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
		consecutive++;
		if( D > 0 )
		{
			if(width == 1)
			{
				xline(x-consecutive+1, y, consecutive, data, colorCycleLength, startColorOffset, false);
			}
			else
			{
				rectangle(x-consecutive+1, y+shift-halfWidth, x, y+halfWidth, data, true, colorCycleLength, startColorOffset, reverseGradient, false); 
			}
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, consecutive);

			consecutive = 0;

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

hyperdisplay_dim_check_t hyperdisplay::enforceLimits(int32_t * var, bool axisSelect)
{
	bool low = false, high = false;
	if(var == NULL){ return hyperdisplay_dim_no_val; }
	if(axisSelect)
	{
		// y axis
		*var = (uint16_t)((*var) + (pCurrentWindow->yMin));													// Cast to uint16_t and apply transformation into hw coordiantes for comparison + usage in hw_____ functions
		if( *var > (int32_t)pCurrentWindow->yMax ){ high = true; *var = (int32_t)pCurrentWindow->yMax; }	// Compare to hardware coordinate limits of the window
		if( *var < (int32_t)pCurrentWindow->yMin ){ low = true;  *var = (int32_t)pCurrentWindow->yMin; }	// Compare to hardware coordinate limits of the window
	}
	else
	{
		// x axis
		*var = (uint16_t)((*var) + (pCurrentWindow->xMin)); 												// Cast to uint16_t and apply transformation into hw coordiantes for comparison + usage in hw_____ functions
		if( *var > (int32_t)pCurrentWindow->xMax ){ high = true; *var = (int32_t)pCurrentWindow->xMax; }	// Compare to hardware coordinate limits of the window
		if( *var < (int32_t)pCurrentWindow->xMin ){ low = true;  *var = (int32_t)pCurrentWindow->xMin; }	// Compare to hardware coordinate limits of the window
	}
	if(high){ return hyperdisplay_dim_high; }
	if(low ){ return hyperdisplay_dim_low;  }
	return hyperdisplay_dim_ok;
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
