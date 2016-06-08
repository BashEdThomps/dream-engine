#!/bin/bash

cd ..
mkdir DreamXcode
cd DreamXcode
cmake ../Dream -G Xcode
open Dream.xcodeproj
