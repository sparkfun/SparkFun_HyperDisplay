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
#include "util/hyperdisplay_default_conf.h" // The default configuration in case one is not provided by the user
#endif

#if HYPERDISPLAY_USE_MATH                   // Extra special math functions for later
    #include <math.h>
#endif

#if HYPERDISPLAY_USE_RAY_TRACING			
	#include "util/SparkFun_2DRayTracing.h"
#endif

#if HYPERDISPLAY_USE_PRINT                  // 
    #if HYPERDISPLAY_INCLUDE_DEFAULT_FONT   
        #include "util/font5x7.h"
        #define HYPERDISPLAY_DEFAULT_FONT_WIDTH 5
        #define HYPERDISPLAY_DEFAULT_FONT_HEIGHT 8
    #endif
#endif

template <typename T>
void swap(T* i1, T* i2){
	T s = *i1;
	*i1 = *i2;
	*i2 = s;
}

template <typename T>
T uabslen(T i1, T i2){
	if(i1 >= i2){ return i1-i2 + 1; }
	else{ return i2-i1 + 1; }
}

typedef double hd_extent_t;
typedef uint16_t hd_hw_extent_t;	// Represents the limit for the width of height of the display in pixels
typedef uint8_t hd_font_extent_t; 	// Represents the limiting value for width or height of font characters
typedef uint32_t hd_colors_t;   	// Represents the limiting value of how many colors can be stored inside a color cycle
typedef hd_colors_t hd_pixels_t;

typedef void * color_t; 

const bool hdX = false;
const bool hdY = true;
const bool hdW = true;
const bool hdH = false;

typedef struct character_info{
	color_t            	data;		           		// deprecated - fonts now use the first color in the window's color sequence
	hd_font_extent_t*  	xLoc;		       			// x location data relative to the upper left-corner of the character area
	hd_font_extent_t*	yLoc;		       			// y location data relative to the upper left-corner of the character area
    hd_font_extent_t    xDim;                  		// The maximum value of xLoc
    hd_font_extent_t    yDim;                  		// The maximum value of yLoc - also the number of pixels to move down for characters that cause new lines
    hd_pixels_t    		numPixels;           		// The number of color_t types that pdata points to
	bool            	show;				    	// Whether or not to actually show the character
    bool            	causesNewline;				// This indicates if the given chracter is meant to cause a newline
}char_info_t;                           	// Character information structure for placing pixels in a window

typedef struct window_info{
    hd_hw_extent_t  xMin;                  		// FYI window min/max use the hardware frame of reference
    hd_hw_extent_t  xMax;						//
    hd_hw_extent_t  yMin;						//
    hd_hw_extent_t  yMax;						//
    hd_extent_t     cursorX;               		// Where the cursor is currently in window-coordinates
    hd_extent_t     cursorY;               		// Where the cursor is currently in window-coordinates
    hd_extent_t     xReset;            			// Where the cursor goes on a reset location (window coordinates)
    hd_extent_t     yReset;            			// Where the cursor goes on a reset location (window coordinates)
    char_info_t     lastCharacter;      	  	// Information about the last character written.
    color_t         currentSequenceData;		// The data that is used as the default color sequence
    hd_colors_t     currentColorCycleLength;	// The default color sequence number of pixels
    hd_colors_t     currentColorOffset;			// The current offset
    bool            bufferMode;                 // Indicates either buffer or direct mode (direct is default)
    color_t         data;                       // A pointer to pixel data that is specific to the window. Can be left as NULL
    hd_pixels_t		numPixels;					// The number of pixel types that data points to
    bool            dynamic;                    // Indicator if the current buffer memory was dynamically allocated - so that it can be disposed of automatically
}wind_info_t;                       		// Window infomation structure for placing objects on the display

typedef enum{
    hyperdisplay_dim_ok = 0,
    hyperdisplay_dim_low,
    hyperdisplay_dim_high,
    hyperdisplay_dim_no_val
}hyperdisplay_dim_check_t;

extern wind_info_t hyperdisplayDefaultWindow;       // By declaring these as 'extern' we are making them available to the user
extern char_info_t hyperdisplayDefaultCharacter;  

class hyperdisplay : public Print{
    private:
    protected:

    	hyperdisplay(uint16_t xSize, uint16_t ySize);	// Constructor - used to make sure member parameters are setup correctly

