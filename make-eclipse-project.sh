#!/bin/bash

DREAM_DIR=../Dream
DIR=DreamEclipse
cd ..
mkdir ${DIR}
cd ${DIR} 
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE  ${DREAM_DIR}
