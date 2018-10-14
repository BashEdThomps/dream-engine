#include "TextEditorTabController.h"

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

TextEditorTabController::TextEditorTabController
(
    QString fileFormat,
    IAssetDefinition* assetDefinition,
    TemplatesModel* templatesModel,
    ProjectDirectoryModel* dirModel,
    QWidget *parent
)
    : AbstractEditorWidget(assetDefinition, dirModel, parent),
      mTextHasChanged(false),
      mFileFormat(fileFormat),
      mTemplatesModelHandle(templatesModel),
      mTextEdit(nullptr)

{
    auto log = spdlog::get("EditorTabController");
    if (log == nullptr)
    {

        log = spdlog::stdout_color_mt("EditorTabController");
    }
    log->debug("Constructing");
    mForm.setupUi(this);
    mTextEdit = mForm.textEdit;
    mTextEdit->setUndoRedoEnabled(true);

    QTextDocument *doc = mTextEdit->document();
    QFont font = doc->defaultFont();
    font.setFamily("Courier New");
    font.setPointSize(16);
    doc->setDefaultFont(font);

    mSaveButton = mForm.saveButton;
    setupSaveSignals();
    createTemplatesComboBox(mForm.templateComboBox);
    connect(mTextEdit,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
}

TextEditorTabController::~TextEditorTabController
()
{
    auto log = spdlog::get("EditorTabController");
    log->debug("Destructing");
}

void
TextEditorTabController::useLuaHighlighter
()
{
    auto log = spdlog::get("EditorTabController");
    log->debug("Using LuaSyntaxHighlighter");
    mHighlighter = make_shared<LuaHighlighter>(mTextEdit->document());
}

void
TextEditorTabController::useGLSLHighlighter
()
{
    auto log = spdlog::get("EditorTabController");
    log->debug("Using GLSLSyntaxHighlighter");
    mHighlighter = make_shared<GlslSyntaxHighlighter>(mTextEdit->document());
}

void
TextEditorTabController::setPlainText
(QString data)
{
    mTextEdit->setPlainText(data);
    mTextHasChanged = false;
}

QString
TextEditorTabController::getPlainText
()
{
    return mTextEdit->toPlainText();
}

bool
TextEditorTabController::hasTextChanged
()
{
    return mTextHasChanged;
}

void
TextEditorTabController::onTextChanged
()
{
   mTextHasChanged = true;
}

void TextEditorTabController::clearTextChanged()
{
    mTextHasChanged = false;
}

void TextEditorTabController::setupSaveSignals()
{
    connect(mSaveButton,SIGNAL(clicked(bool)),this,SLOT(onSaveButtonClicked(bool)));
}

void
TextEditorTabController::setProjectDirectoryModelHandle
(ProjectDirectoryModel* projectDirectoryModelHandle)
{
    mProjectDirectoryModelHandle = projectDirectoryModelHandle;
}

void
TextEditorTabController::onComboTemplateChanged
(const QString& templateName)
{
    auto log = spdlog::get("EditorTabController");
    log->debug("Selected template {}",templateName.toStdString());

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
TextEditorTabController::setTemplatesModelHandle
(TemplatesModel* templatesModelHandle)
{
    mTemplatesModelHandle = templatesModelHandle;
}

void
TextEditorTabController::createTemplatesComboBox
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
    log->debug("Got script templates {}",scriptTemplates.count());
    editor->addItems(scriptTemplates);

    QString shaderPrompt = "Shader Templates";
    editor->addItem(shaderPrompt);
    int shaderPromptIndex = editor->findText(shaderPrompt);
    qobject_cast<QStandardItemModel*>
    (
        editor->model()
    )->item(shaderPromptIndex)->setEnabled(false);

    QStringList shaderTemplates = mTemplatesModelHandle->getShaderTemplateNames();
    log->debug("Got shader templates {}",shaderTemplates.count());
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
TextEditorTabController::onSaveButtonClicked
(bool checked)
{
    Q_UNUSED(checked)
    auto log = spdlog::get("EditorTabController");

    if (mDefinitionHandle != nullptr)
    {
        log->debug("Saving {}",mDefinitionHandle->getNameAndUuidString());
        QString data = getPlainText();

        bool writeResult = false;

        if (mDefinitionHandle->isTypeShader())
        {
            writeResult = mProjectDirectoryModelHandle->writeAssetData
            (
                data,
                mDefinitionHandle,
                mFileFormat
            );
        }
        else if (mDefinitionHandle->isTypeScript())
        {
            writeResult = mProjectDirectoryModelHandle->writeAssetData(data,mDefinitionHandle);
        }

        if (writeResult)
        {
            emit msgToStatusBar
            (
                QString("Saved %1")
                    .arg(QString::fromStdString(mDefinitionHandle->getNameAndUuidString()))
            );
        }
        else
        {
            QMessageBox::critical
            (
                this,
                "Save Failed",
                QString("Unable to save %1")
                .arg(QString::fromStdString(mDefinitionHandle->getName()))
            );
        }
    }
}
