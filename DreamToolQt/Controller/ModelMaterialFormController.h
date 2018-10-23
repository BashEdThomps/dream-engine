#pragma once

#include <QWidget>
#include <memory>
#include "Model/ModelMaterialTableModel.h"
#include "Model/ModelMaterialTableDelegate.h"
#include "ui_ModelMaterialTableForm.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>


namespace Dream
{
    class ModelDefinition;
    class MaterialDefinition;
    class ModelCache;
    class Project;
}

using Assimp::Importer;
using Dream::ModelDefinition;
using Dream::MaterialDefinition;
using Dream::Project;
using std::unique_ptr;

class ModelMaterialFormController : public QWidget
{
    Q_OBJECT
public:
    explicit ModelMaterialFormController(QWidget *parent = nullptr);
    ~ModelMaterialFormController() override;

    void setProjectPath(QString projectPath);
    void setModelDefinition(ModelDefinition* def);
    void setMaterialHandlesVector(vector<MaterialDefinition*> materials);
    void readMaterials();
    Ui::ModelMaterialTableForm mUi;

private:
    void populate();
    //int processAssimpNode(aiNode* node, const aiScene* scene);

protected:
    unique_ptr<ModelMaterialTableDelegate> mTableDelegate;
    //shared_ptr<Importer> loadImporter(string path);
    ModelDefinition* mModelDefinitionHandle;
    QString mProjectPath;
    ModelMaterialTableModel mTableModel;
};
