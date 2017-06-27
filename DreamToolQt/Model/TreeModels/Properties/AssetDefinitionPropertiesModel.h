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

#include "PropertiesModel.h"
#include <DreamCore.h>
#include <memory>

using std::unique_ptr;
using Dream::AssetDefinition;

class ModelFileBrowseDelegate;
class BrowseForAdditionalFilesDelegate;

class AssetDefinitionPropertiesModel : public PropertiesModel
{
    Q_OBJECT
private:
    const static int NAME_INDEX;
    const static int TYPE_INDEX;
    const static int FORMAT_INDEX;
    const static int SCRIPT_TEMPLATE_INDEX;
    const static int SHADER_TEMPLATE_INDEX;
    const static int MODEL_ASSIMP_FILE_INDEX;
    const static int MODEL_OTHER_FILE_INDEX;
public:
    AssetDefinitionPropertiesModel(Dream::AssetDefinition *definition, QTreeView *parent = 0);
    ~AssetDefinitionPropertiesModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void createRoot() override;
    void createProperties() override;

private:
    AssetDefinition* mAssetDefinitionHandle;
    ModelFileBrowseDelegate* mModelFileBrowseDelegateHandle;
    BrowseForAdditionalFilesDelegate* mModelAdditionalFilesDelegateHandle;

    void createAudioLoopProperty();
    void createAudioFileProperty();
    void createFontColorProperty();
    void createFontSizeProperty();
    void createFontFileProperty();
    void createLightColorProperty();
    void createModelFileProperty();
    void createModelAdditionalFilesProperty();
    void craetePhysicsCollisionShapeProperty();
    void createPhysicsMassProperty();
    void createPhysicsMarginProperty();
    void createPhysicsKinematicProperty();
    void createScriptTemplateProperty();
    void createShaderTemplateProperty();
    void createSpriteTileSizeProperty();
    void createSpriteFileProperty();
    void createFormatProperty();
    void createTypeProperty();
    void createNameProperty();

public slots:
    void onModelFileBrowseButtonClicked(AssetDefinition*);
    void onModelAdditionalFilesButtonClicked(AssetDefinition*);

signals:
    void notifyModelFileBrowseButtonClicked(AssetDefinition*);
    void notifyModelAdditionalFilesButtonClicked(AssetDefinition*);
};

