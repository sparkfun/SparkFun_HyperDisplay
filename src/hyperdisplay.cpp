/*
hyperdisplay.cpp

header file: hyperdisplay.h
*/




#include "hyperdisplay.h"					// Click here to get the library: http://librarymanager/SparkFun_HyperDisplay

wind_info_t hyperdisplayDefaultWindow;		// This window is used by default so that the user does not have to worry about windows if they don't want to
char_info_t hyperdisplayDefaultCharacter;	// The default character to use

#if HYPERDISPLAY_USE_PRINT
    #if HYPERDISPLAY_INCLUDE_DEFAULT_FONT   
		uint16_t hyperdisplayDefaultXloc[HYPERDISPLAY_DEFAULT_FONT_WIDTH*HYPERDISPLAY_DEFAULT_FONT_HEIGHT];
		uint16_t hyperdisplayDefaultYloc[HYPERDISPLAY_DEFAULT_FONT_WIDTH*HYPERDISPLAY_DEFAULT_FONT_HEIGHT];
	#endif
#endif





/////////////////////////////////////////////
// 					Constructor
/////////////////////////////////////////////
hyperdisplay::hyperdisplay(uint16_t xSize, uint16_t ySize)
{
	xExt = xSize;
	yExt = ySize;

	pCurrentWindow = &hyperdisplayDefaultWindow;	// Sets the current window to the default window structure
	setWindowDefaults(pCurrentWindow);				// Sets reasonable (uninitialized) values for the current window structure.
}






/////////////////////////////////////////////
// 				Display APIs
/////////////////////////////////////////////
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

void hyperdisplay::hwrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool filled, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool reverseGradient, bool gradientVertical)
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
					hwxline(x1, y, xlen, value, 1, 0, reverseGradient);
					startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
					value = getOffsetColor(data, startColorOffset);
				}
			}
			else
			{
				for(uint16_t y = y0; y <= y1; y++)
				{
					hwxline(x0, y, xlen, value, 1, 0, reverseGradient);
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
	for(uint16_t x = x0; x <= x1; x++)
	{
		for(uint16_t y = y0; y <= y1; y++)
		{
			hwpixel(x, y, getOffsetColor(data, pixel_count++));
			if(pixel_count >= numPixels)
			{
				pixel_count = 0;
			}
		}
	}
	hyperdisplayFillFromArrayCallback(x0, y0, x1, y1, numPixels, data);
}














/////////////////////////////////////////////
// 			Primitive Drawing Functions
/////////////////////////////////////////////
void hyperdisplay::pixel(int32_t x0, int32_t y0, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
{
	hyperdisplay_dim_check_t x0c = enforceLimits(&x0, false);				// if the dimension was off-window then it will now be on the edge. 
	hyperdisplay_dim_check_t y0c = enforceLimits(&y0, true); 
	if((x0c != hyperdisplay_dim_ok) || (y0c != hyperdisplay_dim_ok))	// Next use 'enforceLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwpixel()
	{
		return;	// Do not print a single pixel at the wrong location ever
	}
	// Check if we are to use the default color
	if(data == NULL){ hwpixel(x0, y0, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset); }
	else{ hwpixel(x0, y0, data, colorCycleLength, startColorOffset); }
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
	// Check if default color
	if(data == NULL){ hwxline(x0, y0, len, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, goLeft); }
	else{ hwxline(x0, y0, len, data, colorCycleLength, startColorOffset, goLeft); }
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
	if(data == NULL){ hwyline(x0, y0, len, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, goUp); }
	else{ hwyline(x0, y0, len, data, colorCycleLength, startColorOffset, goUp); }
}

void hyperdisplay::rectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, bool filled, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool gradientVertical, bool reverseGradient)
{
	hyperdisplay_dim_check_t y0c = enforceLimits(&y0, true);				// if the dimension was off-window then it will now be on the edge. enforceLimits also applies the transformation into hw coordinates
	hyperdisplay_dim_check_t y1c = enforceLimits(&y1, true);
	hyperdisplay_dim_check_t x0c = enforceLimits(&x0, false); 
	hyperdisplay_dim_check_t x1c = enforceLimits(&x1, false);						// Next use 'enforceLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwrectangle()
	if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom line at yMin)
	if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom line at yMax)
	if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom line at xMin)
	if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom line at xMax)
	if(data == NULL){  hwrectangle(x0, y0, x1, y1, filled, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, gradientVertical, reverseGradient); }
	else{ hwrectangle(x0, y0, x1, y1, filled, data, colorCycleLength, startColorOffset, gradientVertical, reverseGradient); }
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
	if(data != NULL){ hwfillFromArray(x0, y0, x1, y1, numPixels, data); }
}

