# Dream

## What
* Platform Agnostic
* Test Driven
* High Level
* Scene Graph with
	* 3D Collision Detection
	* Keyframe Animation

## Why
* Bare Essentials
* For making multimedia content
* Targeting Older, Resource Limited and Embedded Platforms

## How

* How to Build
```
	# Select a <target> platform 'linux' or 'psp'
	# Don't use make -j*
	# make -f <file> invokes make with the specified makefile.

	$ alias make="make -f Makefile.<target>"
	$ make clean
	$ make

	# Optionally
	$ make docs
```

This will produce a build in ./build/target.

* How to Use
```
	# Headers
	  -I/path/to/dream-engine/build/target/include

	  #include <deDreamEngine.h>

	# Libs
	  -L/path/to/dream-engine/build/target/lib
	  -lDreamEngine
```
