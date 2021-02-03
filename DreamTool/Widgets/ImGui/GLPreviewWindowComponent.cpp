#include "GLPreviewWindowComponent.h"

#include "DreamToolContext.h"
#include <glm/gtc/matrix_transform.hpp>

using octronic::dream::ProjectRuntime;

namespace octronic::dream::tool
{
    GLPreviewWindowComponent::GLPreviewWindowComponent
    (DreamToolContext* project, bool visible)
        : ImGuiWidget(project,visible), WindowComponent("GLPreviewWindowComponent"),
          mTexture(0),
          mDepthBuffer(0),
          mLastWidth(0.f),
          mLastHeight(0.f)
    {
        LOG_TRACE("GLPreviewWindowComponent: {}", __FUNCTION__);
    }


    GLPreviewWindowComponent::~GLPreviewWindowComponent ()
    {
        LOG_TRACE("GLPreviewWindowComponent: {}", __FUNCTION__);
        if (mFBO > 0)
        {
            glDeleteFramebuffers(1,&mFBO);
            mFBO = 0;
        }

        if (mDepthBuffer > 0)
        {
            glDeleteRenderbuffers(1,&mDepthBuffer);
            mDepthBuffer = 0;
        }

        if (mTexture > 0)
        {
            glDeleteTextures(1, &mTexture);
            mTexture = 0;
        }
    }

    bool GLPreviewWindowComponent::init()
    {
        LOG_DEBUG("GLPreviewWindowComponent: {}", __FUNCTION__);
        if (!initFrameBuffer()) return false;
        return true;
    }

    bool GLPreviewWindowComponent::initFrameBuffer()
    {
        LOG_DEBUG("GLPreviewWindowComponent: {}", __FUNCTION__);

        if (mWidth == 0 || mHeight == 0)
        {
            LOG_ERROR("GLPreviewWindowComponent: Can't create render targets yet, size == 0");
            return false;
        }

        // Create Window FrameBuffer ===========================================

        if (mFBO != 0)
        {
            LOG_DEBUG("GLPreviewWindowComponent: Clearing existing FBO");
            glDeleteFramebuffers(1,&mFBO);
            GLCheckError();
            mFBO = 0;
        }

        LOG_DEBUG("GLPreviewWindowComponent: Generating new FBO");
        glGenFramebuffers(1, &mFBO);
        GLCheckError();
        assert (mFBO != 0);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
        GLCheckError();

        // Create Texture  =====================================================

        // Clear Existing Texture
        if (mTexture != 0)
        {
            LOG_DEBUG("GLPreviewWindowComponent: Clearing existing Texture");
            glDeleteTextures(1,&mTexture);
            GLCheckError();
            mTexture = 0;
        }

        LOG_DEBUG("GLPreviewWindowComponent: Generating new Texture");
        glGenTextures(1, &mTexture);
        assert(mTexture != 0);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
        GLCheckError();

        // Create Depth Buffer =================================================

        if (mDepthBuffer != 0)
        {
            LOG_DEBUG("GLPreviewWindowComponent: Clearing existing Depth Buffer");
            glDeleteRenderbuffers(1,&mDepthBuffer);
            mDepthBuffer = 0;
            GLCheckError();
        }

        LOG_DEBUG("GLPreviewWindowComponent: Generating new Depth Buffer");
        glGenRenderbuffers(1,&mDepthBuffer);
        assert(mDepthBuffer != 0);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
        //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,mWidth,mHeight);
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
        GLCheckError();

        // Final Check =========================================================

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            GLuint fb_error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            LOG_ERROR("GLPreviewWindowComponent: Unable to create window framebuffer, error = {}",
                      GLGetFrameBufferError(fb_error));
            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }

        glBindFramebuffer(GL_FRAMEBUFFER,0);

        return true;
    }

    void GLPreviewWindowComponent::bindFrameBuffer()
    {
        LOG_TRACE("GLPreviewWindowComponent: {}", __FUNCTION__);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
        GLCheckError();
    }


    void GLPreviewWindowComponent::updateWindow(SceneRuntime *pr)
    {
        LOG_TRACE("GLPreviewWindowComponent: {}", __FUNCTION__);
    }

    void GLPreviewWindowComponent::draw()
    {
        LOG_TRACE("GLPreviewWindowComponent: {}", __FUNCTION__);
        mLastWidth = mWidth;
        mLastHeight = mHeight;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0.0f,0.0f));
        ImGui::Begin("GL Preview", &mVisible);
        ImVec2 contentAreaSize = ImGui::GetContentRegionAvail();
        mWidth = contentAreaSize.x;
        mHeight = contentAreaSize.y;

        glViewport(0,0,mWidth,mHeight);

        LOG_TRACE("GLPreviewWindowComponent: Got content area {}x{}", mWidth, mHeight);

        if (mWidth != mLastWidth || mHeight != mLastHeight)
        {
            LOG_DEBUG("GLPreviewWindowComponent: Got content area {}x{} -> {}x{}", mLastWidth, mLastHeight, mWidth, mHeight);
            init();
            setWindowSizeChangedFlag(true);
        }

        ImGuiIO& io = ImGui::GetIO();
        Project* project = mContext->getProject();
        if (project)
        {
            ProjectRuntime* pRunt = project->getRuntime();
            if (pRunt)
            {
                SceneRuntime* asRunt = pRunt->getActiveSceneRuntime();
                if (asRunt)
                {
                    ImGui::Image((void*)(intptr_t)mTexture,ImVec2(mWidth,mHeight),UV1, UV2);
                }
                else
                {
                    string nas = "No Active Scene";
                    const char* nas_c = nas.c_str();
                    ImVec2 str_sz = ImGui::CalcTextSize(nas_c);
                    ImGui::SetCursorPos(ImVec2(
                    	(mWidth  / 2.f) - (str_sz.x /2.f),
                    	(mHeight / 2.f) - (str_sz.y /2.f)));
                    ImGui::Text("%s",nas_c);
                }
            }
            else
            {
                string npr = "No Project Runtime Loaded";
				const char* npr_c = npr.c_str();
				ImVec2 str_sz = ImGui::CalcTextSize(npr_c);
				ImGui::SetCursorPos(ImVec2(
					(mWidth  / 2.f) - (str_sz.x /2.f),
					(mHeight / 2.f) - (str_sz.y /2.f)));
				ImGui::Text("%s",npr_c);
            }
        }
        else
        {
            string npl = "No Project Loaded";
			const char* npl_c = npl.c_str();
			ImVec2 str_sz = ImGui::CalcTextSize(npl_c);
			ImGui::SetCursorPos(ImVec2(
				(mWidth  / 2.f) - (str_sz.x /2.f),
				(mHeight / 2.f) - (str_sz.y /2.f)));
			ImGui::Text("%s",npl_c);
	}

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void GLPreviewWindowComponent::getCurrentDimensions()
    {
        // This is done in draw()
        LOG_DEBUG("GLPreviewWindowComponent: {}", __FUNCTION__);
    }

    void GLPreviewWindowComponent::swapBuffers()
    {
        LOG_TRACE("GLPreviewWindowComponent: {}", __FUNCTION__);
    }

    GLuint GLPreviewWindowComponent::getFrameBuffer() const
    {
        return mFBO;
    }

    GLuint GLPreviewWindowComponent::getDepthBuffer() const
    {
        return mDepthBuffer;
    }

    ImVec2 GLPreviewWindowComponent::UV1 = ImVec2(0,1);
    ImVec2 GLPreviewWindowComponent::UV2 = ImVec2(1,0);
}

