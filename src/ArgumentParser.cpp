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

#include "ArgumentParser.h"
#include "Environment.h"

namespace Dream {
  ArgumentParser::ArgumentParser(int argc, const char** argv) {
    mArgc = argc;
    mArgv = argv;
    mUsingHttp = false;
    parse();
  }

  ArgumentParser::~ArgumentParser() {}

  void ArgumentParser::parse() {
    string *nextArg;
    for (int i=0;i<mArgc;i++) {
      nextArg = new string(mArgv[i]);
      if (nextArg->compare(PROJECT_DIRECTORY_ARG) == 0) {
        if (mArgc > i) {
          mDreamPath = string(mArgv[i+1]);
          if (DEBUG) {
            cout << "ArgumentParser: Got Dream Path " << mDreamPath << endl;
          }
        } else {
          return;
        }
      }
      else if (nextArg->compare(PROJECT_UUID_ARG) == 0) {
        if (mArgc > i) {
          mProjectUUID = string(mArgv[i+1]);
          if (DEBUG) {
            cout << "ArgumentParser: Got Project UUID " << mProjectUUID << endl;
          }
        } else {
          return;
        }
      }
      else if (nextArg->compare(HTTP_ARG) == 0) {
          mUsingHttp = true;
          if (mArgc > i) {
            mHttpUrl = string(mArgv[i+1]);
            if (DEBUG) {
                cout << "ArgumentParser: Loading project over HTTP from " << mHttpUrl << endl;
            }
          } else {
              cerr << "ArgumentParser: --http passed without URL argument." << endl;
          }
      }
      delete nextArg;
      nextArg = nullptr;
    }

    if (mDreamPath.size() == 0) {
      mDreamPath = Environment::getHomeDirectory() + PROJECT_PATH_SEP + DREAM_TOOL_DIR;
    }

    mProjectPath = mDreamPath + PROJECT_PATH_SEP + mProjectUUID;

    if (DEBUG) {
      cout << "ArgumentParser: Got Project Path " << mProjectPath << endl;
    }
    mProjectFilePath = mProjectPath + PROJECT_PATH_SEP + mProjectUUID + PROJECT_EXTENSION;

    if (DEBUG) {
      cout << "ArgumentParser: Got Project File Path " << mProjectFilePath << endl;
    }
  }

  string ArgumentParser::getProjectPath() {
    return mProjectPath;
  }

  string ArgumentParser::getProjectUUID() {
    return mProjectUUID;
  }

  string ArgumentParser::getDreamPath(){
    return mDreamPath;
  }

  string ArgumentParser::getProjectFilePath() {
    return mProjectFilePath;
  }

  bool ArgumentParser::getUsingHttp() {
      return mUsingHttp;
  }

  string ArgumentParser::getProjectHttpUrl() {
      return mHttpUrl;
  }

} // End of Dream
