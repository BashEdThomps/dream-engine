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
using Dream::AssetDefinition;

class ModelFileBrowseDelegate;
class BrowseForAdditionalFilesDelegate;

class AssetDefinitionPropertiesModel : public AbstractPropertiesModel
{
    Q_OBJECT
public:
    AssetDefinitionPropertiesModel(AssetDefinition *definition, QTreeView *parent = 0);
    ~AssetDefinitionPropertiesModel();

    void createRoot() override;
    void createProperties() override;

private:
    AssetDefinition* mAssetDefinitionHandle;

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

