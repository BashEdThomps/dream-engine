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
#include "../../Components/Scripting/ScriptInstance.h"
#include "../../Components/Scripting/Lua/LuaComponent.h"
#include "../../Components/Scripting/Lua/LuaScriptInstance.h"

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
        mTransform(nullptr),
        mModelInstance(nullptr),
        mSceneRuntimeHandle(sr),
        mParentRuntimeHandle(nullptr),
        mLoaded(false),
        mHasFocus(false),
        mDeleted(false),
        mHidden(false),
        mAlwaysDraw(false),
        mFollowsCamera(false)
    {
        setLogClassName("SceneObjectRuntime");
        auto log = getLog();
        log->trace( "Constructing Object" );
        mTransform = new Transform3D();
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

        if (mTransform != nullptr)
        {
            delete mTransform;
            mTransform = nullptr;
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
        if (hasScriptInstance())
        {
            auto scriptComp =
            mSceneRuntimeHandle
                ->getProjectRuntime()
                ->getScriptComponent();
            if(scriptComp != nullptr)
            {
                scriptComp->removeFromScriptMap(this);
            }
        }

        if (mScriptInstance != nullptr)
        {
            delete mScriptInstance;
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

    void
    SceneObjectRuntime::resetTransform
    ()
    {
        resetTranslation();
        resetRotation();
        resetScale();
    }

    void
    SceneObjectRuntime::resetTranslation
    ()
    {
        setTranslation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObjectRuntime::resetRotation
    ()
    {
        setRotation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObjectRuntime::resetScale
    ()
    {
        setScale(1.0f, 1.0f, 1.0f);
    }

    void
    SceneObjectRuntime::setTranslation
    (vec3 translation)
    {
        mTransform->setTranslation(translation);
    }

    void
    SceneObjectRuntime::setRotation
    (vec3 rotation)
    {
        mTransform->setRotation(rotation);
    }

    void
    SceneObjectRuntime::setScale
    (vec3 scale)
    {
        mTransform->setScale(scale);
    }

    void
    SceneObjectRuntime::setTranslation
    (float x, float y, float z)
    {
        mTransform->setTranslation(x,y,z);
    }

    void
    SceneObjectRuntime::setRotation
    (float x, float y, float z)
    {
        mTransform->setRotation(x,y,z);
    }

    void
    SceneObjectRuntime::setScale
    (float x, float y, float z)
    {
        mTransform->setScale(x,y,z);
    }

    vec3
    SceneObjectRuntime::getRotation
    ()
    {
        return getTransform()->getRotation();
    }

    vec3
    SceneObjectRuntime::getScale
    ()
    {
        return getTransform()->getScale();
    }

    vec3
    SceneObjectRuntime::getTranslation
    ()
    {
        return getTransform()->getTranslation();
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

    string
    SceneObjectRuntime::getTransformType
    ()
    const
    {
        return mTransform->getTransformType();
    }

    void
    SceneObjectRuntime::setTransformType
    (string transformType)
    {
        mTransform->setTransformType(transformType);
    }

    Transform3D*
    SceneObjectRuntime::getTransform
    ()
    {
        return mTransform;
    }

    void
    SceneObjectRuntime::initialTransform()
    {
        auto log = getLog();
        Transform3D* initial = dynamic_cast<SceneObjectDefinition*>(mDefinition)->getTransform();
        if (initial->isTypeOffset())
        {
            log->debug("Inheriting Offset Transform for {}",getNameAndUuidString());
            mTransform->offsetFrom(getParentRuntime()->getTransform(), initial);
        }
        else
        {
            mTransform->setTranslation(initial->getTranslation());
            mTransform->setOrientation(initial->getOrientation());
            mTransform->setScale(initial->getScale());
        }
    }

    void
    SceneObjectRuntime::setTransform
    (Transform3D* transform)
    {
        mTransform = transform;
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
    SceneObjectRuntime::getLoadedFlag
    ()
    const
    {
        return mLoaded;
    }

    void
    SceneObjectRuntime::setLoadedFlag
    (bool loaded)
    {
        mLoaded = loaded;
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
        log->debug
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
        log->debug("Clearing event queue");
        mEventQueue.clear();
    }

    void SceneObjectRuntime::collectGarbage()
    {
        auto log = getLog();
        log->debug("Collecting Garbage {}" ,getNameAndUuidString());

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
            log->debug("Deleting child {}",child->getNameAndUuidString());
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
            log->debug("Creating {}",def->getNameAndUuidString());
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
        log->debug( "Creating Physics Object Asset Instance." );
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
        log->debug( "Creating Path asset instance." );
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
            log->debug( "Creating Audio asset instance." );
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
        }
        return mModelInstance != nullptr;
    }

    bool
    SceneObjectRuntime::createScriptInstance
    (ScriptDefinition* definition)
    {
        auto log = getLog();
        removeScriptInstance();
        log->debug("Creating Script asset instance.");
        mScriptInstance = new LuaScriptInstance(definition, this);
        bool result = mScriptInstance->load(mSceneRuntimeHandle->getProjectRuntime()->getProjectPath());
        if (!result)
        {
            log->error("Error loading script");
            return false;
        }
        auto scriptComp = mSceneRuntimeHandle->getProjectRuntime()->getScriptComponent();
        if (scriptComp != nullptr)
        {
            scriptComp->addToScriptMap(this,mScriptInstance);
            return true;
        }
        log->error("Script component is null");
        return false;
    }

    bool
    SceneObjectRuntime::createLightInstance
    (LightDefinition* definition)
    {
        auto log = getLog();
        removeLightInstance();
        log->debug( "Creating Light Asset instance." );
        mLightInstance = new LightInstance(
            definition,
            this
        );
        return mLightInstance->load(mSceneRuntimeHandle->getProjectRuntime()->getProjectPath());
    }

    quat
    SceneObjectRuntime::getOrientation
    ()
    {
       return mTransform->getOrientation();
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
        log->debug( "Using Definition {}", def->getNameAndUuidString());
        setName(def->getName());
        setUuid(def->getUuid());
        setFollowsCamera(def->getFollowsCamera());
        setAssetDefinitionsMap(def->getAssetDefinitionsMap());
        setHasFocus(def->getHasFocus());
        setHidden(def->getHidden());
        initialTransform();
        if (!createAssetInstances()) return false;
        if( !loadChildrenFromDefinition(def)) return false;
        setLoadedFlag(true);
        return true;
    }

    bool
    SceneObjectRuntime::loadChildrenFromDefinition
    (SceneObjectDefinition* definition)
    {
        auto log = getLog();

        vector<SceneObjectDefinition*> definitions = definition->getChildDefinitionsList();

        for (auto it = begin(definitions); it != end(definitions); it++)
        {
            SceneObjectRuntime* child = new SceneObjectRuntime(*it, mSceneRuntimeHandle);
            child->setParentRuntime(this);
            if (!child->useDefinition())
            {
                log->error("Error creating child runtime");
                return false;
            }
            mChildRuntimes.push_back(child);
        }
        return true;
    }

    bool
    SceneObjectRuntime::followsCamera
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

    void
    SceneObjectRuntime::drive
    (float leftStickX, float leftStickY)
    {
        auto log = getLog();
        log->trace("Walk: ({},{}) ",leftStickX,leftStickY);

        if (leftStickX == 0.0f && leftStickY == 0.0f)
        {
            return;
        }

        // Translate to current
        mat4 mtx;
        mtx = glm::translate(mat4(1), mTransform->getTranslation());
         // Rotate to current
        mat4 rotMat = mat4_cast(mTransform->getOrientation());
        mtx = mtx * rotMat;

        // Rotate to new
        quat q;
        quat newRotation = glm::rotate(q,leftStickX,vec3(0,1,0));
        mat4 newRotMat = mat4_cast(newRotation);
        mtx = mtx * newRotMat;

        // Translate to new
        mtx = glm::translate(mtx,vec3(0,0,-leftStickY));
        mTransform->setOrientation(quat(mtx));
        mTransform->setTranslation(glm::vec3(mtx[3]));

        if (mPhysicsObjectInstance != nullptr)
        {
            auto rb = mPhysicsObjectInstance->getRigidBody();
            if (rb!=nullptr)
            {
                rb->setCenterOfMassTransform(mTransform->getTransformAsBtTransform());
            }
        }
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
    SceneObjectRuntime::walk
    (float leftStickX, float leftStickY)
    {

    }
}
