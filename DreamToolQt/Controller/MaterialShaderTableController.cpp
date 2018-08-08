#include "MaterialShaderTableController.h"
#include <spdlog/spdlog.h>
#include <DreamCore.h>
#include <assimp/scene.h>

MaterialShaderTableController::MaterialShaderTableController
(QWidget *parent)
    : QWidget(parent),
      mModelDefinitionHandle(nullptr)
{
    auto log = spdlog::get("MaterialShaderTableController");
    if (log==nullptr)
    {

        log = spdlog::stdout_color_mt("MaterialShaderTableController");
    }
    log->info("Constructing");
    mUi.setupUi(this);
    setWindowTitle("Material<->Shader Mappings");
    connect(mUi.addButton,SIGNAL(clicked(bool)),this,SLOT(onAddButtonClicked(bool)));
    connect(mUi.removeButton,SIGNAL(clicked(bool)),this,SLOT(onRemoveButtonClicked(bool)));
    connect(mUi.readMaterialsButton,SIGNAL(clicked(bool)),this,SLOT(onReadMaterialsButtonClicked(bool)));
}

MaterialShaderTableController::~MaterialShaderTableController
()
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->info("Destructing");
}

void MaterialShaderTableController::setProjectPath(QString projectPath)
{
    mProjectPath = projectPath;
}

void
MaterialShaderTableController::setModelDefinition
(Dream::ModelDefinition* def)
{
    mModelDefinitionHandle = def;
    populate();
}

void
MaterialShaderTableController::onAddButtonClicked
(bool)
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->info("Add Button Clicked");
    mTableModel.insertRows(mTableModel.rowCount(QModelIndex()),1,QModelIndex());
}

void
MaterialShaderTableController::onRemoveButtonClicked
(bool)
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->info("Remove Button Clicked");
    auto selected = mUi.tableView->currentIndex();
    mTableModel.removeRows(selected.row(),1,QModelIndex());
}

void MaterialShaderTableController::onReadMaterialsButtonClicked(bool)
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->info("Read Mateerials from Model Button Clicked");

    if (mProjectPath.length() == 0)
    {
        log->error("Cannot read model. Project Path is not set");
        return;
    }

    string assetPath = mModelDefinitionHandle->getAssetPath();
    string absolutePath = mProjectPath.toStdString()+assetPath;
    log->info("Reading model from {}",absolutePath);
    auto model = mAssimpCache.getModelFromCache(absolutePath);

    if (model == nullptr)
    {
        log->error("Model from assimp is null");
        return;
    }

    const aiScene* scene = model->GetScene();

    if(scene == nullptr)
    {
        log->error("Scene from assimp model is null");
        return;
    }

    int rows = processAssimpNode(scene->mRootNode, scene);
    mTableModel.insertRows(0,rows,QModelIndex());
}

void
MaterialShaderTableController::populate
()
{
   auto log = spdlog::get("MaterialShaderTableController");
   if (mModelDefinitionHandle != nullptr)
   {
       mTableModel.setModelDefinition(mModelDefinitionHandle);
       mUi.tableView->setModel(&mTableModel);
       mUi.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   }
   else
   {
    log->error("Cannot populate MaterialShader table, model definition is null");
   }
}

void MaterialShaderTableController::getAllUpInYourFace()
{
    show();
    activateWindow();
    raise();
    setFocus();
}

int
MaterialShaderTableController::processAssimpNode
(aiNode* node, const aiScene* scene)
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->info("processAssimpNode");

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
        log->info("Adding material {}",materialStr);
        if (mModelDefinitionHandle->addMaterialShader(materialStr,"<Not Set>"))
        {
            materialCount++;
        }
    }
    // Then do the same for each of its children
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
       materialCount += processAssimpNode(node->mChildren[i], scene);
    }
    return materialCount;
}

AssimpCache MaterialShaderTableController::mAssimpCache;
