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

#pragma once

#include <iostream>

#include "../Common/Constants.h"
#include "../Common/DreamObject.h"

using namespace std;

namespace Dream
{
    class ArgumentParser : DreamObject
    {

    private:
        int mArgc;
        const char** mArgv;
        string mDreamPath;
        string mProjectPath;
        string mProjectUUID;
        string mProjectFilePath;
        bool mUsingHttp;
        string mHttpUrl;

    public:
        ArgumentParser(int,const char**);
        ~ArgumentParser(void);
        void parse();
        string getDreamPath();
        string getProjectPath();
        string getProjectUUID();
        string getProjectFilePath();
        string getProjectHttpUrl();
        bool getUsingHttp();

    }; // End of ArgumentParser

} // End of Dream
