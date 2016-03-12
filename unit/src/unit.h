#ifndef UNIT_H_
#define UNIT_H_

#define UNIT_NORMAL  "\x1B[0m"
#define UNIT_RED     "\x1B[31m"
#define UNIT_GREEN   "\x1B[32m"
#define UNIT_YELLOW  "\x1B[33m"
#define UNIT_BLUE    "\x1B[34m"
#define UNIT_MAGENTA "\x1B[35m"
#define UNIT_CYNAN   "\x1B[36m"
#define UNIT_WHITE   "\x1B[37m"
#define UNIT_BOLD    "\x1B[1m"

#define UNIT_PASS "PASS"
#define UNIT_FAIL "FAIL"
#define BUFFER_SZ 256

//! Assert Test Passed
void unitAssertPass (char*);
//! Assert Test Failed 
void unitAssertFail (char*);
//! Assert Value is not equal to zero
void unitAssertNotZero(char*, int);
//! Assert Value is equal to zero.
void unitAssertZero   (char*, int);
//! Assert two chars are equal.
void unitAssertEqualChar       (char*, char, char);
//! Assert two shorts are equal.
void unitAssertEqualShort      (char*, short, short);
//! Assert two ints are equal.
void unitAssertEqualInt        (char*, int, int);
//! Assert two longs are equal.
void unitAssertEqualLong       (char*, long, long);
//! Assert two floats are equal. 
void unitAssertEqualFloat      (char*, float, float);
//! Assert two floats are not equal.
void unitAssertNotEqualFloat   (char*, float, float);
//! Assert two doubles are equal.
void unitAssertEqualDouble     (char*, double, double);
//! Assert two doubles are not equal.
void unitAssertEqualLongDouble (char*, long double, long double);
//! Assert two pointers are equal.
void unitAssertEqualVoidP      (char*, void*, void*);
//! Assert pointer is not null;
void unitAssertNotNull (char*, void*);
//! Assert pointer is null. 
void unitAssertNull    (char*, void*);
//! Print the heading for a module 
void unitModuleHeading(char*);
//! Print the footer for a module 
void unitModuleFooter(char*);
//! Print the heading for a test section 
void unitTestHeading(char*);
//! Print the footer for a test section
void unitTestFooter(char*);
//! Print a comment 
void unitPrintComment(char*);
//! Colour a string green (ANSI)
char* unitGreen(char*);
//! Colour a string red (ANSI) 
char* unitRed(char*);
//! Turn a string bold (ANSI) 
char* unitBold(char*);

#endif // UNIT_H_