void hyperdisplay::fillWindow(color_t color)
{
	rectangle(0, 0, pCurrentWindow->xMax-pCurrentWindow->xMin, pCurrentWindow->yMax-pCurrentWindow->yMin, true, color, 1, 0, false, false);
}


















void hyperdisplay::setCurrentWindowColorSequence(color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
{
	pCurrentWindow->currentSequenceData = data;
	pCurrentWindow->currentColorCycleLength = colorCycleLength;
	pCurrentWindow->currentColorOffset = startColorOffset;
}





















#if HYPERDISPLAY_USE_PRINT
	// FYI this virtual function can be overwritten. It is just the most basic default version
	size_t hyperdisplay::write(uint8_t val)
	{
		size_t numWritten = 0;
		getCharInfo(val, &hyperdisplayDefaultCharacter);

		// Check to see if current cursor coordinates work for the requested character
		if(((pCurrentWindow->xMax - pCurrentWindow->xMin) - hyperdisplayDefaultCharacter.xDim) < pCurrentWindow->cursorX)
		{
			if(((pCurrentWindow->yMax - pCurrentWindow->yMin) - hyperdisplayDefaultCharacter.yDim) < pCurrentWindow->cursorY)
			{
				return numWritten;	// return because there is no more room in the x or y directions of the window
			}
			pCurrentWindow->cursorX = pCurrentWindow->xReset;				// Put x cursor back to reset location
			pCurrentWindow->cursorY += hyperdisplayDefaultCharacter.yDim;	// Move the cursor down by the size of the character
		}

		// Now write the character
		if(hyperdisplayDefaultCharacter.show)
		{
			//fillFromArray(pCurrentWindow->cursorX, pCurrentWindow->cursorY, pCurrentWindow->cursorX+hyperdisplayDefaultCharacter.xDim, pCurrentWindow->cursorY+hyperdisplayDefaultCharacter.yDim, hyperdisplayDefaultCharacter.numPixels, hyperdisplayDefaultCharacter.data);
			for(uint32_t indi = 0; indi < hyperdisplayDefaultCharacter.numPixels; indi++)
			{
				pixel(((pCurrentWindow->cursorX)+*(hyperdisplayDefaultCharacter.xLoc + indi)), ((pCurrentWindow->cursorY)+*(hyperdisplayDefaultCharacter.yLoc + indi)), NULL, 1, 0);
			}

			numWritten = 1;

			// Now advance the cursor in the x direction so that you don't overwrite the work you just did
			pCurrentWindow->cursorX += hyperdisplayDefaultCharacter.xDim + 1;
		}

		pCurrentWindow->lastCharacter = hyperdisplayDefaultCharacter;	// Set this character as the previous character - the info will persist because this is direct 
		return numWritten;				
	}

#if HYPERDISPLAY_INCLUDE_DEFAULT_FONT 			        
		void hyperdisplay::getCharInfo(uint8_t character, character_info * character_info) 
		{
			// This is the most basic font implementation, it only prints a monochrome character using the first color of the current window's current color sequence
			// If you want any more font capabilities then you should re-implement this function :D

			character_info->data = NULL;	// Use the window's current color

			// Link the default cordinate arrays
			character_info->xLoc = hyperdisplayDefaultXloc;
			character_info->yLoc = hyperdisplayDefaultYloc;

			character_info->xDim = 5;
			character_info->yDim = 8;

			// Figure out if the character should cause a newline
			if (character == '\r' || character == '\n')
			{
				character_info->causesNewline = true;
			}
			else
			{
				character_info->causesNewline = false;
			} 

			// Figure out if you need to actually show the chracter
			if((character >= ' ') && (character <= '~'))
			{
				character_info->show = true;
			}
			else
			{
				character_info->show = false;
				return;								// No point in continuing;
			}

			// Load up the character data and fill in coordinate data
			uint8_t values[5];							// Holds the 5 bytes for the character
			uint16_t offset = 6 + 5 * (character - 0);
			character_info->numPixels = 0;
			uint16_t n = 0;
			for(uint8_t indi = 0; indi < 5; indi++)
			{
				values[indi] = pgm_read_byte(font5x7 + offset + indi);
				for(uint8_t indj = 0; indj < 8; indj++)
				{
					if(values[indi] & (0x01 << indj))
					{
						character_info->numPixels++;
						*(character_info->xLoc + n) = indi;
						*(character_info->yLoc + n) = indj;
						n++;
					}
				}
			}
		}
#endif /* HYPERDISPLAY_INCLUDE_DEFAULT_FONT */

#else /* HYPERDISPLAY_USE_PRINT */
	// This is here in case you choose not to implement printing functions
	size_t hyperdisplay::write(uint8_t val)
	{
		Serial.write(val);
	}
#endif /* HYPERDISPLAY_USE_PRINT */


void hyperdisplay::setTextCursor(int32_t x0, int32_t y0, wind_info_t * window){
	if(!window) window = pCurrentWindow; //default to current
    window->cursorX = x0;               	// Where the cursor is currently in window-coordinates
    window->cursorY = y0;               	// Where the cursor is currently in window-coordinates
}
























/////////////////////////////////////////////
// 			Level 1 Drawing Functions
/////////////////////////////////////////////
#if HYPERDISPLAY_DRAWING_LEVEL > 0
uint16_t hyperdisplay::line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool reverseGradient)
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
	    	if(reverseGradient)
	    	{
	    		return lineLowReverse(x1, y1, x0, y0, width, data, colorCycleLength, startColorOffset);
	    	}
	    	else
	    	{
	    		return lineLowNorm(x1, y1, x0, y0, width, data, colorCycleLength, startColorOffset);
	    	}
	    }
	    else
	    {
	    	if(reverseGradient)
	    	{
	    		return lineLowReverse(x0, y0, x1, y1, width, data, colorCycleLength, startColorOffset);
	    	}
	    	else
	    	{
	    		return lineLowNorm(x0, y0, x1, y1, width, data, colorCycleLength, startColorOffset);
	    	}
	    }
	}
  	else
	{
    	if( y0 > y1 )
      	{

      		if(reverseGradient)
      		{
      			return lineHighReverse(x1, y1, x0, y0, width, data, colorCycleLength, startColorOffset);
      		}
      		else
      		{
      			return lineHighNorm(x1, y1, x0, y0, width, data, colorCycleLength, startColorOffset);
      		}
      	}
    	else
    	{
    		if(reverseGradient)
    		{
				return lineHighReverse(x0, y0, x1, y1, width, data, colorCycleLength, startColorOffset);
    		}
    		else
    		{
    			return lineHighNorm(x0, y0, x1, y1, width, data, colorCycleLength, startColorOffset);
    		}
		}
	}
}

