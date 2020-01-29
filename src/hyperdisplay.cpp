/*
hyperdisplay.cpp

header file: hyperdisplay.h
*/


#include "hyperdisplay.h"					// Click here to get the library: http://librarymanager/SparkFun_HyperDisplay

wind_info_t hyperdisplayDefaultWindow;		// This window is used by default so that the user does not have to worry about windows if they don't want to
char_info_t hyperdisplayDefaultCharacter;	// The default character to use

#if HYPERDISPLAY_USE_PRINT
	#if HYPERDISPLAY_INCLUDE_DEFAULT_FONT   
		hd_font_extent_t hyperdisplayDefaultXloc[HYPERDISPLAY_DEFAULT_FONT_WIDTH*HYPERDISPLAY_DEFAULT_FONT_HEIGHT];
		hd_font_extent_t hyperdisplayDefaultYloc[HYPERDISPLAY_DEFAULT_FONT_WIDTH*HYPERDISPLAY_DEFAULT_FONT_HEIGHT];
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
// void hyperdisplay::hwpixel(hd_hw_extent_t x0, hd_hw_extent_t y0, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset)
// {
	// TIP: When you implement hwpixel for a particular device x0 and y0 are already guaranteed to be valid for your display area
	// (Subject to a few assumptions, of course, like that you're using a rectangular display and you've properly set xExt and yExt)
// }

