#!/bin/bash

HOST=`uname -a`
BUILD=out
MAKE_ARGS=-j4

echo HOST is $HOST

if [[ "$HOST" == *"Darwin"* ]];
then
	echo Building for MacOS X
	if [[ "$HOST" == *"PPC"* ]];
	then
		echo Building for PowerPC with MacPorts GCC
		export CC=/opt/local/bin/gcc-mp-4.9
		export CXX=/opt/local/bin/g++-mp-4.9
	else
		echo Building for Intel
	fi
fi

rm -rf $BUILD
mkdir -p $BUILD
cd $BUILD
cmake ..
VEROBSE=1 make ${MAKE_ARGS}
VERBOSE=1 CTEST_OUTPUT_ON_FAILURE=TRUE make ${MAKE_ARGS} test
