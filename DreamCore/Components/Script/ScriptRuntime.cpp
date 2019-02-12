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
          mError(false),
          mInitialised(false),
          mSource(""),
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
            auto scriptModule = ScriptComponent::Engine->GetModule(mUuidString.c_str(),asGM_CREATE_IF_NOT_EXISTS);
            scriptModule->AddScriptSection(mUuidString.c_str(),mSource.c_str());
            int r = scriptModule->Build();
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
            auto scriptModule = ScriptComponent::Engine->GetModule(mUuidString.c_str(),asGM_ONLY_IF_EXISTS);
            if (scriptModule)
            {
                auto context = ScriptComponent::Engine->CreateContext();
                if(context)
                {
                    auto updateFunction = scriptModule->GetFunctionByName(Constants::SCRIPT_UPDATE_FUNCTION.c_str());
                    if (updateFunction)
                    {
                        context->Prepare(updateFunction);
                        context->SetArgObject(0,sor);
                        int r = context->Execute();
                        if (r < 0)
                        {
                            mError = true;
                        }
                        context->Release();
                    }
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
            auto scriptModule = ScriptComponent::Engine->GetModule(mUuidString.c_str(),asGM_ONLY_IF_EXISTS);
            if (scriptModule)
            {
                auto context = ScriptComponent::Engine->CreateContext();
                if(context)
                {
                    auto initFunction =  scriptModule->GetFunctionByName(Constants::SCRIPT_INIT_FUNCTION.c_str());
                    if (initFunction)
                    {
                        context->Prepare(initFunction);
                        context->SetArgObject(0,sor);
                        int r = context->Execute();
                        if (r < 0)
                        {
                            mError = true;
                        }
                        else
                        {
                            mInitialised = true;
                        }
                        context->Release();
                    }
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
            auto scriptModule = ScriptComponent::Engine->GetModule(mUuidString.c_str(),asGM_ONLY_IF_EXISTS);
            if (scriptModule)
            {
                auto context = ScriptComponent::Engine->CreateContext();
                if(context)
                {
                    auto eventFunction =  scriptModule->GetFunctionByName(Constants::SCRIPT_EVENT_FUNCTION.c_str());
                    if (eventFunction)
                    {
                        for (Event e : sor->getEventQueue())
                        {
                            context->Prepare(eventFunction);
                            context->SetArgObject(0,sor);
                            context->SetArgObject(1,&e);
                            int r = context->Execute();
                            if (r < 0)
                            {
                                mError = true;
                                break;
                            }
                        }
                    }
                    context->Release();
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
            auto scriptModule = ScriptComponent::Engine->GetModule(mUuidString.c_str(),asGM_ONLY_IF_EXISTS);
            if (scriptModule)
            {
                auto context = ScriptComponent::Engine->CreateContext();
                if(context)
                {
                    auto inputFunction = scriptModule->GetFunctionByName(Constants::SCRIPT_INPUT_FUNCTION.c_str());
                    if (inputFunction)
                    {
                        context->Prepare(inputFunction);
                        context->SetArgObject(0,inputComp);
                        context->SetArgObject(1,sr);
                        int r = context->Execute();
                        if (r < 0)
                        {
                            mError = true;
                        }
                    }
                    context->Release();
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
            auto scriptModule = ScriptComponent::Engine->GetModule(mUuidString.c_str(),asGM_ONLY_IF_EXISTS);
            if (scriptModule)
            {
                auto context = ScriptComponent::Engine->CreateContext();
                if(context)
                {
                    auto nanoVGFunction = scriptModule->GetFunctionByName(Constants::SCRIPT_NANOVG_FUNCTION.c_str());
                    if (nanoVGFunction)
                    {
                        context->Prepare(nanoVGFunction);
                        context->SetArgObject(0,nvg);
                        context->SetArgObject(1,sr);
                        int r = context->Execute();
                        if (r < 0)
                        {
                            mError = true;
                        }
                    }
                    context->Release();
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
