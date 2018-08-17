/*

SparkFun Master Display Library

Created: July 2018
Modified: August 2018

Authors:
	Ciara Jekel
	Owen Lyke

Purpose: This library standardizes interfaces to displays of many types. 
         This header provides the most abstract class called 'display'
         Other categories of display such as E-Ink, TFT, and OLED can 
         derive from this class 

*/

#ifndef HYPERDISPLAY_H             		
#define HYPERDISPLAY_H

#include <Arduino.h>

#ifndef HYPERDISPLAY_HAVE_CUSTOM_CONFIG
#warning "No custom configuration supplied for hyperdisplay, using default"
#include "util\hyperdisplay_default_conf.h" // The default configuration in case one is not provided by the user
#endif

#if HYPERDISPLAY_USE_MATH                   // Extra special math functions for later
    #include <math.h>
#endif

#if HYPERDISPLAY_USE_PRINT                  // 
    #if HYPERDISPLAY_INCLUDE_DEFAULT_FONT   
        #include "util\font5x7.h"
        #define HYPERDISPLAY_DEFAULT_FONT_WIDTH 5
        #define HYPERDISPLAY_DEFAULT_FONT_HEIGHT 8
    #endif
#endif


#define SWAP_COORDS(a, b) uint16_t temp = a; \
              						  a = b; \
              					   b = temp; \

typedef void * color_t; 

typedef struct character_info{
	color_t  data;		           		// The data that is used to fill the character frame
	uint16_t * xLoc;		       		// x location data relative to the upper left-corner of the character area
	uint16_t * yLoc;		       		// y location data relative to the upper left-corner of the character area
    uint16_t xDim;                  	// The maximum value of xLoc
    uint16_t yDim;                  	// The maximum value of yLoc - also the number of pixels to move down for characters that cause new lines
    uint32_t numPixels;           		// The number of color_t types that pdata points to
	bool show;				       		// Whether or not to actually show the character
    bool causesNewline;					// This indicates if the given chracter is meant to cause a carriage return (newline)
}char_info_t;				       		// Character information structure for placing pixels in a window

typedef struct window_info{
    uint16_t xMin;                  	// FYI window min/max use the hardware frame of reference
    uint16_t xMax;						//
    uint16_t yMin;						//
    uint16_t yMax;						//
    int32_t  cursorX;               	// Where the cursor is currently in window-coordinates
    int32_t  cursorY;               	// Where the cursor is currently in window-coordinates
    uint16_t xReset;            		// Where the cursor goes on a reset location (window coordinates)
    uint16_t yReset;            		// Where the cursor goes on a reset location (window coordinates)
    char_info_t lastCharacter;      	// Information about the last character written.
    color_t currentSequenceData;		// The data that is used as the default color sequence
    uint16_t currentColorCycleLength;	// The default color sequence number of pixels
    uint16_t currentColorOffset;		// The current offset
    color_t data;                   	// A pointer to pixel data that is specific to the window. Can be left as NULL
}wind_info_t;                       	// Window infomation structure for placing text on the display

typedef enum{
    hyperdisplay_dim_ok = 0,
    hyperdisplay_dim_low,
    hyperdisplay_dim_high,
    hyperdisplay_dim_no_val
}hyperdisplay_dim_check_t;

class hyperdisplay : public Print{
    private:
    protected:

    	hyperdisplay(uint16_t xSize, uint16_t ySize);	// Constructor - used to make sure member parameters are setup correctly

    	// Utility functions 
    	uint16_t 					getNewColorOffset(uint16_t colorCycleLength, uint16_t startColorOffset, int32_t numWritten);		// Returns a valid offset for a given color sequence and number of pixels drawn
        hyperdisplay_dim_check_t 	enforceLimits(int32_t * var, bool axisSelect);                   									// Returns info about if a variable was within current window limits. Also applis the transformation from window to hardware coordinates to that variable
       public: // temporary
        void 						setCurrentWindowColorSequence(color_t data, uint16_t colorCycleLength, uint16_t startColorOffset);	// Sets up a color sequence for the current window to default to

        // User-defined utilities
        virtual color_t getOffsetColor(color_t base, uint32_t numPixels) = 0;  									// This pure virtual function is required to get the correct pointer after incrementing by a number of pixels (which could have any amount of data behind them depending on how the color is stored)
        virtual void 	setWindowDefaults(wind_info_t * pwindow);                         						// Fills out the default window structure and associates it to the current window  // User can override this function        

