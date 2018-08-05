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
 *
 */

#include "ProjectDefinition.h"

#include "Project.h"

#include "../Common/Constants.h"
#include "../Scene/SceneDefinition.h"
#include "../Utilities/Uuid.h"

#include "../Components/IAssetDefinition.h"
#include "../Components/Animation/AnimationDefinition.h"
#include "../Components/Audio/AudioDefinition.h"
#include "../Components/Graphics/Font/FontDefinition.h"
#include "../Components/Graphics/Light/LightDefinition.h"
#include "../Components/Graphics/Model/ModelDefinition.h"
#include "../Components/Physics/PhysicsObjectDefinition.h"
#include "../Components/Graphics/Shader/ShaderDefinition.h"
#include "../Components/Graphics/Sprite/SpriteDefinition.h"
#include "../Lua/ScriptDefinition.h"

namespace Dream
{
    ProjectDefinition::ProjectDefinition(json data)
        : IDefinition(data) ,
          ILoggable("ProjectDefinition")
    {
        {
            auto log = getLog();
            log->info( "ProjectDefinition: Constructing {}", getNameAndUuidString() );
        }
        loadChildDefinitions();
    }


    ProjectDefinition::~ProjectDefinition
    ()
    {
            auto log = getLog();
        {
            log->info( "ProjectDefinition: Destructing {}",getNameAndUuidString() );
        }
    }

    void
    ProjectDefinition::showStatus
    ()
    {
            auto log = getLog();
        {
            log->info( "ProjectDefinition: {}" , mJson.dump(1) );
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
        return mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_WIDTH];
    }

