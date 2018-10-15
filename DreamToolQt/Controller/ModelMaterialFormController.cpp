#include "ModelMaterialFormController.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <DreamCore.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QMessageBox>

ModelMaterialFormController::ModelMaterialFormController
(QWidget *parent)
    : QWidget(parent),
      mModelDefinitionHandle(nullptr)
{
    auto log = spdlog::get("MaterialShaderTableController");
    if (log==nullptr)
    {

        log = spdlog::stdout_color_mt("MaterialShaderTableController");
    }
    log->debug("Constructing");
    mUi.setupUi(this);
    setWindowTitle("Material Mapping");
    mTableDelegate = unique_ptr<ModelMaterialTableDelegate>(new ModelMaterialTableDelegate(mUi.tableView));
    mUi.tableView->setItemDelegate(mTableDelegate.get());
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

ModelMaterialFormController::~ModelMaterialFormController
()
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->debug("Destructing");
}

void
ModelMaterialFormController::setProjectPath
(QString projectPath)
{
    mProjectPath = projectPath;
}

void
ModelMaterialFormController::setMaterialHandlesVector
(vector<MaterialDefinition*> shaders)
{
    mTableModel.setMaterialDefinitions(shaders);
    mTableDelegate->setMaterialDefinitions(shaders);
}

void
ModelMaterialFormController::setModelDefinition
(ModelDefinition* def)
{
    mModelDefinitionHandle = def;
    populate();
}

void ModelMaterialFormController::readMaterials()
{

    auto log = spdlog::get("MaterialShaderTableController");
    log->debug("Read Mateerials from Model Button Clicked");

    if (mProjectPath.length() == 0)
    {
        log->error("Cannot read model. Project Path is not set");
        return;
    }

    string assetPath = mModelDefinitionHandle->getAssetPath();
    string absolutePath = mProjectPath.toStdString()+assetPath;
    log->debug("Reading model from {}",absolutePath);
    auto model = loadImporter(absolutePath);

    if (model == nullptr)
    {
        log->error("Model from assimp is null");
        mModelDefinitionHandle->clearModelMaterialList();
        QMessageBox::warning(this, "No Model Available","Cannot populate materials, model data not found.");
        return;
    }

    const aiScene* scene = model->GetScene();

    if(scene == nullptr)
    {
        log->error("Scene from assimp model is null");
        return;
    }

    processAssimpNode(scene->mRootNode, scene);
    mUi.tableView->update();
}

void
ModelMaterialFormController::populate
()
{
   auto log = spdlog::get("MaterialShaderTableController");
   log->debug("Populating");
   if (mModelDefinitionHandle != nullptr)
   {
       log->debug("ModelDefinition is present");
       readMaterials();
       mTableModel.setModelDefinition(mModelDefinitionHandle);
       mUi.tableView->setModel(&mTableModel);
       mUi.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   }
   else
   {
    log->error("Cannot populate MaterialShader table, model definition is null");
   }
}

int
ModelMaterialFormController::processAssimpNode
(aiNode* node, const aiScene* scene)
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->debug("processAssimpNode");

    int materialCount = 0;
    // Process all the node's meshes (if any)
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        // Process material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString name;
        aiGetMaterialString(material,AI_MATKEY_NAME,&name);
        string materialStr = string(name.C_Str());
        log->debug("Adding material {}",materialStr);
        if (mModelDefinitionHandle->addModelMaterial(materialStr,""))
        {
            materialCount++;
        }
        else
        {
            log->debug("{} was all ready in the SD JSON",materialStr);
        }
    }
    // Then do the same for each of its children
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
       materialCount += processAssimpNode(node->mChildren[i], scene);
    }
    return materialCount;
}

shared_ptr<Importer>
ModelMaterialFormController::loadImporter
(string path)
{
    auto log = spdlog::get("MaterialShaderTableController");

    log->debug("Loading {} from disk",  path);

    auto importer = make_shared<Importer>();
    importer->ReadFile(path, aiProcess_Triangulate);

    const aiScene* scene = importer->GetScene();
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        log->error( "Error {}" ,importer->GetErrorString() );
        return nullptr;
    }

    return importer;
}
