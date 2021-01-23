//
// Created by Ashley Thompson on 04/01/21.
//

#pragma once

#include <DreamCore.h>

using octronic::dream::WindowComponent;
using glm::ivec2;

namespace octronic::dream::android
{
    class AndroidWindow : public WindowComponent
    {
    public:
        AndroidWindow(ConfigModel* model);
        void ProcessInput() override;
        void Clear() override;
        void SwapBuffer() override;
        ivec2 GetWindowSize() const override;
        double GetTime() const override;
        bool ShouldClose() const override;
        void SetWindowSize(int w, int h);
        void UpdateProjectionMatrix();
        void UpdateViewPort();

        bool Load() override;

    protected:
        bool LoadGL() override;

    };
}


