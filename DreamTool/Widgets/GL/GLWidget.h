//
//  GLWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once


#include "../DreamCore/Common/GLHeader.h"
#include "../DTWidget.h"
#include "glm/glm.hpp"

using namespace Dream;
using namespace glm;

namespace DreamTool
{
    struct GLWidgetVertex
    {
        vec3 Position;
        vec3 Color;
    };

    class GLWidget : public DTWidget
    {
    public:
        GLWidget(DTState* project, bool visible = true);
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
