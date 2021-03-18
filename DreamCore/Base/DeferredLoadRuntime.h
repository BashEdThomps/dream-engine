#pragma once

#include "Runtime.h"
#include <memory>

using std::shared_ptr;

namespace octronic::dream
{
	class ProjectRuntime;

    class DeferredLoadRuntime : public Runtime
    {
    public:
        DeferredLoadRuntime(
                const weak_ptr<ProjectRuntime>& pr,
                const weak_ptr<Definition>& def);
        virtual ~DeferredLoadRuntime();
		virtual bool init();
        weak_ptr<RuntimeLoadFromDefinitionTask> getLoadFromDefinitionTask() const;
		bool getLoaded() const;
        void setLoaded(bool);
        bool getLoadError() const;
        void setLoadError(bool);
        weak_ptr<ProjectRuntime> getProjectRuntime() const;

    protected:
        /**
         * @brief Flag set when the runtime data has been loaded
         */
        bool mLoaded;
        bool mLoadError;
        weak_ptr<ProjectRuntime> mProjectRuntime;
        /**
          * @brief Task to inflate the Runtime from it's definition
          */
        shared_ptr<RuntimeLoadFromDefinitionTask> mLoadFromDefinitionTask;
    };
}
