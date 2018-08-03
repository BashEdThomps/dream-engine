#pragma once

#include <QObject>
#include <DreamCore.h>
#include <QDialog>

#include "ui_ScriptEditorForm.h"
#include "../Controller/ScriptEditorTabController.h"

class QSyntaxHighlighter;
class ProjectDirectoryModel;

namespace Dream {
    class ScriptDefinition;
    class ShaderDefinition;
}

using Dream::ScriptDefinition;
using Dream::ShaderDefinition;

class ScriptEditorController : public QDialog
{
    Q_OBJECT
public:
    ScriptEditorController(QWidget* parent = nullptr);
    ~ScriptEditorController();

    void openScript(ScriptDefinition* scriptDefinitionHandle);
    void openShader(ShaderDefinition* shaderDefinitionHandle);

    void setProjectDirectoryModelHandle(ProjectDirectoryModel* projectDirectoryModelHandle);
    void getAllUpInYourFace();

protected:
    void setupRevertSaveSignals();
protected slots:
    void onTabCloseRequested(int index);
    void onRevertButtonClicked(bool);
    void onSaveButtonClicked(bool);

private:
    int isAssetOpen(IAssetDefinition* definition);
    void setupCloseButtonSignal();
    void clearExistingTabs();

    ProjectDirectoryModel* mProjectDirectoryModelHandle;
    Ui::ScriptEditorForm mScriptEditor;
    vector<shared_ptr<ScriptEditorTabController>> mTabForms;
    QTabWidget* mTabWidget;
    QPushButton* mSaveButton;
    QPushButton* mRevertButton;
};
