/*
 * ProjectJsonData.cpp
 *
 * Created: 16 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "ProjectJsonData.h"

#include "../Common/Constants.h"

namespace Dream
{
    ProjectJsonData::ProjectJsonData(json data)
        : JsonData(data)
    {

    }


    ProjectJsonData::~ProjectJsonData
    ()
    {

    }

    string
    ProjectJsonData::getName
    ()
    {
        return mJson[Constants::PROJECT_NAME];
    }

    void
    ProjectJsonData::setName
    (string name)
    {
        mJson[Constants::PROJECT_NAME] = name;
    }

    void
    ProjectJsonData::setUuid
    (string uuid)
    {
        mJson[Constants::PROJECT_UUID] = uuid;
    }

    string
    ProjectJsonData::getUuid
    ()
    {
        return mJson[Constants::PROJECT_UUID];
    }

    void
    ProjectJsonData::setStartupSceneUuid
    (string sceneUuid)
    {
        mJson[Constants::PROJECT_STARTUP_SCENE] = sceneUuid;
    }

    string
    ProjectJsonData::getStartupSceneUuid
    ()
    {
        return mJson[Constants::PROJECT_STARTUP_SCENE];
    }

    void
    ProjectJsonData::setDescription
    (string description)
    {
        mJson[Constants::PROJECT_DESCRIPTION] = description;
    }

    string
    ProjectJsonData::getDescription
    (void)
    {
        return mJson[Constants::PROJECT_DESCRIPTION];
    }

    void
    ProjectJsonData::setAuthor
    (string author)
    {
        mJson[Constants::PROJECT_AUTHOR] = author;
    }

    string
    ProjectJsonData::getAuthor
    ()
    {
        return mJson[Constants::PROJECT_AUTHOR];
    }

    int
    ProjectJsonData::getWindowWidth
    ()
    {
        return mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WIDTH];
    }

    void
    ProjectJsonData::setWindowWidth
    (int width)
    {
        mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WIDTH] = width;
    }

    int
    ProjectJsonData::getWindowHeight
    ()
    {
       return mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_HEIGHT];
    }

    void
    ProjectJsonData::setWindowHeight
    (int height)
    {
        mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_HEIGHT] = height;
    }

}
