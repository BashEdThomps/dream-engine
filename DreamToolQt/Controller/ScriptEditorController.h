#pragma once

#include <QObject>
#include <DreamCore.h>
#include <QWidget>

#include "ui_ScriptEditorForm.h"
#include "../Controller/ScriptEditorTabController.h"

class QSyntaxHighlighter;
class ProjectDirectoryModel;
class TemplatesModel;
class QComboBox;

namespace Dream
{
    class ScriptDefinition;
    class ShaderDefinition;
}

using Dream::ScriptDefinition;
using Dream::ShaderDefinition;

class ScriptEditorController : public QWidget
{
    Q_OBJECT
public:
    ScriptEditorController(QWidget* parent = nullptr);
    ~ScriptEditorController() override;

    void openScript(ScriptDefinition* scriptDefinitionHandle);
    void openShader(ShaderDefinition* shaderDefinitionHandle);

    void setProjectDirectoryModelHandle(ProjectDirectoryModel* projectDirectoryModelHandle);
    void getAllUpInYourFace();

    void setTemplatesModelHandle(TemplatesModel* templatesModelHandle);

protected:
    void setupRevertSaveSignals();
    void createTemplatesComboBox(QComboBox* editor);
    void setupCloseButtonSignal();
    void clearExistingTabs();
    int isAssetOpen(IAssetDefinition* definition);

protected slots:
    void onTabCloseRequested(int index);
    void onRevertButtonClicked(bool);
    void onSaveButtonClicked(bool);
    void onComboTemplateChanged(const QString&);

private:

    ProjectDirectoryModel* mProjectDirectoryModelHandle;
    TemplatesModel* mTemplatesModelHandle;
    Ui::ScriptEditorForm mScriptEditor;
    vector<shared_ptr<ScriptEditorTabController>> mTabForms;
    // Widgets
    QTabWidget* mTabWidget;
    QPushButton* mSaveButton;
    QPushButton* mRevertButton;
};
