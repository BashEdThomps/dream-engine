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
    class SceneObjectJsonData;

    class SceneObject
    {
    private:
        unique_ptr<SceneObjectRuntime> mRuntime;
        unique_ptr<SceneObjectJsonData> mJsonData;

    public:
        SceneObject(Scene* scene = nullptr, nlohmann::json soJson = nlohmann::json());
        ~SceneObject();

        SceneObjectRuntime* getRuntime();
        SceneObjectJsonData* getJsonData();

        string getNameAndUuidString();

        void showStatus();

        void* applyToAll(function<void*(SceneObject*)>);
        bool applyToAll(function<bool(SceneObject*)>);

    }; // End of SceneObject

} // End of Dream

