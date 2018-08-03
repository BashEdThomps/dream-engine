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
class TemplatesModel;

class AssetDefinitionPropertiesModel : public AbstractPropertiesModel
{
    Q_OBJECT
public:
    AssetDefinitionPropertiesModel(IAssetDefinition *definition, TemplatesModel* templatesModel, QTreeView *parent = 0);
    ~AssetDefinitionPropertiesModel();

    void createRoot() override;
    void createProperties() override;
    void createDelegateConnections() override;

    IAssetDefinition* getAssetDefinitionHandle();

private:
    IAssetDefinition* mAssetDefinitionHandle;

    void createFormatProperty();
    void createTypeProperty();
    void createNameProperty();
    void createRemoveFilesProperty();
    void createTemplateProperty();

    void createAudioLoopProperty();
    void createAudioFileProperty();

    void createFontColorProperty();
    void createFontSizeProperty();
    void createFontFileProperty();

    void createLightColorProperty();

    void createModelFileProperty();
    void createModelAdditionalFilesProperty();

    void createPhysicsMassProperty();
    void createPhysicsMarginProperty();
    void createPhysicsKinematicProperty();
    void createPhysicsHalfExtentsProperty();
    void createPhysicsConstantProperty();
    void createPhysicsNormalProperty();
    void createPhysicsRadiusProperty();
    void createPhysicsBvhTriangleMeshFileProperty();

    void createSpriteTileSizeProperty();
    void createSpriteFileProperty();

    void createShaderEditProperties();
    void createShaderEditFilesProperty();

    void createScriptFileProperty();


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

signals:
    void notifyButton_RemoveFiles(IAssetDefinition*);

    void notifyButton_AudioFile(IAssetDefinition*);

    void notifyButton_FontFile(IAssetDefinition*);

    void notifyButton_ModelFile(IAssetDefinition*);
    void notifyButton_ModelAdditionalFiles(IAssetDefinition*);

    void notifyButton_EditScript(IAssetDefinition*);
    void notifyCombo_ScriptTemplateChanged(IAssetDefinition*, const QString&);

    void notifyButton_EditShader(IAssetDefinition*);
    void notifyCombo_ShaderTemplateChanged(IAssetDefinition*, const QString&);

    void notifyButton_PhysicsBvhTriangleMeshFile(IAssetDefinition*);
};

