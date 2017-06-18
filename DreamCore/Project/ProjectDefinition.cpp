/*
 * ProjectDefinition.cpp
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
#include "ProjectDefinition.h"

#include "Project.h"

#include "../Components/AssetDefinition.h"
#include "../Scene/SceneDefinition.h"

#include "../Common/Constants.h"

using std::cout;
using std::endl;

namespace Dream
{
    ProjectDefinition::ProjectDefinition(json data)
        : IDefinition(data)
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: Constructing "
                 << getNameAndUuidString() << endl;
        }
    }


    ProjectDefinition::~ProjectDefinition
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: Destructing "
                 << getNameAndUuidString() << endl;
        }
    }

    void
    ProjectDefinition::showStatus
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: " << mJson.dump(1) << endl;
        }
    }

    void
    ProjectDefinition::setStartupSceneUuid
    (string sceneUuid)
    {
        mJson[Constants::PROJECT_STARTUP_SCENE] = sceneUuid;
    }

    string
    ProjectDefinition::getStartupSceneUuid
    ()
    {
        return mJson[Constants::PROJECT_STARTUP_SCENE];
    }

    void
    ProjectDefinition::setDescription
    (string description)
    {
        mJson[Constants::PROJECT_DESCRIPTION] = description;
    }

    string
    ProjectDefinition::getDescription
    (void)
    {
        return mJson[Constants::PROJECT_DESCRIPTION];
    }

    void
    ProjectDefinition::setAuthor
    (string author)
    {
        mJson[Constants::PROJECT_AUTHOR] = author;
    }

    string
    ProjectDefinition::getAuthor
    ()
    {
        return mJson[Constants::PROJECT_AUTHOR];
    }

    int
    ProjectDefinition::getWindowWidth
    ()
    {
        return mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WIDTH];
    }

    void
    ProjectDefinition::setWindowWidth
    (int width)
    {
        mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WIDTH] = width;
    }

    int
    ProjectDefinition::getWindowHeight
    ()
    {
       return mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_HEIGHT];
    }

    void
    ProjectDefinition::setWindowHeight
    (int height)
    {
        mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_HEIGHT] = height;
    }

    void
    ProjectDefinition::loadChildDefinitions
    ()
    {
        loadAssetDefinitions();
        loadSceneDefinitions();
    }

    void
    ProjectDefinition::loadAssetDefinitions
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: Loading AssetDefinitions from JSON" << endl;
        }

        for (nlohmann::json it : mJson[Constants::PROJECT_ASSET_ARRAY])
        {
            addAssetDefinition(it);
        }
    }

    void
    ProjectDefinition::loadSceneDefinitions
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: Loading ScenesDefinitions from JSON" << endl;
        }

        for (nlohmann::json it : mJson[Constants::PROJECT_SCENE_ARRAY])
        {
            addSceneDefinition(it);
        }
    }

    void
    ProjectDefinition::addAssetDefinition
    (json assetDefinition)
    {
        mAssetDefinitions.push_back
        (
            unique_ptr<AssetDefinition>
            (
                new AssetDefinition(this,assetDefinition)
            )
        );
    }

    void
    ProjectDefinition::removeAssetDefinition
    (AssetDefinition* assetDefinitionHandle)
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: Removing AssetDefinition "
                 << assetDefinitionHandle->getNameAndUuidString() << endl;
        }

        remove_if(begin(mAssetDefinitions),end(mAssetDefinitions),
            [&](const unique_ptr<AssetDefinition>& thisDefinition)
            {
                return thisDefinition.get() == assetDefinitionHandle;
            }
        );
    }

    size_t
    ProjectDefinition::countAssetDefinitions
    ()
    {
        return mAssetDefinitions.size();
    }

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionHandleByUuid
    (string uuid)
    {
        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it).get();
            }
        }
        return nullptr;
    }

    void
    ProjectDefinition::addSceneDefinition
    (json scene)
    {
        mSceneDefinitions.push_back
        (
            unique_ptr<SceneDefinition>
            (
                new SceneDefinition(this,scene)
            )
        );
    }

    size_t
    ProjectDefinition::countScenesDefinitions
    ()
    {
        return mSceneDefinitions.size();
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionHandleByUuid
    (string uuid)
    {
        for(auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it).get();
            }
        }
        return nullptr;
    }
}
