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

#include "../../Common/Constants.h"

#include "../../Components/Event.h"
#include "../../Components/Path/PathInstance.h"
#include "../../Components/Audio/AudioInstance.h"
#include "../../Components/Audio/AudioComponent.h"
#include "../../Components/Graphics/Font/FontInstance.h"
#include "../../Components/Graphics/Model/AssimpModelInstance.h"
#include "../../Components/Graphics/Shader/ShaderInstance.h"
#include "../../Components/Graphics/Light/LightInstance.h"
#include "../../Components/Graphics/Sprite/SpriteInstance.h"
#include "../../Components/Physics/PhysicsObjectInstance.h"
#include "../../Components/Physics/PhysicsComponent.h"
#include "../../Components/IAssetDefinition.h"

#include "../../Components/Path/PathDefinition.h"
#include "../../Components/Audio/AudioDefinition.h"
#include "../../Components/Graphics/Font/FontDefinition.h"
#include "../../Components/Graphics/Light/LightDefinition.h"
#include "../../Components/Graphics/Model/ModelDefinition.h"
#include "../../Components/Physics/PhysicsObjectDefinition.h"
#include "../../Components/Graphics/Shader/ShaderDefinition.h"
#include "../../Components/Graphics/Sprite/SpriteDefinition.h"

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
using std::flush;

using glm::vec3;

namespace Dream
{
    SceneObjectRuntime::SceneObjectRuntime(shared_ptr<SceneObjectDefinition> sd, shared_ptr<SceneRuntime> sr)
        : // Init list
          IRuntime(sd),
          mSceneRuntime(sr),
          mParentRuntime(nullptr),
          mLoaded(false),
          mHasFocus(false),
          mFollowsCamera(false)

    {
        setLogClassName("SceneObjectRuntime");
        auto log = getLog();
        log->trace( "Constructing Object" );
        mTransform = make_shared<Transform3D>();
    }

