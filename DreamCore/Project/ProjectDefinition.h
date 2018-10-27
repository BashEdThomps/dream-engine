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

namespace Dream
{
    class Project;
    class SceneDefinition;
    class IAssetDefinition;
    class ShaderDefinition;
    class TextureDefinition;
    class MaterialDefinition;

    class ProjectDefinition : public IDefinition
    {
    private:
        vector<SceneDefinition*> mSceneDefinitions;
        vector<IAssetDefinition*> mAssetDefinitions;
    public:
        ProjectDefinition(json data);
        ~ProjectDefinition() override;

        string getAuthor();
        void setAuthor(string author);

        string getDescription();
        void setDescription(string description);

        string getStartupSceneUuid();
        void setStartupSceneUuid(string sceneUuid);
        SceneDefinition* getStartupSceneDefinition();

        int getWindowWidth();
        void setWindowWidth(int width);

        int getWindowHeight();
        void setWindowHeight(int height);

        void loadChildDefinitions();

        void showStatus() override;
        size_t countAssetDefinitions();
        IAssetDefinition* getAssetDefinitionByUuid(string uuid);

        size_t countScenesDefinitions();
        SceneDefinition* getSceneDefinitionByUuid(string uuid);
        SceneDefinition* getSceneDefinitionByName(string uuid);
        vector<SceneDefinition*> getSceneDefinitionsVector();
        void removeSceneDefinition(SceneDefinition* sceneDef);

        void removeAssetDefinition(IAssetDefinition* assetDef);
        vector<IAssetDefinition*> getAssetDefinitionsVector();
        vector<IAssetDefinition*> getAssetDefinitionsVector(AssetType type);
        SceneDefinition* createNewSceneDefinition();
        IAssetDefinition* createNewAssetDefinition(AssetType type);
        map<AssetType,vector<IAssetDefinition*>> getAssetDefinitionsMap();
        vector<ShaderDefinition*> getShaderDefinitionsVector();
        vector<TextureDefinition*> getTextureDefinitionVector();
        vector<MaterialDefinition*> getMaterialDefinitionVector();

        json getJson() override;

        bool getCaptureKeyboard();
        void setCaptureKeyboard(bool);

        bool getCaptureMouse();
        void setCaptureMouse(bool);

        bool getCaptureJoystick();
        void setCaptureJoystick(bool);

        void deleteAssetDefinitions();
        void deleteSceneDefinitions();

        long getSceneDefinitionIndex(SceneDefinition*);
        long getAssetDefinitionIndex(AssetType, IAssetDefinition*);
        long getAssetDefinitionIndex(AssetType, string);
        ShaderDefinition* getShaderDefinitionAtIndex(int);

        vector<string> getAssetNamesVector(AssetType);

    private:
        void loadSceneDefinitions();
        void loadAssetDefinitions();
        void loadAssetDefinition(json assetDefinition);
        void loadSceneDefinition(json sceneDefinition);
        IAssetDefinition* createAssetDefinitionInstance(json assetDefinitionJs);
    };
}
