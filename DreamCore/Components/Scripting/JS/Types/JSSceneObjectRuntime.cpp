#include "JSSceneObjectRuntime.h"
#include "../JSScriptObject.h"
#include <string>
#include <glm/glm.hpp>

using std::string;

namespace Dream
{
    JSSceneObjectRuntime::JSSceneObjectRuntime()
    {
    }

    JSSceneObjectRuntime::JSSceneObjectRuntime(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        /*
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        float x, y, z;
        x = y = z = 0.0f;

        if(info.Length() == 1)
        {
            assert(info[0]->IsNumber());
            x = y = z = (float)info[0]->NumberValue();
        }
        else if(info.Length() == 3)
        {
            assert(info[0]->IsNumber());
            assert(info[1]->IsNumber());
            assert(info[2]->IsNumber());

            x = (float)info[0]->ToNumber(isolate)->Value();
            y = (float)info[1]->ToNumber(isolate)->Value();
            z = (float)info[2]->ToNumber(isolate)->Value();
        }

        m_vector.x = x;
        m_vector.y = y;
        m_vector.z = z;
        */
    }

    JSSceneObjectRuntime::~JSSceneObjectRuntime()
    {

    }

    void JSSceneObjectRuntime::Initialize(v8::Isolate* isolate, v8::Local<v8::Context> context)
    {
        StartTemplate(isolate);
        SetTypeName("SceneObjectRuntime");
        AddVariable("name", GetName, SetName);
        EndTemplate(context);
    }

    v8::Local<v8::Value> JSSceneObjectRuntime::GetName(v8::Isolate* isolate, void* obj)
    {
        JSSceneObjectRuntime* object = (JSSceneObjectRuntime*)obj;
        return (v8::String::NewFromUtf8(isolate, object->mSceneObjectRuntime->getName().c_str()));
    }

    void JSSceneObjectRuntime::SetName(void* obj, v8::Local<v8::Value> value)
    {
        JSSceneObjectRuntime* object = (JSSceneObjectRuntime*)obj;
        // get the param
        v8::String::Utf8Value nameJS(value->ToString());
        // convert it to string
        std::string name = std::string(*nameJS);
        object->mSceneObjectRuntime->setName(name);
    }
 }
