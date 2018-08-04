#include "DreamCore.h"
#include "ScriptEditorController.h"
#include <spdlog/spdlog.h>

#include "Model/ProjectDirectoryModel.h"
#include "Model/TemplatesModel.h"
#include <QMessageBox>
#include <QStandardItemModel>

using Dream::Constants;
using Dream::IAssetDefinition;

ScriptEditorController::ScriptEditorController
(QWidget* parent) : QWidget(parent),
    mProjectDirectoryModelHandle(nullptr),
    mTemplatesModelHandle(nullptr)
{
    auto log = spdlog::get("ScriptEditorController");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("ScriptEditorController");
    }
    log->info("Constructing");
    mScriptEditor.setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    mTabWidget = mScriptEditor.tabWidget;
    mSaveButton = mScriptEditor.saveButton;
    mRevertButton = mScriptEditor.revertButton;
    setupCloseButtonSignal();
    setupRevertSaveSignals();
    createTemplatesComboBox(mScriptEditor.templateComboBox);
    clearExistingTabs();
}

ScriptEditorController::~ScriptEditorController
()
{
    auto log = spdlog::get("ScriptEditorController");
    log->info("Destructing");
}

void ScriptEditorController::setupRevertSaveSignals()
{
    connect(mSaveButton,SIGNAL(clicked(bool)),this,SLOT(onSaveButtonClicked(bool)));
    connect(mRevertButton,SIGNAL(clicked(bool)),this,SLOT(onRevertButtonClicked(bool)));
}

void
ScriptEditorController::openScript
(ScriptDefinition* scriptDefinitionHandle)
{
    auto log = spdlog::get("ScriptEditorController");
    if (mProjectDirectoryModelHandle != nullptr)
    {
        //clearExistingTabs();
        auto form = make_shared<ScriptEditorTabController>(QString::fromStdString(Constants::ASSET_FORMAT_SCRIPT_LUA), this);

        QByteArray data = mProjectDirectoryModelHandle->readScriptData(scriptDefinitionHandle);
        form->useLuaHighlighter();
        form->setPlainText(QString(data));
        form->setAssetDefinitionHandle(scriptDefinitionHandle);
        mTabForms.push_back(form);
        int index = mTabWidget->addTab(
                    form.get(),
                    QString::fromStdString(scriptDefinitionHandle->getName())
                    .append(" (%1)")
                    .arg(QString::fromStdString(Constants::ASSET_FORMAT_SCRIPT_LUA))
                    );
        getAllUpInYourFace();
    }

}

void ScriptEditorController::setupCloseButtonSignal()
{
    connect(
                mTabWidget,SIGNAL(tabCloseRequested(int)),
                this, SLOT(onTabCloseRequested(int))
                );
}

void
ScriptEditorController::openShader
(ShaderDefinition* shaderDefinitionHandle)
{
    auto log = spdlog::get("ScriptEditorController");
    if (mProjectDirectoryModelHandle != nullptr)
    {
        //clearExistingTabs();
        ShaderFileTuple data = mProjectDirectoryModelHandle->readShaderData(shaderDefinitionHandle);

        auto vertexForm = make_shared<ScriptEditorTabController>(
            QString::fromStdString(Constants::SHADER_VERTEX_FILE_NAME),
            this
        );

        auto fragmentForm = make_shared<ScriptEditorTabController>(
            QString::fromStdString(Constants::SHADER_FRAGMENT_FILE_NAME),
            this
        );

        vertexForm->useGLSLHighlighter();
        vertexForm->setPlainText(data.vertexShader);
        vertexForm->setAssetDefinitionHandle(shaderDefinitionHandle);

        fragmentForm->useGLSLHighlighter();
        fragmentForm->setPlainText(data.fragmentShader);
        fragmentForm->setAssetDefinitionHandle(shaderDefinitionHandle);

        mTabForms.push_back(vertexForm);
        mTabForms.push_back(fragmentForm);

        mTabWidget->addTab(
                    vertexForm.get(),
                    QString::fromStdString(shaderDefinitionHandle->getName())
                    .append(" (%1)")
                    .arg(QString::fromStdString(Constants::SHADER_VERTEX))
                    );

        mTabWidget->addTab(
                    fragmentForm.get(),
                    QString::fromStdString(shaderDefinitionHandle->getName())
                    .append(" (%1)")
                    .arg(QString::fromStdString(Constants::SHADER_FRAGMENT))
                    );

        getAllUpInYourFace();
    }
}

void
ScriptEditorController::setProjectDirectoryModelHandle
(ProjectDirectoryModel* projectDirectoryModelHandle)
{
    mProjectDirectoryModelHandle = projectDirectoryModelHandle;
}

