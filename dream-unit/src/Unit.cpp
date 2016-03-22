#include <string>
#include <iostream>
#include <sstream>
#include "Unit.h"

namespace DreamUnit {

	const std::string Unit::UNIT_PASS = "PASS";
	const std::string Unit::UNIT_FAIL = "FAIL";

	void Unit::moduleHeading(std::string name) {
		std::stringstream stream;
		stream << "===============[ Start Testing Module: " << name << " ]===============";
		std::string formatted = bold(stream.str());
		std::cout << formatted << std::endl;
		return;
	}

	void Unit::testHeading(std::string name) {
		std::stringstream stream;
		stream << "-----[ Testing: "<< name <<" ]-----";
		std::string formatted = bold(stream.str());
		std::cout << formatted << std::endl;;
		return;
	}

	void Unit::moduleFooter(std::string name) {
		std::stringstream stream;
		stream << "===============[ Finished Testing Module: "<< name << " ]===============";
		std::string formatted = bold(stream.str());
		std::cout << formatted << std::endl;
		return;
	}

	void Unit::testFooter(std::string name) {
		std::stringstream stream;
		stream << "-----[ Finished Testing: " << name << " ]-----";
		std::string formatted = bold(stream.str());
		std::cout << formatted << std::endl;
		return;
	}

	std::string Unit::green(std::string src) {
	    std::stringstream dest;
	    dest << UNIT_BOLD << UNIT_GREEN << src << UNIT_NORMAL;
	    return dest.str();
	}

	std::string Unit::red(std::string src) {
	    std::stringstream dest;
	    dest << UNIT_BOLD << UNIT_RED << src << UNIT_NORMAL;
	    return dest.str();
	}

	std::string Unit::bold(std::string src) {
	    std::stringstream dest;
	    dest << UNIT_BOLD << src << UNIT_NORMAL;
	    return dest.str();
	}

	void Unit::assertFail(std::string name) {
	    std::string formattedText = red(UNIT_FAIL);
	    std::cout << name << ": Assert fail " << formattedText << std::endl;
		return;
	}

	void Unit::assertPass(std::string name) {
	    std::string formattedText = green(UNIT_PASS);
	    std::cout << name << ": Assert pass " << formattedText;
		return;
	}

	template <typename T>
	void Unit::assertNotZero(std::string name, T value) {
	    std::string formattedText;
	    if (value != 0) {
	        formattedText = green(UNIT_PASS);
	    } else {
	        formattedText = red(UNIT_FAIL);
	    }
	    std::cout << name << ": Assert not zero: " << formattedText << std::endl;
		return;
	}

	template <typename T>
	void Unit::assertZero(std::string name, T value) {
	    std::string formattedText;
	    if (value == 0) {
	        formattedText = green(UNIT_PASS);
	    } else {
	        formattedText = red(UNIT_FAIL);
	    }
	    std::cout << name << ": Assert zero: " << formattedText << std::endl;
		return;
	}

	template <typename T>
	void Unit::assertEqual(std::string name, T val1, T val2) {
	    std::string formattedText;
	    if (val1 == val2) {
	        formattedText = green(UNIT_PASS);
	    } else {
	        formattedText = red(UNIT_FAIL);
	    }
	    std::cout << name << ": Assert zero: " << formattedText << std::endl;
		return;
	}

	template <typename T>
	void Unit::assertNotEqual(std::string name, T val1, T val2) {
	    std::string formattedText;

	    if (val1 != val2) {
	        formattedText = green(UNIT_PASS);
	    } else {
	        formattedText = red(UNIT_FAIL);
	    }
	    std::cout << name << ": Assert '"<< val1 << "' and '"<< val2 << "' are not equal: " << formattedText << std::endl;
		return;
	}

	void Unit::assertNotNull(std::string name, void* val) {
	    std::string formattedText;
	    if (val != NULL) {
	        formattedText = green(UNIT_PASS);
	    } else {
	        formattedText = red(UNIT_FAIL);
	    }
	    std::cout << name << ": Assert not null: " << formattedText << std::endl;
		return;
	}

	void Unit::assertNull(std::string name, void* val) {
	    std::string formattedText;
	    if (val == NULL) {
	        formattedText = green(UNIT_PASS);
	    } else {
	        formattedText = red(UNIT_FAIL);
	    }
	    std::cout << name << ": Assert null: " << formattedText << std::endl;
		return;
	}

	void Unit::printComment(std::string comment) {
		std::stringstream buf;
		buf << "-[ " << comment;
		std::string formatted = bold(buf.str());
		std::cout << formatted << std::endl;
		return;
	}
}
