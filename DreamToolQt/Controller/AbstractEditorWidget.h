#pragma once

#include <QWidget>
#include <DreamCore.h>

using namespace Dream;

class ProjectDirectoryModel;

class AbstractEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractEditorWidget(
        IAssetDefinition* def,
        ProjectDirectoryModel* dirModel,
        QWidget *parent = nullptr
    );
    virtual ~AbstractEditorWidget();

    IAssetDefinition* getDefinitionHandle() const;
    void setDefinitionHandle(IAssetDefinition* definitionHandle);

    ProjectDirectoryModel* getProjectDirectoryModelHandle() const;
    void setProjectDirectoryModelHandle(ProjectDirectoryModel* projectDirectoryModel);

protected:
    IAssetDefinition* mDefinitionHandle;
    ProjectDirectoryModel* mProjectDirectoryModelHandle;
};
