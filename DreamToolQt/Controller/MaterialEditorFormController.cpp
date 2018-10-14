#include "MaterialEditorFormController.h"
#include "../Model/ProjectDirectoryModel.h"
#include <QDebug>
#include <QColorDialog>

MaterialEditorFormController::MaterialEditorFormController
(IAssetDefinition* def, ProjectDirectoryModel* dirModel, QWidget *parent)
    : AbstractEditorWidget(def, dirModel, parent),
      mTableModel(dynamic_cast<MaterialDefinition*>(def),this)
{
    mUi.setupUi(this);
    mUi.splitter->setStretchFactor(0,0);
    mUi.splitter->setStretchFactor(1,1);
    mUi.tableView->setModel(&mTableModel);
    mUi.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MaterialEditorFormController::~MaterialEditorFormController
()
{

}

// Setup ------------------------------------------------------------------------

void
MaterialEditorFormController::populateShaderComboBox
(vector<ShaderDefinition*> shaders)
{
    mShaderDefinitions = shaders;

    auto combo = mUi.shaderComboBox;
    for (auto shaderDef : shaders)
    {
        auto name = QString::fromStdString(shaderDef->getName());
        auto uuid = QString::fromStdString(shaderDef->getUuid());
        qDebug() << "Adding shader to combo" << name << uuid;
        combo->addItem(name,uuid);
    }
}

void
MaterialEditorFormController::populateTexturesMenu
(vector<TextureDefinition*> textures)
{
    mTextureDefinitions = textures;

    populateThisTextureMenu(TextureTypeDiffuse, &mDiffuseTexturesMenu, textures);
    mUi.diffuseTextureButton->setMenu(&mDiffuseTexturesMenu);
    mUi.diffuseTextureButton->setPopupMode(QToolButton::InstantPopup);

    populateThisTextureMenu(TextureTypeSpecular, &mSpecularTexturesMenu, textures);
    mUi.specularTextureButton->setMenu(&mSpecularTexturesMenu);
    mUi.specularTextureButton->setPopupMode(QToolButton::InstantPopup);

    populateThisTextureMenu(TextureTypeNormal, &mNormalTexturesMenu, textures);
    mUi.normalTextureButton->setMenu(&mNormalTexturesMenu);
    mUi.normalTextureButton->setPopupMode(QToolButton::InstantPopup);

    populateThisTextureMenu(TextureTypeDisplacement, &mDisplacementTexturesMenu, textures);
    mUi.displacementTextureButton->setMenu(&mDisplacementTexturesMenu);
    mUi.displacementTextureButton->setPopupMode(QToolButton::InstantPopup);
}

void
MaterialEditorFormController::populateThisTextureMenu
(TextureType type, QMenu* menu, vector<TextureDefinition*> textures)
{
    for (auto texture : textures)
    {
        auto nextTexture = new QAction(QString::fromStdString(texture->getName()));
        nextTexture->setData(QString::fromStdString(texture->getUuid()));
        menu->addAction(nextTexture);

        switch(type)
        {
            case TextureTypeDiffuse:
                connect(nextTexture,SIGNAL(triggered(bool)),this,SLOT(onDiffuseTextureAction(bool)));
                break;
            case TextureTypeSpecular:
                connect(nextTexture,SIGNAL(triggered(bool)),this,SLOT(onSpecularTextureAction(bool)));
                break;
            case TextureTypeNormal:
                connect(nextTexture,SIGNAL(triggered(bool)),this,SLOT(onNormalTextureAction(bool)));
                break;
            case TextureTypeDisplacement:
                connect(nextTexture,SIGNAL(triggered(bool)),this,SLOT(onDisplacementTextureAction(bool)));
                break;
        }
    }

    menu->addSeparator();
    QAction* newTexture = new QAction("New Texture...");
    menu->addAction(newTexture);
    connect(newTexture, SIGNAL(triggered(bool)), this, SLOT(onNewTextureAction(bool)));
}

void
MaterialEditorFormController::readDefinitionIntoUi
()
{
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    // Set Name
    mUi.nameLineEdit->setText(QString::fromStdString(matDef->getName()));
    // Set Shader
    QString shaderStr = QString::fromStdString(matDef->getShader());
    auto shaderValIndex = mUi.shaderComboBox->findData(shaderStr);
    qDebug() << "Setting Shader from def"
             << shaderStr
             << shaderValIndex
             << mUi.shaderComboBox->itemData(shaderValIndex);
    mUi.shaderComboBox->setCurrentIndex(shaderValIndex);
    // Colour Frames
    setDiffuseFrameColour(wrapQColor(matDef->getDiffuseColour()));
    setSpecularFrameColour(wrapQColor(matDef->getSpecularColour()));
    setAmbientFrameColour(wrapQColor(matDef->getAmbientColour()));
    setReflectiveFrameColour(wrapQColor(matDef->getReflectiveColour()));
    setEmissiveFrameColour(wrapQColor(matDef->getEmissiveColour()));
    // Texture Frames
    setDiffuseFrameTexture(QString::fromStdString(matDef->getDiffuseTexture()));
    setSpecularFrameTexture(QString::fromStdString(matDef->getSpecularTexture()));
    setNormalFrameTexture(QString::fromStdString(matDef->getNormalTexture()));
    setDisplacementFrameTexture(QString::fromStdString(matDef->getDepthmapTexture()));

    connectSignals();
}

void
MaterialEditorFormController::connectSignals
()
{
    // Name
    connect(mUi.nameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(onNameTextEdited(QString)));
    // Shader
    connect(mUi.shaderComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onShaderComboIndexChanged(int)));
    // Colour Buttons
    connect(mUi.diffuseColourButton, SIGNAL(clicked(bool)),this,SLOT(onDiffuseColourButtonClicked(bool)));
    connect(mUi.specularColourButton, SIGNAL(clicked(bool)),this,SLOT(onSpecularColourButtonClicked(bool)));
    connect(mUi.ambientColourButton, SIGNAL(clicked(bool)),this,SLOT(onAmbientColourButtonClicked(bool)));
    connect(mUi.reflectiveColourButton, SIGNAL(clicked(bool)),this,SLOT(onReflectiveColourButtonClicked(bool)));
    connect(mUi.emisiveColourButton, SIGNAL(clicked(bool)),this,SLOT(onEmisiveColourButtonClicked(bool)));
}