void ScriptEditorController::getAllUpInYourFace()
{
    show();
    activateWindow();
    raise();
    setFocus();
    mTabWidget->setCurrentIndex(mTabWidget->count()-1);
}

void ScriptEditorController::onTabCloseRequested(int index)
{

    auto tabWidget = mTabForms.at(index);

    if (tabWidget->hasTextChanged())
    {
        int response = QMessageBox::question(
                    this,
                    "Save before closing?",
                    "File contents has changed. Do you want to save before closing?"
                    );

        if (response == QMessageBox::Yes)
        {
            onSaveButtonClicked(true);
        }
    }

    mTabWidget->removeTab(index);
    mTabForms.erase(mTabForms.begin() + index);
}

void ScriptEditorController::onRevertButtonClicked(bool checked)
{
    Q_UNUSED(checked)
    auto log = spdlog::get("ScriptEditorController");
    int tabIndex = mTabWidget->currentIndex();
    if (tabIndex < 0)
    {
        return;
    }
    auto currentWidgetHandle = mTabForms.at(tabIndex).get();
    IAssetDefinition* adHandle = currentWidgetHandle->getAssetDefinitionHandle();
    if (adHandle != nullptr)
    {
        int result = QMessageBox::question(
                    this,
                    "Revert File",
                    "Are you sure you want to revert to the last saved state?"
                    );

        if (result == QMessageBox::Ok)
        {
            log->info("Reverting {} back to saved state",adHandle->getNameAndUuidString());
        }
    }
}

void ScriptEditorController::onSaveButtonClicked(bool checked)
{
    Q_UNUSED(checked)
    auto log = spdlog::get("ScriptEditorController");
    int tabIndex = mTabWidget->currentIndex();
    if (tabIndex < 0)
    {
        return;
    }

    auto currentWidgetHandle = mTabForms.at(tabIndex).get();
    IAssetDefinition* adHandle = currentWidgetHandle->getAssetDefinitionHandle();
    if (adHandle != nullptr)
    {
        log->info("Saving {}",adHandle->getNameAndUuidString());
        QString data = currentWidgetHandle->getPlainText();

        bool writeResult = false;

        if (adHandle->isTypeShader())
        {
            writeResult = mProjectDirectoryModelHandle->writeAssetData(
                data,
                adHandle,
                currentWidgetHandle->getFileType()
            );
        }
        else if (adHandle->isTypeScript())
        {
            writeResult = mProjectDirectoryModelHandle->writeAssetData(data,adHandle);
        }

        if (writeResult)
        {
            QMessageBox::information(
                    this,
                    "Asset Saved",
                    QString("Successfully saved %1")
                    .arg(QString::fromStdString(adHandle->getName()))
                    );
        }
        else
        {
            QMessageBox::critical(
                this,
                "Save Failed",
                QString("Unable to save %1")
                .arg(QString::fromStdString(adHandle->getName()))
            );
        }
    }
}

void
ScriptEditorController::onComboTemplateChanged
(const QString& templateName)
{
    auto log = spdlog::get("ScriptEditorController");
    log->info("Selected template {}",templateName.toStdString());
}

int
ScriptEditorController::isAssetOpen
(Dream::IAssetDefinition* definition)
{
    return -1;
}

void
ScriptEditorController::clearExistingTabs
()
{
    mTabWidget->clear();
    mTabForms.clear();
}

void
ScriptEditorController::setTemplatesModelHandle
(TemplatesModel* templatesModelHandle)
{
    mTemplatesModelHandle = templatesModelHandle;
}

void
ScriptEditorController::createTemplatesComboBox
(QComboBox* editor)
{
    auto log = spdlog::get("ScriptEditorController");
    editor->setDuplicatesEnabled(false);
    editor->setEditable(false);

    QString scriptPrompt = "Script Templates";
    editor->addItem(scriptPrompt);
    int promptIndex = editor->findText(scriptPrompt);
    qobject_cast<QStandardItemModel*>
    (
        editor->model()
    )->item(promptIndex)->setEnabled(false);

    QStringList scriptTemplates = mTemplatesModelHandle->getScriptTemplateNames();
    log->info("Got script templates {}",scriptTemplates.count());
    editor->addItems(scriptTemplates);

    QString shaderPrompt = "Shader Templates";
    editor->addItem(shaderPrompt);
    int shaderPromptIndex = editor->findText(shaderPrompt);
    qobject_cast<QStandardItemModel*>
    (
        editor->model()
    )->item(shaderPromptIndex)->setEnabled(false);

    QStringList shaderTemplates = mTemplatesModelHandle->getShaderTemplateNames();
    log->info("Got shader templates {}",shaderTemplates.count());
    editor->addItems(shaderTemplates);

    connect
    (
        editor,
        SIGNAL(activated(const QString&)),
        this,
        SLOT(onComboTemplateChanged(const QString&))
    );
}
