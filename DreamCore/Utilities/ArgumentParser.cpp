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

#ifdef DREAM_LOG
        auto log = getLog();
        log->trace( "Constructing" );
#endif
        mArgc = argc;
        mArgv = argv;
        mUsingHttp = false;
        parse();
    }

    ArgumentParser::~ArgumentParser
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace( "Destructing" );
#endif
    }

    void
    ArgumentParser::parse
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        string *nextArg;
        for (int i=0;i<mArgc;i++)
        {
            nextArg = new string(mArgv[i]);
            if (nextArg->compare(Constants::PROJECT_DIRECTORY_ARG) == 0)
            {
                if (mArgc > i)
                {
                    mDreamPath = string(mArgv[i+1]);
#ifdef DREAM_LOG
                    log->debug( "Got Dream Path {}" , mDreamPath );
#endif
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
#ifdef DREAM_LOG
                    log->debug( "Got Project UUID {}" , mProjectUUID );
#endif
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
#ifdef DREAM_LOG
                    log->debug( "Loading project over HTTP from {}" , mHttpUrl );
#endif
                }
                else
                {
#ifdef DREAM_LOG
                    log->error( "--http passed without URL argument." );
#endif
                }
            }
            delete nextArg;
            nextArg = nullptr;
        }

        mProjectPath = mDreamPath + Constants::PROJECT_PATH_SEP + mProjectUUID;

#ifdef DREAM_LOG
        log->debug( "Got Project Path {}" , mProjectPath );
#endif
        mProjectFilePath = mProjectPath + Constants::PROJECT_PATH_SEP + mProjectUUID + Constants::PROJECT_EXTENSION;

#ifdef DREAM_LOG
        log->debug( "Got Project File Path {} " , mProjectFilePath );
#endif
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
