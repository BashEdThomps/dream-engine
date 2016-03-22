/*
 * TestMain.cpp
 * Author: Ash Thompson
 * Email: ashthompson06@gmail.com
 */
#include "../src/Unit.h"

int main(int argc, char** argv) {
    DreamUnit::Unit *unit = new DreamUnit::Unit();
    unit->moduleHeading ("Unit Library");
    unit->testHeading   ("Unit Tests");
    unit->assertPass    ("This shuld pass");
    unit->assertZero    ("Equal to Zero",0);
    unit->assertNotZero ("Not equal to Zero",3);
}
