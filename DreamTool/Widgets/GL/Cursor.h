#pragma once
#include "GLWidget.h"
#include "Grid.h"

namespace octronic::dream::tool
{
    class Cursor : public GLWidget
    {
    public:
        enum CursorAction
        {
            XPlus,
            XMinus,
            YPlus,
            YMinus,
            ZPlus,
            ZMinus,
            StepMajor,
            StepMinor
        };
    private:
        const static vector<GLWidgetVertex> ModelVertices;
        const static vector<GLuint> ModelIndices;

        bool mStepMajor;
    public:
        Cursor(DreamToolContext* st);
        ~Cursor() override;
        void draw() override;
        void init();
        void onAction(CursorAction a);
        void setPosition(vec3 pos, bool snap);
        void onAxisPairChanged(Grid::AxisPair);
        void setMousePosition(float x, float y);
        vec3 mouseToWorldSpace(float x, float y);
    };
}
