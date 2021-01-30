/*
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

#include "Common/GLHeader.h"
#include "Components/Component.h"
#include "GraphicsComponentTask.h"

#include <glm/matrix.hpp>
#include <string>
#include <map>
#include <vector>

using glm::mat4;
using std::vector;
using std::shared_ptr;

namespace octronic::dream
{
    class ShaderCache;
    class WindowComponent;
    class ModelRuntime;
    class ShaderRuntime;
    class LightRuntime;
    class SceneRuntime;
    class EntityRuntime;
    class Texture;
    class ModelMesh;
    class MaterialRuntime;
    class ShaderCache;
    class ShaderRuntime;

    /**
     * @brief GraphicsComponent is responsible for managing Dream's graphics pipeline.
     *
     * Dream implements a multistage rendering pipline. Each stage renders to it's
     * associated Textures or FrameBuffers allowing it to pass data onto the next
     * stage. Stages execute as follows:
     *
     * 1. Geometry Pass:
     * 		- Renders all 3D objects using GL instanced rendering, arranged
     *        hierarchally to minimise shader/material/buffer switching.
     *
     *        For All meshes
     *        	Render All Meshes that use the same Shader, then switch to next shader
     *        		Render All Meshes that use the same Material, then switch to next Material
     *
     *        The resulting buffers are passed to the lighting shader
     *
     * 2. Shadow Pass:
     * 		- Once all 3D objects have been rendered, a Shadow depth buffer is
     *        rendered.
     *
     *        The resulting buffers are passed to the lighting shader
     *
     * 3. Lighting Pass:
     * 		- Once Shadow depth buffer is rendered, the generated textures
     *        Position, Albedo, Normal, Depth, Ignore & Shadow are passed to
     *        the Lighting shader for Lighting calculations.
     *
     *        The result of the Lighting Shader is rendered to a full-screen
     *        quad.
     *
     * 4. Font Rendering
     * 		- After the 3D scene has been rendered and lit, 2D elements are drawn
     *        on top. The FontRuntime cache is iterated and all instances of
     *        text are drawn to the screen in screen-space.
     */
    class GraphicsComponent : public Component
    {
    public:
        GraphicsComponent(ProjectRuntime* pr, WindowComponent*);
        ~GraphicsComponent() override;


        // Geometry ============================================================
        bool setupGeometryBuffers();
        void freeGeometryBuffers();
        void renderGeometryPass(SceneRuntime*);
        GLuint getGeometryPassPositionBuffer() const;
        GLuint getGeometryPassAlbedoBuffer() const;
        GLuint getGeometryPassNormalBuffer() const;
        GLuint getGeometryPassDepthBuffer() const;
        GLuint getGeometryPassIgnoreBuffer() const;
        // Shadow ==============================================================
        bool setupShadowBuffers();
        void freeShadowBuffers();
        void renderShadowPass(SceneRuntime*);
        ShaderRuntime* getShadowPassShader() const;
        void setShadowPassShader(ShaderRuntime* shadowPassShader);
        GLuint getShadowPassDepthBuffer() const;
        // Light ===============================================================
        void addToLightQueue(EntityRuntime*);
        void clearLightQueue();
        bool setupScreenQuad();
        void renderLightingPass(SceneRuntime* sr);
        ShaderRuntime* getLightingShader() const;
        void setLightingShader(ShaderRuntime* lightingShader);
        // Font ================================================================
        void renderFonts(SceneRuntime* sceneRuntime);
        ShaderRuntime* getFontShader() const;
        void setFontShader(ShaderRuntime* fontShader);
        // Task ================================================================
        void pushTask(GraphicsComponentTask* t);
        void pushDestructionTask(const shared_ptr<GraphicsComponentDestructionTask>& t);
        void executeTaskQueue();
        void executeDestructionTaskQueue();
        const vector<GraphicsComponentTask*>& getDebugTaskQueue();
        // Misc ================================================================
        bool init() override;
        void onWindowDimensionsChanged();
        void handleResize();
        void setShaderCache(ShaderCache* cache);

    protected:
        void checkFrameBufferDimensions() const;

    private:
        // Geometry ============================================================
        GLuint mGeometryPassFB;
        GLuint mGeometryPassPositionBuffer;
        GLuint mGeometryPassAlbedoBuffer;
        GLuint mGeometryPassNormalBuffer;
        GLuint mGeometryPassDepthBuffer;
        GLuint mGeometryPassIgnoreBuffer;
        // Shadow ==============================================================
        EntityRuntime* mShadowLight;
        ShaderRuntime* mShadowPassShader;
        GLuint mShadowPassFB;
        GLuint mShadowPassDepthBuffer;
        mat4 mShadowMatrix;
        const int SHADOW_SIZE = 1024;
        // Lighting ============================================================
        ShaderRuntime* mLightingPassShader;
        GLuint mScreenQuadVAO;
        GLuint mScreenQuadVBO;
        vector<LightRuntime*> mLightQueue;
        // Font ================================================================
        ShaderRuntime* mFontShader;
        // Task ================================================================
        vector<GraphicsComponentTask*> mTaskQueue;
        vector<GraphicsComponentTask*> mDebugTaskQueue;
        vector<shared_ptr<GraphicsComponentDestructionTask> > mDestructionTaskQueue;
        // Misc ================================================================
        WindowComponent* mWindowComponent;
        ShaderCache* mShaderCache;
        GLint mMaxFrameBufferSize;
    };
}
