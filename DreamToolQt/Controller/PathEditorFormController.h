#pragma once

#include <QWidget>
#include <memory>
#include "Model/PathEditorTableModel.h"
#include "ui_PathEditorTableForm.h"
#include "DreamCore.h"


namespace Dream
{
    class PathDefinition;
}

using Dream::PathDefinition;
using Dream::AssimpCache;
using Dream::Project;
using std::unique_ptr;

class PathEditorFormController : public QWidget
{
    Q_OBJECT
public:
    explicit PathEditorFormController(QWidget *parent = nullptr);
    ~PathEditorFormController() override;

    void setProjectPath(QString projectPath);
    void setPathDefinition(PathDefinition* def);
    void getAllUpInYourFace();

private slots:
    void onAddButtonClicked(bool);
    void onRemoveButtonClicked(bool);
    void onTableChanged();
    void onTableSelectionChanged(const QItemSelection&, const QItemSelection&);
signals:
    void notifyTableChanged();
    void notifySelectedRowChanged(int);
    void notifyCloseEvent();

private:
    void populate();

protected:
    PathDefinition* mPathDefinitionHandle;
    QString mProjectPath;
    PathEditorTableModel mTableModel;
    Ui::PathEditorTableForm mUi;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent* event) override;
};