    	// Utility functions 
    	uint16_t 					getNewColorOffset(uint16_t colorCycleLength, uint16_t startColorOffset, int32_t numWritten);		// Returns a valid offset for a given color sequence and number of pixels drawn
        hyperdisplay_dim_check_t 	enforceHWLimits(hd_extent_t * windowvar, hd_hw_extent_t* hardwarevar, bool axisSelect);				// Returns info about if a variable was within current window limits. Also applis the transformation from window to hardware coordinates to that variable       
        hyperdisplay_dim_check_t	enforceSWLimits(hd_extent_t* windowvar, bool axisSelect);											// Same job as HW limits except does not convert to hardware coordinates

		#if HYPERDISPLAY_DRAWING_LEVEL > 0
    	// Protected drawing functions
    	hd_extent_t 	lineHighNorm(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset);
    	hd_extent_t 	lineLowNorm(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset);
        hd_extent_t 	lineHighReverse(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset);
        hd_extent_t 	lineLowReverse(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset);
    	void 			circle_Bresenham(hd_extent_t x0, hd_extent_t y0, hd_extent_t radius, color_t color, bool fill);
		void 			circle_midpoint(hd_extent_t x0, hd_extent_t y0, hd_extent_t radius, color_t color, bool fill);
		void 			circle_eight(hd_extent_t xc, hd_extent_t yc, hd_extent_t dx, hd_extent_t dy, color_t color, bool fill);
		#endif /* HYPERDISPLAY_DRAWING_LEVEL > 0 */ 

        // Lowest level APIs 
        // These functions are in hardware coordinates. The only one you need to implement is hwpixel, but you can implement the others if you have a more efficient way to do it
        // Furthermore when the high-level functions call these functions it will be guaranteed that the pixel locations respect the limits of the display
        // And additionally v0 will be always less than or equal to v1 (variables are provided in ascending order)
        virtual void 	hwpixel(hd_hw_extent_t x0, hd_hw_extent_t y0, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0) = 0;											// Made a pure virtual function so that derived classes are forced to implement the pixel function
        virtual void    hwxline(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t len, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool goLeft = false);            // Default implementation provided, suggested to overwrite
        virtual void    hwyline(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t len, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool goUp = false);          // Default implementation provided, suggested to overwrite
        virtual void 	hwrectangle(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t x1, hd_hw_extent_t y1, bool filled = false, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false, bool gradientVertical = false); 
        virtual void 	hwfillFromArray(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t x1, hd_hw_extent_t y1, color_t data = NULL, hd_pixels_t numPixels = 0,  bool Vh = false );																// Default implementation provided, suggested to overwrite

		// Buffer writing functions - all buffers areread left-to-right and top to bottom. Width and height are specified in the associated window's settings. Coordinates are window coordinates
		hd_pixels_t		wToPix( wind_info_t* wind, hd_hw_extent_t x0, hd_hw_extent_t y0); // Computes the pixel offset for a window-coordinate pair (multiply by bytes per pixel to get a byte offset)
		virtual void    swpixel( hd_extent_t x0, hd_extent_t y0, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0) = 0;                                              // Made pure vitual to force control of buffered memory access to be handled by derived classes
		void            swxline( hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool goLeft = false);
		void            swyline( hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool goUp = false);
		void            swrectangle( hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, bool filled = false, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false, bool gradientVertical = false);
		void            swfillFromArray( hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, color_t data = NULL, hd_pixels_t numPixels = 0,  bool Vh = false );


    public:
    // Parameters
        hd_hw_extent_t xExt, yExt;        	// The rectilinear extent of the display in two dimensions (number of pixels)
        wind_info_t * pCurrentWindow;	// A pointer to the active window information structure.

        // User-defined utilities
        virtual color_t getOffsetColor(color_t base, uint32_t numPixels) = 0;                                   // This pure virtual function is required to get the correct pointer after incrementing by a number of pixels (which could have any amount of data behind them depending on how the color is stored)
        virtual void    setWindowDefaults(wind_info_t* pwindow);                                                // Fills out the default window structure and associates it to the current window  // User can override this function        


    // Methods
        // // Consider adding some functions like first/lastPixel() in x and y directions, and perhaps for window and hw coordinates 
        // hd_extent_t     fWP( bool axisSelect, bool coordSelect = hdW, wind_info_t* pwindow = NULL, wind_info_t* ptowindow = NULL );     // First window pixel, returns the index of the first pixel of the specified window in either x or y axes and in either window or hardware coordinates. Can request the window coordinates in another window's coordinates
        // hd_extent_t     lWP( bool axisSelect, bool coordSelect = hdW, wind_info_t* pwindow = NULL, wind_info_t* ptowindow = NULL );
        // hd_extent_t     fHP( bool axisSelect, bool coordSelect = hdH, wind_info_t* ptowindow = NULL ); 
        // hd_extent_t     lHP( bool axisSelect, bool coordSelect = hdH, wind_info_t* ptowindow = NULL );        

