/*
 * ProjectDefinition.h
 *
 * Created: 16 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include "../Common/IDefinition.h"

using std::string;
using std::vector;
using std::unique_ptr;

namespace Dream
{
    class Project;
    class SceneDefinition;
    class AssetDefinition;

    class ProjectDefinition : public IDefinition
    {
    private:
        vector<unique_ptr<SceneDefinition>> mSceneDefinitions;
        vector<unique_ptr<AssetDefinition>> mAssetDefinitions;
    public:
        ProjectDefinition(json data);
        ~ProjectDefinition();

        string getAuthor();
        void setAuthor(string author);

        string getDescription();
        void setDescription(string description);

        string getStartupSceneUuid();
        void setStartupSceneUuid(string sceneUuid);
        SceneDefinition* getStartupSceneDefinitionHandle();

        int getWindowWidth();
        void setWindowWidth(int width);

        int getWindowHeight();
        void setWindowHeight(int height);

        void loadChildDefinitions();

        void showStatus();
        size_t countAssetDefinitions();
        AssetDefinition *getAssetDefinitionHandleByUuid(string uuid);

        size_t countScenesDefinitions();
        SceneDefinition *getSceneDefinitionHandleByUuid(string uuid);
        vector<SceneDefinition*> getSceneDefinitionsHandleList();

        void removeAssetDefinition(AssetDefinition* assetDef);
        vector<AssetDefinition*> getAssetDefinitionsHandleList();
    private:
        void loadSceneDefinitions();
        void loadAssetDefinitions();

        void addAssetDefinition(json assetDefinition);
        void addSceneDefinition(json sceneDefinition);
    };
}
