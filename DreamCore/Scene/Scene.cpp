/*
 * Scene
 *
 * Created: 13 2017 by Ashley
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

#include "Scene.h"

#include <functional>
#include <algorithm>

#include "SceneDefinition.h"
#include "SceneRuntime.h"

#include "SceneObject/SceneObject.h"
#include "SceneObject/SceneObjectRuntime.h"
#include "SceneObject/SceneObjectDefinition.h"

#include "../Common/Constants.h"

#include "../Components/Transform3D.h"

#include "../Lua/LuaEngine.h"

#include "../Project/Project.h"
#include "../Project/ProjectRuntime.h"

#include "../Utilities/String.h"

namespace Dream
{
    Scene::Scene
    (Project* project, json json)
        : mProjectHandle(project),
          mDefinition(new SceneDefinition(json)),
          mRuntime(new SceneRuntime())
    {
        mRuntime->setState(SCENE_STATE_NOT_LOADED);

    }

    Scene::~Scene
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: Destroying Object" << endl;
        }
    }

    SceneObject*
    Scene::getSceneObjectHandleByUuid
    (string uuid)
    {
        return static_cast<SceneObject*>
        (
            mRootSceneObject->applyToAll
            (
                function<void*(SceneObject*)>
                (
                    [&](SceneObject* currentSo)
                    {
                        if (currentSo->getDefinitionHandle()->hasUuid(uuid))
                        {
                            return currentSo;
                        }
                        return static_cast<SceneObject*>(0);
                    }
                )
            )
        );
    }

    SceneObject*
    Scene::getSceneObjectHandleByName
    (string name)
    {
        return static_cast<SceneObject*>
        (
            mRootSceneObject->applyToAll
            (
                function<void*(SceneObject*)>
                (
                    [&](SceneObject* currentSo)
                    {
                        if (currentSo->getDefinitionHandle()->hasName(name))
                        {
                            return currentSo;
                        }
                        return static_cast<SceneObject*>(0);
                    }
                )
            )
        );
    }

    int
    Scene::countSceneObjects
    ()
    {
        int count = 0;
        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject*)
                {
                    count++;
                    return nullptr;
                }
            )
        );
        return count;
    }

    void
    Scene::showScenegraph
    ()
    {
        if (!mRootSceneObject)
        {
            cout << "Scene: Scenegraph is empty (no root SceneObject)" << endl;
            return;
        }

        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* obj)
                {
                    obj->getDefinitionHandle()->showStatus();
                    return nullptr;
                }
            )
        );
    }

    void
    Scene::setRootSceneObjectHandle
    (SceneObject* root)
    {
        mRootSceneObject.reset(root);
    }

    SceneObject*
    Scene::getRootSceneObjectHandle
    ()
    {
        return mRootSceneObject.get();
    }

    void
    Scene::findDeleteFlaggedSceneObjects
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "Scene: findDeleteFlaggedSceneObjects Called" << endl;
        }

        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* obj)
                {
                    if (obj->getRuntimeHandle()->getDeleteFlag())
                    {
                        mRuntime->addToDeleteQueue(obj);
                    }
                    return nullptr;
                }
            )
        );
    }

    void
    Scene::createAllAssetInstances
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "Secne: Create All Asset Instances Called" << endl;
        }

        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* sceneObj)
                {
                    // Not loaded && not marked to delete
                    if (!sceneObj->getRuntimeHandle()->getLoadedFlag() && !sceneObj->getRuntimeHandle()->getDeleteFlag())
                    {
                        sceneObj->getRuntimeHandle()->createAssetInstances();
                    }
                    return nullptr;
                }
            )
        );
    }

    void
    Scene::loadAllAssetInstances
    ()
    {
        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* sceneObj)
                {
                    // Not loaded && not marked to delete
                    if (!sceneObj->getRuntimeHandle()->getLoadedFlag())
                    {
                        if(!sceneObj->getRuntimeHandle()->getDeleteFlag())
                        {
                            sceneObj->getRuntimeHandle()->loadAssetInstances();
                        }
                    }
                    return nullptr;
                }
            )
        );

        mRuntime->setState(SCENE_STATE_LOADED);
    }
    void
    Scene::findDeleteFlaggedScripts
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "Scene: Cleanup Deleted Scripts Called" << endl;
        }
        vector<SceneObject*> objects = mRuntime->getDeleteQueue();

        for (SceneObject* it : objects)
        {
            mProjectHandle->getRuntimeHandle()->getLuaEngineHandle()->removeFromScriptMap(it);
        }
    }

    void
    Scene::cleanUpSceneObjects
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: cleanUpSceneObjects" << endl;
        }
        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* obj)
                {
                    obj->getRuntimeHandle()->setDeleteFlag(true);
                    return nullptr;
                }
            )
        );
    }

    void
    Scene::flush
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: flush" << endl;
        }
        findDeleteFlaggedSceneObjects();
        findDeleteFlaggedScripts();
        mRuntime->destroyDeleteQueue();
    }

    void
    Scene::cleanUpRuntime
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: cleanUp called on "
                 <<  mDefinition->getNameAndUuidString()
                 << endl;
        }

        if(mRuntime->getState() == SCENE_STATE_DONE)
        {
           cleanUpSceneObjects();
           flush();
           mRuntime->setState(SCENE_STATE_CLEANED_UP);
        }
        else
        {
            cerr << "Scene: Cannot cleanUp Scene "
                 << mDefinition->getNameAndUuidString()
                 << " State != DONE"
                 << endl;
        }
    }

    Project*
    Scene::getProjectHandle
    ()
    {
        return mProjectHandle;
    }


    SceneRuntime*
    Scene::getRuntimeHandle
    ()
    const
    {
        return mRuntime.get();
    }

    SceneDefinition*
    Scene::getDefinitionHandle
    ()
    const
    {
        return mDefinition.get();
    }

} // End of Dream
