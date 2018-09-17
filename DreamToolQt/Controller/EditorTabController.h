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

class QSyntaxHighlighter;
class ProjectDirectoryModel;
class TemplatesModel;
class QComboBox;

using std::shared_ptr;
using Dream::ScriptDefinition;
using Dream::ShaderDefinition;
using Dream::IAssetDefinition;

class EditorTabController : public QWidget
{
    Q_OBJECT
public:
    explicit EditorTabController
    (
        QString fileFormat,
        IAssetDefinition* assetDefinition,
        TemplatesModel* templatesModel,
        ProjectDirectoryModel* projectDirectoryModel,
        QWidget *parent = nullptr
    );

    ~EditorTabController() override;

    void setAssetDefinitionHandle(IAssetDefinition* definitionHandle);
    IAssetDefinition* getAssetDefinitionHandle();
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
    void setupRevertSaveSignals();
    void createTemplatesComboBox(QComboBox* editor);
    void setupCloseButtonSignal();

public slots:
    void onTextChanged();
    void onRevertButtonClicked(bool);
    void onSaveButtonClicked(bool);
    void onComboTemplateChanged(const QString&);


private:
    bool mTextHasChanged;
    QString mFileFormat;
    IAssetDefinition* mAssetDefinitionHandle;
    ProjectDirectoryModel* mProjectDirectoryModelHandle;
    TemplatesModel* mTemplatesModelHandle;

    // Widgets
    shared_ptr<QSyntaxHighlighter> mHighlighter;
    Ui::ScriptEditorTabForm mForm;
    QTabWidget* mTabWidget;
    QPushButton* mSaveButton;
    QPushButton* mRevertButton;
    CodeEditorWidget* mTextEdit;
};
