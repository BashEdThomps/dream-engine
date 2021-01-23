//
//  GLWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once


#include "Widgets/DreamToolWidget.h"

#include <DreamCore.h>
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;

namespace octronic::dream::tool
{
    struct GLWidgetVertex
    {
        vec3 Position;
        vec3 Color;
    };

    class GLWidget : public DreamToolWidget
    {
    public:
        GLWidget(DreamToolContext* project, bool visible = true);
        virtual ~GLWidget() override;
        void setViewMatrix(mat4);
        void setProjectionMatrix(mat4);
        virtual void draw() override;
        void init();
        void setPosition(vec3);
        vec3 getPosition();
    protected:
        void addVertex(GLWidgetVertex lv);
        virtual void setShader();
        void initShader();
        void initVaoVbo();

    protected:
        mat4 mModelMatrix;
        mat4 mViewMatrix;
        mat4 mProjectionMatrix;
        GLuint mVao;
        GLuint mVbo;
        GLuint mShaderProgram;
        vector<GLWidgetVertex> mVertexBuffer;
        vector<GLuint> mIndexBuffer;
        string mVertexShaderSource;
        string mFragmentShaderSource;
        GLint mModelUniform;
        GLint mViewUniform;
        GLint mProjectionUniform;

        // Inherited via DTWidget
    };
}
