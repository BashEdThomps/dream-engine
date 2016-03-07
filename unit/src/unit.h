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

/*
 * Assertions
 */
void unitAssertPass (char*);
void unitAssertFail (char*);

void unitAssertNotZero(char*, int);
void unitAssertZero   (char*, int);

void unitAssertEqualChar       (char*, char, char);
void unitAssertEqualShort      (char*, short, short);
void unitAssertEqualInt        (char*, int, int);
void unitAssertEqualLong       (char*, long, long);

void unitAssertEqualFloat      (char*, float, float);
void unitAssertNotEqualFloat   (char*, float, float);

void unitAssertEqualDouble     (char*, double, double);
void unitAssertEqualLongDouble (char*, long double, long double);
void unitAssertEqualVoidP      (char*, void*, void*);

void unitAssertNotNull (char*, void*);
void unitAssertNull    (char*, void*);
/*
 * Interface
 */
void unitModuleHeading(char*);
void unitModuleFooter(char*);
void unitTestHeading(char*);
void unitTestFooter(char*);
void unitPrintComment(char*);
char* unitGreen(char*);
char* unitRed(char*);
char* unitBold(char*);
#endif // UNIT_H_
