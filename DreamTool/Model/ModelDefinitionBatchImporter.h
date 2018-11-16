#pragma once

#include "../../DreamCore/Common/DreamObject.h"
#include "../../DreamCore/Utilities/Directory.h"

namespace Dream
{
    class ProjectDefinition;
    class ModelDefinition;
}

using namespace Dream;

namespace DreamTool
{
    class DTState;
    enum ModelImportResult
    {
        CREATED,
        REPLACED,
        SKIPPED
    };

    struct ModelDefinitionBatchImportResult
    {
        ModelDefinition* definition;
        ModelImportResult result;
    };

    class ModelDefinitionBatchImporter : public DreamObject
    {
    public:
        ModelDefinitionBatchImporter(DTState* state);
        ~ModelDefinitionBatchImporter();
        void findModels();

        void import();
        void setDirectory(string dir);
        void clearImportParameters();
        void clearModelsFound();
        void clearImportResults();

        bool getAppendDirectoryName() const;
        void setAppendDirectoryName(bool appendDirectoryName);

        vector<string> getModelsFoundNames();
        bool* getModelsToImport() const;

        bool hasModels();
        vector<ModelDefinitionBatchImportResult> getImportResults() const;
        void clear();

        bool getReplaceExisting() const;
        void setReplaceExisting(bool replaceExisting);
        string getDirectoryName();
        bool hasResults();
        static string resultString(ModelImportResult result);

    private:
        DTState* mState;
        Directory mDirectory;
        vector<string> mModelsFound;
        vector<ModelDefinitionBatchImportResult> mImportResults;
        bool* mModelsToImport;
        bool mReplaceExisting;
        bool mAppendDirectoryName;
        void initImportParameters();
    };

}
