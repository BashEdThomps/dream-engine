/*
  tests_main.c

  Author: Ash Thompson
  Email: ashthompson06@gmail.com
*/
#include "../unit.h"

int main() {
    unitModuleHeading("Something");
    unitTestHeading("Some Test");
    unitAssertPass("This shuld pass");
    unitAssertFail("My attempts to get laid in high school...");
    unitAssertZero("Equal to Zero",0);
    unitAssertNotZero("Not equal to Zero",3);
    unitAssertZero("This will fail",2);
    unitAssertNotZero("This will also fail",0);
    unitTestFooter("Something");
    unitModuleFooter("Some Module");
}
