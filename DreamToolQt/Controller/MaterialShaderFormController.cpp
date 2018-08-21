#include "MaterialShaderFormController.h"
#include <spdlog/spdlog.h>
#include <DreamCore.h>
#include <assimp/scene.h>
#include <QMessageBox>

MaterialShaderFormController::MaterialShaderFormController
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
    setWindowTitle("Material to Shader Mappings");
    mTableDelegate = unique_ptr<MaterialShaderTableDelegate>(new MaterialShaderTableDelegate(mUi.tableView));
    mUi.tableView->setItemDelegate(mTableDelegate.get());
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

MaterialShaderFormController::~MaterialShaderFormController
()
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->info("Destructing");
}

void MaterialShaderFormController::setProjectPath(QString projectPath)
{
    mProjectPath = projectPath;
}

void MaterialShaderFormController::setShaderHandlesVector(vector<ShaderDefinition*> shaders)
{
    mTableDelegate->setShaderDefinitions(shaders);
}

void
MaterialShaderFormController::setModelDefinition
(Dream::ModelDefinition* def)
{
    mModelDefinitionHandle = def;
    populate();
}

void
MaterialShaderFormController::onAddButtonClicked
(bool)
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->info("Add Button Clicked");
    mTableModel.insertRows(mTableModel.rowCount(QModelIndex()),1,QModelIndex());
}

void
MaterialShaderFormController::onRemoveButtonClicked
(bool)
{
    auto log = spdlog::get("MaterialShaderTableController");
    log->info("Remove Button Clicked");
    auto selected = mUi.tableView->currentIndex();
    mTableModel.removeRows(selected.row(),1,QModelIndex());
}

void MaterialShaderFormController::onReadMaterialsButtonClicked(bool)
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
        mModelDefinitionHandle->clearMaterialShaderList();
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
MaterialShaderFormController::populate
()
{
   auto log = spdlog::get("MaterialShaderTableController");
   log->info("Populating");
   if (mModelDefinitionHandle != nullptr)
   {
       log->info("ModelDefinition is present");
       onReadMaterialsButtonClicked(true);
       mTableModel.setModelDefinition(mModelDefinitionHandle);
       mUi.tableView->setModel(&mTableModel);
       mUi.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   }
   else
   {
    log->error("Cannot populate MaterialShader table, model definition is null");
   }
}

void MaterialShaderFormController::getAllUpInYourFace()
{
    show();
    activateWindow();
    raise();
    setFocus();
}

int
MaterialShaderFormController::processAssimpNode
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
        if (mModelDefinitionHandle->addMaterialShader(materialStr,""))
        {
            materialCount++;
        }
        else
        {
            log->info("{} was all ready in the SD JSON",materialStr);
        }
    }
    // Then do the same for each of its children
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
       materialCount += processAssimpNode(node->mChildren[i], scene);
    }
    return materialCount;
}

AssimpCache MaterialShaderFormController::mAssimpCache;
