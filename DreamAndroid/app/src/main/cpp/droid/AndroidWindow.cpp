//
// Created by Ashley Thompson on 04/01/21.
//

#include "AndroidWindow.h"

namespace octronic::dream::android
{
   AndroidWindow::AndroidWindow(ConfigModel* config)
   : Window(config)
   {
       LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
   }

    void AndroidWindow::ProcessInput() {}

    void AndroidWindow::Clear()
    {
        LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void AndroidWindow::SwapBuffer() {}

    ivec2 AndroidWindow::GetWindowSize() const
    {
        LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
       return glm::ivec2((int)mWidth, (int)mHeight);
    }

    double AndroidWindow::GetTime() const
    {
        LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        auto seconds = now.tv_sec;
        auto nano_seconds = now.tv_nsec;

        return ((seconds) + (nano_seconds/1000000000.0)) * 1.0;
    }

    bool AndroidWindow::ShouldClose() const
    {
        LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
       return false;
    }

    bool AndroidWindow::LoadGL()
    {
        LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
        GLCheckError();
        //Set the viewport
        UpdateViewPort();
        UpdateProjectionMatrix();
        //Initialize clear color
        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, 1.f);
        GLCheckError();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GLCheckError();
       return true;
    }

    void AndroidWindow::UpdateViewPort()
    {
        LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
        glViewport(0.f, 0.f, mWidth, mHeight);
        GLCheckError();
    }

    void AndroidWindow::UpdateProjectionMatrix()
    {
        LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
        mProjectionMatrix = glm::ortho(
            0.f, (float)mWidth,
            0.f, (float)mHeight,
            1.f, -1.f
        );
    }

    void AndroidWindow::SetWindowSize(int w, int h)
    {
        LOG_TRACE("AndroidWindow: Updating size {} x {}", w, h);
       mWidth = w;
       mHeight = h;
    }

    bool AndroidWindow::Load()
    {
        LOG_TRACE("AndroidWindow: {}", __FUNCTION__);
       Window::Load();
        if (!Window::Load()) return false;
        if (!LoadGL()) return false;
        return true;
    }
}