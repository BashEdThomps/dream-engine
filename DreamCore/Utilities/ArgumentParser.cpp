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

namespace Dream
{
    ArgumentParser::ArgumentParser
    (int argc, const char** argv)
        : DreamObject ("ArgumentParser")
    {
        auto log = getLog();
        log->trace( "Constructing" );
        mArgc = argc;
        mArgv = argv;
        mUsingHttp = false;
        parse();
    }

    ArgumentParser::~ArgumentParser
    ()
    {
        auto log = getLog();
        log->trace( "Destructing" );
    }

    void
    ArgumentParser::parse
    ()
    {
        auto log = getLog();
        string *nextArg;
        for (int i=0;i<mArgc;i++)
        {
            nextArg = new string(mArgv[i]);
            if (nextArg->compare(Constants::PROJECT_DIRECTORY_ARG) == 0)
            {
                if (mArgc > i)
                {
                    mDreamPath = string(mArgv[i+1]);
                    log->debug( "Got Dream Path {}" , mDreamPath );
                }
                else
                {
                    return;
                }
            }
            else if (nextArg->compare(Constants::PROJECT_UUID_ARG) == 0)
            {
                if (mArgc > i)
                {
                    mProjectUUID = string(mArgv[i+1]);
                    log->debug( "Got Project UUID {}" , mProjectUUID );
                }
                else
                {
                    return;
                }
            }
            else if (nextArg->compare(Constants::HTTP_ARG) == 0)
            {
                mUsingHttp = true;
                if (mArgc > i)
                {
                    mHttpUrl = string(mArgv[i+1]);
                    log->debug( "Loading project over HTTP from {}" , mHttpUrl );
                }
                else
                {
                    log->error( "--http passed without URL argument." );
                }
            }
            delete nextArg;
            nextArg = nullptr;
        }

        if (mDreamPath.size() == 0)
        {
            mDreamPath = Environment::getHomeDirectory() + Constants::PROJECT_PATH_SEP + Constants::DREAM_TOOL_DIR;
        }

        mProjectPath = mDreamPath + Constants::PROJECT_PATH_SEP + mProjectUUID;

        log->debug( "Got Project Path {}" , mProjectPath );
        mProjectFilePath = mProjectPath + Constants::PROJECT_PATH_SEP + mProjectUUID + Constants::PROJECT_EXTENSION;

        log->debug( "Got Project File Path {} " , mProjectFilePath );
    }

    string
    ArgumentParser::getProjectPath
    ()
    {
        return mProjectPath;
    }

    string
    ArgumentParser::getProjectUUID
    ()
    {
        return mProjectUUID;
    }

    string
    ArgumentParser::getDreamPath
    ()
    {
        return mDreamPath;
    }

    string
    ArgumentParser::getProjectFilePath
    ()
    {
        return mProjectFilePath;
    }

    bool
    ArgumentParser::getUsingHttp
    ()
    {
        return mUsingHttp;
    }

    string
    ArgumentParser::getProjectHttpUrl
    ()
    {
        return mHttpUrl;
    }

} // End of Dream
