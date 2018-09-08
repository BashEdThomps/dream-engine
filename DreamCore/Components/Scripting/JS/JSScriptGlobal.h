#pragma once

#include <libplatform/libplatform.h>
#include <v8.h>
/**
 * Getter and setter functions for our classes to V8
 */
typedef v8::Local<v8::Value> (*ScriptGlobalGetterFunc)(v8::Isolate*);

namespace Dream
{

    /**
     * A storage for getter and setter to variable pairs
     */
    class ScriptGlobalCallbackPair
    {
    public:
        ScriptGlobalCallbackPair
        () :
            variableName(nullptr),
            getter(nullptr),
            next(nullptr)
        { }

        ~ScriptGlobalCallbackPair
        ()
        {
        }

    public:
        char* variableName;
        ScriptGlobalGetterFunc getter;
        ScriptGlobalCallbackPair* next;
    };

    /**
     * A scripted type that can be accessed and created in JS
     */
    template<class T>
    class ScriptGlobal : public ScriptObject
    {
    public:
        ScriptGlobal()
        { }
        ~ScriptGlobal()
        { }

        static void StartTemplate(v8::Isolate* isolate)
        {
            // Start template
            v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New(isolate);
            m_objectTemplate.Reset(isolate, tpl);
            m_isolate = isolate;
        }

        static void SetGlobalName(char* name)
        {
            m_typeName = static_cast<char*>(malloc(strlen(name) + 1));
            strcpy(m_typeName, name);
        }

        static void AddVariable(char* name, ScriptGlobalGetterFunc getter)
        {
            // Create a new variable function callback
            ScriptGlobalCallbackPair* pair = new ScriptGlobalCallbackPair();

            // Copy params
            pair->variableName = static_cast<char*>(malloc(strlen(name) + 1));
            strcpy(pair->variableName, name);
            pair->getter = getter;

            // Set this up as an accessible variable name
            v8::Local<v8::ObjectTemplate> tpl = m_objectTemplate.Get(m_isolate);
            tpl->SetAccessor(v8::String::NewFromUtf8(m_isolate, name), Getter);

            if(m_varList == 0)
            {
                m_varList = pair;
                return;
            }

            ScriptGlobalCallbackPair* current = m_varList;
            while(current->next)
            {
                current = current->next;
            }

            current->next = pair;
        }

        static void EndTemplate(v8::Local<v8::Context> context)
        {
            // Inject this type name into V8
            v8::Local<v8::ObjectTemplate> tpl = m_objectTemplate.Get(m_isolate);
            context->Global()->Set(v8::String::NewFromUtf8(m_isolate, m_typeName), tpl->NewInstance());
        }

        static void AddFunction(char* name, v8::FunctionCallback func)
        {
            v8::Local<v8::ObjectTemplate> tpl = m_objectTemplate.Get(m_isolate);
            tpl->Set(v8::String::NewFromUtf8(m_isolate, name), v8::FunctionTemplate::New(m_isolate, func));
        }

        static void Getter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
        {
            // Iterate through the callback list, looking for a registered callback function
            ScriptGlobalCallbackPair* current = m_varList;
            while(current)
            {
                v8::String::Utf8Value propName(property);
                if(strcmp(current->variableName, *propName) == 0)
                {
                    if(current->getter != nullptr)
                    {
                        v8::Local<v8::Value> val = current->getter(m_isolate);
                        info.GetReturnValue().Set(val);
                        return;
                    }
                }
            }
        }

    protected:
        // The function template
        static v8::Persistent<v8::ObjectTemplate> m_objectTemplate;

        // The name of this type inside the JS context
        static char* m_typeName;

        // The current isolate
        static v8::Isolate* m_isolate;

        // Linked list of getter/setter combinations for vars
        static ScriptGlobalCallbackPair* m_varList;
    };

    template<class T> v8::Persistent<v8::ObjectTemplate> ScriptGlobal<T>::m_objectTemplate;
    template<class T> char* ScriptGlobal<T>::m_typeName = nullptr;
    template<class T> v8::Isolate* ScriptGlobal<T>::m_isolate = nullptr;
    template<class T> ScriptGlobalCallbackPair* ScriptGlobal<T>::m_varList = nullptr;
}
