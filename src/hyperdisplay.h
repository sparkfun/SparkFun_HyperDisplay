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

#ifndef HYPERDISPLAY_H // We are going to need a cooler name than 'display' in the long run
#define HYPERDISPLAY_H

#include <Arduino.h>

typedef void * color_t; 

class hyperdisplay : public Print{
    private:
    protected:
    public:
    // Parameters
        uint16_t xExt, yExt;        // The rectilinear extent of the display in two dimensions
        uint8_t colorDepth;         // The number of bits of color depth for each pixel.
        color_t * pScratch;         // A pointer to a scratch space for maniuplating data
        int32_t cursorX, cursorY;   // Large signed cursor coordinates allow for off-screen handling
    
    // Methods
        // 'primitive' drawing functions
        virtual void pixel(uint16_t x0, uint16_t y0, color_t color); // 
        virtual void xline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t width = 1); // Default implementation using individual pixels so that user CAN add just a way to write to a pixel,  but highly reccommend optimizing
        virtual void yline(uint16_t x0, uint16_t y0, uint16_t len, color_t color[], uint16_t colorCycleLength, uint16_t width = 1); //^
        virtual void rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color, bool filled = false); 
        virtual void fillFromArray(uint16_t x, uint16_t y, uint16_t size, color_t data[]); 

        // These use the 'primitive' functions
        void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color , uint16_t width = 1); 
        void polygon(uint16_t x[], uint16_t y[], uint8_t numSides, uint16_t width = 1);
        void circle(uint16_t x0, uint16_t y0, uint16_t radius, color_t color, bool filled = false);
        void fillScreen(color_t color);
    
        // Printing
        virtual size_t write(uint8_t); // This is the implementation of write that is inherited from, left as virtual to be implementation specific
};

#endif /* HYPERDISPLAY_H */


