/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "Components/AssetDefinition.h"

namespace Dream
{
    class ModelDefinition : public AssetDefinition
    {
    public:
        ModelDefinition(ProjectDefinition* pd, const json& js);
        ~ModelDefinition() override;
        bool isFormatAssimp();

        bool addModelMaterial(const string& material, uint32_t shader);
        json* getModelMaterials();
        void removeModelMaterial(const string& material);
        void clearModelMaterialList();
        uint32_t getDreamMaterialForModelMaterial(const string& mat);
    };
}
