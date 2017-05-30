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
#ifndef ASSETDEFINITIONPROPERTIESMODEL_H
#define ASSETDEFINITIONPROPERTIESMODEL_H

#include "PropertiesModel.h"
#include <DreamCore.h>

class AssetDefinitionPropertiesModel : public PropertiesModel
{
public:
    AssetDefinitionPropertiesModel(Dream::AssetDefinition *definition, QObject *parent = 0);
    ~AssetDefinitionPropertiesModel();
    void createRoot() override;
    void createProperties() override;
    void createDelegates() override;
private:
    Dream::AssetDefinition* mAssetDefinition;

    void createAudioLoopProperty();
    void createAudioFileProperty();
    void createFontColorProperty();
    void createFontSizeProperty();
    void createFontFileProperty();
    void createLightColorProperty();
    void createModelFileProperty();
    void createModelOtherFilesProperty();
    void craetePhysicsCollisionShapeProperty();
    void createPhysicsMassProperty();
    void createPhysicsMarginProperty();
    void createPhysicsKinematicProperty();
    void createScriptTemplateProperty();
    void createShaderTemplateProperty();
    void createSpriteTileSizeProperty();
    void createSpriteFileProperty();

};

#endif // ASSETDEFINITIONPROPERTIESMODEL_H
