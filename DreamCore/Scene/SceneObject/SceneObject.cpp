
#include "SceneObject.h"

#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>

#include "SceneObjectRuntime.h"
#include "SceneObjectDefinition.h"

#include "../Scene.h"

#include "../../Common/Constants.h"

#include "../../Project/Project.h"

#include "../../Utilities/String.h"
#include "../../Utilities/Uuid.h"

namespace Dream
{
    SceneObject::SceneObject
    (Scene* parentScene, json soJson)
        : // Init List
          mRuntime(new SceneObjectRuntime(this)),
          mDefinition(new SceneObjectDefinition(soJson)),
          mSceneHandle(parentScene),
          mParentHandle(nullptr)
    {
        mDefinition->applyDataToRuntime(mRuntime.get());
    }


    SceneObject::~SceneObject
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Destroying Object "
                 << getDefinitionHandle()->getNameAndUuidString()
                 << endl;
        }
    }


    bool
    SceneObject::applyToAll
    (function<bool(SceneObject*)> funk)
    {
        bool retval = funk(this);

        for (SceneObject* it : mChildren)
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

        for (SceneObject* it : mChildren)
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
    SceneObject::getRuntimeHandle
    ()
    {
        return mRuntime.get();
    }

    SceneObjectDefinition*
    SceneObject::getDefinitionHandle
    ()
    {
        return mDefinition.get();
    }

    SceneObject*
    SceneObject::getChildByUuid
    (string uuid)
    {
       for (SceneObject* so : mChildren)
       {
           if (so->getDefinitionHandle()->hasUuid(uuid))
           {
               return so;
           }
       }
       return nullptr;
    }

    void
    SceneObject::addChild
    (SceneObject *child)
    {
        mChildren.push_back(child);
    }

    bool
    SceneObject::isParentOf
    (SceneObject *child)
    {
       for (SceneObject* so : mChildren)
       {
           if (so == child)
           {
               return true;
           }
       }
       return false;
    }

    void
    SceneObject::setParentHandle
    (SceneObject *parent)
    {
       mParentHandle = parent;
    }

    SceneObject*
    SceneObject::getParentHandle
    ()
    {
       return mParentHandle;
    }

    Scene*
    SceneObject::getSceneHandle
    ()
    {
        return mSceneHandle;
    }

} // End of Dream
