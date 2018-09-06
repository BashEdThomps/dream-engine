/*
 * AssetDefinitionPropertiesModel.h
 *
 * Created: 17 2017 by Ashley
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

#include "../AbstractPropertiesModel.h"
#include <DreamCore.h>
#include <memory>

using std::unique_ptr;
using Dream::IAssetDefinition;

class ModelFileBrowseDelegate;
class BrowseForAdditionalFilesDelegate;

class AssetDefinitionPropertiesModel : public AbstractPropertiesModel
{
    Q_OBJECT
public:
    AssetDefinitionPropertiesModel(shared_ptr<IAssetDefinition> definition, QTreeView *parent = nullptr);
    ~AssetDefinitionPropertiesModel() override;

    void createRoot() override;
    void createProperties() override;
    void createDelegateConnections() override;

    shared_ptr<IAssetDefinition> getAssetDefinitionHandle();

private:
    shared_ptr<IAssetDefinition> mAssetDefinitionHandle;

    void createFormatProperty();
    void createTypeProperty();
    void createNameProperty();
    void createRemoveFilesProperty();

    void createAudioLoopProperty();
    void createAudioFileProperty();

    void createFontColorProperty();
    void createFontSizeProperty();
    void createFontFileProperty();

    void createLightAmbientProperty();
    void createLightDiffuseProperty();
    void createLightSpecularProperty();
    void createLightDirectionProperty();
    void createLightConstantProperty();
    void createLightQuadraticProperty();
    void createLightLinearProperty();
    void createLightCutOffProperty();
    void createLightOuterCutOffProperty();

    void createModelFileProperty();
    void createModelAdditionalFilesProperty();
    void createModelMaterialShaderProperty();
    void createPhysicsMassProperty();
    void createPhysicsMarginProperty();
    void createPhysicsKinematicProperty();
    void createPhysicsHalfExtentsProperty();
    void createPhysicsConstantProperty();
    void createPhysicsNormalProperty();
    void createPhysicsRadiusProperty();
    void createPhysicsBvhTriangleMeshFileProperty();
    void createPhysicsHeightProperty();
    void createPhysicsControllableProperty();

    void createSpriteTileSizeProperty();
    void createSpriteFileProperty();

    void createShaderEditProperties();
    void createShaderEditFilesProperty();

    void createScriptFileProperty();

    void createPathListProperty();
    void createPathStepProperty();
    void createPathSplineFormatProperty();

public slots:
    void onButton_RemoveFiles();
    void onButton_AudioFile();
    void onButton_FontFile();
    void onButton_ModelFile();
    void onButton_ModelAdditionalFiles();
    void onButton_EditScript();
    void onCombo_ScriptTemplateChanged(const QString&);
    void onButton_EditShader();
    void onCombo_ShaderTemplateChanged(const QString&);
    void onButton_PhysicsBvhTriangleMeshFile();
    void onButton_LightChooseAmbient();
    void onButton_LightChooseSpecular();
    void onButton_LightChooseDiffuse();
    void onButton_ModelMaterialShaderMap();
    void onButton_PathList();
signals:
    void notifyButton_RemoveFiles(shared_ptr<IAssetDefinition>);
    void notifyButton_AudioFile(shared_ptr<IAssetDefinition>);
    void notifyButton_FontFile(shared_ptr<IAssetDefinition>);
    void notifyButton_ModelFile(shared_ptr<IAssetDefinition>);
    void notifyButton_ModelAdditionalFiles(shared_ptr<IAssetDefinition>);
    void notifyButton_EditScript(shared_ptr<IAssetDefinition>);
    void notifyCombo_ScriptTemplateChanged(shared_ptr<IAssetDefinition>, const QString&);
    void notifyButton_EditShader(shared_ptr<IAssetDefinition>);
    void notifyCombo_ShaderTemplateChanged(shared_ptr<IAssetDefinition>, const QString&);
    void notifyButton_PhysicsBvhTriangleMeshFile(shared_ptr<IAssetDefinition>);
    void notifyButton_ModelMaterialShaderMap(shared_ptr<IAssetDefinition>);
    void notifyButton_PathList(shared_ptr<IAssetDefinition>);

    void notifyButton_LightChooseAmbient(shared_ptr<IAssetDefinition>);
    void notifyButton_LightChooseSpecular(shared_ptr<IAssetDefinition>);
    void notifyButton_LightChooseDiffuse(shared_ptr<IAssetDefinition>);
};
