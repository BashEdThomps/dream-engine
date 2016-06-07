#!/bin/bash

cd ..
mkdir DreamXcode
cd DreamXcode
cmake .. -G Xcode
open Dream.xcodeproj
