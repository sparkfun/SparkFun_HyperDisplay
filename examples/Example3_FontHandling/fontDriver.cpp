#include "fontDriver.h"


// Constructor: at minimum pass in the size of the display 
/*
	xSize: number of pixels in the x direction of the display
	ySize: number of pixels in the y direction of the display

	*Note:
	This notation allows you to explicitly state what variables are passed to the parent class's constructor when the derived class' constructor is called.
	Additional direct or virtual base classes can also be initialized by a comma separated list with the same syntax - the 'deepest' base class is listed first.
*/
fontDriver::fontDriver(uint16_t xSize, uint16_t ySize /* Additional Parameters */) : hyperdisplay(xSize, ySize)	/* , anotherVirtualBaseClass(params), aDirectBaseClass(moreParams) */
{
	// Perform setup of the derived class with any additional parameters here.
}

	

// getoffsetColor: allows hyperdisplay to use your custom color type
/*
	base: the pointer to the first byte of the array that holds the color data
	numPixels: the number of pixels away from the beginning that the function should return the pointer to
*/
color_t 	fontDriver::getOffsetColor(color_t base, uint32_t numPixels)
{
	// This method is requried so that your color type can be totally flexible - be it an enumeration of three colors for an E-ink
	// display or a structure of bytes for 24-bit color it is totally up to you and how your display works.

	// This implementation will depend on how you choose to store colors, however one decent way to do it is provided as a reference:
	// This function returns an offset pointer according to the number of pixels and the _colorMode of the object
	
	// color_t pret = NULL;										
	// your_color_type * ptemp = (your_color_type *)base;	// Here's the magic. Cast the base pointer to a pointer of your color type to allow pointer arithmetic
	// pret = (color_t)(ptemp + numPixels);					// The offset by the number of pixels. This will account for the number of bytes that your color type occupies
	// return pret; 										// And return the offset pointer
}

// hwPixel: the method by which hyperdisplay actually changes your screen
/*
	x0, y0: the x and y coordinates at which to place the pixel. 0,0 is the upper-left corner of the screen, x is horizontal and y is vertical
	data: the pointer to where the color data is stored
	colorCycleLength: this indicates how many pixels worth of valid color data exist contiguously after the memory location pointed to by color.  
	startColorOffset: this indicates how many pixels to offset by from the color pointer to arrive at the actual color to display
*/
void 		fontDriver::hwpixel(hd_hw_extent_t x0, hd_hw_extent_t y0, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset)
{
	// Here you write the code that sets a pixel. It is up to you what to do with that data. Here are two basic options:

	// 1) Write directly to display ram: if you choose this option and your display supports it then this is all you need to show an image
	// 2) Write to a scratch space: you might use this option to compose a whole image and then show it all on the screen at once. 
	// 		In that case you would need your own function that actually gets all that information to the display when the time is right.
}

// Additional hardware drawing functions
/*
	There are additional hardware drawing functions beyond hwpixel. They are already implemented by default using
	hwpixel so they are not required in order to start drawing. However implementing them with more efficient 
	methods for your particular hardware can reduce overhead and speed up the drawing process.  

	In these functions the coordiantes x0, x1, y0, and y1 are always with respect to the hardware screen. (0,0) is the upper-left pixel
	The variables pertaining to color sequences (data, colorCycleLength, and startColorOffset) always have the same meaning as in hwpixel
	Additional variables will be described in the function prototype in bareMinimumDerivedClass.cpp
*/
// void fontDriver::hwxline(uint16_t x0, uint16_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goLeft = false)	
// void fontDriver::hwyline(uint16_t x0, uint16_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goUp = false); 			
// void fontDriver::hwrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t data, bool filled = false, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool gradientVertical = false, bool reverseGradient = false); 	
// void fontDriver::hwfillFromArray(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t numPixels, color_t data); 			

// Additional optional implementations by the user:
// ================================================

