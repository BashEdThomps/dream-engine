#include "ScriptEditorTabController.h"
#include "View/SyntaxHighlighters/GLSLHighlighter.h"
#include "View/SyntaxHighlighters/LuaHighlighter.h"
#include "DreamCore.h"
#include <spdlog/spdlog.h>

using std::make_shared;

ScriptEditorTabController::ScriptEditorTabController(QWidget *parent)
    : QWidget(parent),
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
}

ScriptEditorTabController::~ScriptEditorTabController()
{
    auto log = spdlog::get("ScriptEditorTabController");
    log->info("Destructing");
}

void ScriptEditorTabController::setAssetDefinitionHandle(Dream::IAssetDefinition* definitionHandle)
{
    auto log = spdlog::get("ScriptEditorTabController");
    log->info("Using asset {}", definitionHandle->getNameAndUuidString());
    mAssetDefinitionHandle = definitionHandle;
}

Dream::IAssetDefinition*ScriptEditorTabController::getAssetDefinitionHandle()
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
}

QString ScriptEditorTabController::getPlainText()
{
   return mTextEdit->toPlainText();
}

