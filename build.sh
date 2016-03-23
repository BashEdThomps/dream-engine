#!/bin/bash

BUILD=build

rm -rf $BUILD
mkdir -p $BUILD
cd $BUILD
cmake ..
make
make test
