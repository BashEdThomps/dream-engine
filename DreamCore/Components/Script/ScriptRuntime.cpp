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

#include "../../deps/angelscript/angelscript.h"
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
          mError(false),
          mSource(""),
          mUuidString(std::to_string(mUuid)),
          mScriptModule(nullptr),
          mInitFunction(nullptr),
          mEventFunction(nullptr),
          mUpdateFunction(nullptr),
          mDestroyFunction(nullptr),
          mInputFunction(nullptr),
          mNanoVGFunction(nullptr)
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
        mConstructionTask.setScript(this);
        mConstructionTask.setState(TaskState::QUEUED);
        mLoaded = false;
        return true;
    }

    bool ScriptRuntime::createScript()
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
            mScriptModule = ScriptComponent::Engine->GetModule(mUuidString.c_str(),asGM_CREATE_IF_NOT_EXISTS);
            mScriptModule->AddScriptSection(mUuidString.c_str(),mSource.c_str());
            #ifdef DREAM_LOG
            getLog()->trace("Loaded Script Source\n{}\n",getSource());
            #endif
            int r = mScriptModule->Build();
            if(r < 0)
            {
                #ifdef DREAM_LOG
                getLog()->error("Create script error");
                mError = true;
                #endif
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
            if (mScriptModule)
            {
                if (!mUpdateFunction)
                {
                    mUpdateFunction = mScriptModule->GetFunctionByName
                        (Constants::SCRIPT_UPDATE_FUNCTION.c_str());
                }
                if (mUpdateFunction)
                {
                    auto ctx = ScriptComponent::Engine->RequestContext();
                    ctx->Prepare(mUpdateFunction);
                    ctx->SetArgObject(0,sor);
                    int r = ctx->Execute();
                    if (r < 0)
                    {
                        mError = true;
                    }
                    ScriptComponent::Engine->ReturnContext(ctx);
                }
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::executeOnDestroy
    (uint32_t destroyedSo, SceneObjectRuntime* parent)
    {
        if (mError)
        {
           #ifdef DREAM_LOG
            getLog()->error("Cannot run destroy, script for {} in error state.",destroyedSo);
            #endif
            return true;
        }

        if (!mInitialised[destroyedSo])
        {
            #ifdef DREAM_LOG
            getLog()->trace("Attempted to destroy uninitialised object {}",destroyedSo);
            #endif
            assert(false);
            return true;
        }

        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (mScriptModule)
            {
                if (!mDestroyFunction)
                {
                    mDestroyFunction = mScriptModule->GetFunctionByName
                        (Constants::SCRIPT_DESTROY_FUNCTION.c_str());
                }

                if (mDestroyFunction)
                {
                    auto ctx = ScriptComponent::Engine->RequestContext();
                    ctx->Prepare(mInitFunction);
                    ctx->SetArgDWord(0,destroyedSo);
                    ctx->SetArgObject(1,parent);
                    int r = ctx->Execute();
                    if (r < 0)
                    {
                        mError = true;
                    }
                    removeInitialisedFlag(destroyedSo);
                    ScriptComponent::Engine->ReturnContext(ctx);
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

        if (mInitialised[sor->getUuid()])
        {
           #ifdef DREAM_LOG
            getLog()->trace("Script has all ready been initialised for {}",sor->getNameAndUuidString());
            #endif
            return true;
        }

        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (mScriptModule)
            {
                if (!mInitFunction)
                {
                    mInitFunction =  mScriptModule->GetFunctionByName
                        (Constants::SCRIPT_INIT_FUNCTION.c_str());
                }
                if (mInitFunction)
                {
                    auto ctx = ScriptComponent::Engine->RequestContext();
                    ctx->Prepare(mInitFunction);
                    ctx->SetArgObject(0,sor);
                    int r = ctx->Execute();
                    if (r < 0)
                    {
                        mError = true;
                    }
                    else
                    {
                        mInitialised[sor->getUuid()] = true;
                    }
                    ScriptComponent::Engine->ReturnContext(ctx);
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

        if (!mInitialised[sor->getUuid()])
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
            if (mScriptModule)
            {
                if (!mEventFunction)
                {
                    mEventFunction = mScriptModule->GetFunctionByName
                        (Constants::SCRIPT_EVENT_FUNCTION.c_str());
                }
                if (mEventFunction)
                {
                    for (Event e : sor->getEventQueue())
                    {
                        auto ctx = ScriptComponent::Engine->RequestContext();
                        ctx->Prepare(mEventFunction);
                        ctx->SetArgObject(0,sor);
                        ctx->SetArgObject(1,&e);
                        int r = ctx->Execute();
                        if (r < 0)
                        {
                            mError = true;
                            break;
                        }
                        ScriptComponent::Engine->ReturnContext(ctx);
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
            if (mScriptModule)
            {
                if (!mInputFunction)
                {
                    mInputFunction = mScriptModule->GetFunctionByName
                        (Constants::SCRIPT_INPUT_FUNCTION.c_str());
                }
                if (mInputFunction)
                {
                    auto ctx = ScriptComponent::Engine->RequestContext();
                    ctx->Prepare(mInputFunction);
                    ctx->SetArgObject(0,inputComp);
                    ctx->SetArgObject(1,sr);
                    int r = ctx->Execute();
                    if (r < 0)
                    {
                        mError = true;
                    }
                    ScriptComponent::Engine->ReturnContext(ctx);
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
        #ifdef DREAM_LOG
        getLog()->info( "Calling onNanoVG for {}" , getNameAndUuidString() );
        #endif

        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (mScriptModule)
            {
                if (!mNanoVGFunction)
                {
                    mNanoVGFunction = mScriptModule->GetFunctionByName(Constants::SCRIPT_NANOVG_FUNCTION.c_str());
                }
                if (mNanoVGFunction)
                {
                    auto ctx = ScriptComponent::Engine->RequestContext();
                    ctx->Prepare(mNanoVGFunction);
                    ctx->SetArgObject(0,nvg);
                    ctx->SetArgObject(1,sr);
                    int r = ctx->Execute();
                    if (r < 0)
                    {
                        mError = true;
                    }
                    ScriptComponent::Engine->ReturnContext(ctx);
                }
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::getInitialised
    (SceneObjectRuntime* sor)
    {
       return mInitialised[sor->getUuid()];
    }

    ScriptConstructionTask*
    ScriptRuntime::getConstructionTask
    ()
    {
        return &mConstructionTask;
    }

    void
    ScriptRuntime::removeInitialisedFlag(uint32_t id)
    {
        auto itr = mInitialised.find(id);
        if (itr != mInitialised.end())
        {
            mInitialised.erase(itr);
        }
    }
}
