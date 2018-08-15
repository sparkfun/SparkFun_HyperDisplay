/*

This is a custom configuration file for the hyperdisplay library

The purpose is to customize the instance of the library
for the particular needs of the application. For example
on a small system the size of included code is a concern
and so fewer optional items would be included. On a larger
system however more advanced features may be desired, and
they will require additional inclusions

*/ 

#define HYPERDISPLAY_HAVE_CUSTOM_CONFIG	// This line signifies to the library that a custom configuration exists.

/*
DRAWING_LEVEL: 
0 - includes only primitive drawing functions:
	- pixel
	- xline
	- yline
	- rectangle
	- fillFromArray
	- fillWindow

1 - level 0 and: 
	- line
	- polygon
	- circle

2 - level 1 and:
	-

*/
#define HYPERDISPLAY_DRAWING_LEVEL 0	


/*
USE_PRINT: 
0 - disables printing text
1 - enables printing text, requires users to implement "getCharInfo()""
*/
#define HYPERDISPLAY_USE_PRINT 1


/*
INCLUDE_DEFAULT_FONT
0 - don't include the default 5x7 program memory font (to save on program space)
1 - do include the default 5x7 program memory font (to NOT save on program space)
*/
#define HYPERDISPLAY_INCLUDE_DEFAULT_FONT 1

/*
USE_MATH: 
0 - disables mathematical drawing
1 - enables mathematical drawing, includes math.h and defines a mathematical object type
*/
#define HYPERDISPLAY_USE_MATH 0	