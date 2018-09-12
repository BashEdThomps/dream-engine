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
#include "../Components/Path/PathDefinition.h"
#include "../Components/Audio/AudioDefinition.h"
#include "../Components/Graphics/Font/FontDefinition.h"
#include "../Components/Graphics/Light/LightDefinition.h"
#include "../Components/Graphics/Model/ModelDefinition.h"
#include "../Components/Physics/PhysicsObjectDefinition.h"
#include "../Components/Graphics/Shader/ShaderDefinition.h"
#include "../Components/Graphics/Sprite/SpriteDefinition.h"
#include "../Components/Scripting/ScriptDefinition.h"

using std::make_shared;

namespace Dream
{
    ProjectDefinition::ProjectDefinition(json data)
        : IDefinition(data)

    {

        setLogClassName("ProjectDefinition");
        auto log = getLog();
        log->trace( "Constructing {}", getNameAndUuidString() );
    }


    ProjectDefinition::~ProjectDefinition
    ()
    {
        auto log = getLog();
        log->trace( "Destructing {}",getNameAndUuidString() );
    }

    void
    ProjectDefinition::showStatus
    ()
    {
        auto log = getLog();
        log->info(mJson.dump(1) );
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
        log->info( "Loading AssetDefinitions from JSON" );

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
        log->info( "Loading ScenesDefinitions from JSON" );

        for (nlohmann::json it : mJson[Constants::PROJECT_SCENE_ARRAY])
        {
            loadSceneDefinition(it);
        }
    }


