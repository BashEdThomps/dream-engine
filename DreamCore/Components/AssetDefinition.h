#pragma once

#include "Common/AssetType.h"
#include "Base/Definition.h"
#include "Math/Transform.h"

#include <memory>

using std::reference_wrapper;
using std::vector;

namespace octronic::dream
{
    class ProjectDefinition;

    /**
     * @brief Base class used to define an Asset's initial properties.
     */
    class AssetDefinition : public Definition
    {

    public:
        /**
         * @brief Default Constructur
         * @param project ProjectDefinition that owns this AssetDefinition
         * @param data JSON object from which to construct the AssetDefinition
         */
        AssetDefinition(ProjectDefinition& project, const json& data);


        AssetDefinition(AssetDefinition&&) = default;
        AssetDefinition& operator=(AssetDefinition&&) = default;

        /**
         * @return Returns the Dream::AssetType of the AssetDefinition
         * @see Dream::AssetType
         */
        AssetType getAssetType() const;

        /**
         * @return Get the pointer to the parant ProjectDefinition
         */
        ProjectDefinition& getProject() const;

        /**
         * @brief Set the type from a string
         */
        void setType(const string&);

        /**
         * @return Get the type as a string
         */
        string getType() const;

        void setFormat(const string&);
        string getFormat() const;

        void duplicateInto(AssetDefinition&);

        bool hasType(AssetType) const;
        bool hasType(string) const;

    protected:
        reference_wrapper<ProjectDefinition> mProjectDefinition;
    };
}
