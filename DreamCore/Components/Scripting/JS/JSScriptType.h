#pragma once

#include "JSScriptObject.h"
#include <string>

using std::string;

/**
 * Getter and setter functions for our classes to V8
 */
typedef v8::Local<v8::Value> (*ScriptTypeGetterFunc)(v8::Isolate*,void*);
typedef void (*ScriptTypeSetterFunc)(void*,v8::Local<v8::Value>);

/**
 * A storage for getter and setter to variable pairs
 */
namespace Dream
{
    class JSScriptTypeCallbackPair
    {
    public:
        JSScriptTypeCallbackPair()
            :  getter(nullptr),
              setter(nullptr),
              next(nullptr)
        {

        }

        ~JSScriptTypeCallbackPair()
        {
        }

    public:
        string variableName;
        ScriptTypeGetterFunc getter;
        ScriptTypeSetterFunc setter;
        JSScriptTypeCallbackPair* next;
    };

    /**
     * A scripted global object that can be accessed and created in JS
     */
    template<class T>
    class JSScriptType : public JSScriptObject
    {
    public:
        JSScriptType()
        {
        }

        ~JSScriptType()
        {
        }

        static void
        StartTemplate
        (v8::Isolate* isolate)
        {
            // Start template
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
            mFunctionTemplate.Reset(isolate, tpl);
            mIsolate = isolate;
        }

        static void
        SetTypeName
        (string name)
        {
            mTypeName = name;
            v8::Local<v8::FunctionTemplate> tpl = mFunctionTemplate.Get(mIsolate);
            tpl->SetClassName(v8::String::NewFromUtf8(mIsolate, mTypeName.c_str()));
        }

        static void
        AddVariable
        (string name, ScriptTypeGetterFunc getter, ScriptTypeSetterFunc setter)
        {
            // Create a new variable function callback
            JSScriptTypeCallbackPair* pair = new JSScriptTypeCallbackPair();

            // Copy params
            pair->variableName = name;
            pair->getter = getter;
            pair->setter = setter;

            // Set this up as an accessible variable name
            v8::Local<v8::FunctionTemplate> tpl = mFunctionTemplate.Get(mIsolate);
            tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(mIsolate, name.c_str()), Getter, Setter);

            if(mVarList == nullptr)
            {
                mVarList = pair;
                return;
            }

            JSScriptTypeCallbackPair* current = mVarList;
            while(current->next)
            {
                current = current->next;
            }

            current->next = pair;
        }

        static void
        EndTemplate
        (v8::Local<v8::Context> context)
        {
            // Set our internal field count to 1 to we can house our "this" object for reference later
            v8::Local<v8::FunctionTemplate> tpl = mFunctionTemplate.Get(mIsolate);
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Get our constructor for this type
            mConstructor.Reset(mIsolate, tpl->GetFunction());
            v8::Local<v8::Function> constructor = mConstructor.Get(mIsolate);

            // Inject this type name into V8
            context->Global()->Set(v8::String::NewFromUtf8(mIsolate, mTypeName.c_str()), constructor);
        }

        static void
        AddFunction
        (string name, v8::FunctionCallback func)
        {
            v8::Local<v8::FunctionTemplate> tpl = mFunctionTemplate.Get(mIsolate);
            tpl->PrototypeTemplate()->Set
            (
                v8::String::NewFromUtf8(mIsolate, name.c_str()),
                v8::FunctionTemplate::New(mIsolate, func)
            );
        }

        static void
        New
        (const v8::FunctionCallbackInfo<v8::Value>& info)
        {
            v8::HandleScope scope(info.GetIsolate());
            T* obj = new T(info);
            obj->Wrap(info.This());
            return info.GetReturnValue().Set(info.This());
        }

        static v8::Local<v8::Object>
        NewInstance
        ()
        {
            v8::HandleScope scope(mIsolate);
            v8::Local<v8::Function> constructor = mConstructor.Get(mIsolate);
            v8::Local<v8::Object> instance = constructor->NewInstance
            (mIsolate->GetCurrentContext()).ToLocalChecked();
            return instance;
        }

        static void
        Getter
        (v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
        {
            // Unwrap our object
            const T* obj = JSScriptObject::Unwrap<T>(info.This());
            // Iterate through the callback list, looking for a registered callback function
            JSScriptTypeCallbackPair* current = mVarList;
            while(current)
            {
                v8::String::Utf8Value propName(property);
                if(current->variableName.compare(*propName) == 0)
                {
                    if(current->getter != nullptr)
                    {
                        v8::Local<v8::Value> val = current->getter(mIsolate, (void*)(obj));
                        info.GetReturnValue().Set(val);
                        return;
                    }
                }
            }
        }

        static void
        Setter
        (
            v8::Local<v8::String> property,
            v8::Local<v8::Value> value,
            const v8::PropertyCallbackInfo<void>& info
        )
        {
            T* obj = JSScriptObject::Unwrap<T>(info.This());
            // Iterate through the callback list, looking for a registered callback function
            JSScriptTypeCallbackPair* current = mVarList;
            while(current)
            {
                v8::String::Utf8Value propName(property);
                if(current->variableName.compare(*propName) == 0)
                {
                    if(current->setter != nullptr)
                    {
                        current->setter(static_cast<void*>(obj), value);
                        return;
                    }
                }
            }
        }

    protected:
        // Our constructor to create new objects of this type
        static v8::Persistent<v8::Function> mConstructor;
        // The function template
        static v8::Persistent<v8::FunctionTemplate> mFunctionTemplate;
        // The name of this type inside the JS context
        static string mTypeName;
        // The current isolate
        static v8::Isolate* mIsolate;
        // Linked list of getter/setter combinations for vars
        static JSScriptTypeCallbackPair* mVarList;
    };

    template<class T> v8::Persistent<v8::Function> JSScriptType<T>::mConstructor;
    template<class T> v8::Persistent<v8::FunctionTemplate> JSScriptType<T>::mFunctionTemplate;
    template<class T> string JSScriptType<T>::mTypeName;
    template<class T> v8::Isolate* JSScriptType<T>::mIsolate = nullptr;
    template<class T> JSScriptTypeCallbackPair* JSScriptType<T>::mVarList = nullptr;
}
