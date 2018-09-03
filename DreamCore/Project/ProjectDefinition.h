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
#include "../Common/ILoggable.h"
#include "../Common/Constants.h"

using std::string;
using std::vector;

namespace Dream
{
    class Project;
    class SceneDefinition;
    class IAssetDefinition;
    class ShaderDefinition;

    class ProjectDefinition : public IDefinition, ILoggable
    {
    private:
        vector<shared_ptr<SceneDefinition>> mSceneDefinitions;
        vector<shared_ptr<IAssetDefinition>> mAssetDefinitions;
        shared_ptr<ProjectDefinition> mThisShared;
    public:
        ProjectDefinition(json data);
        ~ProjectDefinition() override;

        string getAuthor();
        void setAuthor(string author);

        string getDescription();
        void setDescription(string description);

        string getStartupSceneUuid();
        void setStartupSceneUuid(string sceneUuid);
        shared_ptr<SceneDefinition> getStartupSceneDefinition();

        int getWindowWidth();
        void setWindowWidth(int width);

        int getWindowHeight();
        void setWindowHeight(int height);

        void loadChildDefinitions();

        void showStatus() override;
        size_t countAssetDefinitions();
        shared_ptr<IAssetDefinition> getAssetDefinitionByUuid(string uuid);

        size_t countScenesDefinitions();
        shared_ptr<SceneDefinition> getSceneDefinitionByUuid(string uuid);
        shared_ptr<SceneDefinition> getSceneDefinitionByName(string uuid);
        vector<shared_ptr<SceneDefinition>> getSceneDefinitionsList();
        void removeSceneDefinition(shared_ptr<SceneDefinition> sceneDef);

        void removeAssetDefinition(shared_ptr<IAssetDefinition> assetDef);
        vector<shared_ptr<IAssetDefinition>> getAssetDefinitionsList();
        shared_ptr<SceneDefinition> createNewSceneDefinition();
        shared_ptr<IAssetDefinition> createNewAssetDefinition(AssetType type);
        json getJson() override;

        map<AssetType,vector<shared_ptr<IAssetDefinition>>> getAssetDefinitionsMap();
        vector<shared_ptr<ShaderDefinition>> getShaderAssetDefinitionVector();

    private:
        void loadSceneDefinitions();
        void loadAssetDefinitions();
        void loadAssetDefinition(json assetDefinition);
        void loadSceneDefinition(json sceneDefinition);
        shared_ptr<IAssetDefinition>createAssetDefinitionInstance(json assetDefinitionJs);
    };
}
