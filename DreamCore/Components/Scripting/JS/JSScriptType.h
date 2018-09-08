#pragma once

#include "JSScriptObject.h"

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
    class ScriptTypeCallbackPair
    {
    public:
        ScriptTypeCallbackPair()
            : variableName(nullptr),
              getter(nullptr),
              setter(nullptr),
              next(nullptr)
        {

        }

        ~ScriptTypeCallbackPair()
        {
        }

    public:
        char* variableName;
        ScriptTypeGetterFunc getter;
        ScriptTypeSetterFunc setter;
        ScriptTypeCallbackPair* next;
    };

    /**
     * A scripted global object that can be accessed and created in JS
     */
    template<class T>
    class ScriptType : public ScriptObject
    {
    public:
        ScriptType()
        {
        }

        ~ScriptType()
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
        (char* name)
        {
            mTypeName = static_cast<char*>(malloc(strlen(name) + 1));
            strcpy(mTypeName, name);
            v8::Local<v8::FunctionTemplate> tpl = mFunctionTemplate.Get(mIsolate);
            tpl->SetClassName(v8::String::NewFromUtf8(mIsolate, mTypeName));
        }

        static void
        AddVariable
        (char* name, ScriptTypeGetterFunc getter, ScriptTypeSetterFunc setter)
        {
            // Create a new variable function callback
            ScriptTypeCallbackPair* pair = new ScriptTypeCallbackPair();

            // Copy params
            pair->variableName = static_cast<char*>(malloc(strlen(name) + 1));
            strcpy(pair->variableName, name);
            pair->getter = getter;
            pair->setter = setter;

            // Set this up as an accessible variable name
            v8::Local<v8::FunctionTemplate> tpl = mFunctionTemplate.Get(mIsolate);
            tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(mIsolate, name), Getter, Setter);

            if(mVarList == nullptr)
            {
                mVarList = pair;
                return;
            }

            ScriptTypeCallbackPair* current = mVarList;
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
            context->Global()->Set(v8::String::NewFromUtf8(mIsolate, mTypeName), constructor);
        }

        static void
        AddFunction
        (char* name, v8::FunctionCallback func)
        {
            v8::Local<v8::FunctionTemplate> tpl = mFunctionTemplate.Get(mIsolate);
            tpl->PrototypeTemplate()->Set
            (
                v8::String::NewFromUtf8(mIsolate, name),
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
            const T* obj = ScriptObject::Unwrap<T>(info.This());
            // Iterate through the callback list, looking for a registered callback function
            ScriptTypeCallbackPair* current = mVarList;
            while(current)
            {
                v8::String::Utf8Value propName(property);
                if(strcmp(current->variableName, *propName) == 0)
                {
                    if(current->getter != nullptr)
                    {
                        v8::Local<v8::Value> val = current->getter(mIsolate, static_cast<void*>(obj));
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
            T* obj = ScriptObject::Unwrap<T>(info.This());
            // Iterate through the callback list, looking for a registered callback function
            ScriptTypeCallbackPair* current = mVarList;
            while(current)
            {
                v8::String::Utf8Value propName(property);
                if(strcmp(current->variableName, *propName) == 0)
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
        static char* mTypeName;
        // The current isolate
        static v8::Isolate* mIsolate;
        // Linked list of getter/setter combinations for vars
        static ScriptTypeCallbackPair* mVarList;
    };

    template<class T> v8::Persistent<v8::Function> ScriptType<T>::mConstructor;
    template<class T> v8::Persistent<v8::FunctionTemplate> ScriptType<T>::mFunctionTemplate;
    template<class T> char* ScriptType<T>::mTypeName = nullptr;
    template<class T> v8::Isolate* ScriptType<T>::mIsolate = nullptr;
    template<class T> ScriptTypeCallbackPair* ScriptType<T>::mVarList = nullptr;
}
