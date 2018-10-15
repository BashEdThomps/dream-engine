/*
* GraphicsComponent
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once



#include <string>
#include <map>
#include <vector>

#include <glm/matrix.hpp>
#include "../IComponent.h"

using std::string;
using std::map;
using std::vector;
using glm::mat4;

typedef struct NVGcontext NVGcontext;

namespace Dream
{
    class IWindowComponent;
    class Camera;
    class ModelInstance;
    class ShaderInstance;
    class LightInstance;
    class SceneRuntime;
    class SceneObjectRuntime;
    class Texture;
    class ModelMesh;
    class MaterialInstance;
    class ShaderCache;

    class GraphicsComponent : public IComponent
    {
    private:
        mat4 mProjectionMatrix;
        mat4 mOrthoProjection;
        mat4 mViewMatrix;

        Camera* mCamera;

        float mMinimumDraw;
        float mMaximumDraw;
        float mMeshCullDistance;

        vector<SceneObjectRuntime*> mModelQueue;
        vector<LightInstance*> mLightQueue;

        IWindowComponent* mWindowComponent;
        ShaderCache* mShaderCacheHandle;
    public:
        GraphicsComponent(Camera*,IWindowComponent*);
        ~GraphicsComponent() override;

        void addToLightQueue(LightInstance*);
        void clearLightQueue();

        void clearModelQueue();
        void addToModelQueue(SceneObjectRuntime*);
        void drawModelQueue();
        void preModelRender();
        void postModelRender();

        void clearFontQueue();
        void addToFontQueue(SceneObjectRuntime*);
        void drawFontQueue();
        void preFontRender();
        void postFontRender();

        void addToInstanceMap(SceneObjectRuntime*);

        bool init(void) override;
        void updateComponent() override;
        void drawModel(SceneObjectRuntime*);
        void setWindowShouldClose(bool);
        Camera* getCamera();
        mat4 getViewMatrix();
        mat4 getProjectionMatrix();
        void onWindowDimensionsChanged();
        void handleResize();
        float getMeshCullDistance() const;
        void setMeshCullDistance(float meshCullDistance);
        void debugOptimisedModelQueue();
        void setMinimumDraw(float minimumDraw);
        void setMaximumDraw(float maximumDraw);
        bool getUpdateInstanceMapFlag() const;
        void setUpdateInstanceMapFlag(bool updateInstanceMapFlag);
        void setShaderCache(ShaderCache* cache);

    }; // End of GraphicsComponent
} // End of Dream
