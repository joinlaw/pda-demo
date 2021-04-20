# Pizza Detection Demo (General Algorithm)

This project demonstrate the implementation of collision detection on segments of a circle.

## How it works

This algorithm check the distance between circle center point and mouse position if the distance less than or equals the radius then the point is inside the circle which lead us to the next step to calculate the angle between the center and mouse which then compare it with the evenly distributed slices of the circle.


## Dependencies

To be able to compile this project you should have:

* Any C compiler (eg GCC)
* GNU Make
* git version control system
* pkg-config package configuration utility
* GTK4 GUI library
* libcairo low-level postscript like drawing library

## Build Instructions

To build this project follow these steps:

	$ git clone https://github.com/joinlaw/pda-demo
	$ cd pda-demo
	$ make

to run the executable type:

	$ ./demo

or to run the project in one drop:

	$ make && ./demo

to clean build files:

	$ make clean

## Hacking

To hack on this project use the common pull request workflow to push changes to repo but first follow the GNU coding standard and GNOME coding standard when writing your changes.

Generally don't put braces with curly brackets in the same line ;).

## Known bugs

when rotating too much the algorithm will lose the control partially on some segments and rotating even further will broke the collision detection.

## License

Distributed under the GPLv3 or later License. See LICENSE for more information.