    void
    ProjectDefinition::setWindowWidth
    (int width)
    {
        mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_WIDTH] = width;
    }

    int
    ProjectDefinition::getWindowHeight
    ()
    {
        return mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_HEIGHT];
    }

    void
    ProjectDefinition::setWindowHeight
    (int height)
    {
        mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_HEIGHT] = height;
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
            auto log = getLog();
            log->info( "ProjectDefinition: Loading AssetDefinitions from JSON" );

        for (nlohmann::json it : mJson[Constants::PROJECT_ASSET_ARRAY])
        {
            loadAssetDefinition(it);
        }
    }

    void
    ProjectDefinition::loadSceneDefinitions
    ()
    {
            auto log = getLog();
            log->info( "ProjectDefinition: Loading ScenesDefinitions from JSON" );

        for (nlohmann::json it : mJson[Constants::PROJECT_SCENE_ARRAY])
        {
            loadSceneDefinition(it);
        }
    }


    IAssetDefinition*
    ProjectDefinition::createAssetDefinitionInstance
    (json assetDefinitionJs)
    {
            auto log = getLog();
        IAssetDefinition* newDef = nullptr;
        AssetType type = Constants::getAssetTypeEnumFromString(assetDefinitionJs[Constants::ASSET_TYPE]);

        switch (type)
        {
            case ANIMATION:
                newDef = new AnimationDefinition(this,assetDefinitionJs);
                break;
            case AUDIO:
                newDef = new AudioDefinition(this,assetDefinitionJs);
                break;
            case FONT:
                newDef = new FontDefinition(this,assetDefinitionJs);
                break;
            case LIGHT:
                newDef = new LightDefinition(this,assetDefinitionJs);
                break;
            case MODEL:
                newDef = new ModelDefinition(this,assetDefinitionJs);
                break;
            case PHYSICS_OBJECT:
                newDef = new PhysicsObjectDefinition(this,assetDefinitionJs);
                break;
            case SCRIPT:
                newDef = new ScriptDefinition(this,assetDefinitionJs);
                break;
            case SHADER:
                newDef = new ShaderDefinition(this,assetDefinitionJs);
                break;
            case SPRITE:
                newDef = new SpriteDefinition(this,assetDefinitionJs);
                break;
            case NONE:
                log->error( "ProjectDefinition: Unable to create Asset Definition. Unknown Type" );
                newDef = nullptr;
                break;
        }
        return newDef;
    }

    void
    ProjectDefinition::loadAssetDefinition
    (json assetDefinitionJs)
    {
        IAssetDefinition *newDef = createAssetDefinitionInstance(assetDefinitionJs);
        if (newDef)
        {
            mAssetDefinitions.push_back(unique_ptr<IAssetDefinition>(newDef));
        }
    }

    void
    ProjectDefinition::removeAssetDefinition
    (IAssetDefinition* assetDefinitionHandle)
    {
        auto log = getLog();
        log->info(
            "Removing AssetDefinition {} from {}",
            assetDefinitionHandle->getNameAndUuidString(),
            getNameAndUuidString()
        );
        auto iter = begin(mAssetDefinitions);
        auto endPos = end(mAssetDefinitions);
        while (iter != endPos)
        {
            if ((*iter).get() == assetDefinitionHandle)
            {
                log->info(
                    "Found AssetDefinition to {} remove from {}",
                    assetDefinitionHandle->getNameAndUuidString(),
                    getNameAndUuidString()
                );
                mAssetDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    size_t
    ProjectDefinition::countAssetDefinitions
    ()
    {
        return mAssetDefinitions.size();
    }

    IAssetDefinition*
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
    ProjectDefinition::loadSceneDefinition
    (json scene)
    {
        mSceneDefinitions.push_back(unique_ptr<SceneDefinition>(new SceneDefinition(this,scene)));
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

    vector<SceneDefinition *> ProjectDefinition::getSceneDefinitionsHandleList()
    {
        vector<SceneDefinition*> list;
        for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            list.push_back((*it).get());
        }
        return list;
    }

    void ProjectDefinition::removeSceneDefinition(SceneDefinition* sceneDef)
    {
        auto log = getLog();
        log->info(
            "Removing SceneDefinition {} from {}",
            sceneDef->getNameAndUuidString(),
            getNameAndUuidString()
        );

        auto iter = begin(mSceneDefinitions);
        auto endPos = end(mSceneDefinitions);
        while (iter != endPos)
        {
            if ((*iter).get() == sceneDef)
            {
                log->info(
                    "Found scene to {} remove from {}",
                    sceneDef->getNameAndUuidString(),
                    getNameAndUuidString()
                );
                mSceneDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    vector<IAssetDefinition*> ProjectDefinition::getAssetDefinitionsHandleList()
    {
        vector<IAssetDefinition*> definitionsList;
        for (auto it = begin(mAssetDefinitions); it!= end(mAssetDefinitions); it++)
        {
            definitionsList.push_back((*it).get());
        }
        return definitionsList;
    }

    SceneDefinition*
    ProjectDefinition::createNewSceneDefinition
    ()
    {
        json scene;
        scene[Constants::UUID] = Uuid::generateUuid();
        scene[Constants::NAME] = Constants::SCENE_DEFAULT_NAME;
        Transform3D camTransform;
        scene[Constants::SCENE_CAMERA_TRANSFORM] = camTransform.getJson();
        SceneDefinition *sdHandle = new SceneDefinition(this,scene);
        sdHandle->createNewRootSceneObjectDefinition();
        mSceneDefinitions.push_back(unique_ptr<SceneDefinition>(sdHandle));
        return sdHandle;
    }

    IAssetDefinition*
    ProjectDefinition::createNewAssetDefinition
    (AssetType type)
    {
            auto log = getLog();
        json assetDefinitionJson;

        string defaultFormat = (*Constants::DREAM_ASSET_FORMATS_MAP.at(type).begin());
        {
            log->info( "ProjectDefinition: Creating new AssetDefinition with default Format {}", defaultFormat);
        }

        assetDefinitionJson[Constants::NAME] = Constants::ASSET_DEFINITION_DEFAULT_NAME;
        assetDefinitionJson[Constants::UUID] = Uuid::generateUuid();
        assetDefinitionJson[Constants::ASSET_TYPE] = Constants::getAssetTypeStringFromTypeEnum(type);
        assetDefinitionJson[Constants::ASSET_FORMAT] = defaultFormat;
        IAssetDefinition* adHandle = createAssetDefinitionInstance(assetDefinitionJson);
        mAssetDefinitions.push_back(unique_ptr<IAssetDefinition>(adHandle));

        return adHandle;
    }

    SceneDefinition*
    ProjectDefinition::getStartupSceneDefinitionHandle
    ()
    {
        return getSceneDefinitionHandleByUuid(getStartupSceneUuid());
    }

    json
    ProjectDefinition::getJson
    ()
    {
        mJson[Constants::PROJECT_ASSET_ARRAY] = json::array();
        for (IAssetDefinition *adHandle : getAssetDefinitionsHandleList())
        {
           mJson[Constants::PROJECT_ASSET_ARRAY].push_back(adHandle->getJson());
        }

        mJson[Constants::PROJECT_SCENE_ARRAY] = json::array();
        for (SceneDefinition* sdHandle : getSceneDefinitionsHandleList())
        {
            mJson[Constants::PROJECT_SCENE_ARRAY].push_back(sdHandle->getJson());
        }

        return mJson;
    }

    map<AssetType, vector<IAssetDefinition*>>
    ProjectDefinition::getAssetDefinitionHandlesMap
    ()
    {
        vector<IAssetDefinition*> adHandles = getAssetDefinitionsHandleList();
        map<AssetType,vector<IAssetDefinition*>> handlesMap;

        auto beginHandles =  begin(adHandles);
        auto endHandles = end(adHandles);
        auto currentHandle = beginHandles;

        auto endMap = end(handlesMap);

        // Iterate over AD Handles
        while (currentHandle != endHandles)
        {
            // Current AD type
            AssetType currentType = Constants::getAssetTypeEnumFromString((*currentHandle)->getType());
            // Find type vector in map
            auto typeVector = handlesMap.find(currentType);
            // Vector does not exist
            if (typeVector == endMap)
            {
                // Create it
                vector<IAssetDefinition*> typeVector;
                handlesMap.insert(std::pair<AssetType,vector<IAssetDefinition*>>(currentType,typeVector));
            }
            handlesMap.at(currentType).push_back((*currentHandle));
            currentHandle++;
        }
        return handlesMap;
    }
}