// getCharInfo: you can create custom fonts without changing how printing functions work
/*
	character: the byte-sized character to show on screen
	pchar: a pointer to a valid char_info_t object that needs to be filled out peroperly for the given character
*/
void fontDriver::getCharInfo(uint8_t character, char_info_t * pchar)
{
	/* 	This function supports the exisitng HyperDisplay::write() function. It can produce fonts all on its own without the user implementing 'write()'.
			It works by filling out a character info structure (char_info_t) for the given character. The character info structure allows you to show any glyph within a xDim * yDim rectangle.
			The user must:
			- indicate how many pixels are to be rendered for the given glyph with 'numPixels'
			- indicate the x and y dimensions of the glyph with 'xDim' and 'yDim' 
			- provide a pointer to an array of hd_font_extent_t variables for x and y that indicate where to render pixels 'xLoc' and 'yLoc' (must contain at least 'numPixels' valid entries)
			- indicate whether or not to actually render anything for this character with 'show' (false means HyperDisplay will ignore that operation)
			- indicate if you want the character to cause a line feed + carriage return 'causesNewline'
	*/

	/*
	typedef struct character_info{
		color_t            	data;		           		// deprecated - don't use
		hd_font_extent_t*  	xLoc;		       				// x location data relative to the upper left-corner of the character area
		hd_font_extent_t*		yLoc;		       				// y location data relative to the upper left-corner of the character area
		hd_font_extent_t    xDim;           			// The maximum value of xLoc
		hd_font_extent_t    yDim;              		// The maximum value of yLoc - also the number of pixels to move down for characters that cause new lines
		hd_pixels_t    		numPixels;           		// The number of color_t types that pdata points to
		bool            	show;				    				// Whether or not to actually show the character
		bool            	causesNewline;					// This indicates if the given chracter is meant to cause a newline
	}char_info_t;                           		// Character information structure for placing pixels in a window
	*/

	// Example:

	const myFontWidth = 1;
	const myFontHeight = 8;

	bool charIsRendered = (((character >= 'a') && (character <= 'z')) || ((character >= 'A') && (character <= 'Z')));

	static hd_font_extent_t xLocations[myFontHeight*myFontWidth]; // myFontHeight*myFontWidth 'x' coordinates here to be used by HyperDisplay later (static)
	static hd_font_extent_t yLocations[myFontHeight*myFontWidth]; // myFontHeight*myFontWidth 'y' coordinates here


	if(character == '\n'){
		pchar->causesNewline = true;
		pchar->numPixels = 0; 				// no pixels to render for newline
		pchar->xLoc = NULL;						// just being explicit - HD won't use this since numPixels = 0
		pchar->yLoc = NULL;						// just being explicit - HD won't use this since numPixels = 0
		pchar->xDim = 0;							// '\n' should not have any width - otherwise it would look like a space
		pchar->yDim = myFontHeight;		// 
		pchar->show = false;					// no pixels to show
	}else if(charIsRendered){
		pchar->causesNewline = false;
		pchar->xLoc = xLocations;			// Point at the location arrays (which we will shortly make sure are filled out correctly)
		pchar->yLoc = yLocations;			//
		pchar->xDim = myFontWidth;		// 
		pchar->yDim = myFontHeight;		// 
		pchar->show = true;						// we want to actually show the pixels

		pchar->numPixels = 0; 				// we are going to figure out how many to show based on the character...

		// Here's how we determine where to put pixels for this font... (remember, setting x and y locations and 'numPixels' determines where pixels are drawn for this character, in local coordinates)
		for(uint8_t ix = 0; ix < 8; ix++){		// loop through 8 bits of the character
			if(character & (0x01 << ix)){				// if this bit is set...
				xLocations[pchar->numPixels] = 0; // 		the x location will always be zero
				xLocations[pchar->numPixels] = ix;// 		the y location will be equal to the index of the bit that was set
				pchar->numPixels++;								// increment the number of pixels to show (also increments where to put the next location info)
			}
		}
	}
}
									

	// write: you decide what happens when someone calls fontDriver.print or fontDriver.println
/*
	val: the byte-sized character value to display
*/
size_t fontDriver::write(uint8_t val){
	/* ALTERNATELY: if you don't like how fonts are drawn by default then you can make your own 'write()' function */

	// Use your imagination!
	// Maybe you want to draw a random line for every letter....

	line(random(64), random(64), random(64), random(64), random(4));

	// Or maybe something completely different. Anyway it is up to you.

}
