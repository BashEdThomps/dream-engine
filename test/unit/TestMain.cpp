/*
 * TestMain.cpp
 * Author: Ash Thompson
 * Email: ashthompson06@gmail.com
 */

#include "UnitTest.h"

int main(int argc, char** argv) {
    DreamUnitTest::UnitTest *unit = new DreamUnitTest::UnitTest();
    unit->run();
    return unit->getResult();
}
