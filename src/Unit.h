#ifndef UNIT_H_
#define UNIT_H_

#include <iostream>

#define UNIT_NORMAL  "\x1B[0m"
#define UNIT_RED     "\x1B[31m"
#define UNIT_GREEN   "\x1B[32m"
#define UNIT_YELLOW  "\x1B[33m"
#define UNIT_BLUE    "\x1B[34m"
#define UNIT_MAGENTA "\x1B[35m"
#define UNIT_CYNAN   "\x1B[36m"
#define UNIT_WHITE   "\x1B[37m"
#define UNIT_BOLD    "\x1B[1m"

namespace Dream {

  using namespace std;

  class Unit {
  private:
    static const string UNIT_PASS;
    static const string UNIT_INCONCLUSIVE;
    static const string UNIT_FAIL;
    int         mResult;
    string mName;
  public:
    //! Default Constructor
    Unit(string);

    //! Default Destructor
    virtual ~Unit(void) = 0;

    //! Run Method
    virtual void run(void) = 0;

    //! Assert Test Passed
    void assertPass(string);

    //! Assert Test Failed
    void assertFail(string);

    //! Assert Test Failed
    void assertInconclusive(string);

    //! Assert Value is not equal to zero
    void assertNotZero(string, int);

    //! Assert Value is equal to zero.
    void assertZero(string, int);

    //! Assert Value is equal to true.
    void assertTrue(string, bool);

    //! Assert Value is equal to false.
    void assertFalse(string, bool);

    //! Assert two objects are equal.
    void assertEqual(string, int, int);

    //! Assert two floats are not equal.
    void assertNotEqual(string, int, int);

    //! Assert pointer is not null;
    void assertNotNull(string, void*);

    //! Assert pointer is null.
    void assertNull(string, void*);

    //! Print the heading for a module
    void header();

    //! Print a comment
    void comment(string);

    //! Get testing result
    int getResult(void);
  private:
    //! Colour a string green (ANSI)
    string green(string);

    //! Colour a string red (ANSI)
    string red(string);

    //! Colour a string red (ANSI)
    string yellow(string);

    //! Turn a string bold (ANSI)
    string bold(string);
  };

} // End of Dream

#endif // UNIT_H_
