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
 */

#include <iostream>
#include "SceneObjectRuntime.h"
#include "SceneObjectDefinition.h"
#include "../SceneRuntime.h"
#include "../../Components/Event.h"
#include "../../Components/Path/PathRuntime.h"
#include "../../Components/Animation/AnimationDefinition.h"
#include "../../Components/Animation/AnimationRuntime.h"
#include "../../Components/Audio/AudioCache.h"
#include "../../Components/Audio/AudioRuntime.h"
#include "../../Components/Audio/AudioComponent.h"
#include "../../Components/Graphics/Model/ModelRuntime.h"
#include "../../Components/Graphics/Model/ModelCache.h"
#include "../../Components/Graphics/Light/LightRuntime.h"
#include "../../Components/Graphics/ParticleEmitter/ParticleEmitterRuntime.h"
#include "../../Components/Physics/PhysicsObjectRuntime.h"
#include "../../Components/Physics/PhysicsComponent.h"
#include "../../Components/AssetDefinition.h"
#include "../../Components/Audio/AudioDefinition.h"
#include "../../Components/Graphics/Font/FontDefinition.h"
#include "../../Components/Graphics/Light/LightDefinition.h"
#include "../../Components/Graphics/Model/ModelDefinition.h"
#include "../../Components/Graphics/ParticleEmitter/ParticleEmitterDefinition.h"
#include "../../Components/Path/PathDefinition.h"
#include "../../Components/Physics/PhysicsObjectDefinition.h"
#include "../../Components/Scripting/ScriptDefinition.h"
#include "../../Components/Scripting/ScriptComponent.h"
#include "../../Components/Scripting/ScriptRuntime.h"
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
    ):  Runtime(sd),
        mAnimationInstance(nullptr),
        mAudioInstance(nullptr),
        mLightInstance(nullptr),
        mParticleEmitterInstance(nullptr),
        mPathInstance(nullptr),
        mPhysicsObjectInstance(nullptr),
        mScriptInstance(nullptr),
        mModelInstance(nullptr),
        mSceneRuntime(sr),
        mParentRuntime(nullptr),
        mBoundingBox(),
        mHasCameraFocus(false),
        mDeleted(false),
        mHidden(false),
        mAlwaysDraw(false)
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

        removeAnimationInstance();
        removeAudioInstance();
        removeLightInstance();
        removeModelInstance();
        removeParticleEmitterInstance();
        removePathInstance();
        removePhysicsObjectInstance();
        removeScriptInstance();
    }

    void
    SceneObjectRuntime::removeAnimationInstance
    ()
    {
        if (mAnimationInstance != nullptr)
        {
            delete mAnimationInstance;
            mAnimationInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeAudioInstance
    ()
    {
        /*
        if (mAudioInstance != nullptr)
        {
            delete mAudioInstance;
            mAudioInstance = nullptr;
        }
        */
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
        if (mModelInstance != nullptr)
        {
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
            auto physicsComp = mSceneRuntime
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

    AnimationRuntime*
    SceneObjectRuntime::getAnimationInstance
    ()
    {
        return mAnimationInstance;
    }

    PathRuntime*
    SceneObjectRuntime::getPathInstance
    ()
    {
        return mPathInstance;
    }

    AudioRuntime*
    SceneObjectRuntime::getAudioInstance
    ()
    {
        return mAudioInstance;
    }

    ModelRuntime*
    SceneObjectRuntime::getModelInstance
    ()
    {
        return mModelInstance;
    }

    ScriptRuntime*
    SceneObjectRuntime::getScriptInstance
    ()
    {
        return mScriptInstance;
    }

    LightRuntime*
    SceneObjectRuntime::getLightInstance
    ()
    {
        return mLightInstance;
    }

    ParticleEmitterRuntime*
    SceneObjectRuntime::getParticleEmitterInstance
    ()
    {
       return mParticleEmitterInstance;
    }

    AssetRuntime*
    SceneObjectRuntime::getAssetInstance
    (AssetType type)
    {
       switch (type)
       {
           case Dream::ANIMATION:
               return getAnimationInstance();
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
    SceneObjectRuntime::hasAnimationInstance
    ()
    {
        return mAnimationInstance != nullptr;
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

    PhysicsObjectRuntime*
    SceneObjectRuntime::getPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance;
    }

    Transform&
    SceneObjectRuntime::getTransform
    ()
    {
        return mTransform;
    }

    void
    SceneObjectRuntime::initTransform()
    {
        auto log = getLog();
        auto definedTransform = dynamic_cast<SceneObjectDefinition*>(mDefinition)->getTransform();
        mInitialTransform = Transform(definedTransform);
        mTransform = Transform(definedTransform);
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
    (const Event& event)
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
            AssetDefinition* def = getAssetDefinitionByUuid(assetPair.second);
            bool result = false;
            if (def == nullptr)
            {
                log->error("Could not find asset definition {}", assetPair.second);
                continue;
            }
            log->trace("Creating {}",def->getNameAndUuidString());
            switch (assetPair.first)
            {
                case AssetType::ANIMATION:
                    result = createAnimationInstance(dynamic_cast<AnimationDefinition*>(def));
                    break;
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

    AssetDefinition*
    SceneObjectRuntime::getAssetDefinitionByUuid
    (const string& uuid)
    {
        auto log = getLog();
        auto project = mSceneRuntime->getProjectRuntime()->getProject();
        if (project == nullptr)
        {
            log->error("Project is not found");
            return nullptr;
        }
        auto assetDefinition = project->getDefinition()->getAssetDefinitionByUuid(uuid);
        if (assetDefinition == nullptr)
        {
            log->error("AssetDefinition not found");
        }
        return assetDefinition;
    }

    bool
    SceneObjectRuntime::replaceAssetUuid
    (AssetType type, const string& uuid)
    {
        auto log = getLog();
        log->info("REPLACING asset instance from uuid {}", uuid);
        auto project = mSceneRuntime->getProjectRuntime()->getProject();
        if (project == nullptr)
        {
            log->error("Project is not found");
            return false;
        }
        auto def = project->getDefinition()->getAssetDefinitionByUuid(uuid);
        if (def == nullptr)
        {
            log->error("AssetDefinition not found");
        }
        switch (type)
        {
            case AssetType::ANIMATION:
                return createAnimationInstance(dynamic_cast<AnimationDefinition*>(def));
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
        mPhysicsObjectInstance = new PhysicsObjectRuntime(
            definition,
            mSceneRuntime->getProjectRuntime()->getPhysicsComponent(),
            mSceneRuntime->getProjectRuntime()->getModelCache(),
            this
        );
        return mPhysicsObjectInstance->useDefinition();
    }

    bool
    SceneObjectRuntime::createParticleEmitterInstance
    (ParticleEmitterDefinition* definition)
    {
        auto log = getLog();
        log->trace( "Creating ParticleEmitter asset instance." );
        removeParticleEmitterInstance();
        mParticleEmitterInstance = new ParticleEmitterRuntime(definition,this);
        return mParticleEmitterInstance->useDefinition();
    }

    bool
    SceneObjectRuntime::createAnimationInstance
    (AnimationDefinition* definition)
    {
        auto log = getLog();
        log->trace( "Creating Animation asset instance." );
        removeAnimationInstance();
        mAnimationInstance = new AnimationRuntime(definition,this);
        return mAnimationInstance->useDefinition();
    }

    bool
    SceneObjectRuntime::createPathInstance
    (PathDefinition* definition)
    {
        auto log = getLog();
        log->trace( "Creating Path asset instance." );
        removePathInstance();
        mPathInstance = new PathRuntime(definition,this);
        return mPathInstance->useDefinition();
    }

    bool
    SceneObjectRuntime::createAudioInstance
    (AudioDefinition* definition)
    {
        auto log = getLog();
        auto cache = mSceneRuntime->getProjectRuntime()->getAudioCache();
        if (cache != nullptr)
        {
            mAudioInstance = dynamic_cast<AudioRuntime*>(cache->getInstance(definition));
            return mAudioInstance != nullptr;
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
        auto cache = mSceneRuntime->getProjectRuntime()->getModelCache();
        if (cache != nullptr)
        {
            mModelInstance = dynamic_cast<ModelRuntime*>(cache->getInstance(definition));
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
        auto scriptCache = (mSceneRuntime->getProjectRuntime()->getScriptCache());
        if (scriptCache)
        {
            mScriptInstance = dynamic_cast<ScriptRuntime*>(scriptCache->getInstance(definition));
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
        mLightInstance = new LightRuntime(definition,this);
        return mLightInstance->useDefinition();
    }

    bool
    SceneObjectRuntime::applyToAll
    (const function<bool(SceneObjectRuntime*)>& fn)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(bool) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        bool retval = fn(this);

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if (*it)
            {
                retval = retval || (*it)->applyToAll(fn);
            }
        }
        return retval;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::applyToAll
    (const function<SceneObjectRuntime*(SceneObjectRuntime*)>& fn)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(void*) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        SceneObjectRuntime* retval = fn(this);

        if (retval != nullptr)
        {
            return retval;
        }

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it) != nullptr)
            {
                retval = (*it)->applyToAll(fn);
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
    (const string& uuid)
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
        mParentRuntime = parent;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getParentRuntime
    ()
    {
        return mParentRuntime;
    }

    SceneRuntime*
    SceneObjectRuntime::getSceneRuntime
    ()
    {
        return mSceneRuntime;
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
        setHasCameraFocus(def->getHasCameraFocus());
        setHidden(def->getHidden());
        initTransform();
        setAssetDefinitionsMap(def->getAssetDefinitionsMap());
        if (!createAssetInstances()) return false;
        if( !loadChildrenFromDefinition(def)) return false;
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
            auto sor = (*it);
            if (!sor->getIsTemplate())
            {
                createAndAddChildRuntime(*it);
            }
            else
            {
                log->debug("Skipping Template Object: {}",sor->getNameAndUuidString());
            }
        }
        return true;
    }

    void
    SceneObjectRuntime::setTransform
    (const Transform& transform)
    {
        mTransform = transform;
    }

    void
    SceneObjectRuntime::translateWithChildren
    (const vec3& translation)
    {
        applyToAll(
            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
            [&](SceneObjectRuntime* rt)
            {
                rt->getTransform().translate(translation);
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));
    }

    void
    SceneObjectRuntime::preTranslateWithChildren
    (const vec3& translation)
    {
        applyToAll(
            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
            [&](SceneObjectRuntime* rt)
            {
                rt->getTransform().preTranslate(translation);
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));
    }

    Transform&
    SceneObjectRuntime::getInitialTransform
    ()
    {
       return mInitialTransform;
    }

    void
    SceneObjectRuntime::translateOffsetInitialWithChildren
    (const vec3& translation)
    {
        applyToAll(
            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
            [&](SceneObjectRuntime* rt)
            {
                auto& initial = rt->getInitialTransform().getMatrix();
                mat4 mat = glm::translate(mat4(1.0f),translation)*initial;
                rt->getTransform().setMatrix(mat);
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));
    }


    void
    SceneObjectRuntime::transformOffsetInitial
    (const mat4& matrix)
    {
        mTransform.setMatrix(mTransform.getMatrix()*mInitialTransform.getMatrix());
    }

    bool
    SceneObjectRuntime::getHasCameraFocus
    () const
    {
        return mHasCameraFocus;
    }

    void
    SceneObjectRuntime::setHasCameraFocus
    (bool cf)
    {
        mHasCameraFocus = cf;
    }

    bool
    SceneObjectRuntime::getDeleted
    () const
    {
        return mDeleted;
    }

    void
    SceneObjectRuntime::setDeleted
    (bool deleted)
    {
        mDeleted = deleted;
    }

    bool
    SceneObjectRuntime::getHidden
    () const
    {
        return mHidden;
    }

    void
    SceneObjectRuntime::setHidden
    (bool hidden)
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
    SceneObjectRuntime::createAndAddChildRuntime
    (SceneObjectDefinition* def)
    {
        auto log = getLog();
        SceneObjectRuntime* child = new SceneObjectRuntime(def, mSceneRuntime);
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

    BoundingBox&
    SceneObjectRuntime::getBoundingBox
    ()
    {
        if (mModelInstance != nullptr)
        {
            return mModelInstance->getBoundingBox();
        }
        return mBoundingBox;
    }

    void
    SceneObjectRuntime::setBoundingBox
    (const BoundingBox& boundingBox)
    {
        mBoundingBox = boundingBox;
    }

    float
    SceneObjectRuntime::distanceFrom
    (SceneObjectRuntime* other)
    {
        return mTransform.distanceFrom(other->getTransform());
    }

    float
    SceneObjectRuntime::distanceFrom
    (const vec3& other)
    {
        return glm::distance(mTransform.decomposeMatrix().translation,other);
    }

    bool
    SceneObjectRuntime::visibleInFrustum
    ()
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
           return cam->visibleInFrustum(this);
        }
        return false;
    }

    bool
    SceneObjectRuntime::containedInFrustum
    ()
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
           return cam->containedInFrustum(this);
        }
        return false;
    }

    bool
    SceneObjectRuntime::containedInFrustumAfterTransform
    (const mat4& tx)
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
           return cam->containedInFrustumAfterTransform(this,tx);
        }
        return false;

    }

    bool
    SceneObjectRuntime::exceedsFrustumPlaneAtTranslation
    (Frustum::Plane plane, const vec3& tx)
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
            return cam->exceedsFrustumPlaneAtTranslation(plane,this,tx);
        }
        return false;
    }
}
