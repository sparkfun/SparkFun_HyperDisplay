/*

SparkFun Master Display Library

Created: July 2018
Modified: July 2018

Purpose: This library standardizes interfaces to displays of many types. 
         This header provides the most abstract class called 'display'
         Other categories of display such as E-Ink, TFT, and OLED can 
         derive from this class 

*/

#ifndef DISPLAY_H // We are going to need a cooler name than 'display' in the long run
#define DISPLAY_H

#include <Arduino.h>


typedef (void *) color_t;


class display : public Print{
    private:
    protected:
    public:
        // Parameters
        uint16_t xExt, yExt;        // The rectilinear extent of the display in two dimensions
        uint8_t colorDepth;         // The number of bits of color depth for each pixel.
        void * pScratch;            // A void pointer to a scratch space for maniuplating data
        int32_t cursorX, cursorY;   // Large signed cursor coordinates allow for off-screen handling
    
    // Methods
            // 'primitive' drawing functions
        virtual void pixel(uint16_t x, uint16_t y, color_t color); // 
        virtual void xline(uint16_t x, uint16_t y, uint16_t Len, color_t color, uint16_t width = 1); //default implement with many pixels so that user CAN just add a way to write to a pixel,  but highly reccommend optimizing
        virtual void yline(uint16_t x, uint16_t y, uint16_t Len, color_t color, uint16_t width = 1); //^
        virtual void rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color, bool filled = false); 
        virtual void circle(uint16_t x0, uint16_t y0, uint16_t radius, color_t color, bool filled = false);
        virtual void polygon(uint16_t x[], uint16_t y[], uint8_t numSides, uint16_t width = 1);
    

            // These use the 'primitive' functions
        void line(uint16_t x0, uint16_t y0, uint 16_t x1, uint16_t y1, color_t color , uint16_t width = 1); 
        void fillScreen(color_t color);
    
            // Printing
        virtual size_t write(uint8_t); // This is the implementation of write that is inherited from 
    
};

#endif /* DISPLAY_H */


/*

// Ciara.. how will color_t work exactly?
//unsure. always use a pointer? 
// likeee you get 
struct {
    byte r,
    byte g,
    byte b; 
}color_t;
//or

// I think it would be good to make sure we buid in HSV suppport too.. so maybe 
//what's hsv hue sat val nvm
struct {
    byte r,
    byte g,
    byte b; 
    byte h;
    byte s;
    byte v;
    
    boolean useHSV;
}color_t;
// in my head color_t would be implementation defined?     
// Yeah I agree, buuuut maybe 
//ive got 
typedef enum {
    R,
    W,
    B;
}color_t;

// Is it possible for you to have that in one implemenation and me to have 
typdef struct{
    byte R;
    byte G
}
       
//thats what i'm wondering. i think if color_t * is passed in yes?
// I'll try it out with a quick Arduino sketch. Yeah so the display class will just have typedef color_t
//maybe enum won't work but thats nbd if all structs do
// Cool will report back shortly


*/