    shared_ptr<IAssetDefinition>
    ProjectDefinition::createAssetDefinitionInstance
    (json assetDefinitionJs)
    {
        auto log = getLog();
        AssetType type = Constants::getAssetTypeEnumFromString(assetDefinitionJs[Constants::ASSET_TYPE]);

        switch (type)
        {
            case PATH:
                return make_shared<PathDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case AUDIO:
                return make_shared<AudioDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case FONT:
                return make_shared<FontDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case LIGHT:
                return make_shared<LightDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case MODEL:
                return make_shared<ModelDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case PHYSICS_OBJECT:
                return make_shared<PhysicsObjectDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case SCRIPT:
                return make_shared<ScriptDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case SHADER:
                return make_shared<ShaderDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case SPRITE:
                return make_shared<SpriteDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),assetDefinitionJs);
            case NONE:
                log->error( "Unable to create Asset Definition. Unknown Type" );
                return nullptr;
        }
    }

    void
    ProjectDefinition::loadAssetDefinition
    (json assetDefinitionJs)
    {
        shared_ptr<IAssetDefinition> newDef = createAssetDefinitionInstance(assetDefinitionJs);
        if (newDef != nullptr)
        {
            mAssetDefinitions.push_back(newDef);
        }
    }

    void
    ProjectDefinition::removeAssetDefinition
    (shared_ptr<IAssetDefinition> assetDefinition)
    {
        auto log = getLog();
        log->info(
                    "Removing AssetDefinition {} from {}",
                    assetDefinition->getNameAndUuidString(),
                    getNameAndUuidString()
                    );
        auto iter = begin(mAssetDefinitions);
        auto endPos = end(mAssetDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == assetDefinition)
            {
                log->info(
                            "Found AssetDefinition to {} remove from {}",
                            assetDefinition->getNameAndUuidString(),
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

    shared_ptr<IAssetDefinition>
    ProjectDefinition::getAssetDefinitionByUuid
    (string uuid)
    {
        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    void
    ProjectDefinition::loadSceneDefinition
    (json scene)
    {
        auto so = make_shared<SceneDefinition>
        (dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),scene);
        so->loadRootSceneObjectDefinition();
        mSceneDefinitions.push_back(so);
    }

    size_t
    ProjectDefinition::countScenesDefinitions
    ()
    {
        return mSceneDefinitions.size();
    }

    shared_ptr<SceneDefinition>
    ProjectDefinition::getSceneDefinitionByName
    (string name)
    {
        for(auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            if ((*it)->hasName(name))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    shared_ptr<SceneDefinition>
    ProjectDefinition::getSceneDefinitionByUuid
    (string uuid)
    {
        for(auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    vector<shared_ptr<SceneDefinition>>
    ProjectDefinition::getSceneDefinitionsList
    ()
    {
        vector<shared_ptr<SceneDefinition>> list;
        for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            list.push_back((*it));
        }
        return list;
    }

    void
    ProjectDefinition::removeSceneDefinition
    (shared_ptr<SceneDefinition> sceneDef)
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
            if ((*iter) == sceneDef)
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

    vector<shared_ptr<IAssetDefinition>>
    ProjectDefinition::getAssetDefinitionsList
    ()
    {
        vector<shared_ptr<IAssetDefinition>> definitionsList;
        for (auto it = begin(mAssetDefinitions); it!= end(mAssetDefinitions); it++)
        {
            definitionsList.push_back((*it));
        }
        return definitionsList;
    }

    shared_ptr<SceneDefinition>
    ProjectDefinition::createNewSceneDefinition
    ()
    {
        json scene;
        scene[Constants::UUID] = Uuid::generateUuid();
        scene[Constants::NAME] = Constants::SCENE_DEFAULT_NAME;
        Transform3D camTransform;
        scene[Constants::SCENE_CAMERA_TRANSFORM] = camTransform.getJson();
        auto sd = make_shared<SceneDefinition>(dynamic_pointer_cast<ProjectDefinition>(shared_from_this()),scene);
        sd->createNewRootSceneObjectDefinition();
        mSceneDefinitions.push_back(sd);
        return sd;
    }

    shared_ptr<IAssetDefinition>
    ProjectDefinition::createNewAssetDefinition
    (AssetType type)
    {
        auto log = getLog();
        json assetDefinitionJson;

        string defaultFormat = (*Constants::DREAM_ASSET_FORMATS_MAP.at(type).begin());
        {
            log->info( "Creating new AssetDefinition with default Format {}", defaultFormat);
        }

        assetDefinitionJson[Constants::NAME] = Constants::ASSET_DEFINITION_DEFAULT_NAME;
        assetDefinitionJson[Constants::UUID] = Uuid::generateUuid();
        assetDefinitionJson[Constants::ASSET_TYPE] = Constants::getAssetTypeStringFromTypeEnum(type);
        assetDefinitionJson[Constants::ASSET_FORMAT] = defaultFormat;
        shared_ptr<IAssetDefinition> ad = createAssetDefinitionInstance(assetDefinitionJson);
        mAssetDefinitions.push_back(ad);

        return ad;
    }

    shared_ptr<SceneDefinition>
    ProjectDefinition::getStartupSceneDefinition
    ()
    {
        auto log = getLog();
        string startupScene = getStartupSceneUuid();
        log->info("Finding startup scene {}", startupScene);
        return getSceneDefinitionByName(startupScene);
    }

    json
    ProjectDefinition::getJson
    ()
    {
        mJson[Constants::PROJECT_ASSET_ARRAY] = json::array();
        for (shared_ptr<IAssetDefinition> ad : getAssetDefinitionsList())
        {
            mJson[Constants::PROJECT_ASSET_ARRAY].push_back(ad->getJson());
        }

        mJson[Constants::PROJECT_SCENE_ARRAY] = json::array();
        for (shared_ptr<SceneDefinition> sd : getSceneDefinitionsList())
        {
            mJson[Constants::PROJECT_SCENE_ARRAY].push_back(sd->getJson());
        }

        return mJson;
    }

    map<AssetType, vector<shared_ptr<IAssetDefinition>>>
    ProjectDefinition::getAssetDefinitionsMap
    ()
    {
        vector<shared_ptr<IAssetDefinition>> ads = getAssetDefinitionsList();
        map<AssetType,vector<shared_ptr<IAssetDefinition>>> handlesMap;

        auto begins =  begin(ads);
        auto ends = end(ads);
        auto current = begins;

        auto endMap = end(handlesMap);

        // Iterate over AD s
        while (current != ends)
        {
            // Current AD type
            AssetType currentType = Constants::getAssetTypeEnumFromString((*current)->getType());
            // Find type vector in map
            auto typeVector = handlesMap.find(currentType);
            // Vector does not exist
            if (typeVector == endMap)
            {
                // Create it
                vector<shared_ptr<IAssetDefinition>> typeVector;
                handlesMap.insert(
                    std::pair<AssetType,vector<shared_ptr<IAssetDefinition>>>
                    (currentType,typeVector)
                );
            }
            handlesMap.at(currentType).push_back((*current));
            current++;
        }
        return handlesMap;
    }

    vector<shared_ptr<ShaderDefinition>> ProjectDefinition::getShaderAssetDefinitionVector()
    {
        vector<shared_ptr<ShaderDefinition>> shaders;
        for (auto it = begin(mAssetDefinitions); it!= end(mAssetDefinitions); it++)
        {
            shared_ptr<IAssetDefinition> next = (*it);
            if (next->getType() == Constants::ASSET_TYPE_SHADER)
            {
                shaders.push_back(dynamic_pointer_cast<ShaderDefinition>(next));
            }
        }
        return shaders;
    }

    bool ProjectDefinition::getCaptureKeyboard()
    {
       if (mJson[Constants::PROJECT_CAPTURE_KEYBOARD].is_null())
       {
            mJson[Constants::PROJECT_CAPTURE_KEYBOARD] = false;
       }
       return mJson[Constants::PROJECT_CAPTURE_KEYBOARD];
    }

    void ProjectDefinition::setCaptureKeyboard(bool cap)
    {
       mJson[Constants::PROJECT_CAPTURE_KEYBOARD] = cap;
    }

    bool ProjectDefinition::getCaptureMouse()
    {
        if (mJson[Constants::PROJECT_CAPTURE_MOUSE].is_null())
       {
            mJson[Constants::PROJECT_CAPTURE_MOUSE] = false;
       }
       return mJson[Constants::PROJECT_CAPTURE_MOUSE];
    }

    void ProjectDefinition::setCaptureMouse(bool cap)
    {
       mJson[Constants::PROJECT_CAPTURE_MOUSE] = cap;
    }

    bool ProjectDefinition::getCaptureJoystick()
    {
       if (mJson[Constants::PROJECT_CAPTURE_JOYSTICK].is_null())
       {
            mJson[Constants::PROJECT_CAPTURE_JOYSTICK] = false;
       }
       return mJson[Constants::PROJECT_CAPTURE_JOYSTICK];
    }

    void ProjectDefinition::setCaptureJoystick(bool cap)
    {
       mJson[Constants::PROJECT_CAPTURE_JOYSTICK] = cap;
    }
}
