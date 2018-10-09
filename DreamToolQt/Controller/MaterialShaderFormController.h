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
using Dream::ModelCache;
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
    void setShaderHandlesVector(vector<ShaderDefinition*> shaders);
    void readMaterials();
    Ui::MaterialShaderTableForm mUi;

private:
    void populate();
    int processAssimpNode(aiNode* node, const aiScene* scene);

protected:
    unique_ptr<MaterialShaderTableDelegate> mTableDelegate;
    ModelDefinition* mModelDefinitionHandle;
    QString mProjectPath;
    MaterialShaderTableModel mTableModel;
    shared_ptr<Importer> loadImporter(string path);
};
