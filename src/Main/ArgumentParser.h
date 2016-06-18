/*
* Dream::ArgumentParser
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

#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <iostream>

#define PROJECT_DIRECTORY_ARG "--project-dir"
#define PROJECT_UUID_ARG      "--uuid"
#define PROJECT_UUID_LENGTH   19
#define PROJECT_EXTENSION     ".json"
#define PROJECT_PATH_SEP      "/"
#define DREAM_TOOL_DIR        ".dreamtool"

namespace Dream {
	class ArgumentParser {
	private:
		int           mArgc;
		const char**  mArgv;
		std::string   mDreamPath;
		std::string   mProjectPath;
		std::string   mProjectUUID;
		std::string   mProjectFilePath;
	public:
		ArgumentParser(int,const char**);
		~ArgumentParser(void);
		void parse();
		std::string getDreamPath();
		std::string getProjectPath();
		std::string getProjectUUID();
		std::string getProjectFilePath();
	}; // End of ArgumentParser
} // End of Dream

#endif // End of ARGUMENTPARSER_H
