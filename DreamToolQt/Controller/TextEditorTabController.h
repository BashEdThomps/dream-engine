#pragma once

#include "AbstractEditorWidget.h"
#include <memory>
#include <QSyntaxHighlighter>

#include <QTextEdit>
#include "ui_TextEditorTabForm.h"

namespace Dream
{
    class ScriptDefinition;
    class ShaderDefinition;
}

class QSyntaxHighlighter;
class ProjectDirectoryModel;
class TemplatesModel;
class QComboBox;

using std::shared_ptr;
using Dream::ScriptDefinition;
using Dream::ShaderDefinition;
using Dream::IAssetDefinition;

class TextEditorTabController : public AbstractEditorWidget
{
    Q_OBJECT
public:
    explicit TextEditorTabController
    (
        QString fileFormat,
        IAssetDefinition* assetDefinition,
        TemplatesModel* templatesModel,
        ProjectDirectoryModel* projectDirectoryModel,
        QWidget *parent = nullptr
    );

    ~TextEditorTabController() override;

    void useLuaHighlighter();
    void useGLSLHighlighter();
    void setPlainText(QString data);
    QString getPlainText();
    bool hasTextChanged();
    void clearTextChanged();

    QString getFileType() ;

    void openScript(ScriptDefinition* scriptDefinitionHandle);
    void openShader(ShaderDefinition* shaderDefinitionHandle);

    void setProjectDirectoryModelHandle(ProjectDirectoryModel* projectDirectoryModelHandle);
    void getAllUpInYourFace();

    void setTemplatesModelHandle(TemplatesModel* templatesModelHandle);

protected:
    void setupSaveSignals();
    void createTemplatesComboBox(QComboBox* editor);
    void setupCloseButtonSignal();

public slots:
    void onTextChanged(bool);
    void onSaveButtonClicked(bool);
    void onComboTemplateChanged(const QString&);

signals:
    void msgToStatusBar(QString);


private:
    bool mTextHasChanged;
    QString mFileFormat;
    TemplatesModel* mTemplatesModelHandle;

    // Widgets
    shared_ptr<QSyntaxHighlighter> mHighlighter;
    Ui::TextEditorTabForm mForm;
    QToolButton* mSaveButton;
    CodeEditorWidget* mTextEdit;
};
