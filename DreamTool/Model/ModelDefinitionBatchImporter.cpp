#include "ModelDefinitionBatchImporter.h"
#include <sstream>
#include "../DTState.h"
#include "../../DreamCore/Project/Project.h"
#include "../../DreamCore/Project/ProjectDefinition.h"
#include "../../DreamCore/Project/ProjectDirectory.h"
#include "../../DreamCore/Components/Graphics/Model/ModelDefinition.h"
#include "../../DreamCore/Utilities/File.h"
#include "../../DreamCore/Utilities/Directory.h"

using namespace std;
using namespace Dream;

namespace DreamTool
{
    ModelDefinitionBatchImporter::ModelDefinitionBatchImporter
    (DTState* state)
        : DreamObject("BatchModelDefinitionImporter"),
          mState(state),
          mDirectory(Directory(".")),
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
        return mDirectory.getName();
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
           case DreamTool::CREATED:
               return "Created";
           case DreamTool::REPLACED:
               return "Replaced";
           case DreamTool::SKIPPED:
               return "Skipped";
       }
       return "";
    }

    void
    ModelDefinitionBatchImporter::findModels
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        mModelsFound.clear();
        if(mDirectory.exists())
        {
            for(string objFileName : mDirectory.list("\\.obj$"))
            {
                File objFile = mDirectory.file(objFileName);
                string justName = objFile.nameWithoutExtension();
                File mtlFile = mDirectory.file(justName+".mtl");
                if (objFile.exists() && mtlFile.exists())
                {
#ifdef DREAM_LOG
                   log->error("Found Valid Model {}", justName);
#endif
                   mModelsFound.push_back(justName);
                }
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
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        auto projDef = mState->project->getDefinition();
        if (!projDef)
        {
#ifdef DREAM_LOG
            log->error("Import failed. No Project Definition");
#endif
            return;
        }
#ifdef DREAM_LOG
        log->error("Importing Models from path : {} ({})",mDirectory.getPath(), mDirectory.getName());
#endif
        clearImportResults();

        if (!mModelsToImport)
        {
#ifdef DREAM_LOG
            log->error("Model Parameters are not set");
#endif
            return;
        }

        for (int idx = 0; idx<mModelsFound.size();idx++)
        {
            string justName = mModelsFound.at(idx);
            stringstream assetName;
            if (mAppendDirectoryName)
            {
               assetName << mDirectory.getName();
               assetName << ".";
            }
            assetName << justName;
            if (mModelsToImport[idx])
            {
                ModelDefinition* modelDef = nullptr;
                auto existing = dynamic_cast<ModelDefinition*>(projDef->getAssetDefinitionByName(assetName.str()));
                if (existing)
                {
                   modelDef = existing;
                   if (!mReplaceExisting)
                   {
#ifdef DREAM_LOG
                       log->error("Not set to replace models");
#endif
                       mImportResults.push_back(ModelDefinitionBatchImportResult{modelDef,ModelImportResult::SKIPPED});
                       continue;
                   }
                   mImportResults.push_back(ModelDefinitionBatchImportResult{modelDef,ModelImportResult::REPLACED});
                }
                else
                {
                    modelDef = dynamic_cast<ModelDefinition*>(projDef->createNewAssetDefinition(AssetType::MODEL));
                    mImportResults.push_back(ModelDefinitionBatchImportResult{modelDef,ModelImportResult::CREATED});
                }

                modelDef->setName(assetName.str());

                File objFile = mDirectory.file(justName+".obj");
                File mtlFile = mDirectory.file(justName+".mtl");
                if (objFile.exists() && mtlFile.exists())
                {
#ifdef DREAM_LOG
                    log->error("Copying asset data for {}", modelDef->getNameAndUuidString());
#endif
                    mState->projectDirectory.writeAssetData(modelDef,objFile.readBinary());
                    mState->projectDirectory.writeAssetData(modelDef,mtlFile.readBinary(),mtlFile.nameWithExtension());
                }
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
    (string dir)
    {
        mDirectory = Directory(dir);
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
