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

class WindowInputState;

class PathEditorFormController : public QWidget
{
    Q_OBJECT
public:
    explicit PathEditorFormController(QWidget *parent = nullptr);
    ~PathEditorFormController() override;

    void setProjectPath(QString projectPath);
    void setPathDefinition(PathDefinition* def);
    void getAllUpInYourFace();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

public slots:
    void onNumberOfTangentsChanged(int num);

private slots:
    void onAddButtonClicked(bool);
    void onRemoveButtonClicked(bool);
    void onTableChanged();
    void onTableSelectionChanged(const QItemSelection&,const  QItemSelection&);
    void onPathVisibleButtonClicked(bool);
    void onTangentVisibleButtonClicked(bool);
    void onTangentIndexChanged(int val);

signals:
    void notifyTableChanged();
    void notifySelectedRowChanged(int);
    void notifyCloseEvent();
    void notifyPathVisibilityChanged(bool);
    void notifyTangentVisibilityChanged(bool);
    void notifyTangentIndexChanged(int);

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


