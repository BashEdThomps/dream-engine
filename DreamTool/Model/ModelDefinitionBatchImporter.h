#pragma once

#include <DreamCore.h>


using Dream::Directory;
using Dream::ProjectDefinition;
using Dream::ModelDefinition;

namespace DreamTool
{
    class DTContext;

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

    class ModelDefinitionBatchImporter
    {
    public:
        ModelDefinitionBatchImporter(DTContext* state);
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
        DTContext* mState;
        Directory mDirectory;
        vector<string> mModelsFound;
        vector<ModelDefinitionBatchImportResult> mImportResults;
        bool* mModelsToImport;
        bool mReplaceExisting;
        bool mAppendDirectoryName;
        void initImportParameters();
    };

}
