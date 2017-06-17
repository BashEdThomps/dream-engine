#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <json.hpp>

using namespace std;

namespace Dream
{
    class Scene;
    class SceneObjectRuntime;
    class SceneObjectDefinition;

    class SceneObject
    {
    private:
        unique_ptr<SceneObjectRuntime> mRuntime;
        unique_ptr<SceneObjectDefinition> mDefinition;
        Scene* mSceneHandle;
        vector<SceneObject*> mChildren;
        SceneObject* mParentHandle;

    public:
        SceneObject(Scene* scene = nullptr, nlohmann::json soJson = nlohmann::json());
        ~SceneObject();

        Scene* getSceneHandle();
        SceneObjectRuntime* getRuntimeHandle();
        SceneObjectDefinition* getDefinitionHandle();

        SceneObject* getChildByUuid(string);

        int countAllChildren();
        size_t countChildren();
        void addChild(SceneObject*);
        void removeChild(SceneObject*);
        bool isChildOf(SceneObject*);
        vector<SceneObject*> getChildren();

        bool isParentOf(SceneObject* child);
        void setParentHandle(SceneObject* parent);
        SceneObject* getParentHandle();

        void* applyToAll(function<void*(SceneObject*)>);
        bool applyToAll(function<bool(SceneObject*)>);

    }; // End of SceneObject

} // End of Dream

