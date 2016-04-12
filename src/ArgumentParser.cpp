/*
* ArgumentParser
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ArgumentParser.h"

namespace Dream {

	ArgumentParser::ArgumentParser(int argc, const char** argv) {
		mArgc = argc;
		mArgv = argv;
		parse();
	}

	ArgumentParser::~ArgumentParser(void) {

	}

	void ArgumentParser::parse() {
		std::string *nextArg;
		for (int i=0;i<mArgc;i++) {
			nextArg = new std::string(mArgv[i]);
			if (nextArg->compare(PROJECT_DIRECTORY_ARG) == 0) {
				if (mArgc > i) {
					mProjectDirectory = std::string(mArgv[i+1]);
					size_t projectDirectoryLength = mProjectDirectory.length();
					size_t uuidStart  = projectDirectoryLength - PROJECT_UUID_LENGTH;
					mProjectUUID      = mProjectDirectory.substr(uuidStart,projectDirectoryLength);
					mProjectFilePath  = mProjectDirectory + PROJECT_PATH_SEP + mProjectUUID + PROJECT_EXTENSION;
				} else {
					return;
				}
			}
			delete nextArg;
			nextArg = NULL;
		}
	}

	std::string ArgumentParser::getProjectDirectory() {
		return mProjectDirectory;
	}

	std::string ArgumentParser::getProjectUUID() {
		return mProjectUUID;
	}

	std::string ArgumentParser::getProjectFilePath() {
		return mProjectFilePath;
	}
} // End of Dream
