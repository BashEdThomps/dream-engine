#pragma once
#include "GLWidget.h"

namespace DreamTool
{
    class LightViewer :
        public GLWidget
    {
    public:
        LightViewer(Project* project);
        ~LightViewer() override;
        void draw() override;
    protected:
        void setShader() override;
        const static vector<LineVertex> LightModelVertices;
        const static vector<GLuint> LightModelIndices;
    };
}

