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

#include "angelscript/angelscript.h"
#include "ScriptRuntime.h"
#include "ScriptDefinition.h"
#include "ScriptComponent.h"

#include "../Input/InputComponent.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/Actor/ActorRuntime.h"
#include "../../Project/ProjectRuntime.h"

namespace Dream
{
    void ScriptRuntime::whyYouFail(int r, int line)
    {
        if (r<0)
        {
            string errStr;
            switch (r)
            {
                case asINVALID_DECLARATION:
                    errStr = "Invalid Declaration";
                    break;
                case asINVALID_NAME:
                    errStr = "Invalid Name";
                    break;
                case asNAME_TAKEN:
                    errStr = "Name Taken";
                    break;
                case asERROR:
                    errStr = "Error: not a proper id";
                    break;
                case asALREADY_REGISTERED:
                    errStr = "Already Registered";
                break;
                default:
                    errStr = "Other, Go Check";
                    break;
            }
            cout << "On line: " << line << " errNo: " << r << " Reason: " << errStr << endl;
            //assert(false);
        }
    }
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
          mInputFunction(nullptr)
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
            //mScriptModule->Discard();
            mScriptModule = nullptr;
        }
    }

    bool
    ScriptRuntime::useDefinition
    ()
    {
        mConstructionTask.setScript(this);
        mConstructionTask.setState(TaskState::NEW);
        mLoaded = false;
        return true;
    }

    bool
    ScriptRuntime::getInitialised
    (ActorRuntime* sor)
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
                whyYouFail(r,__LINE__);
                #ifdef DREAM_LOG
                getLog()->error("Create script error");
                mError = true;
                #endif
            }
            mLoaded = true;
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
    (ActorRuntime* sor)
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
                    int r;
                    r = ctx->Prepare(mUpdateFunction); whyYouFail(r,__LINE__);
                    r = ctx->SetArgObject(0,sor); whyYouFail(r,__LINE__);
                    r = ctx->Execute(); whyYouFail(r,__LINE__);
                    if (r < 0)
                    {
                        whyYouFail(r,__LINE__);
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
    (uint32_t destroyedSo, ActorRuntime* parent)
    {
        if (mError)
        {
            #ifdef DREAM_LOG
            getLog()->error("Cannot run destroy, script for {} in error state.",destroyedSo);
            #endif
            return true;
        }

        if (mInitialised.count(destroyedSo) == 0)
        {
            #ifdef DREAM_LOG
            getLog()->error("Scrip {} probably removed before descruction could be called.",destroyedSo);
            #endif
            return true;
        }

        if (!mInitialised[destroyedSo])
        {
            #ifdef DREAM_LOG
            getLog()->trace("Attempted to destroy uninitialised object {}",destroyedSo);
            #endif
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
                    int r;
                    r = ctx->Prepare(mDestroyFunction); whyYouFail(r,__LINE__);
                    r = ctx->SetArgDWord(0,destroyedSo); whyYouFail(r,__LINE__);
                    r = ctx->SetArgObject(1,parent); whyYouFail(r,__LINE__);
                    r = ctx->Execute(); whyYouFail(r,__LINE__);
                    if (r < 0)
                    {
                        whyYouFail(r,__LINE__);
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
    (ActorRuntime* sor)
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
                    int r;
                    r = ctx->Prepare(mInitFunction); whyYouFail(r,__LINE__);
                    r = ctx->SetArgObject(0,sor); whyYouFail(r,__LINE__);
                    r = ctx->Execute(); whyYouFail(r,__LINE__);
                    if (r < 0)
                    {
                        whyYouFail(r,__LINE__);
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
    (ActorRuntime* sor)
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
                    if (sor->tryLockEventQueue())
                    {
                        for (Event& e : *sor->getEventQueue())
                        {
                            auto ctx = ScriptComponent::Engine->RequestContext();
                            int r;
                            r = ctx->Prepare(mEventFunction); whyYouFail(r,__LINE__);
                            r = ctx->SetArgObject(0,sor); whyYouFail(r,__LINE__);
                            r = ctx->SetArgObject(1,&e); whyYouFail(r,__LINE__);
                            ctx->Execute(); whyYouFail(r,__LINE__);
                            if (r < 0)
                            {
                                whyYouFail(r,__LINE__);
                                mError = true;
                                break;
                            }
                            ScriptComponent::Engine->ReturnContext(ctx);
                        }
                        sor->unlockEventQueue();
                    }
                }
            }
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
                    int r;
                    r = ctx->Prepare(mInputFunction); whyYouFail(r,__LINE__);
                    r = ctx->SetArgObject(0,inputComp); whyYouFail(r,__LINE__);
                    r = ctx->SetArgObject(1,sr); whyYouFail(r,__LINE__);
                    r = ctx->Execute(); whyYouFail(r,__LINE__);
                    if (r < 0)
                    {
                        whyYouFail(r,__LINE__);
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
}
