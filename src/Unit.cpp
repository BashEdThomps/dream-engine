#include <string>
#include <iostream>
#include <sstream>
#include "Unit.h"

namespace Dream {

  using namespace std;

    const string Unit::UNIT_PASS = "PASS";
    const string Unit::UNIT_INCONCLUSIVE = "INCONCLUSIVE";
    const string Unit::UNIT_FAIL = "FAILED";

    Unit::Unit(string name) {
      mName = name;
      mResult = 0;
    }

    Unit::~Unit() {
    }

    void Unit::header() {
      stringstream stream;
      stream << "========== [ Testing: " << mName << " ] ==========";
      string formatted = bold(stream.str());
      cout << formatted << endl;
      return;
    }

    string Unit::green(string src) {
        stringstream dest;
        dest << UNIT_BOLD << UNIT_GREEN << src << UNIT_NORMAL;
        return dest.str();
    }

    string Unit::yellow(string src) {
        stringstream dest;
        dest << UNIT_BOLD << UNIT_YELLOW << src << UNIT_NORMAL;
        return dest.str();
    }

    string Unit::red(string src) {
        stringstream dest;
        dest << UNIT_BOLD << UNIT_RED << src << UNIT_NORMAL;
        return dest.str();
    }


    string Unit::bold(string src) {
        stringstream dest;
        dest << UNIT_BOLD << src << UNIT_NORMAL;
        return dest.str();
    }

    void Unit::assertFail(string name) {
        string formattedText = red(UNIT_FAIL);
        cout << name << ": " << formattedText << endl;
      mResult += 1;
      return;
    }

    void Unit::assertInconclusive(string name) {
      string formattedText = yellow(UNIT_INCONCLUSIVE);
      cout << name << ": " << formattedText << endl;
      mResult += 1;
      return;
    }

    void Unit::assertPass(string name) {
        string formattedText = green(UNIT_PASS);
        cout << name << ": " << formattedText << endl;
      mResult += 0;
      return;
    }

    void Unit::assertNotZero(string name, int value) {
      int retval;
        string formattedText;
        if (value != 0) {
            formattedText = green(UNIT_PASS);
        retval = 0;
        } else {
            formattedText = red(UNIT_FAIL);
        retval = 1;
        }
        cout << name << ": " << formattedText << endl;
      mResult += retval;
      return;
    }

    void Unit::assertTrue(string name, bool value) {
      int retval;
        string formattedText;
        if (value) {
            formattedText = green(UNIT_PASS);
        retval = 0;
        } else {
            formattedText = red(UNIT_FAIL);
        retval = 1;
        }
        cout << name << ": " << formattedText << endl;
      mResult += retval;
      return;
    }

    void Unit::assertFalse(string name, bool value) {
      int retval;
        string formattedText;
        if (!value) {
            formattedText = green(UNIT_PASS);
        retval = 0;
        } else {
            formattedText = red(UNIT_FAIL);
        retval = 1;
        }
        cout << name << ": " << formattedText << endl;
      mResult += retval;
      return;
    }
    void Unit::assertZero(string name, int value) {
      int retval;
        string formattedText;
        if (value == 0) {
            formattedText = green(UNIT_PASS);
        retval = 0;
        } else {
            formattedText = red(UNIT_FAIL);
        retval = 1;
        }
        cout << name << ": " << formattedText << endl;
      mResult += retval;
      return;
    }

    void Unit::assertEqual(string name, int val1, int val2) {
      int retval;
        string formattedText;
        if (val1 == val2) {
            formattedText = green(UNIT_PASS);
        retval = 0;
        } else {
            formattedText = red(UNIT_FAIL);
        retval = 0;
        }
        cout << name << ": " << formattedText << endl;
      mResult += retval;
      return;
    }

      void Unit::assertNotEqual(string name, int val1, int val2) {
      int retval;
        string formattedText;
        if (val1 != val2) {
            formattedText = green(UNIT_PASS);
        retval = 0;
        } else {
            formattedText = red(UNIT_FAIL);
        retval = 1;
        }
        cout << name << ": '"<< val1 << "' and '"<< val2 << "' are not equal: " << formattedText << endl;
      mResult += retval;
      return;
    }

    void Unit::assertNotNull(string name, void* val) {
      int retval;
        string formattedText;
        if (val != NULL) {
            formattedText = green(UNIT_PASS);
        retval = 0;
        } else {
            formattedText = red(UNIT_FAIL);
        retval = 1;
        }
        cout << name << ": " << formattedText << endl;
      mResult += retval;
      return;
    }

    void Unit::assertNull(string name, void* val) {
      int retval;
        string formattedText;
        if (val == NULL) {
            formattedText = green(UNIT_PASS);
        retval = 0;
        } else {
            formattedText = red(UNIT_FAIL);
        retval = 1;
        }
        cout << name << ": " << formattedText << endl;
      mResult += retval;
      return;
    }

    void Unit::comment(string comment) {
      stringstream buf;
      buf << "---> " << comment;
      string formatted = bold(buf.str());
      cout << formatted << endl;
      return;
    }

    int Unit::getResult(void) {
      return mResult;
    }

} // End of Dream
