/*
 * SceneObjectRuntime.cpp
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

#include "SceneObjectRuntime.h"

#include <iostream>

#include "SceneObjectDefinition.h"

#include "../SceneRuntime.h"

#include "../../Components/Event.h"
#include "../../Components/Path/PathInstance.h"
#include "../../Components/Audio/AudioInstance.h"
#include "../../Components/Audio/AudioComponent.h"
#include "../../Components/Graphics/Model/ModelInstance.h"
#include "../../Components/Graphics/Model/ModelCache.h"
#include "../../Components/Graphics/Light/LightInstance.h"
#include "../../Components/Graphics/ParticleEmitter/ParticleEmitterInstance.h"
#include "../../Components/Physics/PhysicsObjectInstance.h"
#include "../../Components/Physics/PhysicsComponent.h"
#include "../../Components/IAssetDefinition.h"

#include "../../Components/Audio/AudioDefinition.h"

#include "../../Components/Graphics/Font/FontDefinition.h"
#include "../../Components/Graphics/Light/LightDefinition.h"
#include "../../Components/Graphics/Model/ModelDefinition.h"
#include "../../Components/Graphics/ParticleEmitter/ParticleEmitterDefinition.h"

#include "../../Components/Path/PathDefinition.h"
#include "../../Components/Physics/PhysicsObjectDefinition.h"

#include "../../Components/Scripting/ScriptDefinition.h"
#include "../../Components/Scripting/ScriptComponent.h"
#include "../../Components/Scripting/ScriptInstance.h"

#include "../../Project/Project.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Project/ProjectDefinition.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;
using glm::vec3;

namespace Dream
{
    SceneObjectRuntime::SceneObjectRuntime(
        SceneObjectDefinition* sd,
        SceneRuntime* sr
    ):  IRuntime(sd, sd->getName(), sd->getUuid()),
        mAudioInstance(nullptr),
        mLightInstance(nullptr),
        mParticleEmitterInstance(nullptr),
        mPathInstance(nullptr),
        mPhysicsObjectInstance(nullptr),
        mScriptInstance(nullptr),
        mModelInstance(nullptr),
        mCurrentTransform(new Transform3D()),
        mDefinedTransform(nullptr),
        mSceneRuntimeHandle(sr),
        mParentRuntimeHandle(nullptr),
        mHasFocus(false),
        mDeleted(false),
        mHidden(false),
        mAlwaysDraw(false),
        mFollowsCamera(false)
    {
        setLogClassName("SceneObjectRuntime");
        auto log = getLog();
        log->trace( "Constructing Object" );
    }

    SceneObjectRuntime::~SceneObjectRuntime
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );

        for (auto child : mChildRuntimes)
        {
            if (child != nullptr)
            {
                delete child;
            }
        }
        mChildRuntimes.clear();

        removeAudioInstance();
        removeLightInstance();
        removeModelInstance();
        removeParticleEmitterInstance();
        removePathInstance();
        removePhysicsObjectInstance();
        removeScriptInstance();

        if (mCurrentTransform != nullptr)
        {
            delete mCurrentTransform;
            mCurrentTransform = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeAudioInstance
    ()
    {
        if (mAudioInstance != nullptr)
        {
            delete mAudioInstance;
            mAudioInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removePathInstance
    ()
    {
        if (mPathInstance != nullptr)
        {
            delete mPathInstance;
            mPathInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeModelInstance
    ()
    {
        auto log = getLog();
        if (mModelInstance != nullptr)
        {
            log->info("Deleting ModelInstance for {}",getNameAndUuidString());
            mModelInstance->removeInstance(this);
        }
    }

    void
    SceneObjectRuntime::removeLightInstance
    ()
    {
        if (mLightInstance != nullptr)
        {
            delete mLightInstance;
            mLightInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeScriptInstance
    ()
    {
        if (mScriptInstance != nullptr)
        {
            mScriptInstance->removeInstance(this);
            mScriptInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removePhysicsObjectInstance
    ()
    {
        if (hasPhysicsObjectInstance())
        {
            auto physicsComp = mSceneRuntimeHandle
                ->getProjectRuntime()
                ->getPhysicsComponent();
           if (physicsComp != nullptr)
           {
                physicsComp->removePhysicsObjectInstance(getPhysicsObjectInstance());
           }
        }

        if (mPhysicsObjectInstance != nullptr)
        {
            delete mPhysicsObjectInstance;
            mPhysicsObjectInstance = nullptr;
        }
    }

    void SceneObjectRuntime::removeParticleEmitterInstance()
    {
        if (mParticleEmitterInstance != nullptr)
        {
            delete mParticleEmitterInstance;
            mParticleEmitterInstance = nullptr;
        }
    }

    PathInstance*
    SceneObjectRuntime::getPathInstance
    ()
    {
        return mPathInstance;
    }

    AudioInstance*
    SceneObjectRuntime::getAudioInstance
    ()
    {
        return mAudioInstance;
    }

    ModelInstance*
    SceneObjectRuntime::getModelInstance
    ()
    {
        return mModelInstance;
    }

    ScriptInstance*
    SceneObjectRuntime::getScriptInstance
    ()
    {
        return mScriptInstance;
    }

    LightInstance*
    SceneObjectRuntime::getLightInstance
    ()
    {
        return mLightInstance;
    }

    ParticleEmitterInstance*
    SceneObjectRuntime::getParticleEmitterInstance
    ()
    {
       return mParticleEmitterInstance;
    }

    IAssetInstance*
    SceneObjectRuntime::getAssetInstance
    (AssetType type)
    {
       switch (type)
       {
           case Dream::PATH:
               return getPathInstance();
           case Dream::AUDIO:
               return getAudioInstance();
           case Dream::LIGHT:
               return getLightInstance();
           case Dream::MODEL:
               return getModelInstance();
           case Dream::PHYSICS_OBJECT:
               return getPhysicsObjectInstance();
           case Dream::SCRIPT:
               return getScriptInstance();
           case Dream::PARTICLE_EMITTER:
               return getParticleEmitterInstance();
           default:
               break;
       }
       return nullptr;
    }

    bool
    SceneObjectRuntime::hasLightInstance
    ()
    {
        return mLightInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasModelInstance
    ()
    {
        return mModelInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasScriptInstance
    ()
    {
        return mScriptInstance != nullptr;
    }

    void
    SceneObjectRuntime::setAssetDefinitionsMap
    (map<AssetType,string> assetMap)
    {
        mAssetDefinitions = assetMap;
    }

    map<AssetType,string>
    SceneObjectRuntime::getAssetDefinitionsMap
    ()
    {
        return mAssetDefinitions;
    }

    bool SceneObjectRuntime::getAlwaysDraw() const
    {
        return mAlwaysDraw;
    }

    void SceneObjectRuntime::setAlwaysDraw(bool alwaysDraw)
    {
        mAlwaysDraw = alwaysDraw;
    }

    PhysicsObjectInstance*
    SceneObjectRuntime::getPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance;
    }

    TransformType
    SceneObjectRuntime::getTransformType
    ()
    const
    {
        return mDefinedTransform->getTransformType();
    }

    void
    SceneObjectRuntime::setTransformType
    (TransformType transformType)
    {
        mDefinedTransform->setTransformType(transformType);
        mCurrentTransform->setTransformType(transformType);
    }

    Transform3D*
    SceneObjectRuntime::getCurrentTransform
    ()
    {
        if (mDefinedTransform->getTransformType() == TransformType::Offset)
        {
            mCurrentTransform->setOffsetFrom(getParentRuntime()->getCurrentTransform(), mDefinedTransform);
            return mCurrentTransform;
        }
        return mCurrentTransform;
    }

    Transform3D*
    SceneObjectRuntime::getDefinedTransform
    ()
    {
        return mDefinedTransform;
    }

    void
    SceneObjectRuntime::initTransform()
    {
        auto log = getLog();
        mDefinedTransform = dynamic_cast<SceneObjectDefinition*>(mDefinition)->getTransform();
        if (mDefinedTransform->isTypeOffset())
        {
            log->trace("Inheriting Offset Transform for {}",getNameAndUuidString());
            auto parent=getParentRuntime()->getCurrentTransform();
            if (parent != nullptr)
            {
                mCurrentTransform->setOffsetFrom(parent, mDefinedTransform);
            }
            else
            {
                log->error("{} has no parent to inherit transform from",getNameAndUuidString());
            }
        }
        else
        {
            mCurrentTransform->setTranslation(mDefinedTransform->getTranslation());
            mCurrentTransform->setOrientation(mDefinedTransform->getOrientation());
            mCurrentTransform->setScale(mDefinedTransform->getScale());
        }
    }

    bool
    SceneObjectRuntime::hasFocus
    ()
    const
    {
        return mHasFocus;
    }

    void
    SceneObjectRuntime::setHasFocus
    (bool focus)
    {
        mHasFocus = focus;
    }

    bool
    SceneObjectRuntime::hasEvents
    ()
    const
    {
        return mEventQueue.size() > 0;
    }

    void
    SceneObjectRuntime::addEvent
    (Event event)
    {
        auto log = getLog();
        log->trace
        (
            "Event posted from {} to {}",
            event.getSender()->getNameAndUuidString(),
            getNameAndUuidString()
        );
        mEventQueue.push_back(event);
    }

    const vector<Event>&
    SceneObjectRuntime::getEventQueue
    ()
    const
    {
        return mEventQueue;
    }

    void SceneObjectRuntime::clearEventQueue()
    {
        auto log = getLog();
        log->trace("Clearing event queue");
        mEventQueue.clear();
    }

    void SceneObjectRuntime::collectGarbage()
    {
        auto log = getLog();
        log->trace("Collecting Garbage {}" ,getNameAndUuidString());

        vector<SceneObjectRuntime*> toDelete;

        for (auto child : mChildRuntimes)
        {
           if (child->getDeleted())
           {
                toDelete.push_back(child);
           }
        }

        for (auto child : toDelete)
        {
            log->trace("Deleting child {}",child->getNameAndUuidString());
            mChildRuntimes.erase
            (
                find
                (
                    mChildRuntimes.begin(),
                    mChildRuntimes.end(),
                    child
                )
            );
            delete child;
        }
    }

    bool
    SceneObjectRuntime::hasPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasPathInstance
    ()
    {
        return mPathInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasAudioInstance
    ()
    {
        return mAudioInstance != nullptr;
    }

    bool
    SceneObjectRuntime::createAssetInstances
    ()
    {
        auto log = getLog();
        for (auto assetPair : mAssetDefinitions)
        {
            IAssetDefinition* def = getAssetDefinitionByUuid(assetPair.second);
            log->trace("Creating {}",def->getNameAndUuidString());
            bool result = false;
            switch (assetPair.first)
            {
                case AssetType::AUDIO:
                    result = createAudioInstance(dynamic_cast<AudioDefinition*>(def));
                    break;
                case AssetType::LIGHT:
                    result = createLightInstance(dynamic_cast<LightDefinition*>(def));
                    break;
                case AssetType::MODEL:
                    result = createModelInstance(dynamic_cast<ModelDefinition*>(def));
                    break;
                case AssetType::PARTICLE_EMITTER:
                    result = createParticleEmitterInstance(dynamic_cast<ParticleEmitterDefinition*>(def));
                    break;
                case AssetType::PATH:
                    result = createPathInstance(dynamic_cast<PathDefinition*>(def));
                    break;
                case AssetType::PHYSICS_OBJECT:
                    result = createPhysicsObjectInstance(dynamic_cast<PhysicsObjectDefinition*>(def));
                    break;
                case AssetType::SCRIPT:
                    result = createScriptInstance(dynamic_cast<ScriptDefinition*>(def));
                    break;
                default:
                    return false;
            }
            if (!result)
            {
                return false;
            }
        }
        return true;
    }

    IAssetDefinition*
    SceneObjectRuntime::getAssetDefinitionByUuid
    (string uuid)
    {
        auto log = getLog();
        auto project = mSceneRuntimeHandle->getProjectRuntime()->getProject();
        if (project == nullptr)
        {
            log->error("Project is not found");
            return nullptr;
        }
        auto assetDefinition = project->getProjectDefinition()->getAssetDefinitionByUuid(uuid);
        if (assetDefinition == nullptr)
        {
            log->error("AssetDefinition not found");
        }
        return assetDefinition;
    }

    string
    SceneObjectRuntime::getProjectPath
    ()
    {
        auto log = getLog();
        auto project = mSceneRuntimeHandle->getProjectRuntime()->getProject();

        if (project == nullptr)
        {
            log->error("Project is null");
            return "";
        }

        return project->getProjectPath();
    }


    bool
    SceneObjectRuntime::replaceAssetUuid
    (AssetType type, string uuid)
    {
        auto log = getLog();
        log->info("REPLACING asset instance from uuid {}", uuid);
        auto project = mSceneRuntimeHandle->getProjectRuntime()->getProject();
        if (project == nullptr)
        {
            log->error("Project is not found");
            return false;
        }
        auto def = project->getProjectDefinition()->getAssetDefinitionByUuid(uuid);
        if (def == nullptr)
        {
            log->error("AssetDefinition not found");
        }
        switch (type)
        {
            case AssetType::AUDIO:
                return createAudioInstance(dynamic_cast<AudioDefinition*>(def));
            case AssetType::LIGHT:
                return createLightInstance(dynamic_cast<LightDefinition*>(def));
            case AssetType::MODEL:
                return createModelInstance(dynamic_cast<ModelDefinition*>(def));
            case AssetType::PARTICLE_EMITTER:
                return createParticleEmitterInstance(dynamic_cast<ParticleEmitterDefinition*>(def));
            case AssetType::PATH:
                return createPathInstance(dynamic_cast<PathDefinition*>(def));
            case AssetType::PHYSICS_OBJECT:
                return createPhysicsObjectInstance(dynamic_cast<PhysicsObjectDefinition*>(def));
            case AssetType::SCRIPT:
                return createScriptInstance(dynamic_cast<ScriptDefinition*>(def));
            default:
                return false;
        }
    }

    bool
    SceneObjectRuntime::createPhysicsObjectInstance
    (PhysicsObjectDefinition* definition)
    {
        removePhysicsObjectInstance();
        auto log = getLog();
        log->trace( "Creating Physics Object Asset Instance." );
        mPhysicsObjectInstance = new PhysicsObjectInstance(
            definition,
            mSceneRuntimeHandle->getProjectRuntime()->getPhysicsComponent(),
            this
        );
        return mPhysicsObjectInstance->load(mSceneRuntimeHandle->getProjectRuntime()->getProjectPath());
    }

    bool
        SceneObjectRuntime::createParticleEmitterInstance(ParticleEmitterDefinition *)
    {
        return false;
    }

    bool
    SceneObjectRuntime::createPathInstance
    (PathDefinition* definition)
    {
        auto log = getLog();
        log->trace( "Creating Path asset instance." );
        removePathInstance();
        mPathInstance = new PathInstance(definition,this);
        return mPathInstance->load(mSceneRuntimeHandle->getProjectRuntime()->getProjectPath());
    }

    bool
    SceneObjectRuntime::createAudioInstance
    (AudioDefinition* definition)
    {
        auto log = getLog();
        auto audioComp = mSceneRuntimeHandle->getProjectRuntime()->getAudioComponent();
        if (audioComp != nullptr)
        {
            removeAudioInstance();
            log->trace( "Creating Audio asset instance." );
            mAudioInstance = audioComp->newAudioInstance(definition,this);
            return mAudioInstance->load(mSceneRuntimeHandle->getProjectRuntime()->getProjectPath());
        }
        else
        {
            log->error("Cannot create AudioInstance. AudioComponent is nullptr");
        }
        return false;
    }

    bool
    SceneObjectRuntime::createModelInstance
    (ModelDefinition* definition)
    {
        auto log = getLog();
        removeModelInstance();
        log->info("Creating Model asset instance.");
        auto cache = mSceneRuntimeHandle->getProjectRuntime()->getModelCache();
        if (cache != nullptr)
        {
            mModelInstance = dynamic_cast<ModelInstance*>(cache->getInstance(definition));
            if (mModelInstance != nullptr)
            {
                mModelInstance->addInstance(this);
            }
            else
            {
                log->error("Error getting model instance, cache returned nullptr");
                return false;
            }
        }
        return mModelInstance != nullptr;
    }

    bool
    SceneObjectRuntime::createScriptInstance
    (ScriptDefinition* definition)
    {
        auto log = getLog();
        removeScriptInstance();
        log->trace("Creating Script asset instance.");
        auto scriptCache = (mSceneRuntimeHandle->getProjectRuntime()->getScriptCache());
        if (scriptCache)
        {
            mScriptInstance = dynamic_cast<ScriptInstance*>(scriptCache->getInstance(definition));
            if (mScriptInstance != nullptr)
            {
                mScriptInstance->addInstance(this);
                return true;
            }
            else
            {
                log->error("Error getting script instance, cache returned nullptr");
                return false;
            }
        }
        log->error("Script cache is null");
        return false;
    }

    bool
    SceneObjectRuntime::createLightInstance
    (LightDefinition* definition)
    {
        auto log = getLog();
        removeLightInstance();
        log->trace( "Creating Light Asset instance." );
        mLightInstance = new LightInstance(definition,this);
        return mLightInstance->load(mSceneRuntimeHandle->getProjectRuntime()->getProjectPath());
    }

    bool
    SceneObjectRuntime::applyToAll
    (function<bool(SceneObjectRuntime*)> funk)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(bool) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        bool retval = funk(this);

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if (*it)
            {
                retval = retval || (*it)->applyToAll(funk);
            }
        }
        return retval;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::applyToAll
    (function<SceneObjectRuntime*(SceneObjectRuntime*)> funk)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(void*) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        SceneObjectRuntime* retval = funk(this);

        if (retval != nullptr)
        {
            return retval;
        }

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it) != nullptr)
            {
                retval = (*it)->applyToAll(funk);
                if (retval != nullptr)
                {
                    return retval;
                }
            }
        }
        return nullptr;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getChildRuntimeByUuid
    (string uuid)
    {
        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if (*it != nullptr)
            {
                if ((*it)->hasUuid(uuid))

                {
                    return *it;
                }
            }
        }
        return nullptr;
    }

    bool
    SceneObjectRuntime::isParentOf
    (SceneObjectRuntime*  child)
    {
        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it) == child)
            {
                return true;
            }
        }
        return false;
    }

    void
    SceneObjectRuntime::setParentRuntime
    (SceneObjectRuntime* parent)
    {
        mParentRuntimeHandle = parent;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getParentRuntime
    ()
    {
        return mParentRuntimeHandle;
    }

    SceneRuntime*
    SceneObjectRuntime::getSceneRuntime
    ()
    {
        return mSceneRuntimeHandle;
    }

    SceneObjectDefinition*
    SceneObjectRuntime::getSceneObjectDefinition()
    {
       return dynamic_cast<SceneObjectDefinition*>(getDefinition());
    }

    bool
    SceneObjectRuntime::useDefinition
    ()
    {
        auto def = dynamic_cast<SceneObjectDefinition*>(mDefinition);
        auto log = getLog();
        log->trace( "Using Definition {}", def->getNameAndUuidString());
        setName(def->getName());
        setUuid(def->getUuid());
        setFollowsCamera(def->getFollowsCamera());
        setAssetDefinitionsMap(def->getAssetDefinitionsMap());
        setHasFocus(def->getHasFocus());
        setHidden(def->getHidden());
        initTransform();
        if (!createAssetInstances()) return false;
        if( !loadChildrenFromDefinition(def)) return false;
        return true;
    }

    bool
    SceneObjectRuntime::loadChildrenFromDefinition
    (SceneObjectDefinition* definition)
    {
        vector<SceneObjectDefinition*> definitions = definition->getChildDefinitionsList();
        for (auto it = begin(definitions); it != end(definitions); it++)
        {
            createChildRuntime(*it);
        }
        return true;
    }

    bool
    SceneObjectRuntime::getFollowsCamera
    () const
    {
        return mFollowsCamera;
    }

    void
    SceneObjectRuntime::setFollowsCamera
    (bool followsCamera)
    {
        mFollowsCamera = followsCamera;
    }

    bool SceneObjectRuntime::getDeleted() const
    {
        return mDeleted;
    }

    void SceneObjectRuntime::setDeleted(bool deleted)
    {
        mDeleted = deleted;
    }

    bool SceneObjectRuntime::getHidden() const
    {
        return mHidden;
    }

    void SceneObjectRuntime::setHidden(bool hidden)
    {
        mHidden = hidden;
    }

    void
    SceneObjectRuntime::removeChildRuntime
    (SceneObjectRuntime* child)
    {
        child->setDeleted(true);
    }

    void
    SceneObjectRuntime::addChildRuntime
    (SceneObjectRuntime* rt)
    {
        mChildRuntimes.push_back(rt);
    }

    SceneObjectRuntime*
    SceneObjectRuntime::createChildRuntime
    (SceneObjectDefinition* def)
    {
        auto log = getLog();
        SceneObjectRuntime* child = new SceneObjectRuntime(def, mSceneRuntimeHandle);
        child->setParentRuntime(this);
        if (!child->useDefinition())
        {
            log->error("Error creating child runtime");
            delete child;
            return nullptr;
        }
        addChildRuntime(child);
        return child;
    }
}