void hyperdisplay::polygon(int32_t x[], int32_t y[], uint8_t numSides, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool reverseGradient)
{
	uint8_t indi = 0;
	int32_t numWritten = 0; 

	int32_t dx;
	int32_t dy;
	bool rev = reverseGradient;

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0); // Condition for safety

	int8_t multiplier = 1;
	if(reverseGradient)
	{
		multiplier = -1;
	}

	for(indi = 0; indi < numSides-1; indi++)
	{
		dx = (*(x+indi+1) - *(x+indi));
		dy = (*(y+indi+1) - *(y+indi));
		rev = reverseGradient;
		// if(dx < 0)
		// {
		// 	rev = !rev;
		// }
		if(dy < 0)
		{
			rev = !rev;
		}
		numWritten = line(*(x+indi), *(y+indi), *(x+indi+1), *(y+indi+1), width, data, colorCycleLength, startColorOffset, rev);
		startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, multiplier*numWritten); 
	}
	if(numSides > 1)
	{
		dx = (*(x) - *(x+indi));
		dy = (*(y) - *(y+indi));
		rev = reverseGradient;
		if(dx < 0)
		{
			rev = !rev;
		}
		if(dy < 0)
		{
			rev = !rev;
		}
		numWritten = line(*(x+indi), *(y+indi), *(x), *(y), width, data, colorCycleLength, startColorOffset, rev);	
	}
}

void hyperdisplay::circle(int32_t x0, int32_t y0, uint16_t radius, bool filled, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool reverseGradient)
{

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// Condition the input for safety
	color_t value = getOffsetColor(data, startColorOffset);							// Get the initial color to pass into the functions
	int8_t colorIncrement = 1;

	if((colorCycleLength <= 1) || (!filled))	// Unfilled circles won't have a gradient because of the way they are drawn
	{
		if(radius < 2)
		{
			circle_Bresenham(x0, y0, radius, value, filled);
		}
		else
		{
			circle_midpoint(x0, y0, radius, value, filled);
		}
	}
	else
	{
		if(reverseGradient)
		{
			colorIncrement = -1;
		}
		for(uint16_t indi = radius; indi > 0; indi--)
		{
			if(indi < 2)
			{
				circle_Bresenham(x0, y0, indi, value, filled);
			}
			else
			{
				circle_midpoint(x0, y0, indi, value, filled);
			}
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, colorIncrement);	// Increment the color
			value = getOffsetColor(data, startColorOffset);									
		}
	}
}
#endif /* HYPERDISPLAY_DRAWING_LEVEL > 0 */








