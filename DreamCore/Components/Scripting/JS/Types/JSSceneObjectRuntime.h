#pragma once

#include <v8.h>
#include <glm/vec3.hpp>
#include "../JSScriptType.h"
#include "../../../../Scene/SceneObject/SceneObjectRuntime.h"

using namespace glm;
using Dream::JSScriptType;
using Dream::SceneObjectRuntime;

namespace Dream {

    /**
     * A "vector3" class exposed to JS
     */
    class JSSceneObjectRuntime : public JSScriptType<JSSceneObjectRuntime>
    {
    public:
        JSSceneObjectRuntime();
        JSSceneObjectRuntime(const v8::FunctionCallbackInfo<v8::Value>& info);
        ~JSSceneObjectRuntime();

        /**
         * Initialize this type into V8's scope
         */
        static void Initialize(v8::Isolate* isolate, v8::Local<v8::Context> context);

        /**
         * Accessor functions
         */
        static v8::Local<v8::Value> GetName(v8::Isolate* isolate, void* obj);
        static void SetName(void* obj, v8::Local<v8::Value> value);


        // Get our vector
        shared_ptr<SceneObjectRuntime>
        GetSceneObjectRuntime
        ()
        {
            return mSceneObjectRuntime;
        }

        void
        SetSceneObjectRuntime
        (shared_ptr<SceneObjectRuntime> obj)
        {
            mSceneObjectRuntime = obj;
        }

    protected:
        // Our internal vector type
        shared_ptr<SceneObjectRuntime> mSceneObjectRuntime;
    };

}
