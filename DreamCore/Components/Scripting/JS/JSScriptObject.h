#pragma once

#include <libplatform/libplatform.h>
#include <v8.h>

namespace Dream
{
    /**
     * A helper class that other types can be inherited from (handles basic V8 integration)
     */
    class ScriptObject {
    public:
        ScriptObject();
        ~ScriptObject();

        /**
         * Unwrap a v8::object back into a C++ object
         */
        template <class T>
        static inline T* Unwrap(v8::Local<v8::Object> handle)
        {
            assert(!handle.IsEmpty());
            assert(handle->InternalFieldCount() > 0);

            // Cast to ObjectWrap before casting to T.  A direct cast from void
            // to T won't work right when T has more than one base class.
            void* ptr = handle->GetAlignedPointerFromInternalField(0);
            ScriptObject* wrap = static_cast<ScriptObject*>(ptr);
            return static_cast<T*>(wrap);
        }

        /**
         * Wrap a c++ object to pass back into v8
         */
        inline void Wrap(v8::Local<v8::Object> handle)
        {
            assert(m_handle.IsEmpty());
            assert(handle->InternalFieldCount() > 0);

            handle->SetAlignedPointerInInternalField(0, this);
            m_handle.Reset(v8::Isolate::GetCurrent(), handle);
        }

    protected:
        v8::Persistent<v8::Object> m_handle;
    };

}
