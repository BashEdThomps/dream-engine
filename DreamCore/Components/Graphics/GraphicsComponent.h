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
#define SHADOW_SIZE  1024

namespace octronic::dream
{
  class WindowComponent;
  class ModelRuntime;
  class ShaderRuntime;
  class SceneRuntime;
  class EntityRuntime;
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
    GraphicsComponent(ProjectRuntime& pr);

    GraphicsComponent(GraphicsComponent&&) = default;
    GraphicsComponent& operator=(GraphicsComponent&&) = default;

    ~GraphicsComponent();

    void clearBuffers(SceneRuntime& sr);
    // Geometry ============================================================
    void renderModels(SceneRuntime&);
    // Environment =========================================================
    void renderEnvironment(SceneRuntime&);
    // Shadow ==============================================================
    bool setupShadowBuffers();
    void freeShadowBuffers();
    void renderShadowPass(SceneRuntime&);
    GLuint getShadowPassDepthBuffer() const;
    // Sprite ===============================================================
    void renderSprites(SceneRuntime& sceneRuntime);
    bool setupSpriteQuad();
    void freeSpriteQuad();
    // Font ================================================================
    void renderFonts(SceneRuntime& sceneRuntime);
    // Task ================================================================
    GraphicsTaskQueue& getTaskQueue();
    GraphicsDestructionTaskQueue& getDestructionTaskQueue();
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

  protected:
    void checkFrameBufferDimensions();

  private:
    // Shadow ==============================================================
    optional<reference_wrapper<EntityRuntime>> mShadowLight;
    GLuint mShadowPassFB;
    GLuint mShadowPassDepthBuffer;
    mat4 mShadowMatrix;
    // Font & Sprite =======================================================
    mat4 mScreenSpaceProjectionMatrix;
    GLuint mSpriteQuadVAO;
    GLuint mSpriteQuadVBO;
    // Task ================================================================
    GraphicsTaskQueue mTaskQueue;
    GraphicsDestructionTaskQueue mDestructionTaskQueue;
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
