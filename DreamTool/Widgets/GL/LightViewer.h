#pragma once
#include "GLWidget.h"

namespace octronic::dream::tool
{
    class LightViewer : public GLWidget
    {
    public:
        LightViewer(DreamToolContext& project);
        ~LightViewer() override;
        void init() override;
        void draw() override;
    protected:
        const static vector<TranslationColorVertex> LightModelVertices;
        const static vector<GLuint> LightModelIndices;
        GLint mLightColorUniform;
    };
}

