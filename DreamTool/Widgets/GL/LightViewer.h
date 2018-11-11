#pragma once
#include "GLWidget.h"

namespace DreamTool
{
    class LightViewer :
        public GLWidget
    {
    public:
        LightViewer(DTState* project);
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

