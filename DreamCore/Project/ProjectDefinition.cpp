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
#include "Scene/SceneDefinition.h"
#include "Components/AssetDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Path/PathDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Sprite/SpriteDefinition.h"
#include "Components/Graphics/Light/LightDefinition.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Components/Physics/PhysicsObjectDefinition.h"
#include "Components/Script/ScriptDefinition.h"
#include "Common/Uuid.h"
#include "Common/Logger.h"
#include "Common/Constants.h"

using std::pair;


namespace octronic::dream
{
    ProjectDefinition::ProjectDefinition
    (const json& data)
        : Definition("ProjectDefinition",data)

    {
        LOG_TRACE("ProjectDefinition: Constructing {}", getNameAndUuidString());
    }

    ProjectDefinition::~ProjectDefinition
    ()
    {
        LOG_TRACE("ProjectDefinition: Destructing {}", getNameAndUuidString());
        deleteSceneDefinitions();
        deleteAssetDefinitions();
    }

    void
    ProjectDefinition::setStartupSceneUuid
    (UuidType sceneUuid)
    {
        if (dreamTryLock())
        {
            dreamLock();
            mJson[Constants::PROJECT_STARTUP_SCENE] = sceneUuid;
        }
        dreamElseLockFailed
    }

    UuidType
    ProjectDefinition::getStartupSceneUuid()
    {
        if (dreamTryLock())
        {
            dreamLock();
            if (!mJson[Constants::PROJECT_STARTUP_SCENE].is_number())
            {
                mJson[Constants::PROJECT_STARTUP_SCENE] = 0;
            }
            return mJson[Constants::PROJECT_STARTUP_SCENE];
        }
        dreamElseLockFailed
    }

    void
    ProjectDefinition::setDescription
    (const string &description)
    {
        if (dreamTryLock())
        {
            dreamLock();
            mJson[Constants::PROJECT_DESCRIPTION] = description;
        }
        dreamElseLockFailed
    }

    string
    ProjectDefinition::getDescription
    (void)
    {
        if (dreamTryLock())
        {
            dreamLock();
            return mJson[Constants::PROJECT_DESCRIPTION];
        }
        dreamElseLockFailed
    }

    void
    ProjectDefinition::setAuthor
    (const string &author)
    {
        if (dreamTryLock())
        {
            dreamLock();
            mJson[Constants::PROJECT_AUTHOR] = author;
        }
        dreamElseLockFailed
    }

