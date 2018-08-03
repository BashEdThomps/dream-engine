#pragma once

#include <memory>
#include <QWidget>
#include <QSyntaxHighlighter>

#include <QTextEdit>
#include "ui_ScriptEditorTabForm.h"

namespace Dream
{
    class IAssetDefinition;
    class ScriptDefinition;
    class ShaderDefinition;
}

using std::shared_ptr;
using Dream::IAssetDefinition;

class ScriptEditorTabController : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptEditorTabController(QWidget *parent = nullptr);
    ~ScriptEditorTabController() override;

    void setAssetDefinitionHandle(IAssetDefinition* definitionHandle);
    IAssetDefinition* getAssetDefinitionHandle();
    void useLuaHighlighter();
    void useGLSLHighlighter();
    void setPlainText(QString data);
    QString getPlainText();

signals:

public slots:

private:
    shared_ptr<QSyntaxHighlighter> mHighlighter;
    IAssetDefinition* mAssetDefinitionHandle;
    CodeEditor* mTextEdit;
    Ui::ScriptEditorTabForm mForm;
};
