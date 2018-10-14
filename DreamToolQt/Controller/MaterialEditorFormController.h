#pragma once

#include "AbstractEditorWidget.h"
#include "ui_MaterialEditorForm.h"
#include <DreamCore.h>
#include <QMenu>
#include "../Model/MaterialParametersTableModel.h"

enum TextureType
{
    TextureTypeDiffuse,
    TextureTypeSpecular,
    TextureTypeNormal,
    TextureTypeDisplacement,
};

class MaterialEditorFormController : public AbstractEditorWidget
{
    Q_OBJECT
public:
    explicit MaterialEditorFormController
    (
        IAssetDefinition* def,
        ProjectDirectoryModel* dirModel,
        QWidget *parent = nullptr
    );
    ~MaterialEditorFormController() override;

    void populateTexturesMenu(vector<TextureDefinition*> textures);
    void populateShaderComboBox(vector<ShaderDefinition*> shaders);
    void readDefinitionIntoUi();

protected:
    void connectSignals();
    QColor colourDialog(QColor current);

    QColor wrapQColor(RGB color);
    RGB unwrapQColor(QColor color);

    void setDiffuseFrameColour(QColor colour);
    void setSpecularFrameColour(QColor colour);
    void setAmbientFrameColour(QColor colour);
    void setReflectiveFrameColour(QColor colour);
    void setEmissiveFrameColour(QColor colour);

    QString bgColourStyle(QColor color);
    QString bgImageStyle(QString imgPath);

    void populateThisTextureMenu(TextureType type, QMenu* menu, vector<TextureDefinition*> textures);

    void setDiffuseFrameTexture(QString uuid);
    void setSpecularFrameTexture(QString uuid);
    void setNormalFrameTexture(QString uuid);
    void setDisplacementFrameTexture(QString uuid);

    TextureDefinition* getTextureByUuid(QString uuid);
    QString getAbsolutePath(IAssetDefinition* def);

protected slots:
    void onNameTextEdited(QString name);
    void onShaderComboIndexChanged(int idx);

    void onDiffuseColourButtonClicked(bool);
    void onSpecularColourButtonClicked(bool);
    void onAmbientColourButtonClicked(bool);
    void onReflectiveColourButtonClicked(bool);
    void onEmisiveColourButtonClicked(bool);

    void onDiffuseTextureAction(bool);
    void onSpecularTextureAction(bool);
    void onNormalTextureAction(bool);
    void onDisplacementTextureAction(bool);
    void onNewTextureAction(bool);
private:
    Ui::MaterialEditorForm mUi;

    MaterialParametersTableModel mTableModel;
    vector<TextureDefinition*> mTextureDefinitions;
    vector<ShaderDefinition*> mShaderDefinitions;

    QMenu mDiffuseTexturesMenu;
    QMenu mSpecularTexturesMenu;
    QMenu mNormalTexturesMenu;
    QMenu mDisplacementTexturesMenu;
};
