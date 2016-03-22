#ifndef UNIT_H_
#define UNIT_H_

#include <string>

#define UNIT_NORMAL  "\x1B[0m"
#define UNIT_RED     "\x1B[31m"
#define UNIT_GREEN   "\x1B[32m"
#define UNIT_YELLOW  "\x1B[33m"
#define UNIT_BLUE    "\x1B[34m"
#define UNIT_MAGENTA "\x1B[35m"
#define UNIT_CYNAN   "\x1B[36m"
#define UNIT_WHITE   "\x1B[37m"
#define UNIT_BOLD    "\x1B[1m"

namespace DreamUnit {
    class Unit {
    private:
        static const std::string UNIT_PASS;
        static const std::string UNIT_FAIL;
    public:
        //! Assert Test Passed
        void assertPass(std::string);

        //! Assert Test Failed
        void assertFail(std::string);

        //! Assert Value is not equal to zero
        template <typename T>
        void assertNotZero(std::string, T);

        //! Assert Value is equal to zero.
        template <typename T>
        void assertZero(std::string, T);

        //! Assert two objects are equal.
        template <typename T>
        void assertEqual(std::string, T, T);

        //! Assert two floats are not equal.
        template <typename T>
        void assertNotEqual(std::string, T, T);

        //! Assert pointer is not null;
        void assertNotNull(std::string, void*);

        //! Assert pointer is null.
        void assertNull(std::string, void*);

        //! Print the heading for a module
        void moduleHeading(std::string);

        //! Print the footer for a module
        void moduleFooter(std::string);

        //! Print the heading for a test section
        void testHeading(std::string);

        //! Print the footer for a test section
        void testFooter(std::string);

        //! Print a comment
        void printComment(std::string);
    private:
        //! Colour a string green (ANSI)
        std::string green(std::string);

        //! Colour a string red (ANSI)
        std::string red(std::string);

        //! Turn a string bold (ANSI)
        std::string bold(std::string);
    };
}

#endif // UNIT_H_
