#pragma once

#include <QWidget>
#include <memory>
#include "Model/MaterialShaderTableModel.h"
#include "Model/MaterialShaderTableDelegate.h"
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

class MaterialShaderFormController : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialShaderFormController(QWidget *parent = nullptr);
    ~MaterialShaderFormController() override;

    void setProjectPath(QString projectPath);
    void setModelDefinition(ModelDefinition* def);
    void getAllUpInYourFace();

    void setShaderHandlesVector(vector<ShaderDefinition*> shaders);
private slots:
    void onAddButtonClicked(bool);
    void onRemoveButtonClicked(bool);
    void onReadMaterialsButtonClicked(bool);

private:
    void populate();
    int processAssimpNode(aiNode* node, const aiScene* scene);

protected:
    unique_ptr<MaterialShaderTableDelegate> mTableDelegate;
    ModelDefinition* mModelDefinitionHandle;
    QString mProjectPath;
    MaterialShaderTableModel mTableModel;
    Ui::MaterialShaderTableForm mUi;
    //static AssimpCache mAssimpCache;
};
