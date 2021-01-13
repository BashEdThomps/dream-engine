/*
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
#include "Logger.h"

namespace Dream
{
    ArgumentParser::ArgumentParser
    (int argc, const char** argv)
    {
        LOG_TRACE( "Constructing" );
        mArgc = argc;
        mArgv = argv;
        mUsingHttp = false;
        parse();
    }

    ArgumentParser::~ArgumentParser
    ()
    {
        LOG_TRACE( "Destructing" );
    }

    void
    ArgumentParser::parse
    ()
    {
        string nextArg;
        for (int i=0;i<mArgc;i++)
        {
            nextArg = string(mArgv[i]);
            if (nextArg == Constants::PROJECT_DIRECTORY_ARG)
            {
                if (mArgc > i)
                {
                    mDreamPath = string(mArgv[i+1]);
                    LOG_DEBUG( "Got Dream Path {}" , mDreamPath );
                }
                else
                {
                    return;
                }
            }
            else if (nextArg == Constants::PROJECT_UUID_ARG)
            {
                if (mArgc > i)
                {
                    mProjectUUID = string(mArgv[i+1]);
                    LOG_DEBUG( "Got Project UUID {}" , mProjectUUID );
                }
                else
                {
                    return;
                }
            }
            else if (nextArg == Constants::HTTP_ARG)
            {
                mUsingHttp = true;
                if (mArgc > i)
                {
                    mHttpUrl = string(mArgv[i+1]);
                    LOG_DEBUG( "Loading project over HTTP from {}" , mHttpUrl );
                }
                else
                {
                    LOG_ERROR( "--http passed without URL argument." );
                }
            }
        }

        mProjectPath = mDreamPath + Constants::PROJECT_PATH_SEP + mProjectUUID;

        LOG_DEBUG( "Got Project Path {}" , mProjectPath );
        mProjectFilePath = mProjectPath + Constants::PROJECT_PATH_SEP + mProjectUUID + Constants::PROJECT_EXTENSION;
        LOG_DEBUG( "Got Project File Path {} " , mProjectFilePath );
    }

    string
    ArgumentParser::getProjectPath
    ()
    const
    {
        return mProjectPath;
    }

    string
    ArgumentParser::getProjectUUID
    ()
    const
    {
        return mProjectUUID;
    }

    string
    ArgumentParser::getDreamPath
    ()
    const
    {
        return mDreamPath;
    }

    string
    ArgumentParser::getProjectFilePath
    ()
    const
    {
        return mProjectFilePath;
    }

    bool
    ArgumentParser::getUsingHttp
    ()
    const
    {
        return mUsingHttp;
    }

    string
    ArgumentParser::getProjectHttpUrl
    ()
    const
    {
        return mHttpUrl;
    }
}