// SLOTS -------------------------------------------------------------------------

// Material Info

void
MaterialEditorFormController::onNameTextEdited
(QString name)
{
    mDefinitionHandle->setName(name.toStdString());
    qDebug() << "Set name to " << name;
}

void
MaterialEditorFormController::onShaderComboIndexChanged
(int idx)
{
    QString shaderUuid = mUi.shaderComboBox->itemData(idx).toString();
    dynamic_cast<MaterialDefinition*>(mDefinitionHandle)->setShader(shaderUuid.toStdString());
    qDebug() << "Changed shader to " << shaderUuid;
}

// Colour Buttons

void
MaterialEditorFormController::onDiffuseColourButtonClicked
(bool)
{
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    auto current = wrapQColor(matDef->getDiffuseColour());
    auto newQColour = colourDialog(current);
    auto newColour = unwrapQColor(newQColour);
    matDef->setDiffuseColour(newColour);
    setDiffuseFrameColour(newQColour);
}

void
MaterialEditorFormController::onSpecularColourButtonClicked
(bool)
{
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    auto current = wrapQColor(matDef->getSpecularColour());
    auto newQColour = colourDialog(current);
    auto newColour = unwrapQColor(newQColour);
    matDef->setSpecularColour(newColour);
    setSpecularFrameColour(newQColour);
}

void
MaterialEditorFormController::onAmbientColourButtonClicked
(bool)
{
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    auto current = wrapQColor(matDef->getAmbientColour());
    auto newQColour = colourDialog(current);
    auto newColour = unwrapQColor(newQColour);
    matDef->setAmbientColour(newColour);
    setAmbientFrameColour(newQColour);
}

void
MaterialEditorFormController::onReflectiveColourButtonClicked
(bool)
{
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    auto current = wrapQColor(matDef->getReflectiveColour());
    auto newQColour = colourDialog(current);
    auto newColour = unwrapQColor(newQColour);
    matDef->setReflectiveColour(newColour);
    setReflectiveFrameColour(newQColour);
}

void
MaterialEditorFormController::onEmisiveColourButtonClicked
(bool)
{
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    auto current = wrapQColor(matDef->getEmissiveColour());
    auto newQColour = colourDialog(current);
    auto newColour = unwrapQColor(newQColour);
    matDef->setEmissiveColour(newColour);
    setEmissiveFrameColour(newQColour);
}

// Texture Menu Actions

void
MaterialEditorFormController::onDiffuseTextureAction
(bool)
{
    auto action = dynamic_cast<QAction*>(sender());
    auto uuid = action->data().toString();
    qDebug() << "Diffuse Texture" << uuid;
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    matDef->setDiffuseTexture(uuid.toStdString());
    setDiffuseFrameTexture(uuid);
}

void
MaterialEditorFormController::onSpecularTextureAction
(bool)
{
    auto action = dynamic_cast<QAction*>(sender());
    auto uuid = action->data().toString();
    qDebug() << "Specular Texture" << uuid;
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    matDef->setSpecularTexture(uuid.toStdString());
    setSpecularFrameTexture(uuid);
}

void
MaterialEditorFormController::onNormalTextureAction
(bool)
{
    auto action = dynamic_cast<QAction*>(sender());
    auto uuid = action->data().toString();
    qDebug() << "Normal Texture" << uuid;
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    matDef->setNormalTexture(uuid.toStdString());
    setNormalFrameTexture(uuid);
}

