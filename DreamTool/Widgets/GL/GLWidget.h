//
//  GLWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once

#include <DreamCore.h>
#include "../DTWidget.h"

using namespace Dream;

namespace DreamTool
{
    struct LineVertex
    {
        vec3 Position;
        vec3 Color;
    };

    class GLWidget : public DTWidget
    {
    public:
        GLWidget(Project* project);
        virtual ~GLWidget() override;
        void setViewMatrix(mat4);
        void setProjectionMatrix(mat4);
        virtual void draw() override;
        void init();
    protected:
        void addLineVertex(LineVertex lv);
        virtual void setShader();
        void initShader();
        void initVaoVbo();
        void setPosition(vec3);
    protected:
        mat4 mModelMatrix;
        mat4 mViewMatrix;
        mat4 mProjectionMatrix;
        GLuint mVao;
        GLuint mVbo;
        GLuint mShaderProgram;
        vector<LineVertex> mVertexBuffer;
        string mVertexShaderSource;
        string mFragmentShaderSource;

        // Inherited via DTWidget
    };
}
