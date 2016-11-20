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
        class Unit {
        private:
            static const std::string UNIT_PASS;
            static const std::string UNIT_INCONCLUSIVE;
            static const std::string UNIT_FAIL;
            int         mResult;
            std::string mName;
        public:
            //! Default Constructor
            Unit(std::string);

            //! Default Destructor
            virtual ~Unit(void) = 0;

            //! Run Method
            virtual void run(void) = 0;

            //! Assert Test Passed
            void assertPass(std::string);

            //! Assert Test Failed
            void assertFail(std::string);

            //! Assert Test Failed
            void assertInconclusive(std::string);

            //! Assert Value is not equal to zero
            void assertNotZero(std::string, int);

            //! Assert Value is equal to zero.
            void assertZero(std::string, int);

            //! Assert Value is equal to true.
            void assertTrue(std::string, bool);

            //! Assert Value is equal to false.
            void assertFalse(std::string, bool);

            //! Assert two objects are equal.
            void assertEqual(std::string, int, int);

            //! Assert two floats are not equal.
            void assertNotEqual(std::string, int, int);

            //! Assert pointer is not null;
            void assertNotNull(std::string, void*);

            //! Assert pointer is null.
            void assertNull(std::string, void*);

            //! Print the heading for a module
            void header();

            //! Print a comment
            void comment(std::string);

            //! Get testing result
            int getResult(void);
        private:
            //! Colour a string green (ANSI)
            std::string green(std::string);

            //! Colour a string red (ANSI)
            std::string red(std::string);

            //! Colour a string red (ANSI)
            std::string yellow(std::string);

            //! Turn a string bold (ANSI)
            std::string bold(std::string);
        };
}

#endif // UNIT_H_
