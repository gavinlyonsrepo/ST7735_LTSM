# CHANGELOG.md

* Version 1.0.0 08-2025
  * first version
* Version 1.1.0 09-2025
  * Adding more examples
* Version 1.1.1 05-2026
  * Added support for 80x160 display
* Version 1.1.2 11-2026
	* Minor update
	* Updated FRAME_BUFFER.ino example for display16_LTSM dependency graphics library version 1.1.0.
	* NOTE: When IN frame buffer mode, on a display whose width and height differ,
	  the buffer is laid out for the current rotation:
		1. Set the rotation before calling setBuffer() in setup(), IF non-default.
		2. If the program switches between landscape and portrait during
		   operation (a 90 or 270 degree change), call setBuffer() again after
		   the rotation change, then redraw.
		3. See the FRAME_BUFFER.ino example, and the buffer mode README in the
		   display16_LTSM dependency library, for details.
		4. This change only effects users using frame buffer mode AND non-default rotation
		AND on displays where width != height.