        // Window Functions
        void		setWindowColorSequence(wind_info_t * wind, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0);	// Sets up a color sequence for the window
        void 		setCurrentWindowColorSequence(color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0);	// Sets up a color sequence for the current window 
        int         setWindowMemory(wind_info_t * wind, color_t data = NULL, hd_pixels_t numPixels = 0, uint8_t bpp = 0, bool allowDynamic = false);
        int         setCurrentWindowMemory( color_t data = NULL, hd_pixels_t numPixels = 0, uint8_t bpp = 0, bool allowDynamic = false);

        	// Buffer and Show
        void        buffer(wind_info_t * wind = NULL); // Puts the current window into buffer mode - drawing commands are performed on the current window's data buffer - if available
        void        direct(wind_info_t * wind = NULL); // Cancels buffer mode. Drawing commands will go straight to display memory. Buffered data will remain and can still be shown later
        void        show(wind_info_t * wind = NULL);   // Outputs the current window's buffered data to the display

        // 'primitive' drawing functions - window coordinates
		void 		pixel(hd_extent_t x0, hd_extent_t y0, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0);
		void 		xline(hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool goLeft = false); 
		void 		yline(hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool goUp = false);
		void 		rectangle(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, bool filled = false, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false, bool gradientVertical = false); 
		void 		fillFromArray(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, color_t data = NULL, hd_pixels_t numPixels = 0, bool Vh = false); 
		void 		fillWindow(color_t color = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0);  

		#if HYPERDISPLAY_DRAWING_LEVEL > 0
        // Level 1 drawing functions - window coordinates
	        uint16_t 	line(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width = 1, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false); 
	        void 		polygon(hd_extent_t x[], hd_extent_t y[], uint8_t numSides, uint16_t width = 1, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false);
	        void 		circle(hd_extent_t x0, hd_extent_t y0, hd_extent_t radius, bool filled = false, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false); 
		#endif /* HYPERDISPLAY_DRAWING_LEVEL > 0 */                                                           																									


		#if HYPERDISPLAY_DRAWING_LEVEL > 1
        	// void draw something complex;
		#endif /* HYPERDISPLAY_DRAWING_LEVEL > 1 */      

		#if HYPERDISPLAY_USE_RAY_TRACING
	        void 		filledPolygon(sf2drt_polygon* poly, uint16_t width = 1, bool filled = false, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false);
	    #endif /* HYPERDISPLAY_USE_RAYTRACING */


        // Printing
        virtual size_t write(uint8_t val);                                      	// This is the implementation of write that is inherited from print.h, left as virtual to be implementation specific
		#if HYPERDISPLAY_USE_PRINT		
        	virtual void getCharInfo(uint8_t character, char_info_t * pchar);       // The pchar pointer argument points to a valid char_info_t object that the function must fill out with the right values
		#endif	/* HYPERDISPLAY_USE_PRINT */
        void setTextCursor(int32_t x0, int32_t y0, wind_info_t * window = NULL);
        void resetTextCursor(wind_info_t * window = NULL);

        // Mathematical drawing tools
		#if HYPERDISPLAY_USE_MATH
            // Write math functions here
		#endif /* HYPERDISPLAY_USE_MATH */
};

// Some callback functions available to the user - they can be overriden with whatever implementation is useful at the time
// Note that the pure virtual function pixel() does not have a callback - if it is needed it should be included in the derived class,
// and that if the user provides implementation specific versions of the other primitive functions then these callbacks will not be
// called, so if the functionality is desired it can be re-implemented.
void hyperdisplayXLineCallback(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goLeft)                                                                       __attribute__ ((weak));
void hyperdisplayYLineCallback(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t len, color_t data, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool goUp) 	                                                                    __attribute__ ((weak));
void hyperdisplayRectangleCallback(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t x1, hd_hw_extent_t y1, color_t data, bool filled, hd_colors_t colorCycleLength, hd_colors_t startColorOffset, bool gradientVertical, bool reverseGradient)    		__attribute__ ((weak));
void hyperdisplayFillFromArrayCallback(hd_hw_extent_t x0, hd_hw_extent_t y0, hd_hw_extent_t x1, hd_hw_extent_t y1, hd_pixels_t numPixels, color_t data)                                                                                                    __attribute__ ((weak));

#endif /* HYPERDISPLAY_H */


