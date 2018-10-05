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
    AssetDefinitionPropertiesModel(IAssetDefinition* definition, QTreeView *parent = nullptr);
    ~AssetDefinitionPropertiesModel() override;

    void createRoot() override;
    void createProperties() override;
    void createDelegateConnections() override;

    IAssetDefinition* getAssetDefinitionHandle();

private:
    IAssetDefinition* mAssetDefinitionHandle;

    void createFormatProperty();
    void createTypeProperty();
    void createUuidProperty();
    void createNameProperty();
    void createRemoveFilesProperty();

    void createAudioLoopProperty();
    void createAudioFFTProperty();
    void createAudioEventsProperty();
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
    void createPhysicsRestitutionProperty();
    void createPhysicsFrictionProperty();

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
    void onButton_AudioEvents();
    void onButton_FontFile();
    void onButton_ModelFile();
    void onButton_ModelAdditionalFiles();
    void onButton_EditScript();
    void onCombo_ScriptTemplateChanged( QString&);
    void onButton_EditShader();
    void onCombo_ShaderTemplateChanged( QString&);
    void onButton_PhysicsBvhTriangleMeshFile();
    void onButton_LightChooseAmbient();
    void onButton_LightChooseSpecular();
    void onButton_LightChooseDiffuse();
    void onButton_ModelMaterialShaderMap();
    void onButton_PathList();
signals:
    void notifyButton_RemoveFiles(IAssetDefinition*);
    void notifyButton_AudioFile(IAssetDefinition*);
    void notifyButton_AudioEvents(IAssetDefinition*);
    void notifyButton_FontFile(IAssetDefinition*);
    void notifyButton_ModelFile(IAssetDefinition*);
    void notifyButton_ModelAdditionalFiles(IAssetDefinition*);
    void notifyButton_EditScript(IAssetDefinition*);
    void notifyCombo_ScriptTemplateChanged(IAssetDefinition*,  QString&);
    void notifyButton_EditShader(IAssetDefinition*);
    void notifyCombo_ShaderTemplateChanged(IAssetDefinition*,  QString&);
    void notifyButton_PhysicsBvhTriangleMeshFile(IAssetDefinition*);
    void notifyButton_ModelMaterialShaderMap(IAssetDefinition*);
    void notifyButton_PathList(IAssetDefinition*);

    void notifyButton_LightChooseAmbient(IAssetDefinition*);
    void notifyButton_LightChooseSpecular(IAssetDefinition*);
    void notifyButton_LightChooseDiffuse(IAssetDefinition*);
};
