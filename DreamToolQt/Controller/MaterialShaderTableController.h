#pragma once

#include <QWidget>
#include <memory>
#include "Model/MaterialShaderTableModel.h"
#include "ui_MaterialShaderTableForm.h"
#include "DreamCore.h"


namespace Dream
{
    class ModelDefinition;
}

using Dream::ModelDefinition;
using Dream::AssimpCache;
using Dream::Project;
using std::unique_ptr;

class MaterialShaderTableController : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialShaderTableController(QWidget *parent = nullptr);
    ~MaterialShaderTableController() override;

    void setProjectPath(QString projectPath);
    void setModelDefinition(ModelDefinition* def);
    void getAllUpInYourFace();

private slots:
    void onAddButtonClicked(bool);
    void onRemoveButtonClicked(bool);
    void onReadMaterialsButtonClicked(bool);

private:
    void populate();
    int processAssimpNode(aiNode* node, const aiScene* scene);

protected:
    ModelDefinition* mModelDefinitionHandle;
    QString mProjectPath;
    MaterialShaderTableModel mTableModel;
    Ui::MaterialShaderTableForm mUi;
    static AssimpCache mAssimpCache;
};
