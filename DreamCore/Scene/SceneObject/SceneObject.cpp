
#include "SceneObject.h"

#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Scene.h"
#include "SceneObjectRuntime.h"
#include "SceneObjectJsonData.h"

#include "../Project/Project.h"
#include "../Utilities/String.h"
#include "../Utilities/Uuid.h"

// Todo: Create SceneObject Runtime Object

namespace Dream
{
    SceneObject::SceneObject
    (Scene* scene, nlohmann::json soJson)
    {
        mRuntime.reset(new SceneObjectRuntime(scene));
        mJsonData.reset(new SceneObjectJsonData(soJson));

        mJsonData->applyDataToRuntime(mRuntime.get());

    }


    SceneObject::~SceneObject
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Destroying Object "
                 << getNameAndUuidString() << endl;
        }
    }


    void
    SceneObject::showStatus
    ()
    {
        if (mJsonData)
        {
            cout << "SceneObject: " << mJsonData->getJson().dump(1) << endl;
            SceneObject* parentHandle = mRuntime->getParentHandle();
            if (parentHandle != nullptr)
            {
                cout << "SceneObject: ParentUuid: " << parentHandle->getJsonData()->getUuid() << endl;
            }
        }
    }


    bool
    SceneObject::applyToAll
    (function<bool(SceneObject*)> funk)
    {
        bool retval = funk(this);

        for (SceneObject* it : mRuntime->getChildren())
        {
            if (it)
            {
                retval = retval || funk(it);
            }
        }
        return retval;
    }

    void*
    SceneObject::applyToAll
    (function<void*(SceneObject*)> funk)
    {
        void* retval = funk(this);
        if (retval)
        {
            return retval;
        }

        for (SceneObject* it : mRuntime->getChildren())
        {
            if (it)
            {
                retval = funk(it);
                if (retval)
                {
                    return retval;
                }
            }
        }
        return nullptr;
    }


    SceneObjectRuntime*
    SceneObject::getRuntime
    ()
    {
        return mRuntime.get();
    }

    string
    SceneObject::getNameAndUuidString
    ()
    {
        return mJsonData->getUuid()+" : "+mJsonData->getName();
    }

    Scene*
    SceneObjectRuntime::getSceneRuntimeHandle
    ()
    {
        return mSceneRuntimeHandle;
    }
} // End of Dream
