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

#pragma once

#include "Common/GLHeader.h"

#include <iostream>
#include <memory>
#include "Components/SharedAssetRuntime.h"
#include "Components/Graphics/GraphicsComponentTasks.h"
#include "TextureTasks.h"

using std::optional;
using std::reference_wrapper;
using std::vector;
using std::string;
using std::shared_ptr;

namespace octronic::dream
{
  class TextureDefinition;
  class EntityRuntime;
  class TextureRemoveFromGLTask;
  class TextureLoadIntoGLTask;
  class ShaderRuntime;

  enum CubeDebugMode
  {
    CUBE_DEBUG_NONE = 0,
    CUBE_DEBUG_ENVIRONMENT,
    CUBE_DEBUG_IRRADIANCE,
    CUBE_DEBUG_PREFILTER
  };

  class TextureRuntime : public SharedAssetRuntime
  {
  public:
    TextureRuntime(ProjectRuntime& pr,TextureDefinition& def);
    TextureRuntime(TextureRuntime&&) = default;
    TextureRuntime& operator=(TextureRuntime&&) = default;

    void pushDestructionTask();


    bool loadFromDefinition() override;
    bool init() override;

    GLuint getTextureID() const;
    void setTextureID(const GLuint& id);

    GLuint getCubeMapTextureID() const;
    GLuint getIrradianceTextureID() const;
    GLuint getPreFilterTextureID() const;
    GLuint getBrdfLutTextureID() const;

    string getPath() const;
    void setPath(const string& path);

    int getWidth() const;
    void setWidth(int width);

    int getHeight() const;
    void setHeight(int height);

    int getChannels() const;
    void setChannels(int channels);

    void* getRawImageData() const;
    void  setRawImageData(void* image);

    void pushTasks() override;

    bool loadTextureIntoGL();
    bool renderEquirectangularToCubeMap();
    bool renderIrradianceCubeMap();
    bool renderPreFilterCubeMap();
    bool renderBrdfLut();
    void renderCube();
    void renderQuad();

    bool getIsEnvironmentTexture() const;
    void setIsEnvironmentTexture(bool b);

    bool isHDR() const;

    CubeDebugMode getCubeDebugMode() const;
    void setCubeDebugMode(const CubeDebugMode& debugMode);

    GLuint getCubeDebugTexture();

  private:
    CubeDebugMode mCubeDebugMode;
    bool mIsHDR;
    GLuint mGLTextureID;
    int mWidth;
    int mHeight;
    int mChannels;
    bool mIsEnvironmentTexture;
    void* mRawImageData;
    GLuint mCaptureFBO;
    GLuint mCaptureRBO;
    // Equirectangular to Cube Map
    GLuint mEquiToCubeTexture;
    optional<reference_wrapper<ShaderRuntime>> mEquiToCubeShader;
    // Irradiance Map
    GLuint mIrradianceMapTexture;
    optional<reference_wrapper<ShaderRuntime>> mIrradianceMapShader;
    // Pre Filter
    GLuint mPreFilterCubeMapTexture;
    optional<reference_wrapper<ShaderRuntime>> mPreFilterShader;
    // BRDF LUT
    GLuint mBrdfLutTexture;
    optional<reference_wrapper<ShaderRuntime>> mBrdfLutShader;
    // Cube
    GLuint mCubeVAO;
    GLuint mCubeVBO;
    // Quad
    GLuint mQuadVAO;
    GLuint mQuadVBO;
    // Tasks
    shared_ptr<TextureLoadIntoGLTask>       mLoadIntoGLTask;
    shared_ptr<TextureSetupEnvironmentTask> mRenderCubeMapTask;
    shared_ptr<TextureRemoveFromGLTask>     mRemoveFromGLTask;
    // Statics
    const static glm::mat4 CubeCaptureProjection;
    const static glm::mat4 CubeCaptureViews[6];
    const static float CubeVertices[288];
    const static float QuadVertices[20];
  };
}
