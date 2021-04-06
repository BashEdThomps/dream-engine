#pragma once
#include "GLWidget.h"
#include "Grid.h"

namespace octronic::dream::tool
{
    enum CursorAction
    {
        XPlus,
        XMinus,
        YPlus,
        YMinus,
        ZPlus,
        ZMinus
    };

    class Cursor : public GLWidget
    {

    public:
        Cursor(DreamToolContext& st);
        ~Cursor() override;
        void init() override;
        void onAction(CursorAction a);
        void setPosition(vec3 pos, bool snap);
        void onAxisPairChanged(Grid::AxisPair);
        void setMousePosition(float x, float y);
        vec3 mouseToWorldSpace(float x, float y);

        vec4 getOutlineColor() const;
        void setOutlineColor(const vec4& outlineColor);

    private:
        const static vector<TranslationColorVertex> ModelVertices;
        const static vector<GLuint> ModelIndices;
    	const static vector<GLuint> OutlineIndices;

    private:
        vec4 mOutlineColor;
        bool mStepMajor;
    };
}
