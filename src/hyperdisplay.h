/*

SparkFun Master Display Library

Created: July 2018
Modified: July 2018

Authors:
	Ciara Jekel
	Owen Lyke

Purpose: This library standardizes interfaces to displays of many types. 
         This header provides the most abstract class called 'display'
         Other categories of display such as E-Ink, TFT, and OLED can 
         derive from this class 

*/

#ifndef HYPERDISPLAY_H              // We are going to need a cooler name than 'display' in the long run
#define HYPERDISPLAY_H

#include <Arduino.h>
#include "hyperdisplay_conf.h"      // The user-modifiable configuration file for the library

#if HYPERDISPLAY_USE_MATH                   // Extra special math functions for later
    #include <math.h>
#endif

#if HYPERDISPLAY_USE_PRINT                  // 
    #if HYPERDISPLAY_INCLUDE_DEFAULT_FONT   
        #include "font5x7.h"
        #define HYPERDISPLAY_DEFAULT_FONT_WIDTH 5
        #define HYPERDISPLAY_DEFAULT_FONT_HEIGHT 8
    #endif
#endif


#define SWAP_COORDS(a, b) uint16_t temp = a; \
              						  a = b; \
              					   b = temp; \

typedef void * color_t; 

typedef struct character_info{
	color_t  data;		    // The data that is used to fill the character frame
	uint16_t * xLoc;		// x location data relative to the upper left-corner of the character area
	uint16_t * yLoc;		// y location data relative to the upper left-corner of the character area
    uint32_t numPixels;     // The number of color_t types that pdata points to
	bool show;				// Whether or not to actually show the character
	bool causedNewline;		// Whether or not the character triggered a new line  - only set true if you want there to be a new line. Otherwise library handles it
}char_info_t;				// Character information structure for placing pixels in a window

typedef struct window_info{
    uint16_t xMin;              // FYI window min/max use the hardware frame of reference
    uint16_t xMax;
    uint16_t yMin;
    uint16_t yMax;
    int32_t  cursorX;           // Where the cursor is currently in window-coordinates
    int32_t  cursorY;           // Where the cursor is currently in window-coordinates
    uint16_t xReset;            // Where the cursor goes on a reset location (window coordinates)
    uint16_t yReset;            // Where the cursor goes on a reset location (window coordinates)
    char_info_t lastCharacter;      // Information about the last character written.
    color_t data;                   // A pointer to pixel data that is specific to the window. Can be left as NULL
}wind_info_t;                       // Window infomation structure for placing text on the display

class hyperdisplay : public Print{
    private:
    protected:

        public: // temporary!

    	// Some protected drawing functions - not to be used directly
    	void lineHigh(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width, bool reverseGradient);
    	void lineLow(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t width, bool reverseGradient);
    	void circle_Bresenham(int32_t x0, int32_t y0, uint16_t radius, color_t color, bool fill);
		void circle_midpoint(uint8_t x0, uint8_t y0, uint8_t radius, color_t color, bool fill);
		void circle_eight(uint8_t xc, uint8_t yc, int16_t dx, int16_t dy, color_t color, bool fill);

        // Utility functions
    	uint16_t getNewColorOffset(uint16_t colorCycleLength, uint16_t startColorOffset, uint16_t numWritten);
        bool enforceWindowLimits(int32_t * x0, int32_t * y0 = NULL, int32_t * x1 = NULL, int32_t * y1 = NULL);       // Used to ensure that given coordiantes do not exceed the current window's extent
        virtual void setupDefaultWindow( void );                        // User can override this function                                                               // Fills out the default window structure and associates it to the current window 
        void setupHyperDisplay(uint16_t xSize, uint16_t ySize);         // Call this function in the begin() function of the derived class to ensure that all necessary paramters for the hyperdisplay parent class are set correctly

        // A method for dealing with the color_t flexibility:
        virtual color_t getOffsetColor(color_t base, uint32_t numPixels) = 0;  // This pure virtual function is required to get the correct pointer after incrementing by a number of pixels (which could have any amount of data behind them)

        // Here are the lowest level APIs to the particular hardware. These functions are in hardware coordinates. The only one you need to implement is hwpixel, but you can implement the others if you a more efficient way to do it
        virtual void hwpixel(uint16_t x0, uint16_t y0, color_t color) = 0; // Made a pure virtual function so that derived classes are forced to implement the pixel function
        virtual void hwxline(uint16_t x0, uint16_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goLeft = false); // Default implementation using individual pixels so that user CAN add just a way to write to a pixel,  but highly reccommend optimizing
        virtual void hwyline(uint16_t x0, uint16_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goUp = false); //^
        virtual void hwrectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t data, bool filled = false, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool gradientVertical = false, bool reverseGradient = false); 
        virtual void hwfillFromArray(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t size, color_t data);

    public:
    // Parameters
        uint16_t xExt, yExt;        	// The rectilinear extent of the display in two dimensions (number of pixels)
        wind_info_t * pCurrentWindow;	// A pointer to the active window information structure.

    // Methods
        // 'primitive' drawing functions - coordinates are with respect to the current window
        void pixel(int32_t x0, int32_t y0, color_t color);
        void xline(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goLeft = false); 
        void yline(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goUp = false);
        void rectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, bool filled = false, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool gradientVertical = false, bool reverseGradient = false); 
        void fillFromArray(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t size, color_t data); 

        // These use the 'primitive' functions and are available to users - coordinates are with respect to the current window
        void line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, uint16_t width = 1, bool reverseGradient = false); 
        void polygon(int32_t x[], int32_t y[], uint8_t numSides, color_t color, uint16_t width = 1);
        void circle(int32_t x0, int32_t y0, uint16_t radius, color_t color, bool filled = false); // Fills the entire current window
        void fillWindow(color_t color);
    
        // Printing
        virtual size_t write(uint8_t val);                                      // This is the implementation of write that is inherited from, left as virtual to be implementation specific
        #if HYPERDISPLAY_USE_PRINT		
            #if HYPERDISPLAY_INCLUDE_DEFAULT_FONT 			        
                virtual void getCharInfo(uint8_t val, char_info_t * pchar) = 0;		// A pure virtual function - you must implement this to be able to instantiate an object. The pchar pointer argument points to a valid char_info_t object that the function must fill out with the right values
            #endif
        #endif

        // Mathematical drawing tools
        #if HYPERDISPLAY_USE_MATH
            // Write math functions here
        #endif
};




// Some callback functions available to the user - they can be overriden with whatever implementation is useful at the time
// Note that the pure virtual function pixel() does not have a callback - if it is needed it should be included in the derived class,
// and that if the user provides implementation specific versions of the other primitive functions then these callbacks will not be
// called, so if the functionality is desired it can be re-implemented.
void hyperdisplayXLineCallback(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goLeft = false)                                                                                 __attribute__ ((weak));
void hyperdisplayYLineCallback(int32_t x0, int32_t y0, uint16_t len, color_t data, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool goUp = false) 	                                                                                __attribute__ ((weak));
void hyperdisplayRectangleCallback(int32_t x0, int32_t y0, int32_t x1, int32_t y1, color_t data, bool filled = false, uint16_t colorCycleLength = 1, uint16_t startColorOffset = 0, bool gradientVertical = false, bool reverseGradient = false)    __attribute__ ((weak));
void hyperdisplayFillFromArrayCallback(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t numPixels, color_t data)                                                                                                                            __attribute__ ((weak));

#endif /* HYPERDISPLAY_H */


