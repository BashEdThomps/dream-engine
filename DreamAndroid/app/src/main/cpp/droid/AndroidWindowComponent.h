#pragma once

#include <DreamCore.h>

using octronic::dream::WindowComponent;

namespace octronic::dream::android
{
    class AndroidWindowComponent : public WindowComponent
    {
    public:
        AndroidWindowComponent();

        ~AndroidWindowComponent() override;

        bool init() override;

        void getCurrentDimensions() override;

        void swapBuffers() override;

        void updateWindow(SceneRuntime *sr) override;

        void bindFrameBuffer() override;

        GLuint getFrameBuffer() const override;

        GLuint getDepthBuffer() const override;
    };
}


