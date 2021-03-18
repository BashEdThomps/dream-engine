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
#include "GraphicsComponentTasks.h"
#include "Task/TaskQueue.h"

#include <glm/matrix.hpp>
#include <string>
#include <map>
#include <vector>
#include <memory>

using glm::mat4;
using glm::vec3;
using std::vector;
using std::shared_ptr;

#define GC_LIGHT_COUNT 4

namespace octronic::dream
{
    class WindowComponent;
    class ModelRuntime;
    class ShaderRuntime;
    class SceneRuntime;
    class EntityRuntime;
    class ModelMesh;
    class MaterialRuntime;
    class ShaderRuntime;

    typedef TaskQueue<GraphicsTask> GraphicsTaskQueue;
    typedef TaskQueue<GraphicsDestructionTask> GraphicsDestructionTaskQueue;


    /**
     * @brief GraphicsComponent is responsible for managing Dream's graphics pipeline.
     *
     * Dream implements a multistage rendering pipline. Each stage renders to it's
     * associated Textures or FrameBuffers allowing it to pass data onto the next
     * stage. Stages execute as follows:
     *
     * 1. Shadow Pass:
     * 		- Once all 3D objects have been rendered, a Shadow depth buffer is
     *        rendered.
     *
     *        The resulting buffers are passed to the lighting shader
     *
     *
     * 2. Geometry Pass:
     * 		- Renders all 3D objects using GL instanced rendering, arranged
     *        hierarchally to minimise shader/material/buffer switching.
     *
     *        For All meshes
     *        	Render All Meshes that use the same Shader, then switch to next shader
     *        		Render All Meshes that use the same Material, then switch to next Material
     *
     *        The resulting buffers are passed to the lighting shader
     *
    *
     * 4. Sprite Rendering
     * 		- After the 3D scene has been rendered and lit, 2D elements are drawn
     *        on top. The TextureCache's Runtimes are iterated and all instances of
     *        sprites are drawn to the screen in screen-space.
     *
     * 5. Font Rendering
     * 		- After the 3D scene & sprites have been rendered, Font instances are
     *        drawn on top. The FontRuntime cache is iterated and all instances of
     *        text are drawn to the screen in screen-space.
     */
    class GraphicsComponent : public Component
    {
    public:
        GraphicsComponent(const weak_ptr<ProjectRuntime>& pr);
        ~GraphicsComponent() override;

        void clearBuffers(const weak_ptr<SceneRuntime>& sr);
        // Geometry ============================================================
        void renderModels(const weak_ptr<SceneRuntime>&);
        // Environment =========================================================
        void renderEnvironment(const weak_ptr<SceneRuntime>&);
        // Shadow ==============================================================
        bool setupShadowBuffers();
        void freeShadowBuffers();
        void renderShadowPass(const weak_ptr<SceneRuntime>&);
	    GLuint getShadowPassDepthBuffer() const;
        // Sprite ===============================================================
        void renderSprites(const weak_ptr<SceneRuntime>& sceneRuntime);
        bool setupSpriteQuad();
        void freeSpriteQuad();
        // Font ================================================================
        void renderFonts(const weak_ptr<SceneRuntime>& sceneRuntime);
        // Task ================================================================
        weak_ptr<GraphicsTaskQueue> getTaskQueue() const;
        weak_ptr<GraphicsDestructionTaskQueue> getDestructionTaskQueue() const;
        // Lights ==============================================================
        vec3 getLightPosition(size_t index) const;
        void setLightPosition(size_t index, const vec3& p);
        vec3 getLightColor(size_t index) const;
        void setLightColor(size_t index, const vec3& p);
        size_t getLightCount() const;
        // Misc ================================================================
        bool init() override;
        bool setupBuffers();
        bool handleResize();
        void pushTasks() override;
    	void logShaders();

    protected:
        void checkFrameBufferDimensions();

    private:
        // Shadow ==============================================================
        weak_ptr<EntityRuntime> mShadowLight;
        GLuint mShadowPassFB;
        GLuint mShadowPassDepthBuffer;
        mat4 mShadowMatrix;
        const int SHADOW_SIZE = 1024;
        // Font & Sprite =======================================================
        mat4 mScreenSpaceProjectionMatrix;
        GLuint mSpriteQuadVAO;
        GLuint mSpriteQuadVBO;
        // Task ================================================================
        shared_ptr<GraphicsTaskQueue> mTaskQueue;
        shared_ptr<GraphicsDestructionTaskQueue> mDestructionTaskQueue;
        // Tasks
        shared_ptr<SetupBuffersTask> mSetupBuffersTask;
        shared_ptr<ResizeTask> mResizeTask;
        shared_ptr<RenderTask> mRenderTask;
        // Misc ================================================================
        GLint mMaxFrameBufferSize;
        // Lighting ============================================================
        vec3 mLightPositions[GC_LIGHT_COUNT];
        vec3 mLightColors[GC_LIGHT_COUNT];
    };
}
