/*

This is a template for the bare minimum implementation that can actually output to a given hardware display.

Created: August 2018
Modified: August 2018

Authors: Owen Lyke

*/

#ifndef BARE_MIN_DERIVED_CLASS_H
#define BARE_MIN_DERIVED_CLASS_H
////////////////////////////////////////////////////////////
//							Includes    				  //
////////////////////////////////////////////////////////////
#include "hyperdisplay.h"										// Click here to get the library: http://librarymanager/All#SparkFun_HyperDisplay

////////////////////////////////////////////////////////////
//							Defines     				  //
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//							Typedefs    				  //
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//					 Class Definition   				  //
////////////////////////////////////////////////////////////
class bareMinDerived : virtual public hyperdisplay{				// Using hyperdisplay as virtual will allow support for future I/O libraries that also inherit from hyperdisplay 
private:
protected:
public:

	// Constructor: at minimum pass in the size of the display 
	/*
		xSize: number of pixels in the x direction of the display
		ySize: number of pixels in the y direction of the display
	*/
	bareMinDerived(uint16_t xSize, uint16_t ySize /* Additional Parameters */);												


	// getoffsetColor: allows hyperdisplay to use your custom color type
	/*
		base: the pointer to the first byte of the array that holds the color data
		numPixels: the number of pixels away from the beginning that the function should return the pointer to
	*/
	color_t 	getOffsetColor(color_t base, uint32_t numPixels);

	// hwPixel: the method by which hyperdisplay actually changes your screen
	/*
		x0, y0: the x and y coordinates at which to place the pixel. 0,0 is the upper-left corner of the screen, x is horizontal and y is vertical
		data: the pointer to where the color data is stored
		colorCycleLength: this indicates how many pixels worth of valid color data exist contiguously after the memory location pointed to by color.  
		startColorOffset: this indicates how many pixels to offset by from the color pointer to arrive at the actual color to display
	*/
    void    hwpixel(hd_hw_extent_t x0, hd_hw_extent_t y0, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0);

    // Additional hardware drawing functions
    /*
		There are additional hardware drawing functions beyond hwpixel. They are already implemented by default using
		hwpixel so they are not required in order to start drawing. However implementing them with more efficient 
		methods for your particular hardware can reduce overhead and speed up the drawing process.  

		In these functions the coordiantes x0, x1, y0, and y1 are always with respect to the hardware screen. (0,0) is the upper-left pixel
		The variables pertaining to color sequences (data, colorCycleLength, and startColorOffset) always have the same meaning as in hwpixel
		Additional variables will be described in the function prototype in bareMinimumDerivedClass.cpp
    */
    // void hwxline(uint16_t x0, uint16_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goLeft = false); 		
    // void hwyline(uint16_t x0, uint16_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goUp = false); 			
    // void hwrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t data, bool filled = false, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool gradientVertical = false, bool reverseGradient = false); 	
    // void hwfillFromArray(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t numPixels, color_t data); 			

    // Additional optional implementations by the user:
    // ================================================

    // getCharInfo: you can create custom fonts without changing how printing functions work
    /*
		character: the byte-sized character to show on screen
		pchar: a pointer to a valid char_info_t object that needs to be filled out peroperly for the given character
    */
	// void getCharInfo(uint8_t character, char_info_t * pchar);       
										

   	// write: you decide what happens when someone calls bareMinDerived.print or bareMinDerived.println
    /*
		val: the byte-sized character value to display
    */
    // size_t write(uint8_t val);                                     
};

#endif /* BARE_MIN_DERIVED_CLASS_H */
