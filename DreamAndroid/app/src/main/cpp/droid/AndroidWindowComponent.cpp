
#include "AndroidWindowComponent.h"
#include <DreamCore.h>

using octronic::dream::WindowComponent;

namespace octronic::dream::android
{
    AndroidWindowComponent::AndroidWindowComponent()
    : WindowComponent()
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
    }

    AndroidWindowComponent::~AndroidWindowComponent()
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
    }

    bool AndroidWindowComponent::init()
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
        return true;
    }

    void AndroidWindowComponent::getCurrentDimensions()
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
    }

    void AndroidWindowComponent::swapBuffers()
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
    }

    void AndroidWindowComponent::updateWindow(SceneRuntime *sr)
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
    }

    void AndroidWindowComponent::bindFrameBuffer()
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
        glBindFramebuffer(GL_FRAMEBUFFER, getFrameBuffer());
    }

    GLuint AndroidWindowComponent::getFrameBuffer() const
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
        return 0;
    }

    GLuint AndroidWindowComponent::getDepthBuffer() const
    {
        LOG_DEBUG("AndroidWindowComponent: {}",__FUNCTION__);
        return 0;
    }
}