    string
    ProjectDefinition::getAuthor
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            return mJson[Constants::PROJECT_AUTHOR];
        }
        dreamElseLockFailed
    }

    void
    ProjectDefinition::loadChildDefinitions
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            loadAssetDefinitions();
            loadSceneDefinitions();
        }
        dreamElseLockFailed
    }

    map<AssetType, vector<string> > &
    ProjectDefinition::getAssetDefinitionGroups
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            return mAssetDefinitionGroups;
        }
        dreamElseLockFailed
    }



    void
    ProjectDefinition::removeAssetDefinition
    (AssetDefinition* assetDefinition)
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_DEBUG(
                        "ProjectDefinition: Removing AssetDefinition {} from {}",
                        assetDefinition->getNameAndUuidString(),
                        getNameAndUuidString()
                        );
            auto iter = begin(mAssetDefinitions);
            auto endPos = end(mAssetDefinitions);
            while (iter != endPos)
            {
                if ((*iter) == assetDefinition)
                {
                    LOG_DEBUG(
                                "ProjectDefinition: Found AssetDefinition to {} remove from {}",
                                assetDefinition->getNameAndUuidString(),
                                getNameAndUuidString()
                                );
                    mAssetDefinitions.erase(iter);
                    return;
                }
                iter++;
            }
        }
        dreamElseLockFailed
    }

    size_t
    ProjectDefinition::countAssetDefinitions
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            return mAssetDefinitions.size();
        }
        dreamElseLockFailed
    }

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionByUuid
    (UuidType uuid)
    {
        if(dreamTryLock()){
            dreamLock();

            for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
            {
                if ((*it)->hasUuid(uuid))
                {
                    return (*it);
                }
            }
            return nullptr;
        }
        dreamElseLockFailed
    }

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionByName
    (const string& name)
    {
        if(dreamTryLock()){
            dreamLock();

            for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
            {
                if ((*it)->hasName(name))
                {
                    return (*it);
                }
            }
            return nullptr;
        }
        dreamElseLockFailed
    }

    void
    ProjectDefinition::addAssetDefinition
    (AssetDefinition* def)
    {
        if(dreamTryLock()){
            dreamLock();

            mAssetDefinitions.push_back(def);
            regroupAssetDefinitions();
        }
        dreamElseLockFailed
    }



    size_t
    ProjectDefinition::countScenesDefinitions
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            return mSceneDefinitions.size();
        }
        dreamElseLockFailed
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionByName
    (const string &name)
    {
        if(dreamTryLock()){
            dreamLock();

            for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
            {
                if ((*it)->hasName(name))
                {
                    return (*it);
                }
            }
            return nullptr;
        }
        dreamElseLockFailed
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionByUuid
    (UuidType uuid)
    {
        if(dreamTryLock()){
            dreamLock();

            for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
            {
                if ((*it)->hasUuid(uuid))
                {
                    return (*it);
                }
            }
            return nullptr;
        }
        dreamElseLockFailed
    }

    vector<SceneDefinition*>
    ProjectDefinition::getSceneDefinitionsVector
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            vector<SceneDefinition*> list;
            for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
            {
                list.push_back((*it));
            }
            return list;
        }
        dreamElseLockFailed
    }

    void
    ProjectDefinition::removeSceneDefinition
    (SceneDefinition* sceneDef)
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_DEBUG(
                        "ProjectDefinition: Removing SceneDefinition {} from {}",
                        sceneDef->getNameAndUuidString(),
                        getNameAndUuidString()
                        );

            auto iter = begin(mSceneDefinitions);
            auto endPos = end(mSceneDefinitions);
            while (iter != endPos)
            {
                if ((*iter) == sceneDef)
                {
                    LOG_DEBUG(
                                "ProjectDefinition: Found scene to {} remove from {}",
                                sceneDef->getNameAndUuidString(),
                                getNameAndUuidString()
                                );
                    delete (*iter);
                    mSceneDefinitions.erase(iter);
                    return;
                }
                iter++;
            }
        }
        dreamElseLockFailed
    }

    vector<AssetDefinition*>
    ProjectDefinition::getAssetDefinitionsVector
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            vector<AssetDefinition*> definitionsList;
            for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
            {
                definitionsList.push_back((*it));
            }
            return definitionsList;
        }
        dreamElseLockFailed
    }

    vector<AssetDefinition*>
    ProjectDefinition::getAssetDefinitionsVector
    (AssetType type)
    {
        if(dreamTryLock()){
            dreamLock();

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
        dreamElseLockFailed
    }


    SceneDefinition*
    ProjectDefinition::createNewSceneDefinition
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            json scene;
            scene[Constants::UUID] = Uuid::generateUuid();
            scene[Constants::NAME] = Constants::SCENE_DEFAULT_NAME;
            Transform camTransform;
            scene[Constants::SCENE_CAMERA_TRANSFORM] = camTransform.getJson();
            auto sd = new SceneDefinition(this, scene);
            sd->createNewRootEntityDefinition();
            mSceneDefinitions.push_back(sd);
            return sd;
        }
        dreamElseLockFailed
    }

    AssetDefinition*
    ProjectDefinition::createNewAssetDefinition
    (AssetType type)
    {
        if(dreamTryLock()){
            dreamLock();

            json assetDefinitionJson;

            string defaultFormat = (*Constants::DREAM_ASSET_FORMATS_MAP.at(type).begin());
            LOG_DEBUG("ProjectDefinition: Creating new AssetDefinition with default Format {}", defaultFormat);

            assetDefinitionJson[Constants::NAME] = Constants::ASSET_DEFINITION_DEFAULT_NAME;
            assetDefinitionJson[Constants::UUID] = Uuid::generateUuid();
            assetDefinitionJson[Constants::ASSET_TYPE] = Constants::getAssetTypeStringFromTypeEnum(type);
            assetDefinitionJson[Constants::ASSET_FORMAT] = defaultFormat;
            AssetDefinition* ad = createAssetDefinition(assetDefinitionJson);
            mAssetDefinitions.push_back(ad);
            return ad;
        }
        dreamElseLockFailed
    }

    SceneDefinition*
    ProjectDefinition::getStartupSceneDefinition
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            UuidType startupScene = getStartupSceneUuid();
            LOG_DEBUG("ProjectDefinition: Finding startup scene {}", startupScene);
            return getSceneDefinitionByUuid(startupScene);
        }
        dreamElseLockFailed
    }

    json
    ProjectDefinition::getJson
    ()
    {
        if(dreamTryLock()){
            dreamLock();

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
        dreamElseLockFailed
    }

    map<AssetType, vector<AssetDefinition*> >
    ProjectDefinition::getAssetDefinitionsMap
    ()
    {
        if(dreamTryLock()){
            dreamLock();

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
        dreamElseLockFailed
    }

    void ProjectDefinition::deleteAssetDefinitions()
    {
        if(dreamTryLock()){
            dreamLock();

            for (auto ad : mAssetDefinitions)
            {
                delete ad;
            }
            mAssetDefinitions.clear();
        }
        dreamElseLockFailed
    }

    void ProjectDefinition::deleteSceneDefinitions()
    {
        if(dreamTryLock()){
            dreamLock();

            for (auto sd : mSceneDefinitions)
            {
                delete sd;
            }
            mSceneDefinitions.clear();
        }
        dreamElseLockFailed
    }

    long
    ProjectDefinition::getSceneDefinitionIndex
    (SceneDefinition* sDef)
    {
        if(dreamTryLock()){
            dreamLock();

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
        dreamElseLockFailed
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionAtIndex
    (int index)
    {
        if(dreamTryLock()){
            dreamLock();

            return getSceneDefinitionsVector().at(index);
        }
        dreamElseLockFailed
    }

    long
    ProjectDefinition::getAssetDefinitionIndex
    (AssetType type, AssetDefinition* sDef)
    {
        if(dreamTryLock()){
            dreamLock();

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
        dreamElseLockFailed
    }

    long
    ProjectDefinition::getAssetDefinitionIndex
    (AssetType type, UuidType uuid)
    {
        if(dreamTryLock()){
            dreamLock();

            vector<AssetDefinition*> defs = getAssetDefinitionsVector(type);
            for (int i = 0; i < defs.size(); i++)
            {
                if (defs.at(i)->getUuid() == uuid) return i;
            }
            return -1;
        }
        dreamElseLockFailed
    }

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionAtIndex
    (AssetType type, int idx)
    {
        if(dreamTryLock()){
            dreamLock();

            return getAssetDefinitionsVector(type).at(idx);
        }
        dreamElseLockFailed
    }

    vector<string>
    ProjectDefinition::getAssetNamesVector
    (AssetType type)
    {
        if(dreamTryLock()){
            dreamLock();

            vector<string> retval;
            auto assets = getAssetDefinitionsVector(type);
            for (auto asset : assets)
            {
                retval.push_back(asset->getName());
            }
            return retval;
        }
        dreamElseLockFailed
    }



    // Private =================================================================

    void
    ProjectDefinition::loadSceneDefinitions
    ()
    {
        LOG_DEBUG("ProjectDefinition: Loading ScenesDefinitions from JSON");

        for (const json& it : mJson[Constants::PROJECT_SCENE_ARRAY])
        {
            loadSingleSceneDefinition(it);
        }
    }

    void
    ProjectDefinition::loadAssetDefinitions
    ()
    {
        LOG_DEBUG("ProjectDefinition: Loading AssetDefinitions from JSON");

        for (const json& it : mJson[Constants::PROJECT_ASSET_ARRAY])
        {
            loadSingleAssetDefinition(it);
        }
        regroupAssetDefinitions();
    }

    void
    ProjectDefinition::loadSingleAssetDefinition
    (const json &assetDefinitionJs)
    {

        AssetDefinition* newDef = createAssetDefinition(assetDefinitionJs);
        if (newDef != nullptr)
        {
            mAssetDefinitions.push_back(newDef);
        }
    }

    void
    ProjectDefinition::loadSingleSceneDefinition
    (const json &scene)
    {

        auto so = new SceneDefinition(this, scene);
        so->loadRootEntityDefinition();
        mSceneDefinitions.push_back(so);
    }

    void
    ProjectDefinition::regroupAssetDefinitions
    ()
    {
        mAssetDefinitionGroups.clear();
        for (const auto& typePair : Constants::DREAM_ASSET_TYPES_MAP)
        {
            mAssetDefinitionGroups.insert(
                        pair<AssetType,vector<string> >(typePair.first,vector<string>()));
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

    AssetDefinition*
    ProjectDefinition::createAssetDefinition
    (const json &assetDefinitionJs)
    {

        AssetType type = Constants::getAssetTypeEnumFromString(assetDefinitionJs[Constants::ASSET_TYPE]);

        switch (type)
        {
            case ASSET_TYPE_ENUM_ANIMATION:
                return new AnimationDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_AUDIO:
                return new AudioDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_FONT:
                return new FontDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_LIGHT:
                return new LightDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_MATERIAL:
                return new MaterialDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_MODEL:
                return new ModelDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_PATH:
                return new PathDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
                return new PhysicsObjectDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_SCRIPT:
                return new ScriptDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_SHADER:
                return new ShaderDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_TEXTURE:
                return new TextureDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_SPRITE:
                return new SpriteDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_NONE:
                LOG_ERROR("ProjectDefinition: Unable to create Asset Definition. Unknown Type");
                break;
        }
        return nullptr;
    }

    // Static ==================================================================

    ProjectDefinition*
    ProjectDefinition::createNewProjectDefinition
    (const string &name)
    {
        json j = json::object();
        j[Constants::NAME] = name;
        j[Constants::UUID] = Uuid::generateUuid();
        j[Constants::PROJECT_AUTHOR] = "";
        j[Constants::PROJECT_DESCRIPTION] = "";
        j[Constants::PROJECT_STARTUP_SCENE] = "";
        j[Constants::PROJECT_ASSET_ARRAY] = json::array();
        j[Constants::PROJECT_SCENE_ARRAY] = json::array();
        return new ProjectDefinition(j);
    }
}
