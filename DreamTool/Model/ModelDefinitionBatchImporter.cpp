#include "ModelDefinitionBatchImporter.h"

#include "DreamToolContext.h"
#include <sstream>

using std::stringstream;
using std::static_pointer_cast;
using octronic::dream::Project;
using octronic::dream::ProjectDefinition;

namespace octronic::dream::tool
{
    ModelDefinitionBatchImporter::ModelDefinitionBatchImporter
    (DreamToolContext* context)
        : Model(context),
          mDirectory(nullptr),
          mModelsToImport(nullptr),
          mReplaceExisting(false),
          mAppendDirectoryName(false)
    {

    }

    ModelDefinitionBatchImporter::~ModelDefinitionBatchImporter
    ()
    {
        clearImportParameters();
    }

    vector<string>
    ModelDefinitionBatchImporter::getModelsFoundNames
    ()
    {
        return mModelsFound;
    }
    bool*
    ModelDefinitionBatchImporter::getModelsToImport
    ()
    const
    {
        return mModelsToImport;
    }

    bool
    ModelDefinitionBatchImporter::hasModels
    ()
    {
       return !mModelsFound.empty();
    }

    vector<ModelDefinitionBatchImportResult>
    ModelDefinitionBatchImporter::getImportResults
    ()
    const
    {
        return mImportResults;
    }

    void
    ModelDefinitionBatchImporter::clear
    ()
    {
       clearImportParameters();
       clearImportResults();
       clearModelsFound();
    }

    bool
    ModelDefinitionBatchImporter::getReplaceExisting
    ()
    const
    {
        return mReplaceExisting;
    }

    void
    ModelDefinitionBatchImporter::setReplaceExisting
    (bool replaceExisting)
    {
        mReplaceExisting = replaceExisting;
    }

    string
    ModelDefinitionBatchImporter::getDirectoryName
    ()
    {
        return mDirectory->getName();
    }

    bool
    ModelDefinitionBatchImporter::hasResults
    ()
    {
        return !mImportResults.empty();
    }

    string ModelDefinitionBatchImporter::resultString(ModelImportResult result)
    {
       switch(result)
       {
           case CREATED:
               return "Created";
           case REPLACED:
               return "Replaced";
           case SKIPPED:
               return "Skipped";
       }
       return "";
    }

    void
    ModelDefinitionBatchImporter::findModels
    ()
    {
        auto fm = mContext->getStorageManager();
        mModelsFound.clear();
        if(mDirectory->exists())
        {
            for(string objFileName : mDirectory->list("\\.obj$"))
            {
                auto objFile = mDirectory->file(objFileName);
                string justName = objFile->getNameWithoutExtension();
                auto mtlFile = mDirectory->file(justName+".mtl");
                if (objFile->exists() && mtlFile->exists())
                {
                   LOG_ERROR("ModelDefinitionBatchImporter: Found Valid Model {}", justName);
                   mModelsFound.push_back(justName);
                }
                fm->closeFile(objFile);
                fm->closeFile(mtlFile);
            }
        }
        std::sort(mModelsFound.begin(),mModelsFound.end());
        initImportParameters();
    }

    void
    ModelDefinitionBatchImporter::initImportParameters
    ()
    {
        clearImportParameters();
        if (mModelsFound.empty())
        {
            return;
        }
        mModelsToImport = static_cast<bool*>(malloc(sizeof(bool)*mModelsFound.size()));
        for (int i=0; i<mModelsFound.size();i++)
        {
            mModelsToImport[i] = true;
        }
    }

    void
    ModelDefinitionBatchImporter::import
    ()
    {
        auto projDef = mContext->getProject()->getDefinition();
        if (!projDef)
        {
            LOG_ERROR("ModelDefinitionBatchImporter: Import failed. No Project Definition");
            return;
        }
        LOG_ERROR("ModelDefinitionBatchImporter: Importing Models from path : {} ({})",
                  mDirectory->getPath(), mDirectory->getName());
        clearImportResults();

        if (!mModelsToImport)
        {
            LOG_ERROR("ModelDefinitionBatchImporter: Model Parameters are not set");
            return;
        }

        for (int idx = 0; idx<mModelsFound.size();idx++)
        {
            string justName = mModelsFound.at(idx);
            stringstream assetName;
            if (mAppendDirectoryName)
            {
               assetName << mDirectory->getName();
               assetName << ".";
            }
            assetName << justName;
            if (mModelsToImport[idx])
            {
                shared_ptr<ModelDefinition> modelDef;
                auto existing = static_pointer_cast<ModelDefinition>(projDef->getAssetDefinitionByName(assetName.str()));
                if (existing)
                {
                   modelDef = existing;
                   if (!mReplaceExisting)
                   {
                       LOG_ERROR("ModelDefinitionBatchImporter: Not set to replace models");
                       mImportResults.push_back(ModelDefinitionBatchImportResult{modelDef,ModelImportResult::SKIPPED});
                       continue;
                   }
                   mImportResults.push_back(ModelDefinitionBatchImportResult{modelDef,ModelImportResult::REPLACED});
                }
                else
                {
                    modelDef = static_pointer_cast<ModelDefinition>(projDef->createNewAssetDefinition(AssetType::ASSET_TYPE_ENUM_MODEL));
                    mImportResults.push_back(ModelDefinitionBatchImportResult{modelDef,ModelImportResult::CREATED});
                }

                modelDef->setName(assetName.str());

                auto fm = mContext->getStorageManager();

                auto objFile = mDirectory->file(justName+".obj");
                auto mtlFile = mDirectory->file(justName+".mtl");

                if (objFile->exists() && mtlFile->exists())
                {
                    LOG_ERROR("ModelDefinitionBatchImporter: Copying asset data for {}", modelDef->getNameAndUuidString());
                    objFile->readBinary();
                    mContext->getProjectDirectory()->writeAssetData(
                    	modelDef,
                    	objFile->getBinaryData());

                    mtlFile->readBinary();
                    mContext->getProjectDirectory()->writeAssetData(
                    	modelDef,
                    	mtlFile->getBinaryData(),
                    	mtlFile->getNameWithExtension());
                }
                fm->closeFile(objFile);
                fm->closeFile(mtlFile);
            }
        }
        clearImportParameters();
        clearModelsFound();
    }

    void
    ModelDefinitionBatchImporter::clearImportParameters
    ()
    {
        mReplaceExisting = false;
        if (mModelsToImport)
        {
            delete mModelsToImport;
            mModelsToImport = nullptr;
        }
    }

    void
    ModelDefinitionBatchImporter::setDirectory
    (const string& dir)
    {
        auto fm = mContext->getStorageManager();
        mDirectory = fm->openDirectory(dir);
    }

    void
    ModelDefinitionBatchImporter::clearModelsFound
    ()
    {
        mModelsFound.clear();
    }

    void
    ModelDefinitionBatchImporter::clearImportResults
    ()
    {
        mImportResults.clear();
    }

    bool
    ModelDefinitionBatchImporter::getAppendDirectoryName
    ()
    const
    {
        return mAppendDirectoryName;
    }

    void
    ModelDefinitionBatchImporter::setAppendDirectoryName
    (bool appendDirectoryName)
    {
        mAppendDirectoryName = appendDirectoryName;
    }
}
