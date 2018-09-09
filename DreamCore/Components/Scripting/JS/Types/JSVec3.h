#pragma once

#include <v8.h>
#include <glm/vec3.hpp>
#include "../JSScriptType.h"

using namespace glm;
using Dream::JSScriptType;

namespace Dream {

    /**
     * A "vector3" class exposed to JS
     */
    class JSVec3 : public JSScriptType<JSVec3>
    {
    public:
        JSVec3();
        JSVec3(const v8::FunctionCallbackInfo<v8::Value>& info);
        ~JSVec3();

        /**
         * Initialize this type into V8's scope
         */
        static void Initialize(v8::Isolate* isolate, v8::Local<v8::Context> context);

        /**
         * Accessor functions
         */
        static v8::Local<v8::Value> GetX(v8::Isolate* isolate, void* obj);
        static v8::Local<v8::Value> GetY(v8::Isolate* isolate, void* obj);
        static v8::Local<v8::Value> GetZ(v8::Isolate* isolate, void* obj);

        static void SetX(void* obj, v8::Local<v8::Value> value);
        static void SetY(void* obj, v8::Local<v8::Value> value);
        static void SetZ(void* obj, v8::Local<v8::Value> value);

        /**
         * Prototype functions
         */
        static void Normalize(const v8::FunctionCallbackInfo<v8::Value>& info);

        // Get our vector
        vec3& GetVector() { return mVector; }
        void SetVector(vec3& vector) { mVector = vector; }

    protected:
        // Our internal vector type
        vec3 mVector;
    };

}
