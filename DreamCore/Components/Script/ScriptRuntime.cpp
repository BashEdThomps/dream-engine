/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <angelscript.h>
#include "ScriptRuntime.h"
#include "ScriptDefinition.h"
#include "ScriptComponent.h"

#include "../Input/InputComponent.h"
#include "../Graphics/NanoVGComponent.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Project/ProjectRuntime.h"

namespace Dream
{
    ScriptRuntime::ScriptRuntime
    (ScriptDefinition* definition, ProjectRuntime* rt)
        : SharedAssetRuntime(definition,rt),
          mSource(""),
          mInitialised(false),
          mError(false),
          mScriptModule(nullptr),
          mInitFunction(nullptr),
          mUpdateFunction(nullptr),
          mEventFunction(nullptr),
          mNanoVGFunction(nullptr),
          mInputFunction(nullptr),
          mContext(nullptr),
          mUuidString(std::to_string(mUuid))
    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptRuntime");
        getLog()->trace( "Constructing {}", getNameAndUuidString());
        #endif
        return;
    }

    ScriptRuntime::~ScriptRuntime
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destructing {}", mDefinition->getNameAndUuidString());
        #endif

        if (mInitFunction)
        {
            mInitFunction->Release();
            mInitFunction = nullptr;
        }

        if (mUpdateFunction)
        {
            mUpdateFunction->Release();
            mUpdateFunction = nullptr;
        }

        if (mEventFunction)
        {
            mEventFunction->Release();
            mEventFunction = nullptr;
        }

        if (mInputFunction)
        {
            mInputFunction->Release();
            mInputFunction = nullptr;
        }

        if (mNanoVGFunction)
        {
            mNanoVGFunction->Release();
            mNanoVGFunction = nullptr;
        }

        if (mContext)
        {
            mContext->Release();
            mContext = nullptr;
        }

        if (mScriptModule)
        {
            mScriptModule->Discard();
            mScriptModule = nullptr;
        }
    }

    bool
    ScriptRuntime::useDefinition
    ()
    {
        auto path = getAssetFilePath();
        #ifdef DREAM_LOG
        getLog()->debug( "Creating Script at {}" , path);
        #endif

        if (mProjectRuntime->getScriptComponent()->tryLock())
        {
            #ifdef DREAM_LOG
            getLog()->debug("createState called for {}", getNameAndUuidString() );
            #endif
            mScriptModule = ScriptComponent::Engine->GetModule(mUuidString.c_str(),asGM_ALWAYS_CREATE);
            mScriptModule->AddScriptSection(mUuidString.c_str(),mSource.c_str());
            int r = mScriptModule->Build();
            if(r < 0)
            {
                #ifdef DREAM_LOG
                getLog()->error("Create script error");
                mError = true;
                #endif
            }
            else if (!mContext)
            {
                mContext = ScriptComponent::Engine->CreateContext();
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    string
    ScriptRuntime::getSource
    ()
    const
    {
        return mSource;
    }

    void
    ScriptRuntime::setSource
    (const string& source)
    {
        mSource = source;
    }

    // Function Execution =======================================================

    bool
    ScriptRuntime::executeOnUpdate
    (SceneObjectRuntime* sor)
    {
        if (mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (!mUpdateFunction)
            {
                mUpdateFunction = mScriptModule->GetFunctionByName(Constants::LUA_UPDATE_FUNCTION.c_str());
            }
            if (mUpdateFunction)
            {
                mContext->Prepare(mUpdateFunction);
                mContext->SetArgObject(0,sor);
                int r = mContext->Execute();
                if (r < 0)
                {
                    mError = true;
                }
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::executeOnInit
    (SceneObjectRuntime* sor)
    {
        if (mError)
        {
           #ifdef DREAM_LOG
            getLog()->error("Cannot init, script for {} in error state.",sor->getNameAndUuidString());
            #endif
            return true;
        }
        if (mInitialised)
        {
           #ifdef DREAM_LOG
            getLog()->trace("Script has all ready been initialised for {}",sor->getNameAndUuidString());
            #endif
            return true;
        }

        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (!mInitFunction)
            {
                mInitFunction =  mScriptModule->GetFunctionByName(Constants::LUA_INIT_FUNCTION.c_str());
            }
            if (mInitFunction)
            {
                mContext->Prepare(mInitFunction);
                mContext->SetArgObject(0,sor);
                int r = mContext->Execute();
                if (r < 0)
                {
                    mError = true;
                }
                else
                {
                    mInitialised = true;
                }
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::executeOnEvent
    (SceneObjectRuntime* sor)
    {
        if (mError)
        {
           #ifdef DREAM_LOG
            getLog()->error( "Cannot execute {} in error state ",  getNameAndUuidString());
            #endif
            sor->clearEventQueue();
            return true;
        }

        if (!mInitialised)
        {
            #ifdef DREAM_LOG
            getLog()->error("Script has not been initialised");
            #endif
            return true;
        }

        if (!sor->hasEvents())
        {
            return true;
        }

        #ifdef DREAM_LOG
        getLog()->debug( "Calling onEvent for {}", sor->getNameAndUuidString());
        #endif

        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (!mEventFunction)
            {
                mEventFunction =  mScriptModule->GetFunctionByName(Constants::LUA_EVENT_FUNCTION.c_str());
            }
            if (mEventFunction)
            {
                for (Event e : sor->getEventQueue())
                {
                    mContext->Prepare(mEventFunction);
                    mContext->SetArgObject(0,sor);
                    mContext->SetArgObject(1,&e);
                    int r = mContext->Execute();
                    if (r < 0)
                    {
                        mError = true;
                        break;
                    }
                }
            }
            sor->clearEventQueue();
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::executeOnInput
    (InputComponent* inputComp, SceneRuntime* sr)
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing onInput function with {}",getUuid());
        #endif

        if (mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (!mInputFunction)
            {
                mInputFunction = mScriptModule->GetFunctionByName(Constants::LUA_INPUT_FUNCTION.c_str());
            }
            if (mInputFunction)
            {
                mContext->Prepare(mNanoVGFunction);
                mContext->SetArgObject(0,inputComp);
                mContext->SetArgObject(1,sr);
                int r = mContext->Execute();
                if (r < 0)
                {
                    mError = true;
                }
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }


    bool
    ScriptRuntime::executeOnNanoVG
    (NanoVGComponent* nvg, SceneRuntime* sr)
    {
        bool retval = false;

        #ifdef DREAM_LOG
        getLog()->info( "Calling onNanoVG for {}" , getNameAndUuidString() );
        #endif

        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (!mNanoVGFunction)
            {
                mNanoVGFunction = mScriptModule->GetFunctionByName(Constants::LUA_NANOVG_FUNCTION.c_str());
            }
            if (mNanoVGFunction)
            {
                mContext->Prepare(mNanoVGFunction);
                mContext->SetArgObject(0,nvg);
                mContext->SetArgObject(1,sr);
                int r = mContext->Execute();
                if (r < 0)
                {
                    mError = true;
                }
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool ScriptRuntime::getInitialised() const
    {
       return mInitialised;
    }
}