    SceneObjectRuntime::~SceneObjectRuntime
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );

        if (hasPhysicsObjectInstance())
        {
            mSceneRuntime
                ->getProjectRuntime()
                ->getPhysicsComponent()
                ->removePhysicsObjectInstance(getPhysicsObjectInstance());
        }

        if (hasScriptInstance())
        {
            mSceneRuntime
                ->getProjectRuntime()
                ->getScriptComponent()
                ->removeFromScriptMap(
                    dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this())
                );
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

    shared_ptr<PathInstance>
    SceneObjectRuntime::getPathInstance
    ()
    {
        return mPathInstance;
    }

    shared_ptr<AudioInstance>
    SceneObjectRuntime::getAudioInstance
    ()
    {
        return mAudioInstance;
    }

    shared_ptr<AssimpModelInstance>
    SceneObjectRuntime::getModelInstance
    ()
    {
        return mModelInstance;
    }

    shared_ptr<ScriptInstance>
    SceneObjectRuntime::getScriptInstance
    ()
    {
        return mScriptInstance;
    }

    shared_ptr<ShaderInstance>
    SceneObjectRuntime::getShaderInstance
    ()
    {
        return mShaderInstance;
    }

    shared_ptr<LightInstance>
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
    SceneObjectRuntime::hasShaderInstance
    ()
    {
        return mShaderInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasScriptInstance
    ()
    {
        return mScriptInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasSpriteInstance
    ()
    {
        return mSpriteInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasFontInstance
    ()
    {
        return mFontInstance != nullptr;
    }

    void
    SceneObjectRuntime::addAssetDefinitionUuidToLoad
    (string def)
    {
        mAssetDefinitionUuidLoadQueue.push_back(def);
    }

    vector<string>
    SceneObjectRuntime::getAssetDefinitionUuidsToLoad
    ()
    {
        return mAssetDefinitionUuidLoadQueue;
    }

    shared_ptr<FontInstance>
    SceneObjectRuntime::getFontInstance
    ()
    {
        return mFontInstance;
    }

    shared_ptr<PhysicsObjectInstance>
    SceneObjectRuntime::getPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance;
    }

    string
    SceneObjectRuntime::getTransformType
    ()
    {
        return mTransform->getTransformType();
    }

    void
    SceneObjectRuntime::setTransformType
    (string transformType)
    {
        mTransform->setTransformType(transformType);
    }

    shared_ptr<Transform3D>
    SceneObjectRuntime::getTransform
    ()
    {
        return mTransform;
    }

    void
    SceneObjectRuntime::initialTransform()
    {
        auto log = getLog();
        shared_ptr<Transform3D> initial = dynamic_pointer_cast<SceneObjectDefinition>(mDefinition)->getTransform();
        if (initial->isTypeOffset())
        {
            log->info("Inheriting Offset Transform for {}",getNameAndUuidString());
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
    (shared_ptr<Transform3D> transform)
    {
        mTransform = transform;
    }

    bool
    SceneObjectRuntime::hasFocus
    ()
    {
        return mHasFocus;
    }

    void
    SceneObjectRuntime::setHasFocus
    (bool focus)
    {
        mHasFocus = focus;
    }

    shared_ptr<SpriteInstance>
    SceneObjectRuntime::getSpriteInstance
    ()
    {
        return mSpriteInstance;
    }

    bool
    SceneObjectRuntime::getLoadedFlag
    ()
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
    {
        return !mEventQueue.empty();
    }

    void
    SceneObjectRuntime::sendEvent
    (Event event)
    {
        mEventQueue.push_back(event);
    }

    vector<Event>
    SceneObjectRuntime::getEventQueue
    ()
    {
        return mEventQueue;
    }

    void SceneObjectRuntime::clearEventQueue()
    {
        mEventQueue.clear();
    }

    void SceneObjectRuntime::collectGarbage()
    {
        auto log = getLog();
        log->info( "Collecting Garbage {}" ,getNameAndUuidString() );
        clearEventQueue();
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

    void
    SceneObjectRuntime::createAssetInstances
    ()
    {
        for (string aDefUuid : mAssetDefinitionUuidLoadQueue)
        {
            createAssetInstanceFromAssetDefinitionByUuid(aDefUuid);
        }
        mAssetDefinitionUuidLoadQueue.clear();
    }

    void
    SceneObjectRuntime::createAssetInstanceFromAssetDefinitionByUuid
    (string uuid)
    {
        shared_ptr<IAssetDefinition> assetDefinition = mSceneRuntime
            ->getProjectRuntime()
                ->getProject()
                    ->getProjectDefinition()
                        ->getAssetDefinitionByUuid(uuid);
        createAssetInstance(assetDefinition);
    }

    void
    SceneObjectRuntime::createAssetInstance
    (shared_ptr<IAssetDefinition> definition)
    {
        auto log = getLog();
        mProjectPath = mSceneRuntime->getProjectRuntime()->getProject()->getProjectPath();

        log->info( "Creating Asset Intance of: ({}) {}", definition->getType() ,  definition->getName());

        if (mParentRuntime)
        {
            log->info( " for {} " ,mParentRuntime->getNameAndUuidString());
        }

        if(definition->isTypePath())
        {
            createPathInstance(dynamic_pointer_cast<PathDefinition>(definition));
        }
        else if (definition->isTypeAudio())
        {
            createAudioInstance(dynamic_pointer_cast<AudioDefinition>(definition));
        }
        else if (definition->isTypeModel())
        {
            createModelInstance(dynamic_pointer_cast<ModelDefinition>(definition));
        }
        else if (definition->isTypeScript())
        {
            createScriptInstance(dynamic_pointer_cast<ScriptDefinition>(definition));
        }
        else if (definition->isTypeShader())
        {
            createShaderInstance(dynamic_pointer_cast<ShaderDefinition>(definition));
        }
        else if (definition->isTypePhysicsObject())
        {
            createPhysicsObjectInstance(dynamic_pointer_cast<PhysicsObjectDefinition>(definition));
        }
        else if (definition->isTypeLight())
        {
            createLightInstance(dynamic_pointer_cast<LightDefinition>(definition));
        }
        else if (definition->isTypeSprite())
        {
            createSpriteInstance(dynamic_pointer_cast<SpriteDefinition>(definition));
        }
        else if (definition->isTypeFont())
        {
            createFontInstance(dynamic_pointer_cast<FontDefinition>(definition));
        }
        else
        {
            log->error( "Invalid Asset Instance Type {}",definition->getType() );
        }

        setLoadedFlag(true);
    }

    void
    SceneObjectRuntime::createPhysicsObjectInstance
    (shared_ptr<PhysicsObjectDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Physics Object Asset Instance." );
        mPhysicsObjectInstance = make_shared<PhysicsObjectInstance>(definition, dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this()));
        mPhysicsObjectInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createPathInstance
    (shared_ptr<PathDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Path asset instance." );
        mPathInstance = make_shared<PathInstance>(definition,dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this()));
        mPathInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createAudioInstance
    (shared_ptr<AudioDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Audio asset instance." );
        // hottest trainwreck 2017!
        mAudioInstance = shared_ptr<AudioInstance>(
            mSceneRuntime
                ->getProjectRuntime()
                    ->getAudioComponent()
                        ->newAudioInstance(definition,dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this()))
        );
        mAudioInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createModelInstance
    (shared_ptr<ModelDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Model asset instance." );
        mModelInstance = make_shared<AssimpModelInstance>
        (
            mSceneRuntime->getProjectRuntime()->getModelCache(),
            mSceneRuntime->getProjectRuntime()->getTextureCache(),
            definition,
            dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this())
        );

        try
        {
            mModelInstance->load(mProjectPath);
        }
        catch (std::exception e)
        {
            log->error("Exception while loading model resource {}",e.what());
            return;
        }
    }

    void
    SceneObjectRuntime::createScriptInstance
    (shared_ptr<ScriptDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Script asset instance." );
        mScriptInstance = dynamic_pointer_cast<ScriptInstance>(
            make_shared<LuaScriptInstance>(
                definition,
                dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this())
            )
        );
        mScriptInstance->load(mProjectPath);
        mSceneRuntime
            ->getProjectRuntime()
            ->getScriptComponent()
            ->addToScriptMap(
                dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this()),
                mScriptInstance
            );
    }

    void
    SceneObjectRuntime::createShaderInstance
    (shared_ptr<ShaderDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Shader asset instance." );
        mShaderInstance = make_shared<ShaderInstance>
        (
            mSceneRuntime->getProjectRuntime()->getShaderCache(),
            definition,
            dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this())
        );
        mShaderInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createLightInstance
    (shared_ptr<LightDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Light Asset instance." );
        mLightInstance = make_shared<LightInstance>(
            definition,
            dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this())
        );
        mLightInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createSpriteInstance
    (shared_ptr<SpriteDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Sprite Asset instance." );
        mSpriteInstance = make_shared<SpriteInstance>
        (
            mSceneRuntime->getProjectRuntime()->getTextureCache(),
            definition,
            dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this())
        );
        mSpriteInstance->load(mProjectPath);
    }

    quat SceneObjectRuntime::getOrientation()
    {
       return mTransform->getOrientation();
    }

    void
    SceneObjectRuntime::createFontInstance
    (shared_ptr<FontDefinition> definition)
    {
        auto log = getLog();
        log->info( "Creating Font Asset instance." );
        mFontInstance = make_shared<FontInstance>
        (
            mSceneRuntime->getProjectRuntime()->getFontCache(),
            definition,
            dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this())
        );
        mFontInstance->load(mProjectPath);
    }

    bool
    SceneObjectRuntime::applyToAll
    (function<bool(shared_ptr<SceneObjectRuntime>)> funk)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(bool) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        bool retval = funk(dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this()));

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it))
            {
                retval = retval || (*it)->applyToAll(funk);
            }
        }
        return retval;
    }

    shared_ptr<SceneObjectRuntime>
    SceneObjectRuntime::applyToAll
    (function<shared_ptr<SceneObjectRuntime>(shared_ptr<SceneObjectRuntime>)> funk)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(void*) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        shared_ptr<SceneObjectRuntime> retval = funk(dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this()));

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

    shared_ptr<SceneObjectRuntime>
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
    (shared_ptr<SceneObjectRuntime> child)
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
    (shared_ptr<SceneObjectRuntime> parent)
    {
        mParentRuntime = parent;
    }

    shared_ptr<SceneObjectRuntime>
    SceneObjectRuntime::getParentRuntime
    ()
    {
        return mParentRuntime;
    }

    shared_ptr<SceneRuntime>
    SceneObjectRuntime::getSceneRuntime
    ()
    {
        return mSceneRuntime;
    }

    shared_ptr<SceneObjectDefinition> SceneObjectRuntime::getSceneObjectDefinition()
    {
       return dynamic_pointer_cast<SceneObjectDefinition>(getDefinition());
    }

    void
    SceneObjectRuntime::useDefinition
    (shared_ptr<IDefinition> iDefinition)
    {
        auto def = dynamic_pointer_cast<SceneObjectDefinition>(iDefinition);
        auto log = getLog();
        log->info( "Using Definition {}", def->getNameAndUuidString());
        setName(def->getName());
        setUuid(def->getUuid());
        setFollowsCamera(def->followsCamera());
        setAssetDefinitionLoadQueue(def->getAssetDefinitionLoadQueue());
        setHasFocus(def->hasFocus());
        initialTransform();
        createAssetInstances();
        loadChildrenFromDefinition(def);
    }

    void
    SceneObjectRuntime::setAssetDefinitionLoadQueue
    (vector<string> loadQueue)
    {
        mAssetDefinitionUuidLoadQueue = loadQueue;
    }

    void
    SceneObjectRuntime::loadChildrenFromDefinition
    (shared_ptr<SceneObjectDefinition> definition)
    {
        vector<shared_ptr<SceneObjectDefinition>> definitions = definition->getChildDefinitionsList();
        for
        (auto it = begin(definitions); it != end(definitions); it++)
        {
            shared_ptr<SceneObjectRuntime> child = make_shared<SceneObjectRuntime>(*it, mSceneRuntime);
            child->setParentRuntime(dynamic_pointer_cast<SceneObjectRuntime>(shared_from_this()));
            child->useDefinition(*it);
            mChildRuntimes.push_back(child);
        }
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
    void
    SceneObjectRuntime::walk
    (float leftStickX, float leftStickY)
    {

    }

}
