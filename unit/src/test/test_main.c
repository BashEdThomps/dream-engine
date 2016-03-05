/*
  tests_main.c

  Author: Ash Thompson
  Email: ashthompson06@gmail.com
*/
#include "../unit.h"

int main() {
    unitModuleHeading("Unit Library");
    unitTestHeading("Unit Tests");
    unitAssertPass("This shuld pass");
    unitAssertZero("Equal to Zero",0);
    unitAssertNotZero("Not equal to Zero",3);
    unitTestFooter("Unit Tests");
    unitModuleFooter("Unit Library");
}
