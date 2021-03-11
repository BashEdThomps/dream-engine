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
using glm::vec4;
using glm::mat4;

namespace octronic::dream::tool
{
    struct TranslationColorVertex
    {
        vec3 Translation;
        vec4 Color;
    };

    class GLWidget : public DreamToolWidget
    {
    public:
        GLWidget(DreamToolContext* project, bool visible = true);
        virtual ~GLWidget() override;
        virtual void draw() override;
        virtual void init();
        void setTransform(const Transform&);
        Transform getTransform() const;

        float getLineWidth() const;
        void setLineWidth(float lineWidth);

    protected:
        void setShaderSources();
        void initShader();

        void drawLineBuffer();
        void drawTriangleBuffer();
        void drawPointBuffer();

        void addLineVertex(const TranslationColorVertex& v);
        void addLineVertices(const vector<TranslationColorVertex>& v);

        void addTriangleVertex(const TranslationColorVertex& v);
        void addTriangleVertices(const vector<TranslationColorVertex>& v);

        void addPointVertex(const TranslationColorVertex& v);
        void addPointVertices(const vector<TranslationColorVertex>& v);

        bool initLineBuffers();
        bool initTriangleBuffers();
        bool initPointBuffers();

        void clearLineVertexBuffer();
        void clearTriangleVertexBuffer();
        void clearPointVertexBuffer();

        void submitLineVertexBuffer();
        void submitTriangleVertexBuffer();
        void submitPointVertexBuffer();

    protected:
        Transform mTransform;
        float mLineWidth;
        // Shader
        GLuint mShaderProgram;
        string mVertexShaderSource;
        string mFragmentShaderSource;
        GLint mModelUniform;
        GLint mViewUniform;
        GLint mProjectionUniform;
        // GPU Buffers
        GLuint mLineVao;
		GLuint mTriangleVao;
		GLuint mPointVao;
		GLuint mLineVbo;
		GLuint mTriangleVbo;
		GLuint mPointVbo;
        // CPU Buffers
		vector<TranslationColorVertex> mLineVertexBuffer;
        vector<TranslationColorVertex> mTriangleVertexBuffer;
        vector<TranslationColorVertex> mPointVertexBuffer;
    };
}
