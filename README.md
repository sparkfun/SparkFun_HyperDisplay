SparkFun HyperDisplay Arduino Library
========================================

This is an abstract display library that makes it easy to get new displays up and running quickly with a full set of drawing and printing functions with multi-window support. This library is relatively flexible allowing for the user to use a completely custom color definition and override default implementations for printing. All that is needed to make a display work is to provide the interface to draw one pixel to the display, and instruct the library how to handle your custom color type. Increased efficiency can be obtained by overriding additional drawing functions. 

Quick-start guide
-----------------
HyperDisplay is an abstract top layer library. That means that in order to use it you must either:
- 1. Use a provided extension (found below) 
- 2. Derive your own class from HD (use the 'Example1_simpleBareNecessities' example as a guide)


Level Two Extensions
------------------------------
Level two libraries support display driver IC's without customization for any particular display hardware.
* **[ILI9163C Drivers](https://github.com/sparkfun/HyperDisplay_ILI9163C_ArduinoLibrary)**
* **[SSD1309 Drivers](https://github.com/sparkfun/HyperDisplay_SSD1309_ArduinoLibrary)**


Level Three Extensions
-------------------------------
Level three libraries suport particular display models - that is to say a combination of a driver IC with actual display hardware with a certain number of pixels and a fixed communication mode.
* **[KWH018ST01 1.8 inch TFT module, with 4-Wire SPI](https://github.com/sparkfun/HyperDisplay_KWH018ST01_4WSPI_ArduinoLibrary)**
* **[UG2856KLBAG01 Transparent Graphical OLED, I2C and SPI](https://github.com/sparkfun/HyperDisplay_UG2856KLBAG01_ArduinoLibrary)**


Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE.
* **library.properties** - General library properties for the Arduino package manager.

Documentation
--------------

* **[HyperDisplay Tutorial](https://learn.sparkfun.com/tutorials/everything-you-should-know-about-hyperdisplay)** - Usage information
* **[Installing an Arduino Library Guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)** - Basic information on how to install an Arduino library.

Products that use this Library 
---------------------------------
* [Transparent Graphical OLED [LCD-15173]](https://www.sparkfun.com/products/15173)
* [SparkFun 1.8 inch TFT Breakout 128x160](https://www.sparkfun.com/products/15143)

Version History
---------------

* [V 1.0.0](https://github.com/sparkfun/SparkFun_HyperDisplay/releases/tag/v1.0.0) -- Initial release

License Information
-------------------

This product is _**open source**_!

The **code** is released under the GPL v3 license. See the included LICENSE.md for more information.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
