#pragma once

#include "ImGuiWidget.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using octronic::dream::WindowComponent;

namespace octronic::dream::tool
{
    /**
     * @brief The GLPreviewWindowComponent class
     *
     * Deferred Rendering explanation found here
     *     https://learnopengl.com/Advanced-OpenGL/Framebuffers
     */
	class GLPreviewWindowComponent : public ImGuiWidget, public WindowComponent
	{
	public:
		GLPreviewWindowComponent(DreamToolContext* ctx, bool visible = true);
		~GLPreviewWindowComponent() override;

		bool init() override;
		void draw() override;
		bool initFrameBuffer();

		void getCurrentDimensions() override;
		void swapBuffers() override;
		void updateWindow() override;
		void bindFrameBuffer() override;
		GLuint getFrameBuffer() const override;
        GLuint getDepthBuffer() const override;
        void pushTasks() override;

	private:
		GLuint mFBO;
		GLuint mTexture;
		GLuint mDepthBuffer;
		float mLastWidth, mLastHeight;
		static ImVec2 UV1;
		static ImVec2 UV2;
	};
}