		#if HYPERDISPLAY_DRAWING_LEVEL > 0
    	// Protected drawing functions
    	uint16_t 		lineHighNorm(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset);
    	uint16_t 		lineLowNorm(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset);
        uint16_t 		lineHighReverse(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset);
        uint16_t 		lineLowReverse(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset);
    	void 			circle_Bresenham(int32_t x0, int32_t y0, uint16_t radius, color_t color, bool fill);
		void 			circle_midpoint(uint8_t x0, uint8_t y0, uint8_t radius, color_t color, bool fill);
		void 			circle_eight(uint8_t xc, uint8_t yc, int16_t dx, int16_t dy, color_t color, bool fill);
		#endif /* HYPERDISPLAY_DRAWING_LEVEL > 0 */ 

        // Lowest level APIs 
        // These functions are in hardware coordinates. The only one you need to implement is hwpixel, but you can implement the others if you have a more efficient way to do it
        virtual void 	hwpixel(uint16_t x0, uint16_t y0, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0) = 0; 											// Made a pure virtual function so that derived classes are forced to implement the pixel function
        virtual void	hwxline(uint16_t x0, uint16_t y0, uint16_t len, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goLeft = false); 			// Default implementation provided, suggested to overwrite
        virtual void 	hwyline(uint16_t x0, uint16_t y0, uint16_t len, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goUp = false); 			// Default implementation provided, suggested to overwrite
        virtual void 	hwrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool filled = false, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool reverseGradient = false, bool gradientVertical = false); 
        virtual void 	hwfillFromArray(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t numPixels, color_t data);																// Default implementation provided, suggested to overwrite

    public:
    // Parameters
        uint16_t xExt, yExt;        	// The rectilinear extent of the display in two dimensions (number of pixels)
        wind_info_t * pCurrentWindow;	// A pointer to the active window information structure.

    // Methods
        // 'primitive' drawing functions - window coordinates
        void 		pixel(int32_t x0, int32_t y0, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0);
        void 		xline(int32_t x0, int32_t y0, uint16_t len, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goLeft = false); 
        void 		yline(int32_t x0, int32_t y0, uint16_t len, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goUp = false);
        void 		rectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, bool filled = false, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool reverseGradient = false, bool gradientVertical = false); 
        void 		fillFromArray(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t numPixels, color_t data = NULL); 
        void 		fillWindow(color_t color);  

		#if HYPERDISPLAY_DRAWING_LEVEL > 0
        // Level 1 drawing functions - window coordinates
	        uint16_t 	line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t width = 1, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool reverseGradient = false); 
	        void 		polygon(int32_t x[], int32_t y[], uint8_t numSides, uint16_t width = 1, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool reverseGradient = false);
	        void 		circle(int32_t x0, int32_t y0, uint16_t radius, bool filled = false, color_t data = NULL, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool reverseGradient = false); 
		#endif /* HYPERDISPLAY_DRAWING_LEVEL > 0 */                                                           																									


		#if HYPERDISPLAY_DRAWING_LEVEL > 1
        	// void draw something complex;
		#endif /* HYPERDISPLAY_DRAWING_LEVEL > 1 */      


        // Printing
        virtual size_t write(uint8_t val);                                      	// This is the implementation of write that is inherited from print.h, left as virtual to be implementation specific
		#if HYPERDISPLAY_USE_PRINT		
        	virtual void getCharInfo(uint8_t character, char_info_t * pchar);       // A pure virtual function - you must implement this to be able to instantiate an object. The pchar pointer argument points to a valid char_info_t object that the function must fill out with the right values
		#endif	/* HYPERDISPLAY_USE_PRINT */

        // Mathematical drawing tools
		#if HYPERDISPLAY_USE_MATH
            // Write math functions here
		#endif /* HYPERDISPLAY_USE_MATH */
};

// Some callback functions available to the user - they can be overriden with whatever implementation is useful at the time
// Note that the pure virtual function pixel() does not have a callback - if it is needed it should be included in the derived class,
// and that if the user provides implementation specific versions of the other primitive functions then these callbacks will not be
// called, so if the functionality is desired it can be re-implemented.
void hyperdisplayXLineCallback(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool goLeft)                                                                       __attribute__ ((weak));
void hyperdisplayYLineCallback(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, bool goUp) 	                                                                    __attribute__ ((weak));
void hyperdisplayRectangleCallback(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, bool filled, uint16_t colorCycleLength, uint16_t startColorOffset, bool gradientVertical, bool reverseGradient)    		__attribute__ ((weak));
void hyperdisplayFillFromArrayCallback(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t numPixels, color_t data)                                                                                                    __attribute__ ((weak));

#endif /* HYPERDISPLAY_H */


