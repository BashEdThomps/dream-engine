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
    explicit ScriptEditorTabController(QString fileType, QWidget *parent = nullptr);
    ~ScriptEditorTabController() override;

    void setAssetDefinitionHandle(shared_ptr<IAssetDefinition> definitionHandle);
    shared_ptr<IAssetDefinition> getAssetDefinitionHandle();
    void useLuaHighlighter();
    void useGLSLHighlighter();
    void setPlainText(QString data);
    QString getPlainText();
    bool hasTextChanged();
    void clearTextChanged();

    QString getFileType() const;

signals:

public slots:
    void onTextChanged();
private:
    QString mFileType;
    bool mTextHasChanged;
    shared_ptr<QSyntaxHighlighter> mHighlighter;
    shared_ptr<IAssetDefinition> mAssetDefinitionHandle;
    CodeEditorWidget* mTextEdit;
    Ui::ScriptEditorTabForm mForm;
};
