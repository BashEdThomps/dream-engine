#include "AbstractEditorWidget.h"

AbstractEditorWidget::AbstractEditorWidget
(IAssetDefinition* def, ProjectDirectoryModel* dirModel, QWidget *parent)
    : QWidget(parent),
      mDefinitionHandle(def),
      mProjectDirectoryModelHandle(dirModel)
{

}

AbstractEditorWidget::~AbstractEditorWidget
()
{

}

IAssetDefinition*
AbstractEditorWidget::getDefinitionHandle
()
const
{
    return mDefinitionHandle;
}

void
AbstractEditorWidget::setDefinitionHandle
(IAssetDefinition* definitionHandle)
{
    mDefinitionHandle = definitionHandle;
}

ProjectDirectoryModel*
AbstractEditorWidget::getProjectDirectoryModelHandle
()
const
{
    return mProjectDirectoryModelHandle;
}

void
AbstractEditorWidget::setProjectDirectoryModelHandle
(ProjectDirectoryModel* projectDirectoryModel)
{
    mProjectDirectoryModelHandle = projectDirectoryModel;
}