void
MaterialEditorFormController::onDisplacementTextureAction
(bool)
{
    auto action = dynamic_cast<QAction*>(sender());
    auto uuid = action->data().toString();
    qDebug() << "Displacement Texture" << uuid;
    auto matDef = dynamic_cast<MaterialDefinition*>(mDefinitionHandle);
    matDef->setDepthmapTexture(uuid.toStdString());
    setDisplacementFrameTexture(uuid);
}

void
MaterialEditorFormController::onNewTextureAction
(bool)
{
    auto action = dynamic_cast<QAction*>(sender());
    qDebug() << "New Texture";
}

// Frame Colours ----------------------------------------------------------------

void
MaterialEditorFormController::setDiffuseFrameColour
(QColor colour)
{
    mUi.diffuseColourFrame->setStyleSheet(bgColourStyle(colour));
}

void
MaterialEditorFormController::setSpecularFrameColour
(QColor colour)
{
    mUi.specularColourFrame->setStyleSheet(bgColourStyle(colour));
}

void
MaterialEditorFormController::setAmbientFrameColour
(QColor colour)
{
    mUi.ambientColourFrame->setStyleSheet(bgColourStyle(colour));
}

void
MaterialEditorFormController::setReflectiveFrameColour
(QColor colour)
{
    mUi.reflectiveColourFrame->setStyleSheet(bgColourStyle(colour));
}

void
MaterialEditorFormController::setEmissiveFrameColour
(QColor colour)
{
    mUi.emissiveColourFrame->setStyleSheet(bgColourStyle(colour));
}

void
MaterialEditorFormController::setDiffuseFrameTexture
(QString uuid)
{
    TextureDefinition* tx = getTextureByUuid(uuid);
    if (tx != nullptr)
    {
        auto absPath = getAbsolutePath(tx);
        mUi.diffuseTextureFrame->setStyleSheet(bgImageStyle(absPath));
    }
}

void
MaterialEditorFormController::setSpecularFrameTexture
(QString uuid)
{
    TextureDefinition* tx = getTextureByUuid(uuid);
    if (tx != nullptr)
    {
        auto absPath = getAbsolutePath(tx);
        mUi.specularTextureFrame->setStyleSheet(bgImageStyle(absPath));
    }

}

void
MaterialEditorFormController::setNormalFrameTexture
(QString uuid)
{
    TextureDefinition* tx = getTextureByUuid(uuid);
    if (tx != nullptr)
    {
        auto absPath = getAbsolutePath(tx);
        mUi.normalTextureFrame->setStyleSheet(bgImageStyle(absPath));
    }
}

void
MaterialEditorFormController::setDisplacementFrameTexture
(QString uuid)
{
    TextureDefinition* tx = getTextureByUuid(uuid);
    if (tx != nullptr)
    {
        auto absPath = getAbsolutePath(tx);
        mUi.displacementTextureFrame->setStyleSheet(bgImageStyle(absPath));
    }

}

// Utils ------------------------------------------------------------------------

QString
MaterialEditorFormController::bgColourStyle
(QColor color)
{
    return QString("background-color: rgb(%1, %2, %3)")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue());
}

QString
MaterialEditorFormController::bgImageStyle
(QString imgPath)
{
    qDebug() << "Using image path" << imgPath;
    return QString("border-image: url(%1) 0 0 0 0 stretch stretch;").arg(imgPath);
}

QColor
MaterialEditorFormController::wrapQColor
(RGB color)
{
    return QColor::fromRgbF
    (
        static_cast<qreal>(color.r),
        static_cast<qreal>(color.g),
        static_cast<qreal>(color.b)
    );
}

RGB
MaterialEditorFormController::unwrapQColor
(QColor color)
{
    return RGB{
        static_cast<float>(color.redF()),
        static_cast<float>(color.greenF()),
        static_cast<float>(color.blueF())
    };
}

QColor
MaterialEditorFormController::colourDialog
(QColor currentColour)
{
    qDebug() << "Choose Colour Dialog";
    auto chosenColour = QColorDialog::getColor(currentColour,this,"Choose Colour");
    if (chosenColour.isValid())
    {
        return chosenColour;
    }
    return currentColour;
}

TextureDefinition*
MaterialEditorFormController::getTextureByUuid
(QString uuid)
{
    for (TextureDefinition* tx : mTextureDefinitions)
    {
       if (tx->getUuid().compare(uuid.toStdString()) == 0)
       {
           return tx;
       }
    }
    return nullptr;
}

QString
MaterialEditorFormController::getAbsolutePath
(IAssetDefinition* def)
{
    return mProjectDirectoryModelHandle->getAssetDataFilePath(def);
}
