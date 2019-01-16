/*
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
#include "../Scene/SceneDefinition.h"
#include "../Components/AssetDefinition.h"
#include "../Components/Audio/AudioDefinition.h"
#include "../Components/Animation/AnimationDefinition.h"
#include "../Components/Graphics/Font/FontDefinition.h"
#include "../Components/Graphics/Light/LightDefinition.h"
#include "../Components/Graphics/Material/MaterialDefinition.h"
#include "../Components/Graphics/Model/ModelDefinition.h"
#include "../Components/Graphics/Shader/ShaderDefinition.h"
#include "../Components/Graphics/ParticleEmitter/ParticleEmitterDefinition.h"
#include "../Components/Graphics/Texture/TextureDefinition.h"
#include "../Components/Path/PathDefinition.h"
#include "../Components/Scroller/ScrollerDefinition.h"
#include "../Components/Physics/PhysicsObjectDefinition.h"
#include "../Components/Scripting/ScriptDefinition.h"
#include "../Utilities/Uuid.h"

namespace Dream
{
    ProjectDefinition::ProjectDefinition
    (json data)
        : Definition(data)

    {
        #ifdef DREAM_LOG
        setLogClassName("ProjectDefinition");
        auto log = getLog();
        log->trace("Constructing {}", getNameAndUuidString());
        #endif
    }


    ProjectDefinition::~ProjectDefinition
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Destructing {}", getNameAndUuidString());
        #endif
        deleteSceneDefinitions();
        deleteAssetDefinitions();
    }

    void
    ProjectDefinition::setStartupSceneUuid
    (uint32_t sceneUuid)
    {
        mJson[Constants::PROJECT_STARTUP_SCENE] = sceneUuid;
    }

    uint32_t ProjectDefinition::getStartupSceneUuid()
    {
        if (!mJson[Constants::PROJECT_STARTUP_SCENE].is_number())
        {
            mJson[Constants::PROJECT_STARTUP_SCENE] = 0;
        }
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
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Loading AssetDefinitions from JSON");
        #endif

        for (nlohmann::json it : mJson[Constants::PROJECT_ASSET_ARRAY])
        {
            loadAssetDefinition(it);
        }
        regroupAssetDefinitions();
    }

    void
    ProjectDefinition::loadSceneDefinitions
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Loading ScenesDefinitions from JSON");
        #endif

        for (nlohmann::json it : mJson[Constants::PROJECT_SCENE_ARRAY])
        {
            loadSceneDefinition(it);
        }
    }


    AssetDefinition*
    ProjectDefinition::createAssetDefinitionRuntime
    (json assetDefinitionJs)
    {
        AssetType type = Constants::getAssetTypeEnumFromString(assetDefinitionJs[Constants::ASSET_TYPE]);

        switch (type)
        {
            case ANIMATION:
                return new AnimationDefinition(this, assetDefinitionJs);
            case AUDIO:
                return new AudioDefinition(this, assetDefinitionJs);
            case FONT:
                return new FontDefinition(this, assetDefinitionJs);
            case LIGHT:
                return new LightDefinition(this, assetDefinitionJs);
            case MATERIAL:
                return new MaterialDefinition(this, assetDefinitionJs);
            case MODEL:
                return new ModelDefinition(this, assetDefinitionJs);
            case PATH:
                return new PathDefinition(this, assetDefinitionJs);
            case PARTICLE_EMITTER:
                return new ParticleEmitterDefinition(this, assetDefinitionJs);
            case PHYSICS_OBJECT:
                return new PhysicsObjectDefinition(this, assetDefinitionJs);
            case SCRIPT:
                return new ScriptDefinition(this, assetDefinitionJs);
            case SCROLLER:
                return new ScrollerDefinition(this,assetDefinitionJs);
            case SHADER:
                return new ShaderDefinition(this, assetDefinitionJs);
            case TEXTURE:
                return new TextureDefinition(this, assetDefinitionJs);
            case NONE:
                #ifdef DREAM_LOG
                auto log = getLog();
                log->error("Unable to create Asset Definition. Unknown Type");
                #endif
                break;
        }
        return nullptr;
    }

    map<AssetType, vector<string> > &
    ProjectDefinition::getAssetDefinitionGroups
    ()
    {
        return mAssetDefinitionGroups;
    }

    void
    ProjectDefinition::loadAssetDefinition
    (json assetDefinitionJs)
    {
        AssetDefinition* newDef = createAssetDefinitionRuntime(assetDefinitionJs);
        if (newDef != nullptr)
        {
            mAssetDefinitions.push_back(newDef);
        }
    }

    void
    ProjectDefinition::removeAssetDefinition
    (AssetDefinition* assetDefinition)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug(
                    "Removing AssetDefinition {} from {}",
                    assetDefinition->getNameAndUuidString(),
                    getNameAndUuidString()
                    );
        #endif
        auto iter = begin(mAssetDefinitions);
        auto endPos = end(mAssetDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == assetDefinition)
            {
                #ifdef DREAM_LOG
                log->debug(
                            "Found AssetDefinition to {} remove from {}",
                            assetDefinition->getNameAndUuidString(),
                            getNameAndUuidString()
                            );
                #endif
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

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionByUuid
    (uint32_t uuid)
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

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionByName
    (string name)
    {
        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            if ((*it)->hasName(name))
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
        auto so = new SceneDefinition(this, scene);
        so->loadRootSceneObjectDefinition();
        mSceneDefinitions.push_back(so);
    }

    size_t
    ProjectDefinition::countScenesDefinitions
    ()
    {
        return mSceneDefinitions.size();
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionByName
    (string name)
    {
        for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            if ((*it)->hasName(name))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionByUuid
    (uint32_t uuid)
    {
        for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    vector<SceneDefinition*>
    ProjectDefinition::getSceneDefinitionsVector
    ()
    {
        vector<SceneDefinition*> list;
        for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            list.push_back((*it));
        }
        return list;
    }

    void
    ProjectDefinition::removeSceneDefinition
    (SceneDefinition* sceneDef)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug(
                    "Removing SceneDefinition {} from {}",
                    sceneDef->getNameAndUuidString(),
                    getNameAndUuidString()
                    );
        #endif

        auto iter = begin(mSceneDefinitions);
        auto endPos = end(mSceneDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == sceneDef)
            {
                #ifdef DREAM_LOG
                log->debug(
                            "Found scene to {} remove from {}",
                            sceneDef->getNameAndUuidString(),
                            getNameAndUuidString()
                            );
                #endif
                delete (*iter);
                mSceneDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    vector<AssetDefinition*>
    ProjectDefinition::getAssetDefinitionsVector
    ()
    {
        vector<AssetDefinition*> definitionsList;
        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            definitionsList.push_back((*it));
        }
        return definitionsList;
    }

    vector<AssetDefinition*>
    ProjectDefinition::getAssetDefinitionsVector
    (AssetType type)
    {
        vector<AssetDefinition*> definitionsList;
        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            auto thisAssetType = Constants::getAssetTypeEnumFromString((*it)->getType());
            if (thisAssetType == type)
            {
                definitionsList.push_back((*it));
            }
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
        Transform camTransform;
        scene[Constants::SCENE_CAMERA_TRANSFORM] = camTransform.getJson();
        auto sd = new SceneDefinition(this, scene);
        sd->createNewRootSceneObjectDefinition();
        mSceneDefinitions.push_back(sd);
        return sd;
    }

    AssetDefinition*
    ProjectDefinition::createNewAssetDefinition
    (AssetType type)
    {
        json assetDefinitionJson;

        string defaultFormat = (*Constants::DREAM_ASSET_FORMATS_MAP.at(type).begin());
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Creating new AssetDefinition with default Format {}", defaultFormat);
        #endif

        assetDefinitionJson[Constants::NAME] = Constants::ASSET_DEFINITION_DEFAULT_NAME;
        assetDefinitionJson[Constants::UUID] = Uuid::generateUuid();
        assetDefinitionJson[Constants::ASSET_TYPE] = Constants::getAssetTypeStringFromTypeEnum(type);
        assetDefinitionJson[Constants::ASSET_FORMAT] = defaultFormat;
        AssetDefinition* ad = createAssetDefinitionRuntime(assetDefinitionJson);
        mAssetDefinitions.push_back(ad);

        return ad;
    }

    SceneDefinition*
    ProjectDefinition::getStartupSceneDefinition
    ()
    {
        uint32_t startupScene = getStartupSceneUuid();
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Finding startup scene {}", startupScene);
        #endif
        return getSceneDefinitionByUuid(startupScene);
    }

    json
    ProjectDefinition::getJson
    ()
    {
        mJson[Constants::PROJECT_ASSET_ARRAY] = json::array();
        for (AssetDefinition* ad : getAssetDefinitionsVector())
        {
            mJson[Constants::PROJECT_ASSET_ARRAY].push_back(ad->getJson());
        }

        mJson[Constants::PROJECT_SCENE_ARRAY] = json::array();
        for (SceneDefinition* sd : getSceneDefinitionsVector())
        {
            mJson[Constants::PROJECT_SCENE_ARRAY].push_back(sd->getJson());
        }

        return mJson;
    }

    map<AssetType, vector<AssetDefinition*> >
    ProjectDefinition::getAssetDefinitionsMap
    ()
    {
        vector<AssetDefinition*> ads = getAssetDefinitionsVector();
        map<AssetType, vector<AssetDefinition*> > handlesMap;

        auto begins = begin(ads);
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
                vector<AssetDefinition*> typeVector;
                handlesMap.insert(
                            std::pair<AssetType, vector<AssetDefinition*>>
                            (currentType, typeVector)
                            );
            }
            handlesMap.at(currentType).push_back((*current));
            current++;
        }
        return handlesMap;
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

    void ProjectDefinition::deleteAssetDefinitions()
    {
        for (auto ad : mAssetDefinitions)
        {
            delete ad;
        }
        mAssetDefinitions.clear();
    }

    void ProjectDefinition::deleteSceneDefinitions()
    {
        for (auto sd : mSceneDefinitions)
        {
            delete sd;
        }
        mSceneDefinitions.clear();
    }

    long
    ProjectDefinition::getSceneDefinitionIndex
    (SceneDefinition* sDef)
    {
        vector<SceneDefinition*> defs = getSceneDefinitionsVector();
        auto it = std::find(defs.begin(), defs.end(), sDef);
        if (it == defs.end())
        {
            return -1;
        }
        else
        {
            return std::distance(defs.begin(), it);
        }
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionAtIndex
    (int index)
    {
        return getSceneDefinitionsVector().at(index);
    }

    long
    ProjectDefinition::getAssetDefinitionIndex
    (AssetType type, AssetDefinition* sDef)
    {
        vector<AssetDefinition*> defs = getAssetDefinitionsVector(type);
        auto it = std::find(defs.begin(), defs.end(), sDef);
        if (it == defs.end())
        {
            return -1;
        }
        else
        {
            return std::distance(defs.begin(), it);
        }
    }

    long
    ProjectDefinition::getAssetDefinitionIndex
    (AssetType type, uint32_t uuid)
    {
        vector<AssetDefinition*> defs = getAssetDefinitionsVector(type);
        for (int i = 0; i < defs.size(); i++)
        {
            if (defs.at(i)->getUuid() == uuid) return i;
        }
        return -1;
    }

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionAtIndex
    (AssetType type, int idx)
    {
        return getAssetDefinitionsVector(type).at(idx);
    }

    vector<string>
    ProjectDefinition::getAssetNamesVector
    (AssetType type)
    {
        vector<string> retval;
        auto assets = getAssetDefinitionsVector(type);
        for (auto asset : assets)
        {
            retval.push_back(asset->getName());
        }
        return retval;
    }

    void
    ProjectDefinition::regroupAssetDefinitions
    ()
    {
        mAssetDefinitionGroups.clear();
        for (auto typePair : Constants::DREAM_ASSET_TYPES_MAP)
        {
            mAssetDefinitionGroups.insert(
                pair<AssetType,vector<string> >(
                    typePair.first,
                    vector<string>()
                )
            );
        }

        for (auto ad : mAssetDefinitions)
        {
            auto group = ad->getGroup();
            auto& groups = mAssetDefinitionGroups[Constants::getAssetTypeEnumFromString(ad->getType())];
            if (find(groups.begin(),groups.end(), group) == groups.end())
            {
                groups.push_back(group);
            }
        }
    }

    ProjectDefinition*
    ProjectDefinition::createNewProjectDefinition
    (string name)
    {
        json j = json::object();

        j[Constants::NAME] = name;
        j[Constants::UUID] = Uuid::generateUuid();
        j[Constants::PROJECT_AUTHOR] = "";
        j[Constants::PROJECT_DESCRIPTION] = "";
        j[Constants::PROJECT_STARTUP_SCENE] = "";
        j[Constants::PROJECT_CAPTURE_JOYSTICK] = false;
        j[Constants::PROJECT_CAPTURE_MOUSE] = false;
        j[Constants::PROJECT_CAPTURE_KEYBOARD] = false;

        j[Constants::PROJECT_WINDOW_SIZE] = json::object();
        j[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_WIDTH] = Constants::PROJECT_DEFAULT_WINDOW_WIDTH;
        j[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_HEIGHT] = Constants::PROJECT_DEFAULT_WINDOW_HEIGHT;

        j[Constants::PROJECT_ASSET_ARRAY] = json::array();
        j[Constants::PROJECT_SCENE_ARRAY] = json::array();

        return new ProjectDefinition(j);
    }
}
