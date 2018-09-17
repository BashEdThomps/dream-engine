#include "EditorTabController.h"

#include "View/SyntaxHighlighters/GLSLHighlighter.h"
#include "View/SyntaxHighlighters/LuaHighlighter.h"
#include "Model/ProjectDirectoryModel.h"
#include "Model/TemplatesModel.h"
#include "DreamCore.h"
#include <spdlog/spdlog.h>

#include <QMessageBox>
#include <QStandardItemModel>

using std::make_shared;
using Dream::Constants;

EditorTabController::EditorTabController
(
    QString fileFormat,
    IAssetDefinition* assetDefinition,
    TemplatesModel* templatesModel,
    ProjectDirectoryModel* dirModel,
    QWidget *parent
)
    : QWidget(parent),
      mTextHasChanged(false),
      mFileFormat(fileFormat),
      mAssetDefinitionHandle(assetDefinition),
      mProjectDirectoryModelHandle(dirModel),
      mTemplatesModelHandle(templatesModel),
      mTextEdit(nullptr)

{
    auto log = spdlog::get("EditorTabController");
    if (log == nullptr)
    {

        log = spdlog::stdout_color_mt("EditorTabController");
    }
    log->info("Constructing");
    mForm.setupUi(this);
    mTextEdit = mForm.textEdit;
    mTextEdit->setUndoRedoEnabled(true);

    QTextDocument *doc = mTextEdit->document();
    QFont font = doc->defaultFont();
    font.setFamily("Courier New");
    font.setPointSize(16);
    doc->setDefaultFont(font);

    mSaveButton = mForm.saveButton;
    mRevertButton = mForm.revertButton;
    setupRevertSaveSignals();
    createTemplatesComboBox(mForm.templateComboBox);
    connect(mTextEdit,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
}

EditorTabController::~EditorTabController
()
{
    auto log = spdlog::get("EditorTabController");
    log->info("Destructing");
}

void
EditorTabController::setAssetDefinitionHandle
(IAssetDefinition* definitionHandle)
{
    auto log = spdlog::get("EditorTabController");
    log->info("Using asset {}", definitionHandle->getNameAndUuidString());
    mAssetDefinitionHandle = definitionHandle;
}

IAssetDefinition*
EditorTabController::getAssetDefinitionHandle
()
{
   return mAssetDefinitionHandle;
}

void
EditorTabController::useLuaHighlighter
()
{
    auto log = spdlog::get("EditorTabController");
    log->info("Using LuaSyntaxHighlighter");
    mHighlighter = make_shared<LuaHighlighter>(mTextEdit->document());
}

void
EditorTabController::useGLSLHighlighter
()
{
    auto log = spdlog::get("EditorTabController");
    log->info("Using GLSLSyntaxHighlighter");
    mHighlighter = make_shared<GlslSyntaxHighlighter>(mTextEdit->document());
}

void
EditorTabController::setPlainText
(QString data)
{
    mTextEdit->setPlainText(data);
    mTextHasChanged = false;
}

QString
EditorTabController::getPlainText
()
{
    return mTextEdit->toPlainText();
}

bool
EditorTabController::hasTextChanged
()
{
    return mTextHasChanged;
}

void
EditorTabController::onTextChanged
()
{
   mTextHasChanged = true;
}

void EditorTabController::clearTextChanged()
{
    mTextHasChanged = false;
}

void EditorTabController::setupRevertSaveSignals()
{
    connect(mSaveButton,SIGNAL(clicked(bool)),this,SLOT(onSaveButtonClicked(bool)));
    connect(mRevertButton,SIGNAL(clicked(bool)),this,SLOT(onRevertButtonClicked(bool)));
}

void
EditorTabController::setProjectDirectoryModelHandle
(ProjectDirectoryModel* projectDirectoryModelHandle)
{
    mProjectDirectoryModelHandle = projectDirectoryModelHandle;
}

void
EditorTabController::onComboTemplateChanged
(const QString& templateName)
{
    auto log = spdlog::get("EditorTabController");
    log->info("Selected template {}",templateName.toStdString());

    // Get file type

    QString vertex = QString::fromStdString(Constants::SHADER_VERTEX_FILE_NAME);
    QString fragment = QString::fromStdString(Constants::SHADER_FRAGMENT_FILE_NAME);
    QString lua = QString::fromStdString(Constants::ASSET_FORMAT_SCRIPT_LUA);
    QString templateContent;

    // Get Lua Template
    if (mFileFormat == lua)
    {
        templateContent = mTemplatesModelHandle->getScriptTemplate(templateName, lua);
    }
    // Get Shader Template
    else if (mFileFormat == vertex)
    {
        templateContent = mTemplatesModelHandle->getShaderTemplate(templateName, vertex);
    }
    else if (mFileFormat == fragment)
    {
        templateContent = mTemplatesModelHandle->getShaderTemplate(templateName, fragment);
    }

    setPlainText(templateContent);
}

void
EditorTabController::setTemplatesModelHandle
(TemplatesModel* templatesModelHandle)
{
    mTemplatesModelHandle = templatesModelHandle;
}

void
EditorTabController::createTemplatesComboBox
(QComboBox* editor)
{
    auto log = spdlog::get("EditorTabController");

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

void
EditorTabController::onSaveButtonClicked
(bool checked)
{
    Q_UNUSED(checked)
    auto log = spdlog::get("EditorTabController");
    int tabIndex = mTabWidget->currentIndex();
    if (tabIndex < 0)
    {
        return;
    }

    if (mAssetDefinitionHandle != nullptr)
    {
        log->info("Saving {}",mAssetDefinitionHandle->getNameAndUuidString());
        QString data = getPlainText();

        bool writeResult = false;

        if (mAssetDefinitionHandle->isTypeShader())
        {
            writeResult = mProjectDirectoryModelHandle->writeAssetData(
                data,
                mAssetDefinitionHandle,
                mFileFormat
            );
        }
        else if (mAssetDefinitionHandle->isTypeScript())
        {
            writeResult = mProjectDirectoryModelHandle->writeAssetData(data,mAssetDefinitionHandle);
        }

        if (writeResult)
        {
            QMessageBox::information(
                    this,
                    "Asset Saved",
                    QString("Successfully saved %1")
                    .arg(QString::fromStdString(mAssetDefinitionHandle->getName()))
                    );
        }
        else
        {
            QMessageBox::critical(
                this,
                "Save Failed",
                QString("Unable to save %1")
                .arg(QString::fromStdString(mAssetDefinitionHandle->getName()))
            );
        }
    }
}

void
EditorTabController::onRevertButtonClicked
(bool checked)
{
    Q_UNUSED(checked)
    auto log = spdlog::get("EditorTabController");
    int tabIndex = mTabWidget->currentIndex();
    if (tabIndex < 0)
    {
        return;
    }

    if (mAssetDefinitionHandle != nullptr)
    {
        int result = QMessageBox::question(
                    this,
                    "Revert File",
                    "Are you sure you want to revert to the last saved state?"
                    );

        if (result == QMessageBox::Ok)
        {
            log->info("Reverting {} back to saved state",mAssetDefinitionHandle->getNameAndUuidString());
        }
    }
}
