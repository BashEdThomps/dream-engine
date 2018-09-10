/*
 * JSScriptInstance
 *
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

#include "JSScriptInstance.h"
#include "../ScriptDefinition.h"
#include "../ScriptCache.h"

#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{

    JSScriptInstance::JSScriptInstance
    (
        shared_ptr<ScriptDefinition> definition,
        shared_ptr<SceneObjectRuntime> transform
    )
        : ScriptInstance(definition,transform),
          mError(false)
    {
        setLogClassName("JSScriptInstance");
        auto log = getLog();
        log->trace( "Constructing {}", mDefinition->getNameAndUuidString() );
        return;
    }

    JSScriptInstance::~JSScriptInstance
    ()
    {
        auto log = getLog();
        log->trace("Destructing ", mDefinition->getNameAndUuidString() );
    }

    bool
    JSScriptInstance::load
    (string projectPath)
    {
        mAbsolutePath = projectPath + mDefinition->getAssetPath();
        return true;
    }

    bool
    JSScriptInstance::inflate
    (shared_ptr<ScriptCache> mCache)
    {
        auto log = getLog();
        log->info( "Inflating script at {}" , mAbsolutePath );

        string path = getAbsolutePath();
        string src = mCache->getScript(path);

        log->info("\n\n{}",src);

        v8::Isolate* isolate = v8::Isolate::GetCurrent();

        log->info("Got Current V8 Isolate");

        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);

        log->info("Got Handle Scope");

        // Create an instance of the global context so we can plug variables in
        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

        log->info("Got Global");

        v8::Local<v8::Context> localContext = v8::Context::New(isolate, nullptr, global);
        mContext.Reset(isolate, localContext);
        localContext->Enter();

        log->info("Entered Local Context");

        // Catch any errors the script might throw
        v8::TryCatch try_catch(isolate);

        // Copy source
        v8::Local<v8::String> scriptSrc = v8::String::NewFromUtf8
        (
            isolate,
            src.c_str(),
            v8::NewStringType::kNormal
        ).ToLocalChecked();

        v8pp::module dream_lib(isolate);

        v8pp::class_<glm::vec3> vec3_ = v8pp::class_<glm::vec3>(isolate);
        vec3_
             .ctor<>()
             .ctor<float>()
             .ctor<float,float,float>()
             .set("x",&glm::vec3::x)
             .set("y",&glm::vec3::y)
             .set("z",&glm::vec3::z);

        dream_lib.set("vec3",vec3_);

        v8pp::class_<Transform3D, v8pp::shared_ptr_traits> transform3D = v8pp::class_<Transform3D, v8pp::shared_ptr_traits>(isolate);
        transform3D
            .set("type",v8pp::property(&Transform3D::getTransformType,&Transform3D::setTransformType));

        dream_lib.set("Transform3D",transform3D);

        v8pp::class_<SceneObjectRuntime, v8pp::shared_ptr_traits> sor = v8pp::class_<SceneObjectRuntime, v8pp::shared_ptr_traits>(isolate);
        sor.set("name",v8pp::property(&SceneObjectRuntime::getName, &SceneObjectRuntime::setName))
           .set("uuid", v8pp::property(&SceneObjectRuntime::getUuid, &SceneObjectRuntime::setUuid))
           .set("translation", v8pp::property(&SceneObjectRuntime::getTranslation))
           .set("transform",v8pp::property(&SceneObjectRuntime::getTransform, &SceneObjectRuntime::setTransform));

        dream_lib.set("SceneObjectRuntime",sor);

        isolate
            ->GetCurrentContext()
            ->Global()
            ->Set(v8::String::NewFromUtf8(isolate, "dream"), dream_lib.new_instance());

        mPersistentSOR.Reset(isolate, sor.reference_external(isolate, mSceneObjectRuntime));

        v8::Local<v8::Script> script;
        if (v8::Script::Compile(localContext, scriptSrc).ToLocal(&script) == false)
        {
            v8::String::Utf8Value error(try_catch.Exception());
            log->error("Unable to compile script file {}", static_cast<char*>(*error));
            return false;
        }

        log->info("Compiled Source");

        v8::Local<v8::Value> result;
        if (script->Run(localContext).ToLocal(&result) == false)
        {
            v8::String::Utf8Value error(try_catch.Exception());
            log->error("Unable to execute script file {}", static_cast<char*>(*error));
            return false;
        }

        log->info("Finished running source");

        // Pull out the functions we care about
        v8::Local<v8::Object> globalSpace = isolate->GetCurrentContext()->Global();

        // Make sure the update function is a function

        // Init =================================================================
        v8::Local<v8::Value> onInitFuncName = globalSpace->Get
        (
            v8::String::NewFromUtf8(isolate, Constants::LUA_INIT_FUNCTION.c_str())
        );
        mOnInitFunction.Reset(isolate, onInitFuncName.As<v8::Function>());
        v8::String::Utf8Value onInitType(onInitFuncName->TypeOf(isolate));
        if(!onInitFuncName->IsFunction())
        {
            log->error("Did not find onInit function in {}", getNameAndUuidString());
            mOnInitFunction.Reset();
        }
        else
        {
            log->info("Found onInit in {}",getNameAndUuidString());
        }

        // Input ================================================================
        v8::Local<v8::Value> onInputFuncName = globalSpace->Get
        (
            v8::String::NewFromUtf8(isolate, Constants::LUA_INPUT_FUNCTION.c_str())
        );
        mOnInputFunction.Reset(isolate, onInputFuncName.As<v8::Function>());
        v8::String::Utf8Value onInputType(onInputFuncName->TypeOf(isolate));
        if(!onInputFuncName->IsFunction())
        {
            log->error("Did not find onInput function in {}", getNameAndUuidString());
            mOnInputFunction.Reset();
        }
        else
        {
            log->info("Found onInput in {}",getNameAndUuidString());
        }

        // Event ================================================================
        v8::Local<v8::Value> onEventFuncName = globalSpace->Get
        (
            v8::String::NewFromUtf8(isolate, Constants::LUA_EVENT_FUNCTION.c_str())
        );
        mOnEventFunction.Reset(isolate, onEventFuncName.As<v8::Function>());
        v8::String::Utf8Value onEventType(onEventFuncName->TypeOf(isolate));
        if(!onEventFuncName->IsFunction())
        {
            log->error("Did not find onEvent function in {}", getNameAndUuidString());
            mOnEventFunction.Reset();
        }
        else
        {
            log->info("Found onEvent in {}",getNameAndUuidString());
        }

        // Update ===============================================================
        v8::Local<v8::Value> onUpdateFuncName = globalSpace->Get
        (
            v8::String::NewFromUtf8(isolate, Constants::LUA_UPDATE_FUNCTION.c_str())
        );
        mOnUpdateFunction.Reset(isolate, onUpdateFuncName.As<v8::Function>());
        v8::String::Utf8Value onUpdateType(onUpdateFuncName->TypeOf(isolate));
        if(!onUpdateFuncName->IsFunction())
        {
            log->error("Did not find onUpdate function in ", getNameAndUuidString());
            mOnUpdateFunction.Reset();
        }
        else
        {
            log->info("Found onUpdate in {}",getNameAndUuidString());
        }

        // Update NanoVG ===========================================================
        v8::Local<v8::Value> onNanoVGFuncName = globalSpace->Get
        (
            v8::String::NewFromUtf8(isolate, Constants::LUA_NANOVG_FUNCTION.c_str())
        );
        mOnNanoVGFunction.Reset(isolate, onNanoVGFuncName.As<v8::Function>());
        v8::String::Utf8Value onNanoVGType(onNanoVGFuncName->TypeOf(isolate));
        if(!onNanoVGFuncName->IsFunction())
        {
            log->error("Did not find onNanoVG function in ", getNameAndUuidString());
            mOnNanoVGFunction.Reset();
        }
        else
        {
            log->info("Found onNanoVG in {}",getNameAndUuidString());
        }
        // Save script
        mScript.Reset(isolate, script);
        localContext->Exit();

        log->info("JS Script loaded successfuly");

        return true;
    }

    void
    JSScriptInstance::update
    ()
    {

    }

    bool
    JSScriptInstance::getError
    ()
    {
        return mError;
    }

    void
    JSScriptInstance::setError
    (bool err)
    {
        mError = err;
    }

    void JSScriptInstance::loadExtraAttributes(nlohmann::json)
    {

    }

    void JSScriptInstance::onInit(shared_ptr<SceneObjectRuntime> so)
    {

    }

    void JSScriptInstance::onEvent(shared_ptr<SceneObjectRuntime> so, Event ev)
    {

    }

    void JSScriptInstance::onInput(shared_ptr<SceneObjectRuntime> so, gainput::InputMap* iMap)
    {

    }

    void JSScriptInstance::onUpdate(shared_ptr<SceneObjectRuntime> so)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);
        // Create an instance of the global context so we can plug variables in
        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
        // Reset our current local context
        v8::Local<v8::Context> localContext = mContext.Get(isolate);
        localContext->Enter();
        // Catch any errors the script might throw
        v8::TryCatch try_catch(isolate);
        // Script local
        v8::Local<v8::Script> script = mScript.Get(isolate);
        v8::Local<v8::Value> result;

        // Run specific functions
        v8::Local<v8::Function> updateFunc = mOnUpdateFunction.Get(isolate);
        v8::Handle<v8::Value> argv[1];

        argv[0] =  mPersistentSOR.Get(isolate);
        result = updateFunc->Call(updateFunc->CreationContext()->Global(), 1, argv);

        if (try_catch.HasCaught())
        {
            v8::String::Utf8Value error(try_catch.Exception());
            cerr << "JSSCriptInstance: "<< getNameAndUuidString()
                 << " Unable to update " << static_cast<char*>(*error) << endl;
        }

        if (!result.IsEmpty())
        {
             // get the param
            v8::String::Utf8Value retvalJS(result->ToString());
            // convert it to string
            std::string retval = std::string(*retvalJS);
            cerr << "V8 ---> Got update result:  update " << retval << endl;
        }

        localContext->Exit();
    }

    void JSScriptInstance::onNanoVG(shared_ptr<SceneObjectRuntime> so)
    {

    }

} // End of Dream
