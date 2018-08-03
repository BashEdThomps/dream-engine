#include "DreamCore.h"
#include "ScriptEditorController.h"
#include <spdlog/spdlog.h>

#include "Model/ProjectDirectoryModel.h"
#include <QMessageBox>

using Dream::Constants;
using Dream::IAssetDefinition;

ScriptEditorController::ScriptEditorController
(QWidget* parent) : QDialog(parent)
{
    auto log = spdlog::get("ScriptEditorController");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("ScriptEditorController");
    }
    log->info("Constructing");
    mScriptEditor.setupUi(this);
    setModal(false);
    setSizeGripEnabled(true);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    mTabWidget = mScriptEditor.tabWidget;
    mSaveButton = mScriptEditor.saveButton;
    mRevertButton = mScriptEditor.revertButton;
    setupCloseButtonSignal();
    setupRevertSaveSignals();
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
        auto form = make_shared<ScriptEditorTabController>(this);

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

        auto vertexForm = make_shared<ScriptEditorTabController>(this);
        auto fragmentForm = make_shared<ScriptEditorTabController>(this);

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

        QMessageBox::information(
            this,
            "Asset Saved",
            QString("Successfully saved %1")
                .arg(QString::fromStdString(adHandle->getName()))
        );
    }
}

int ScriptEditorController::isAssetOpen(Dream::IAssetDefinition* definition)
{
   return -1;
}

void ScriptEditorController::clearExistingTabs()
{
   mTabWidget->clear();
   mTabForms.clear();
}
