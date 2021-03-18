/*
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

#include <memory>
#include "Base/Definition.h"

using std::weak_ptr;
using std::shared_ptr;
using std::string;
using std::vector;

namespace octronic::dream
{
    class Project;
    class SceneDefinition;
    class AssetDefinition;

    class ProjectDefinition : public Definition
    {

    public:
        ProjectDefinition(const json& data);
        ~ProjectDefinition() override;

        UuidType getStartupSceneUuid() const;
        void setStartupSceneUuid(UuidType sceneUuid);
        weak_ptr<SceneDefinition> getStartupSceneDefinition() const;

        void loadChildDefinitions();

        size_t countAssetDefinitions() const;
        weak_ptr<AssetDefinition> getAssetDefinitionByUuid(UuidType uuid) const;
        weak_ptr<AssetDefinition> getAssetDefinitionByName(const string& name) const;
        void addAssetDefinition(const shared_ptr<AssetDefinition>& def);

        size_t countScenesDefinitions() const;
        weak_ptr<SceneDefinition> getSceneDefinitionByUuid(UuidType uuid) const;
        weak_ptr<SceneDefinition> getSceneDefinitionByName(const string& name) const;
        vector<weak_ptr<SceneDefinition>> getSceneDefinitionsVector() const;
        void removeSceneDefinition(const weak_ptr<SceneDefinition>& sceneDef);

        void removeAssetDefinition(const weak_ptr<AssetDefinition>& assetDef);
        vector<weak_ptr<AssetDefinition>> getAssetDefinitionsVectorForType() const;
        vector<weak_ptr<AssetDefinition>> getAssetDefinitionsVectorForType(AssetType type) const;
        weak_ptr<SceneDefinition> createNewSceneDefinition();
        weak_ptr<AssetDefinition> createNewAssetDefinition(AssetType type);
        map<AssetType,vector<weak_ptr<AssetDefinition>>> getAssetDefinitionsMap() const;

        json toJson() override;

        void deleteAssetDefinitions();
        void deleteSceneDefinitions();

        long getSceneDefinitionIndex(const weak_ptr<SceneDefinition>&) const;
        weak_ptr<SceneDefinition> getSceneDefinitionAtIndex(int index) const;
        long getAssetDefinitionIndex(AssetType, const weak_ptr<AssetDefinition>&) const;
        long getAssetDefinitionIndex(AssetType, UuidType) const;
        weak_ptr<AssetDefinition> getAssetDefinitionAtIndex(AssetType, int) const;

        vector<string> getAssetNamesVector(AssetType) const;

        map<AssetType, vector<string>> getAssetDefinitionGroups() const;
        void regroupAssetDefinitions();

    public: // static
        static shared_ptr<ProjectDefinition> createNewProjectDefinition(const string& name = Constants::PROJECT_DEFAULT_NAME);

    private:
        void loadSceneDefinitions();
        void loadAssetDefinitions();
        void loadSingleAssetDefinition(const json& assetDefinition);
        void loadSingleSceneDefinition(const json& sceneDefinition);
        shared_ptr<AssetDefinition> createAssetDefinition(const json& assetDefinitionJs);
    private:
        map<AssetType,vector<string>> mAssetDefinitionGroups;
        vector<shared_ptr<SceneDefinition>> mSceneDefinitions;
        vector<shared_ptr<AssetDefinition>> mAssetDefinitions;
    };
}
