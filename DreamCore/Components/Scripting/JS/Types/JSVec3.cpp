#include "JSVec3.h"
#include "../JSScriptObject.h"
#include <string>
#include <glm/glm.hpp>

using std::string;

namespace Dream
{
    JSVec3::JSVec3()
    {
        mVector.x = mVector.y = mVector.z = 0.0f;
    }

    JSVec3::JSVec3(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
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

        mVector.x = x;
        mVector.y = y;
        mVector.z = z;
    }

    void JSVec3::Initialize(v8::Isolate* isolate, v8::Local<v8::Context> context)
    {
        StartTemplate(isolate);
        SetTypeName("Vector3");
        AddVariable("x", GetX, SetX);
        AddVariable("y", GetY, SetY);
        AddVariable("z", GetZ, SetZ);
        AddFunction("Normalize", Normalize);
        EndTemplate(context);
    }

    v8::Local<v8::Value> JSVec3::GetX(v8::Isolate* isolate, void* obj)
    {
        JSVec3* object = (JSVec3*)obj;
        return(v8::Number::New(isolate, object->mVector.x));
    }

    v8::Local<v8::Value> JSVec3::GetY(v8::Isolate* isolate, void* obj)
    {
        JSVec3* object = (JSVec3*)obj;
        return(v8::Number::New(isolate, object->mVector.y));
    }

    v8::Local<v8::Value> JSVec3::GetZ(v8::Isolate* isolate, void* obj)
    {
        JSVec3* object = (JSVec3*)obj;
        return(v8::Number::New(isolate, object->mVector.z));
    }

    void JSVec3::SetX(void* obj, v8::Local<v8::Value> value)
    {
        JSVec3* object = (JSVec3*)obj;
        object->mVector.x = (float)value->ToNumber(v8::Isolate::GetCurrent())->Value();
    }

    void JSVec3::SetY(void* obj, v8::Local<v8::Value> value)
    {
        JSVec3* object = (JSVec3*)obj;
        object->mVector.y = (float)value->ToNumber(v8::Isolate::GetCurrent())->Value();
    }

    void JSVec3::SetZ(void* obj, v8::Local<v8::Value> value)
    {
        JSVec3* object = (JSVec3*)obj;
        object->mVector.z = (float)value->ToNumber(v8::Isolate::GetCurrent())->Value();
    }

    // Prototype functions
    void JSVec3::Normalize(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        JSVec3* obj = JSScriptObject::Unwrap<JSVec3>(info.This());
        obj->mVector = glm::normalize(obj->mVector);
        return info.GetReturnValue().Set(info.This());
    }
}