#if HYPERDISPLAY_USE_MATH
	// Write math functions here
#endif	










// Protected drawing functions
uint16_t hyperdisplay::lineHighNorm(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
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

	uint16_t y;
	for(y = y0; y < y1; y++)
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
				rectangle(x+shift-halfWidth, y-consecutive+1, x+halfWidth, y, true, data, colorCycleLength, startColorOffset, false, true); 
			}
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, consecutive);

			consecutive = 0;

		   	x = x + xi;
		   	D = D - 2*dy;		
		}
		D = D + 2*dx;
	}
	if(width == 1)
	{
		yline(x, y-consecutive, consecutive, data, colorCycleLength, startColorOffset, false);
	}
	else
	{
		rectangle(x+shift-halfWidth, y-consecutive, x+halfWidth, y, true, data, colorCycleLength, startColorOffset, false, true); 
	}
	return dy;
}

uint16_t hyperdisplay::lineHighReverse(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
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
	uint16_t x = x1;
	uint16_t consecutive = 0;

	uint16_t y;
	for(y = y1; y > y0; y--)
	{
		consecutive++;
		if( D > 0 )
		{
			if(width == 1)
			{
				yline(x, y+consecutive-1, consecutive, data, colorCycleLength, startColorOffset, true);
			}
			else
			{
				rectangle(x+shift-halfWidth, y+consecutive-1, x+halfWidth, y, true, data, colorCycleLength, startColorOffset, true, true); 
			}
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, consecutive);

			consecutive = 0;

		   	x = x - xi;
		   	D = D - 2*dy;		
		}
		D = D + 2*dx;
	}
	if(width == 1)
	{
		yline(x, y+consecutive, consecutive, data, colorCycleLength, startColorOffset, true);
	}
	else
	{
		rectangle(x+shift-halfWidth, y+consecutive, x+halfWidth, y, true, data, colorCycleLength, startColorOffset, true, true); 
	}
	return dy;
}
    	
uint16_t hyperdisplay::lineLowNorm(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
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

	uint16_t x;
	for(x = x0; x < x1; x++)
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
				rectangle(x-consecutive+1, y+shift-halfWidth, x, y+halfWidth, true, data, colorCycleLength, startColorOffset, false, false); 
			}
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, consecutive);

			consecutive = 0;

		   	y = y + yi;
		   	D = D - 2*dx;
		}
		D = D + 2*dy;
	}
	if(width == 1)
	{
		xline(x-consecutive, y, consecutive, data, colorCycleLength, startColorOffset, false);
	}
	else
	{
		rectangle(x-consecutive, y+shift-halfWidth, x, y+halfWidth, true, data, colorCycleLength, startColorOffset, false, false); 
	}
	return dx;
}

uint16_t hyperdisplay::lineLowReverse(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
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
	uint16_t y = y1;
	uint16_t consecutive = 0;

	uint16_t x;
	for(x = x1; x > x0; x--)
	{
		consecutive++;
		if( D > 0 )
		{
			if(width == 1)
			{
				xline(x+consecutive-1, y, consecutive, data, colorCycleLength, startColorOffset, true);
			}
			else
			{
				rectangle(x+consecutive-1, y+shift-halfWidth, x, y+halfWidth, true, data, colorCycleLength, startColorOffset, true, false); 
			}
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, consecutive);

			consecutive = 0;

		   	y = y - yi;
		   	D = D - 2*dx;
		}
		D = D + 2*dy;
	}
	if(width == 1)
	{
		xline(x+consecutive, y, consecutive, data, colorCycleLength, startColorOffset, true);
	}
	else
	{
		rectangle(x+consecutive, y+shift-halfWidth, x, y+halfWidth, true, data, colorCycleLength, startColorOffset, true, false); 
	}
	return dx;
}

