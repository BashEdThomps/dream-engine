#include "ScriptEditorTabController.h"
#include "View/SyntaxHighlighters/GLSLHighlighter.h"
#include "View/SyntaxHighlighters/LuaHighlighter.h"
#include "DreamCore.h"
#include <spdlog/spdlog.h>

using std::make_shared;

ScriptEditorTabController::ScriptEditorTabController(QString fileType, QWidget *parent)
    : QWidget(parent),
      mFileType(fileType),
      mTextHasChanged(false),
      mAssetDefinitionHandle(nullptr),
      mTextEdit(nullptr)
{
    auto log = spdlog::get("ScriptEditorTabController");
    if (log == nullptr)
    {

        log = spdlog::stdout_color_mt("ScriptEditorTabController");
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

    connect(mTextEdit,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
}

ScriptEditorTabController::~ScriptEditorTabController
()
{
    auto log = spdlog::get("ScriptEditorTabController");
    log->info("Destructing");
}

void
ScriptEditorTabController::setAssetDefinitionHandle
(shared_ptr<IAssetDefinition> definitionHandle)
{
    auto log = spdlog::get("ScriptEditorTabController");
    log->info("Using asset {}", definitionHandle->getNameAndUuidString());
    mAssetDefinitionHandle = definitionHandle;
}

shared_ptr<IAssetDefinition> ScriptEditorTabController::getAssetDefinitionHandle()
{
   return mAssetDefinitionHandle;
}

void ScriptEditorTabController::useLuaHighlighter()
{
    auto log = spdlog::get("ScriptEditorTabController");
    log->info("Using LuaSyntaxHighlighter");
    mHighlighter = make_shared<LuaHighlighter>(mTextEdit->document());
}

void ScriptEditorTabController::useGLSLHighlighter()
{
    auto log = spdlog::get("ScriptEditorTabController");
    log->info("Using GLSLSyntaxHighlighter");
    mHighlighter = make_shared<GlslSyntaxHighlighter>(mTextEdit->document());
}

void ScriptEditorTabController::setPlainText(QString data)
{
    mTextEdit->setPlainText(data);
    mTextHasChanged = false;
}

QString ScriptEditorTabController::getPlainText()
{
    return mTextEdit->toPlainText();
}

bool ScriptEditorTabController::hasTextChanged()
{
    return mTextHasChanged;
}

void ScriptEditorTabController::onTextChanged()
{
   mTextHasChanged = true;
}

QString ScriptEditorTabController::getFileType() const
{
    return mFileType;
}

void ScriptEditorTabController::clearTextChanged()
{
    mTextHasChanged = false;
}