void hyperdisplay::hwxline(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goLeft)
{
	// Note: color_t data is always a void pointer. You need to make sure that it points at the correct color type with enough elements.
	// In this case the correct number of elements is colorCycleLength
	if(data == NULL){ return; }
	if(colorCycleLength == 0){ return; }

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset because it could be greater than the cycle length
	color_t value = getOffsetColor(data, startColorOffset);

	if(goLeft)
	{
		for(hd_hw_extent_t indi = 0; indi < len; indi++)
		{
			hwpixel(x0 - indi, y0, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	else
	{
		for(hd_hw_extent_t indi = 0; indi < len; indi++)
		{
			hwpixel(x0 + indi, y0, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	hyperdisplayXLineCallback(x0, y0, len, data, colorCycleLength, startColorOffset, goLeft);
}

void hyperdisplay::hwyline(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goUp)
{
	// Note: color_t data is always a void pointer. You need to make sure that it points at the correct color type with enough elements.
	// In this case the correct number of elements is colorCycleLength
	if(data == NULL){ return; }
	if(colorCycleLength == 0){ return; }

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset because it could be greater than the cycle length
	color_t value = getOffsetColor(data, startColorOffset);

	if(goUp)
	{
		for(hd_hw_extent_t indi = 0; indi < len; indi++)
		{	
			hwpixel(x0, y0 - indi, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	else
	{
		for(hd_hw_extent_t indi = 0; indi < len; indi++)
		{
			hwpixel(x0, y0 + indi, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	hyperdisplayYLineCallback(x0, y0, len, data, colorCycleLength, startColorOffset, goUp);
}

void hyperdisplay::hwrectangle(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t x1, hd_hw_extent_t y1, bool filled, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool reverseGradient, bool gradientVertical)
{
	if(data == NULL){ return; }
	if(colorCycleLength == 0){ return; }

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset because it could be greater than the cycle length

	hd_hw_extent_t xlen = x1 - x0 + 1;
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
		hd_hw_extent_t ylen = y1 - y0 + 1;

		if(reverseGradient)
		{
			hwyline(x0, y0+1, ylen-2, data, colorCycleLength, startColorOffset, !reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen-2);
			hwxline(x0, y1, xlen, data, colorCycleLength, startColorOffset, !reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
			hwyline(x1, y1-1, ylen-2, data, colorCycleLength, startColorOffset, reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen-2);
			hwxline(x1, y0, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
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

void hyperdisplay::hwfillFromArray(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t x1, hd_hw_extent_t y1, color_t data, hd_pixels_t numPixels, bool Vh)
{
	if(data == NULL){ return; }
	hd_pixels_t startColorOffset = 0;

	if( Vh )
	{
		// Vertical lines first format
		for(hd_hw_extent_t x = x0; x <= x1; x++)
		{
			for(hd_hw_extent_t y = y0; y <= y1; y++)
			{
				color_t value = getOffsetColor(data, startColorOffset);
				startColorOffset = getNewColorOffset(numPixels, startColorOffset, 1);

				hwpixel(x, y, value, 1, 0 );
			}
		}
	}
	else
	{
		// Horizontal lines first format
		for(hd_hw_extent_t y = y0; y <= y1; y++)
		{
			for(hd_hw_extent_t x = x0; x <= x1; x++)
			{
				color_t value = getOffsetColor(data, startColorOffset);
				startColorOffset = getNewColorOffset(numPixels, startColorOffset, 1);

				hwpixel(x, y, value, 1, 0 );
			}
		}
	}

	hyperdisplayFillFromArrayCallback(x0, y0, x1, y1, numPixels, data);
}





// Buffer writing functions - all buffers areread left-to-right and top to bottom. Width and height are specified in the associated window's settings. Coordinates are window coordinates

hd_pixels_t		hyperdisplay::wToPix( wind_info_t* wind, hd_hw_extent_t x0, hd_hw_extent_t y0)
{
	if(wind == NULL){ return 0; } // Ideally we would have a better solution here...
	hd_hw_extent_t width = uabslen <hd_hw_extent_t> (wind->xMax, wind->xMin);
	hd_pixels_t pixOffst = x0 + (y0 * width);
	return pixOffst;
}

// void	swpixel( hd_extent_t x0, hd_extent_t y0, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset) = 0;

void	hyperdisplay::swxline( hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goLeft)
{
	// if(data == NULL){ return; }
	// if(colorCycleLength == 0){ return; }

	// startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset because it could be greater than the cycle length
	// color_t value = getOffsetColor(data, startColorOffset);

	// hd_hw_extent_t x0w = (hd_hw_extent_t)x0;	// Cast to hw extent type to be sure of integer values
	// hd_hw_extent_t y0w = (hd_hw_extent_t)y0;
	// hd_hw_extent_t lenw = (hd_hw_extent_t)len;

	// hd_pixels_t pixOffst = wToPix(pCurrentWindow, x0, y0);			// It was already ensured that this will be in range 
	// color_t dest = getOffsetColor(pCurrentWindow->data, pixOffst);	// Rely on the user's definition of a pixel's width in memory
	// uint32_t bpp = (uint32_t)getOffsetColor(0x00, 1);				// Getting the offset from zero for one pixel tells us how many bytes to copy

	// while(lenw > 0){												// Because of the checks already performed we know that len will fit into this row
	// 	hd_hw_extent_t num2copy = min( (colorCycleLength-startColorOffset), lenw);			// Determine how many to copy this time
	// 	memcpy((void*) dest, (void*)value, bpp*num2copy );									// Copy them
	// 	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, num2copy);	// Figure out new start offset
	// 	value = getOffsetColor(data, startColorOffset);										// Change the pointer to the color cycle
	// 	dest = (((uint8_t*)dest) + bpp*num2copy); 											// Increment the destination pointer 
	// 	lenw -= num2copy;																	// Decrement the reamining number of bytes
	// }

	if(data == NULL){ return; }
	if(colorCycleLength == 0){ return; }

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset because it could be greater than the cycle length
	color_t value = getOffsetColor(data, startColorOffset);

	if(goLeft)
	{
		for(hd_hw_extent_t indi = 0; indi < len; indi++)
		{
			swpixel(x0 - indi, y0, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	else
	{
		for(hd_hw_extent_t indi = 0; indi < len; indi++)
		{
			swpixel(x0 + indi, y0, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
}

void	hyperdisplay::swyline( hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goUp)
{
	// if(data == NULL){ return; }
	// if(colorCycleLength == 0){ return; }

	// startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset because it could be greater than the cycle length
	// color_t value = getOffsetColor(data, startColorOffset);

	// hd_hw_extent_t x0w = (hd_hw_extent_t)x0;	// Cast to hw extent type to be sure of integer values
	// hd_hw_extent_t y0w = (hd_hw_extent_t)y0;
	// hd_hw_extent_t lenw = (hd_hw_extent_t)len;

	// while(lenw > 0){												// Because of the checks already performed we know that len will fit into this column
	// 	swpixel( x0, y0, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
	// 	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, num2copy);	// Figure out new start offset
	// 	value = getOffsetColor(data, startColorOffset);										// Change the pointer to the color cycle
	// 	lenw--;																	// Decrement the reamining number of bytes

	// }

	if(data == NULL){ return; }
	if(colorCycleLength == 0){ return; }

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset because it could be greater than the cycle length
	color_t value = getOffsetColor(data, startColorOffset);

	if(goUp)
	{
		for(hd_hw_extent_t indi = 0; indi < len; indi++)
		{	
			swpixel(x0, y0 - indi, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
	else
	{
		for(hd_hw_extent_t indi = 0; indi < len; indi++)
		{
			swpixel(x0, y0 + indi, value, 1, 0);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
			value = getOffsetColor(data, startColorOffset);
		}
	}
}

void	hyperdisplay::swrectangle( hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, bool filled, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool reverseGradient, bool gradientVertical)
{
	// if(data == NULL){ return; }
	// if(colorCycleLength == 0){ return; }

	if(data == NULL){ return; }
	if(colorCycleLength == 0){ return; }

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset because it could be greater than the cycle length

	hd_hw_extent_t xlen = x1 - x0 + 1;
	if(filled)
	{
		color_t value = getOffsetColor(data, startColorOffset);
		if(gradientVertical)
		{
			if(reverseGradient)
			{
				for(uint16_t y = y1; y >= y0; y--)
				{
					swxline(x1, y, xlen, value, 1, 0, reverseGradient);
					startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
					value = getOffsetColor(data, startColorOffset);
				}
			}
			else
			{
				for(uint16_t y = y0; y <= y1; y++)
				{
					swxline(x0, y, xlen, value, 1, 0, reverseGradient);
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
					swxline(x1, y, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
				}
			}
			else
			{
				for(uint16_t y = y0; y <= y1; y++)
				{
					swxline(x0, y, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
				}
			}
		}
	}
	else
	{
		hd_hw_extent_t ylen = y1 - y0 + 1;

		if(reverseGradient)
		{
			swyline(x0, y0+1, ylen-2, data, colorCycleLength, startColorOffset, !reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen-2);
			swxline(x0, y1, xlen, data, colorCycleLength, startColorOffset, !reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
			swyline(x1, y1-1, ylen-2, data, colorCycleLength, startColorOffset, reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen-2);
			swxline(x1, y0, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
		}
		else
		{
			swxline(x0, y0, xlen, data, colorCycleLength, startColorOffset, reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
			swyline(x1, y0+1, ylen-2, data, colorCycleLength, startColorOffset, reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, ylen-2);
			swxline(x1, y1, xlen, data, colorCycleLength, startColorOffset, !reverseGradient);
			startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, xlen);
			swyline(x0, y1-1, ylen-2, data, colorCycleLength, startColorOffset, !reverseGradient);
		}
	}

}

void	hyperdisplay::swfillFromArray( hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, color_t data, hd_pixels_t numPixels,  bool Vh )
{
	if(data == NULL){ return; }
	hd_pixels_t startColorOffset = 0;

	if( Vh )
	{
		// Vertical lines first format
		for(hd_hw_extent_t x = x0; x <= x1; x++)
		{
			for(hd_hw_extent_t y = y0; y <= y1; y++)
			{
				color_t value = getOffsetColor(data, startColorOffset);
				startColorOffset = getNewColorOffset(numPixels, startColorOffset, 1);

				swpixel(x, y, value, 1, 0 );
			}
		}
	}
	else
	{
		// Horizontal lines first format
		for(hd_hw_extent_t y = y0; y <= y1; y++)
		{
			for(hd_hw_extent_t x = x0; x <= x1; x++)
			{
				color_t value = getOffsetColor(data, startColorOffset);
				startColorOffset = getNewColorOffset(numPixels, startColorOffset, 1);

				swpixel(x, y, value, 1, 0 );
			}
		}
	}

}





/////////////////////////////////////////////
// 			Primitive Drawing Functions
/////////////////////////////////////////////
void hyperdisplay::pixel(hd_extent_t x0, hd_extent_t y0, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
{
	if(pCurrentWindow->bufferMode){
		if(wToPix(pCurrentWindow, x0, y0) >= (pCurrentWindow->numPixels)){ return; }	// Since we will never reposition a single pixel we can bail out early if it is beyond the number of pixel types available in the window buffer
		hyperdisplay_dim_check_t x0c = enforceSWLimits(&x0, false);						// Check the x-dimension and convert to hardware coordintaes using the currently assigned window
		hyperdisplay_dim_check_t y0c = enforceSWLimits(&y0, true); 						// Check the y-dimension and convert to hardware coordintaes using the currently assigned window
		if((x0c != hyperdisplay_dim_ok) || (y0c != hyperdisplay_dim_ok)){ return; }		// Do not print a single pixel at the wrong location ever
		if(data == NULL){ swpixel( x0, y0, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset); }
		else{ swpixel( x0, y0, data, colorCycleLength, startColorOffset); }
	}else{
		hd_hw_extent_t x0hw, y0hw;											
		hyperdisplay_dim_check_t x0c = enforceHWLimits(&x0, &x0hw, false);				// Check the x-dimension and convert to hardware coordintaes using the currently assigned window
		hyperdisplay_dim_check_t y0c = enforceHWLimits(&y0, &y0hw, true); 				// Check the y-dimension and convert to hardware coordintaes using the currently assigned window
		if((x0c != hyperdisplay_dim_ok) || (y0c != hyperdisplay_dim_ok)){ return; }		// Do not print a single pixel at the wrong location ever
		// Check if we are to use the default color
		if(data == NULL){ hwpixel(x0hw, y0hw, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset); }
		else{ hwpixel(x0hw, y0hw, data, colorCycleLength, startColorOffset); }
	}
}

void hyperdisplay::xline(hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goLeft)
{
	hd_extent_t x1;
	if(goLeft){ x1 = x0 - len; }else{ x1 = x0 + len; }									// Calculate where x1 would be in window coordinates
	if(pCurrentWindow->bufferMode){
		if(pCurrentWindow->data == NULL){ return; }
		hyperdisplay_dim_check_t x0c = enforceSWLimits(&x0, false);					// 'enforceHWLimits()' converts windows coordinates to hardware coordinates and restricts the dimension to be in drawable area
		hyperdisplay_dim_check_t x1c = enforceSWLimits(&x1, false);					// When the resulting hardware coordinate is in-bounds the function returns "hyperdisplay_dim_ok," otherwise is returns an applicable error
		hyperdisplay_dim_check_t y0c = enforceSWLimits(&y0, true); 					// Next use the 'enforceHWLimits()' results to make sure the desired object is within the active window
		if(y0c != hyperdisplay_dim_ok){ return; }											// Don't do it if y was wrong
		if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom dot at xMin)
		if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom dot at xMax)
		if(wToPix(pCurrentWindow, x0, y0) >= (pCurrentWindow->numPixels)){ return; }		// Now that start/stop points have had the chance to be adjusted we can make sure they fit in the current window buffer (and hence give them the seal of approval)
		if(wToPix(pCurrentWindow, x1, y0) >= (pCurrentWindow->numPixels)){ return; }		// Same thing for the other coordinate pair (jic)
		hd_extent_t rken = uabslen <hd_extent_t> (x1, x0);	
		if(data == NULL){ swxline( x0, y0, len, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, goLeft); }	// Check if default color
		else{ swxline( x0, y0, rken, data, colorCycleLength, startColorOffset, goLeft); }
	}else{
		hd_hw_extent_t x0hw, x1hw, y0hw;
		hyperdisplay_dim_check_t x0c = enforceHWLimits(&x0, &x0hw, false);					// 'enforceHWLimits()' converts windows coordinates to hardware coordinates and restricts the dimension to be in drawable area
		hyperdisplay_dim_check_t x1c = enforceHWLimits(&x1, &x1hw, false);					// When the resulting hardware coordinate is in-bounds the function returns "hyperdisplay_dim_ok," otherwise is returns an applicable error
		hyperdisplay_dim_check_t y0c = enforceHWLimits(&y0, &y0hw, true); 					// Next use the 'enforceHWLimits()' results to make sure the desired object is within the active window
		if(y0c != hyperdisplay_dim_ok){ return; }											// Don't do it if y was wrong
		if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom dot at xMin)
		if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom dot at xMax)
		// Check if default color
		hd_hw_extent_t rken = uabslen <hd_hw_extent_t> (x1hw, x0hw);
		if(data == NULL){ hwxline(x0hw, y0hw, rken, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, goLeft); }
		else{ hwxline(x0hw, y0hw, rken, data, colorCycleLength, startColorOffset, goLeft); }
	}
}

void hyperdisplay::yline(hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goUp)
{
	hd_extent_t y1;
	if(goUp){ y1 = y0 - len; }else{ y1 = y0 + len; }							// Calculate where y1 would be in window coordinates
	if(pCurrentWindow->bufferMode){
		if(pCurrentWindow->data == NULL){ return; }
		hyperdisplay_dim_check_t y0c = enforceSWLimits(&y0, true);						// 'enforceHWLimits()' converts windows coordinates to hardware coordinates and restricts the dimension to be in drawable area
		hyperdisplay_dim_check_t y1c = enforceSWLimits(&y1, true);						// When the resulting hardware coordinate is in-bounds the function returns "hyperdisplay_dim_ok," otherwise is returns an applicable error
		hyperdisplay_dim_check_t x0c = enforceSWLimits(&x0, false); 					// Next use the 'enforceHWLimits()' results to make sure the desired object is within the active window
		if(x0c != hyperdisplay_dim_ok){ return; }											// Don't do it if x was wrong
		if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom dot at yMin)
		if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom dot at yMax)
		if(wToPix(pCurrentWindow, x0, y0) >= (pCurrentWindow->numPixels)){ return; }		// Now that start/stop points have had the chance to be adjusted we can make sure they fit in the current window buffer (and hence give them the seal of approval)
		if(wToPix(pCurrentWindow, x0, y1) >= (pCurrentWindow->numPixels)){ return; }		// Same thing for the other coordinate pair (jic)
		hd_extent_t rken = uabslen <hd_extent_t> (y1, y0);
		if(data == NULL){ swyline( x0, y0, len, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, goUp); }
		else{ swyline( x0, y0, rken, data, colorCycleLength, startColorOffset, goUp); }
	}else{
		hd_hw_extent_t y0hw, y1hw, x0hw;
		hyperdisplay_dim_check_t y0c = enforceHWLimits(&y0, &y0hw, true);						// 'enforceHWLimits()' converts windows coordinates to hardware coordinates and restricts the dimension to be in drawable area
		hyperdisplay_dim_check_t y1c = enforceHWLimits(&y1, &y1hw, true);						// When the resulting hardware coordinate is in-bounds the function returns "hyperdisplay_dim_ok," otherwise is returns an applicable error
		hyperdisplay_dim_check_t x0c = enforceHWLimits(&x0, &x0hw, false); 					// Next use the 'enforceHWLimits()' results to make sure the desired object is within the active window
		if(x0c != hyperdisplay_dim_ok){ return; }											// Don't do it if x was wrong
		if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom dot at yMin)
		if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom dot at yMax)
		hd_hw_extent_t rken = uabslen <hd_hw_extent_t> (y1hw, y0hw);
		if(data == NULL){ hwyline(x0hw, y0hw, rken, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, goUp); }
		else{ hwyline(x0hw, y0hw, rken, data, colorCycleLength, startColorOffset, goUp); }
	}
}

void hyperdisplay::rectangle(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, bool filled, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool reverseGradient, bool gradientVertical)
{
	if(x0 > x1){ swap <hd_extent_t> (&x0, &x1); }
	if(y0 > y1){ swap <hd_extent_t> (&y0, &y1); }
	if(pCurrentWindow->bufferMode){
		if(pCurrentWindow->data == NULL){ return; }
		hyperdisplay_dim_check_t y0c = enforceSWLimits(&y0, true);							// if the dimension was off-window then it will now be on the edge. enforceHWLimits also applies the transformation into hw coordinates
		hyperdisplay_dim_check_t y1c = enforceSWLimits(&y1, true);
		hyperdisplay_dim_check_t x0c = enforceSWLimits(&x0, false); 
		hyperdisplay_dim_check_t x1c = enforceSWLimits(&x1, false);							// Next use 'enforceHWLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwrectangle()
		if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom line at yMin)
		if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom line at yMax)
		if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom line at xMin)
		if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom line at xMax)
		if(wToPix(pCurrentWindow, x0, y0) >= (pCurrentWindow->numPixels)){ return; }		// Now that start/stop points have had the chance to be adjusted we can make sure they fit in the current window buffer (and hence give them the seal of approval)
		if(wToPix(pCurrentWindow, x1, y0) >= (pCurrentWindow->numPixels)){ return; }		// Repeat for all coordinate pairs
		if(wToPix(pCurrentWindow, x0, y1) >= (pCurrentWindow->numPixels)){ return; }		// Repeat for all coordinate pairs
		if(wToPix(pCurrentWindow, x1, y1) >= (pCurrentWindow->numPixels)){ return; }		// Repeat for all coordinate pairs
		if(data == NULL){ swrectangle( x0, y0, x1, y1, filled, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, reverseGradient, gradientVertical); }
		else{ swrectangle( x0, y0, x1, y1, filled, data, colorCycleLength, startColorOffset, reverseGradient, gradientVertical); }
	}else{
		hd_hw_extent_t x0hw, x1hw, y0hw, y1hw;
		hyperdisplay_dim_check_t y0c = enforceHWLimits(&y0, &y0hw, true);						// if the dimension was off-window then it will now be on the edge. enforceHWLimits also applies the transformation into hw coordinates
		hyperdisplay_dim_check_t y1c = enforceHWLimits(&y1, &y1hw, true);
		hyperdisplay_dim_check_t x0c = enforceHWLimits(&x0, &x0hw, false); 
		hyperdisplay_dim_check_t x1c = enforceHWLimits(&x1, &x1hw, false);					// Next use 'enforceHWLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwrectangle()
		if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom line at yMin)
		if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom line at yMax)
		if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom line at xMin)
		if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom line at xMax)
		if(data == NULL){  hwrectangle(x0hw, y0hw, x1hw, y1hw, filled, pCurrentWindow->currentSequenceData, pCurrentWindow->currentColorCycleLength, pCurrentWindow->currentColorOffset, gradientVertical, reverseGradient); }
		else{ hwrectangle(x0hw, y0hw, x1hw, y1hw, filled, data, colorCycleLength, startColorOffset, reverseGradient, gradientVertical); }
	}
}

void hyperdisplay::fillFromArray(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, color_t data, hd_pixels_t numPixels, bool Vh)
{
	if(data == NULL){ return; }
	if(x0 > x1){ swap <hd_extent_t> (&x0, &x1); }
	if(y0 > y1){ swap <hd_extent_t> (&y0, &y1); }
	if(pCurrentWindow->bufferMode){
		if(pCurrentWindow->data == NULL){ return; }
		hyperdisplay_dim_check_t y0c = enforceSWLimits(&y0, true);
		hyperdisplay_dim_check_t y1c = enforceSWLimits(&y1, true);
		hyperdisplay_dim_check_t x0c = enforceSWLimits(&x0, false); 
		hyperdisplay_dim_check_t x1c = enforceSWLimits(&x1, false);	// Next use 'enforceHWLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwfillFromArray()
		if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom line at yMin)
		if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom line at yMax)
		if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom line at xMin)
		if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom line at xMax)
		if(wToPix(pCurrentWindow, x0, y0) >= (pCurrentWindow->numPixels)){ return; }		// Now that start/stop points have had the chance to be adjusted we can make sure they fit in the current window buffer (and hence give them the seal of approval)
		if(wToPix(pCurrentWindow, x1, y0) >= (pCurrentWindow->numPixels)){ return; }		// Repeat for all coordinate pairs
		if(wToPix(pCurrentWindow, x0, y1) >= (pCurrentWindow->numPixels)){ return; }		// Repeat for all coordinate pairs
		if(wToPix(pCurrentWindow, x1, y1) >= (pCurrentWindow->numPixels)){ return; }		// Repeat for all coordinate pairs
		hd_extent_t xlen = uabslen <hd_extent_t> (x1, x0);
		hd_extent_t ylen = uabslen <hd_extent_t> (y1, y0);
		hd_pixels_t rNP = xlen*ylen;		
		swfillFromArray( x0, y0, x1, y1, data, rNP, Vh );
	}else{
		hd_hw_extent_t x0hw, x1hw, y0hw, y1hw;
		hyperdisplay_dim_check_t y0c = enforceHWLimits(&y0, &y0hw, true);
		hyperdisplay_dim_check_t y1c = enforceHWLimits(&y1, &y1hw, true);
		hyperdisplay_dim_check_t x0c = enforceHWLimits(&x0, &x0hw, false); 
		hyperdisplay_dim_check_t x1c = enforceHWLimits(&x1, &x1hw, false);	// Next use 'enforceHWLimits()' to make sure the desired object is within the active window, as well as to convert from window coordiantes to hw coordiantes for the upcoming call to hwfillFromArray()
		if((y0c == hyperdisplay_dim_low) && (y1c == hyperdisplay_dim_low)){ return; }		// Don't do it if y0 and y1 were both low (would cause phantom line at yMin)
		if((y0c == hyperdisplay_dim_high) && (y1c == hyperdisplay_dim_high)){ return; }		// Don't do it if y0 and y1 were both high (would cause phantom line at yMax)
		if((x0c == hyperdisplay_dim_low) && (x1c == hyperdisplay_dim_low)){ return; }		// Don't do it if x0 and x1 were both low (would cause phantom line at xMin)
		if((x0c == hyperdisplay_dim_high) && (x1c == hyperdisplay_dim_high)){ return; }		// Don't do it if x0 and x1 were both high (would cause phantom line at xMax)
		hd_hw_extent_t xlen = uabslen <hd_hw_extent_t> (x1hw, x0hw);
		hd_hw_extent_t ylen = uabslen <hd_hw_extent_t> (y1hw, y0hw);
		hd_pixels_t rNP = xlen*ylen;
		if(numPixels < rNP){ rNP = numPixels; }
		hwfillFromArray(x0hw, y0hw, x1hw, y1hw, data, rNP, Vh);
	}
}

void hyperdisplay::fillWindow(color_t color, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
{
	// The rectangle function uses window coordinates, so to fill a window you go from (0,0) to (xWidth, yWidth)
	rectangle( 0, 0, pCurrentWindow->xMax - pCurrentWindow->xMin, pCurrentWindow->yMax - pCurrentWindow->yMin, true, color, colorCycleLength, startColorOffset, false, false);
}



void hyperdisplay::setWindowColorSequence(wind_info_t * wind, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
{
	if(wind == NULL){ return; }
	wind->currentSequenceData = data;
	wind->currentColorCycleLength = colorCycleLength;
	wind->currentColorOffset = startColorOffset;
}

void hyperdisplay::setCurrentWindowColorSequence(color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
{
	setWindowColorSequence(pCurrentWindow, data, colorCycleLength, startColorOffset);
}

int        hyperdisplay::setWindowMemory(wind_info_t * wind, color_t data, hd_pixels_t numPixels, uint8_t bpp, bool allowDynamic)
{
	if(wind == NULL){ return -1; }
	if (wind->dynamic) {
		if(wind->data != NULL){		// If there was a buffer previously associated you need to handle getting rid of it
			if(wind->dynamic){		// If it was dynamically allocated then free the memory
				free(wind->data); 	// Maybe should use delete?
			}
			wind->numPixels = 0;
			wind->dynamic = false;
		}
	}
	if(data == NULL){			// If the user does not supply a pointer then try dynamic allocation
		if(allowDynamic){
			color_t ptemp = NULL;
			ptemp = (color_t)malloc(numPixels*bpp*sizeof(uint8_t));
			if(ptemp == NULL){
				return -1; // Could not allocate memory
			}
			wind->data = ptemp;
			wind->dynamic = true;
		}
		else{
			return -1;
		}
	}else{
		wind->data = data;
		wind->dynamic = false;
	}
	wind->numPixels = numPixels;
	return 0;	// Good!
}

int        hyperdisplay::setCurrentWindowMemory( color_t data, hd_pixels_t numPixels, uint8_t bpp, bool allowDynamic)
{
	return setWindowMemory(pCurrentWindow, data, numPixels, bpp, allowDynamic);
}



// Buffer and Show
void        hyperdisplay::buffer( wind_info_t * wind ){ // Puts the current window into buffer mode - drawing commands are performed on the current window's data buffer - if available
	if(wind != NULL){
		wind->bufferMode = true;
	}else{
		pCurrentWindow->bufferMode = true;
	}
}

void        hyperdisplay::direct( wind_info_t * wind ){ // Cancels buffer mode. Drawing commands will go straight to display memory. Buffered data will remain and can still be shown later
	if(wind != NULL){
		wind->bufferMode = false;
	}else{
		pCurrentWindow->bufferMode = false;
	}
}

void        hyperdisplay::show( wind_info_t * wind ){   // Outputs the current window's buffered data to the display
	if(wind != NULL){
		wind_info_t* pLastWindow = pCurrentWindow; // Since fillFromArray uses window coordinates we need to change the current window, temporarily
		pCurrentWindow = wind;
		bool lastBuffMode = wind->bufferMode;		// Save the state of bufferMode
		wind->bufferMode = false;					// If we tried to output data while still in buffer mode it would be a mess!
		fillFromArray(0, 0, wind->xMax - wind->xMin, wind->yMax - wind->yMin, wind->data, wind->numPixels, false);	// Use horizontal mode
		wind->bufferMode = lastBuffMode;			// Put the window back into whatever buffer mode it had been in before.
		pCurrentWindow = pLastWindow;				// Switch back to the old window
	}else{
		bool lastBuffMode = pCurrentWindow->bufferMode;		// Save the state of bufferMode
		pCurrentWindow->bufferMode = false;					// If we tried to output data while still in buffer mode it would be a mess!
		fillFromArray(0, 0, pCurrentWindow->xMax - pCurrentWindow->xMin, pCurrentWindow->yMax - pCurrentWindow->yMin, pCurrentWindow->data, pCurrentWindow->numPixels, false); // Use horizontal mode
		pCurrentWindow->bufferMode = lastBuffMode;			// Put the window back into whatever buffer mode it had been in before.
	}
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
		void hyperdisplay::getCharInfo(uint8_t character, char_info_t * character_info) 
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
						*(character_info->xLoc + n) = (hd_font_extent_t)indi;
						*(character_info->yLoc + n) = (hd_font_extent_t)indj;
						n++;
					}
				}
			}
		}
	#else
		#warning "HyperDisplay Default Font Not Included. Printing will not work without a custom implementation"
		void hyperdisplay::getCharInfo(uint8_t character, char_info_t * character_info) 
		{
			// A blank implementation of getCharInfo for when the target does not support <avr/pgmspace.h>
			character_info->show = false;
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
	if(!window){ window = pCurrentWindow; }	//default to current
	if(!window){ return; } 					// Dont operate on null
    window->cursorX = x0;               	// Where the cursor is currently in window-coordinates
    window->cursorY = y0;               	// Where the cursor is currently in window-coordinates
}

void hyperdisplay::resetTextCursor(wind_info_t * window){
	if(!window){ window = pCurrentWindow; }	//default to current
	if(!window){ return; } 					// Dont operate on null
	setTextCursor(window->xReset, window->yReset, window);
}






















/////////////////////////////////////////////
// 			Level 1 Drawing Functions
/////////////////////////////////////////////
#if HYPERDISPLAY_DRAWING_LEVEL > 0
uint16_t hyperdisplay::line(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool reverseGradient)
{
	hd_extent_t absY = uabslen <hd_extent_t> (y1, y0);
	hd_extent_t absX = uabslen <hd_extent_t> (x1, x0);

	startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 0);	// This line is needed to condition the user's input start color offset

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

void hyperdisplay::polygon(hd_extent_t x[], hd_extent_t y[], uint8_t numSides, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool reverseGradient)
{
	uint8_t indi = 0;
	int32_t numWritten = 0; 

	hd_extent_t dx;
	hd_extent_t dy;
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
		if(dx < 0)
		{
			rev = !rev;
		}
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

void hyperdisplay::circle(hd_extent_t x0, hd_extent_t y0, hd_extent_t radius, bool filled, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool reverseGradient)
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




#if HYPERDISPLAY_USE_RAY_TRACING

void hyperdisplay::filledPolygon(sf2drt_polygon* poly, uint16_t width, bool filled, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool reverseGradient)
{
	sf2drt_max_qty_t numSides = poly->_num_sides;

	if(filled)
	{
		// Find a bounding box around the polygon:
		sf2drt_bounds_t bounds = poly->getBounds();

		// Check each and every point within those bounds
		sf2drt_coordinate_t p;
		for(sf2drt_extent_t indi = bounds.minx; indi <= bounds.maxx; indi++)
		{
			p.x = indi;
			for(sf2drt_extent_t indj = bounds.miny; indj <= bounds.maxy; indj++)
			{
				p.y = indj;
				if(poly->contains(p))
				{
					pixel(p.x, p.y, data, colorCycleLength, startColorOffset);
					startColorOffset = getNewColorOffset(colorCycleLength, startColorOffset, 1);
				}
			}
		}
	}
	else
	{
		for(sf2drt_max_qty_t indi = 0; indi < numSides; indi++)
		{
			if(indi == ( numSides - 1) )
			{
				line( (hd_extent_t)((*(poly->_p_points + indi)).x), (hd_extent_t)((*(poly->_p_points + indi)).y), (hd_extent_t)((*(poly->_p_points + 0)).x), (hd_extent_t)((*(poly->_p_points + 0)).y), width, data, colorCycleLength, startColorOffset, reverseGradient); 
			}
			else
			{
				line( (hd_extent_t)((*(poly->_p_points + indi)).x), (hd_extent_t)((*(poly->_p_points + indi)).y), (hd_extent_t)((*(poly->_p_points + (indi+1))).x), (hd_extent_t)((*(poly->_p_points + (indi+1))).y), width, data, colorCycleLength, startColorOffset, reverseGradient); 
			}
		}
	}
}

#endif /* HYPERDISPLAY_USE_RAYTRACING */








// Protected drawing functions
hd_extent_t hyperdisplay::lineHighNorm(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
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

hd_extent_t hyperdisplay::lineHighReverse(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
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
    	
hd_extent_t hyperdisplay::lineLowNorm(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
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

hd_extent_t hyperdisplay::lineLowReverse(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
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

void hyperdisplay::circle_Bresenham(hd_extent_t x0, hd_extent_t y0, hd_extent_t radius, color_t color, bool fill)
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

void hyperdisplay::circle_midpoint(hd_extent_t x0, hd_extent_t y0, hd_extent_t radius, color_t color, bool fill)
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

void hyperdisplay::circle_eight(hd_extent_t x0, hd_extent_t y0, hd_extent_t dx, hd_extent_t dy, color_t color, bool fill)
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

// enforceHWLimits takes a window-coordinate and checks if it is within bounds on either the x or y axes, and also converts the value to a hardware coordinate
hyperdisplay_dim_check_t hyperdisplay::enforceHWLimits(hd_extent_t * windowvar, hd_hw_extent_t* hardwarevar, bool axisSelect)
{
	bool low = false, high = false;
	if(windowvar == NULL){ return hyperdisplay_dim_no_val; }

	if(axisSelect)
	{
		// y axis
		hd_extent_t inter = (*windowvar) + (hd_extent_t)(pCurrentWindow->yMin);								// Inter is the hardware coordintate represented in a window-coordinate variable type (to check if it will come out negative)
		if(inter < 0){ low = true; *hardwarevar = 0; }														// Hardware coordinates are never allowed to be negative
		else{ *hardwarevar = (hd_hw_extent_t)(inter); } 													// Cast to hw_extent_t and apply transformation into hw coordiantes for comparison + usage in hw_____ functions
		if( *hardwarevar > (pCurrentWindow->yMax) ){ high = true; *hardwarevar = pCurrentWindow->yMax; }	// Compare to hardware coordinate limits of the window
		if( *hardwarevar < (pCurrentWindow->yMin) ){ low = true;  *hardwarevar = pCurrentWindow->yMin; }	// Compare to hardware coordinate limits of the window
		if( *hardwarevar >= yExt ){ high = true; *hardwarevar = (yExt-1); }									// Compare to hardware screen dimensions
		if( *hardwarevar < 0 ){ low = true; *hardwarevar = 0; }												// Compare to hardware screen dimensions
	}
	else
	{
		// x axis
		hd_extent_t inter = (*windowvar) + (hd_extent_t)(pCurrentWindow->xMin);								// Inter is the hardware coordintate represented in a window-coordinate variable type (to check if it will come out negative)
		if(inter < 0){ low = true; *hardwarevar = 0; }														// Hardware coordinates are never allowed to be negative
		else{ *hardwarevar = (hd_hw_extent_t)(inter); } 													// Cast to hw_extent_t and apply transformation into hw coordiantes for comparison + usage in hw_____ functions
		if( *hardwarevar > (pCurrentWindow->xMax) ){ high = true; *hardwarevar = pCurrentWindow->xMax; }	// Compare to hardware coordinate limits of the window
		if( *hardwarevar < (pCurrentWindow->xMin) ){ low = true;  *hardwarevar = pCurrentWindow->xMin; }	// Compare to hardware coordinate limits of the window
		if( *hardwarevar >= xExt ){ high = true; *hardwarevar = (xExt-1); }									// Compare to hardware screen dimensions
	}
	if(high){ return hyperdisplay_dim_high; }
	if(low ){ return hyperdisplay_dim_low;  }
	return hyperdisplay_dim_ok;
}

hyperdisplay_dim_check_t	hyperdisplay::enforceSWLimits(hd_extent_t* windowvar, bool axisSelect)
{
	// This function is a LOT like the one before it except that it does not convert into HW coordinates
	bool low = false, high = false;
	if(windowvar == NULL){ return hyperdisplay_dim_no_val; }

	if(axisSelect)
	{
		// y axis
		hd_extent_t height = uabslen <hd_extent_t> (pCurrentWindow->yMax, pCurrentWindow->yMin);			// Get the height of the window in number of pixels
		if(*windowvar < 0){ low = true; *windowvar = 0; }													// The window var must be positive
		if( *windowvar >= height ){ high = true; *windowvar = (height-1); }									// The window var must be less than the height (b/c of indexing)
	}
	else
	{
		// x axis
		hd_extent_t width = uabslen <hd_extent_t> (pCurrentWindow->xMax, pCurrentWindow->xMin);
		if(*windowvar < 0){ low = true; *windowvar = 0; }													// The window var must be positive
		if( *windowvar >= width ){ high = true; *windowvar = (width-1); }									// The window var must be less than the width (b/c of indexing)
	}
	if(high){ return hyperdisplay_dim_high; }
	if(low ){ return hyperdisplay_dim_low;  }
	return hyperdisplay_dim_ok;
}

void hyperdisplay::setWindowDefaults(wind_info_t * pwindow)
{
	if(pwindow == NULL){ return; }

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
	
	pwindow->bufferMode = false;			// Start out in direct mode
	pwindow->data = NULL;				// No window data yet
	pwindow->numPixels = 0;
	pwindow->dynamic = false;
	setWindowColorSequence(pwindow, NULL, 0, 0);	// Setup the default color (Which is NULL, so that you know it is not set yet)
}










// Callbacks default (blank) implementation
void hyperdisplayXLineCallback(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goLeft){}
void hyperdisplayYLineCallback(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goUp){}
void hyperdisplayRectangleCallback(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t x1, hd_hw_extent_t y1, color_t data, bool filled, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool gradientVertical, bool reverseGradient){}
void hyperdisplayFillFromArrayCallback(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t x1, hd_hw_extent_t y1, hd_pixels_t numPixels, color_t data){}
