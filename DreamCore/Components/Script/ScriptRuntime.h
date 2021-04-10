#pragma once

#include "Components/SharedAssetRuntime.h"
#include "Components/Event.h"

namespace octronic::dream
{
    class ScriptDefinition;
    class InputComponent;
    class EntityRuntime;
    class SceneRuntime;

    class ScriptRuntime : public SharedAssetRuntime
    {
    public:
        ScriptRuntime(ProjectRuntime&,ScriptDefinition&);
        ScriptRuntime(ScriptRuntime&&) = default;
        ScriptRuntime& operator=(ScriptRuntime&&) = default;

        bool loadFromDefinition() override;

        bool createEntityState(EntityRuntime& rt);
        bool removeEntityState(UuidType uuid);

        string getSource() const;
        void setSource(const string& source);

        bool executeOnInit(EntityRuntime& state);
        bool executeOnUpdate(EntityRuntime& state);
        bool executeOnEvent(EntityRuntime& state);

        bool executeOnInput(SceneRuntime&);

        bool registerInputScript();
        bool removeInputScript();

        void pushTasks() override;

        bool hasSource() const;

    private:
        string mSource;
    };
}
