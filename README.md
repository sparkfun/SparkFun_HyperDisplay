SparkFun WT2003S MP3 Decoder Arduino Library
========================================

![SparkFun WT2003S MP3 Decoder](https://cdn.sparkfun.com/assets/parts/1/3/0/7/7/14810-MP3_Breakout_WT2003S-01.jpg)

[*SparkFun WT2003S MP3 Decoder (SPX-14810)*](https://www.sparkfun.com/products/14810)

This library provides full access to the functions of the WT2003S MP3 decoder through a serial connection and a "busy" pin. This library is very simple to use with either a hardware or software serial port. The simple dependencies (HardwareSerial and SoftwareSerial) make it suitable for use on any Arduino-compatible platform. 

The WT2003S combines two functions needed to play .mp3 files into one package and adds a simple serial control interface. Normally to play a .mp3 file one would need to choose a decoder IC AND provide a method for storing and selecting the files. The WT2003S removes that complexity by combining an SD card interface and .mp3 decoder. Now in order to play a song all you need to do is save it to an SD card, plug it into the WT2003S breakout, and send a few commands over serial. This allows you to easily integrate sound into any Arduino project.

This library is intended for use with the [WT2003S MP3 Decoder Breakout board](https://www.sparkfun.com/products/14810), as opposed to another option employing the WT2003S -- the [Qwiic MP3 Trigger](https://www.sparkfun.com/products/14808). In case you are wondering which to use here is a little breakdown: If you want direct access to the WT2003S IC for complete control *and* you have a spare serial port (Hardware or Software) in your project then the Breakout board is good for you. If your project is running low on pins and you already use the I2C bus, or you want to trigger .mp3s without even using an external microcontroller then the Qwiic MP3 Trigger is right for you!

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE.
* **library.properties** - General library properties for the Arduino package manager.

Documentation
--------------

* **[Installing an Arduino Library Guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)** - Basic information on how to install an Arduino library.

Products that use this Library 
---------------------------------

* [SPX-14810](https://www.sparkfun.com/products/14810) - SparkX version

Version History
---------------
* [V 1.0.0](https://github.com/sparkfun/SparkFun_WT2003S_MP3_Decoder_Arduino_Library/releases/tag/1.0.0) - Initial release


License Information
-------------------

This product is _**open source**_!

The **code** is released under the GPL v3 license. See the included LICENSE.md for more information.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
