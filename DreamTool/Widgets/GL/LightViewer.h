#pragma once
#include "GLWidget.h"

namespace octronic::dream::tool
{
    class LightViewer :
        public GLWidget
    {
    public:
        LightViewer(DreamToolContext* project);
        ~LightViewer() override;
        void draw() override;
        void init();
    protected:
        void setShader() override;
        const static vector<GLWidgetVertex> LightModelVertices;
        const static vector<GLuint> LightModelIndices;
        GLint mLightColorUniform;
    };
}

