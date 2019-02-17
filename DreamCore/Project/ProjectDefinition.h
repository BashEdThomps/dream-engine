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

#include "../Common/Definition.h"

using std::string;
using std::vector;

namespace Dream
{
    class Project;
    class SceneDefinition;
    class AssetDefinition;
    class ShaderDefinition;
    class TextureDefinition;
    class MaterialDefinition;

    class ProjectDefinition : public Definition
    {
    private:
        vector<SceneDefinition*> mSceneDefinitions;
        vector<AssetDefinition*> mAssetDefinitions;
    public:
        ProjectDefinition(const json& data);
        ~ProjectDefinition() override;

        string getAuthor();
        void setAuthor(const string& author);

        string getDescription();
        void setDescription(const string& description);

        uint32_t getStartupSceneUuid();
        void setStartupSceneUuid(uint32_t sceneUuid);
        SceneDefinition* getStartupSceneDefinition();

        int getWindowWidth();
        void setWindowWidth(int width);

        int getWindowHeight();
        void setWindowHeight(int height);

        void loadChildDefinitions();

        size_t countAssetDefinitions();
        AssetDefinition* getAssetDefinitionByUuid(uint32_t uuid);
        AssetDefinition* getAssetDefinitionByName(const string& name);
        void addAssetDefinition(AssetDefinition* def);

        size_t countScenesDefinitions();
        SceneDefinition* getSceneDefinitionByUuid(uint32_t uuid);
        SceneDefinition* getSceneDefinitionByName(const string& name);
        vector<SceneDefinition*> getSceneDefinitionsVector();
        void removeSceneDefinition(SceneDefinition* sceneDef);

        void removeAssetDefinition(AssetDefinition* assetDef);
        vector<AssetDefinition*> getAssetDefinitionsVector();
        vector<AssetDefinition*> getAssetDefinitionsVector(AssetType type);
        SceneDefinition* createNewSceneDefinition();
        AssetDefinition* createNewAssetDefinition(AssetType type);
        map<AssetType,vector<AssetDefinition*> > getAssetDefinitionsMap();

        json getJson() override;

        void deleteAssetDefinitions();
        void deleteSceneDefinitions();

        long getSceneDefinitionIndex(SceneDefinition*);
        SceneDefinition* getSceneDefinitionAtIndex(int index);
        long getAssetDefinitionIndex(AssetType, AssetDefinition*);
        long getAssetDefinitionIndex(AssetType, uint32_t);
        AssetDefinition* getAssetDefinitionAtIndex(AssetType, int);

        vector<string> getAssetNamesVector(AssetType);
        void regroupAssetDefinitions();

        map<AssetType, vector<string> >& getAssetDefinitionGroups();

        static ProjectDefinition* createNewProjectDefinition(const string& name = Constants::PROJECT_DEFAULT_NAME);

    private:
        void loadSceneDefinitions();
        void loadAssetDefinitions();
        void loadAssetDefinition(const json& assetDefinition);
        void loadSceneDefinition(const json& sceneDefinition);
        AssetDefinition* createAssetDefinition(const json& assetDefinitionJs);
        map<AssetType,vector<string> > mAssetDefinitionGroups;
    };
}