void hyperdisplay::circle_Bresenham(int32_t x0, int32_t y0, uint16_t radius, color_t color, bool fill)
{
	// Thanks to the tutorial here: https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
	int32_t dx = 0;
	int32_t dy = radius;
	int32_t D = 3 - 2*radius;

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

void hyperdisplay::circle_midpoint(int32_t x0, int32_t y0, uint16_t radius, color_t color, bool fill)
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
        	rectangle(x0-dx, y0, x0+dx, y0, true, color);
        	rectangle(x0, y0-dx, x0, y0+dx, true, color);
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
        	rectangle(x0-dx, y0+dy, x0+dx, y0+dy, true, color);
        	rectangle(x0-dx, y0-dy, x0+dx, y0-dy, true, color);
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
	        	rectangle(x0-dy, y0+dx, x0+dy, y0+dx, true, color);
	        	rectangle(x0-dy, y0-dx, x0+dy, y0-dx, true, color);
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
		rectangle(x0-dx, y0+dy, x0+dx, y0+dy, true, color);
    	rectangle(x0-dx, y0-dx, x0+dx, y0-dx, true, color);
    	rectangle(x0-dy, y0+dx, x0+dy, y0+dx, true, color);
    	rectangle(x0-dy, y0-dx, x0+dy, y0-dx, true, color);
	}
}

uint16_t hyperdisplay::getNewColorOffset(uint16_t colorCycleLength, uint16_t startColorOffset, int32_t numWritten)
{
	if(numWritten >= 0)
	{
		return ((numWritten+startColorOffset) % colorCycleLength);
	}
	else
	{
		numWritten *= -1;	// Make positive
		uint16_t remainder = ((uint16_t)numWritten) % colorCycleLength;
		if(startColorOffset >= remainder)
		{
			return (startColorOffset - remainder);
		}
		else
		{
			return (colorCycleLength - (remainder - startColorOffset));
		}
	}
}

hyperdisplay_dim_check_t hyperdisplay::enforceLimits(int32_t * var, bool axisSelect)
{
	bool low = false, high = false;
	if(var == NULL){ return hyperdisplay_dim_no_val; }
	if(axisSelect)
	{
		// y axis
		*var = (int32_t)((*var) + (pCurrentWindow->yMin));													// Cast to uint16_t and apply transformation into hw coordiantes for comparison + usage in hw_____ functions
		if( *var > (int32_t)pCurrentWindow->yMax ){ high = true; *var = (int32_t)pCurrentWindow->yMax; }	// Compare to hardware coordinate limits of the window
		if( *var < (int32_t)pCurrentWindow->yMin ){ low = true;  *var = (int32_t)pCurrentWindow->yMin; }	// Compare to hardware coordinate limits of the window
	}
	else
	{
		// x axis
		*var = (int32_t)((*var) + (pCurrentWindow->xMin)); 												// Cast to uint16_t and apply transformation into hw coordiantes for comparison + usage in hw_____ functions
		if( *var > (int32_t)pCurrentWindow->xMax ){ high = true; *var = (int32_t)pCurrentWindow->xMax; }	// Compare to hardware coordinate limits of the window
		if( *var < (int32_t)pCurrentWindow->xMin ){ low = true;  *var = (int32_t)pCurrentWindow->xMin; }	// Compare to hardware coordinate limits of the window
	}
	if(high){ return hyperdisplay_dim_high; }
	if(low ){ return hyperdisplay_dim_low;  }
	return hyperdisplay_dim_ok;
}

void hyperdisplay::setWindowDefaults(wind_info_t * pwindow)
{
	// Fills out the default window structure with more or less reasonable defaults
	pwindow->xMin = 0;
	pwindow->yMin = 0;
	pwindow->xMax = xExt;
	pwindow->yMax = yExt;
	pwindow->cursorX = 0;
	pwindow->cursorY = 0;
	pwindow->xReset = 0;
	pwindow->yReset = 0;
	
	pwindow->lastCharacter.data = NULL;
	pwindow->lastCharacter.xLoc = NULL;
	pwindow->lastCharacter.yLoc = NULL;
	pwindow->lastCharacter.xDim = 0;
	pwindow->lastCharacter.yDim = 0;
	pwindow->lastCharacter.numPixels = 0;
	pwindow->lastCharacter.show = false;
	pwindow->lastCharacter.causesNewline = false;
	
	pwindow->data = NULL;				// No window data yet
	setCurrentWindowColorSequence(NULL, 1, 0);	// Setup the default color (Which is NULL, so that you know it is not set yet)
}










// Callbacks default (blank) implementation
void hyperdisplayXLineCallback(int32_t x0, int32_t y0, uint16_t len, color_t color, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width){}
void hyperdisplayYLineCallback(int32_t x0, int32_t y0, uint16_t len, color_t color, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width){}
void hyperdisplayRectangleCallback(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, bool filled, uint16_t colorCycleLength, uint16_t startColorOffset, bool gradientVertical, bool reverseGradient){}
void hyperdisplayFillFromArrayCallback(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t size, color_t data){}
