#!/bin/bash

BUILD=build

rm -rf $BUILD
mkdir -p $BUILD
cd $BUILD
cmake ..
make
CTEST_OUTPUT_ON_FAILURE=TRUE make test
