#include "Cursor.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

using glm::radians;

namespace octronic::dream::tool
{
    Cursor::Cursor(DreamToolContext* state)
        : GLWidget(state,false),
          mStepMajor(false),
          mOutlineColor(0.f,0.f,0.f,1.f)
    {
        LOG_TRACE("Cursor: Constructing");

    }

    Cursor::~Cursor()
    {
        LOG_TRACE("Cursor: Destructing");
    }

    void Cursor::init()
    {
        GLWidget::init();
        setLineWidth(3.f);
        clearTriangleVertexBuffer();
        clearLineVertexBuffer();
        for (auto index : ModelIndices)
        {
            addTriangleVertex(ModelVertices.at(index));
        }

        for (auto index : OutlineIndices)
        {
            TranslationColorVertex v = ModelVertices.at(index);
            v.Color = mOutlineColor;
            addLineVertex(v);
        }

        submitTriangleVertexBuffer();
        submitLineVertexBuffer();
    }

    void
    Cursor::onAction
    (CursorAction a)
    {
        Grid* grid = mContext->getGrid();
        vec3 txDelta(0.0f);
        float step = mStepMajor ? grid->getMajorSpacing() : grid->getMinorSpacing();

        // Make a move
        switch(a)
        {
            case CursorAction::XPlus:
                txDelta.x = step;
                break;
            case CursorAction::XMinus:
                txDelta.x = -step;
                break;
            case CursorAction::YPlus:
                txDelta.y = step;
                break;
            case CursorAction::YMinus:
                txDelta.y = -step;
                break;
            case CursorAction::ZPlus:
                txDelta.z = step;
                break;
            case CursorAction::ZMinus:
                txDelta.z = -step;
                break;
        }

        // Cancel unused axis
        switch (grid->getAxisPair())
        {
            case Grid::XY:
                txDelta.z = 0.0f;
                break;
            case Grid::XZ:
                txDelta.y = 0.0f;
                break;
            case Grid::YZ:
                txDelta.x =  0.0f;
                break;
        }

        auto currentTx = mTransform.getTranslation();
        mTransform.setTranslation(currentTx + txDelta);
    }

    void
    Cursor::setPosition
    (vec3 pos, bool snap)
    {
        Grid* grid = mContext->getGrid();
        if (snap)
        {
            float minor = grid->getMinorSpacing();
            pos.x = pos.x-fmod(pos.x,minor);
            pos.y = pos.y-fmod(pos.y,minor);
            pos.z = pos.z-fmod(pos.z,minor);
        }

        switch (grid->getAxisPair())
        {
            case Grid::XY:
                pos.z = 0.0f;
                break;
            case Grid::XZ:
                pos.y = 0.0f;
                break;
            case Grid::YZ:
                pos.x = 0.0f;
                break;
        }
        mTransform.setTranslation(pos);
    }

    void
    Cursor::onAxisPairChanged
    (Grid::AxisPair ap)
    {
        vec3 current = mTransform.getTranslation();

        switch (ap)
        {
            case Grid::XY:
                current.z = 0.0f;
                mTransform.setPitch(radians(90.f));
                mTransform.setYaw(0.f);
                mTransform.setRoll(0.f);
                break;
            case Grid::XZ:
                current.y = 0.0f;
                mTransform.setPitch(0.f);
                mTransform.setYaw(0.f);
                mTransform.setRoll(0.f);
                break;
            case Grid::YZ:
                current.x = 0.0f;
                mTransform.setPitch(0.f);
                mTransform.setYaw(0.f);
                mTransform.setRoll(radians(270.f));
                break;
        }
        mTransform.setTranslation(current);
    }

    void
    Cursor::setMousePosition
    (float x, float y)
    {
        vec3 pos = mouseToWorldSpace(x,y);
        mTransform.setTranslation(pos);
    }

    vec3
    Cursor::mouseToWorldSpace
    (float x, float y)
    {
        // TODO
        return vec3(0.f);
    }

    vec4 Cursor::getOutlineColor() const
    {
        return mOutlineColor;
    }

    void Cursor::setOutlineColor(const vec4& outlineColor)
    {
        mOutlineColor = outlineColor;
    }

    const vector<TranslationColorVertex> Cursor::ModelVertices =
    {
        {{ 0.f, 0.f,  0.f}, {0.0f, 1.0f, 0.0f, 1.f}}, // 0 Bottom
        {{-1.f, 2.f,  1.f}, {0.0f, 1.0f, 0.0f, 1.f}}, // 1 Top back  Left
        {{-1.f, 2.f, -1.f}, {0.0f, 1.0f, 0.0f, 1.f}}, // 2 Top front Left
        {{ 1.f, 2.f,  1.f}, {0.0f, 1.0f, 0.0f, 1.f}}, // 3 Top back  Right
        {{ 1.f, 2.f, -1.f}, {0.0f, 1.0f, 0.0f, 1.f}}, // 4 Top front Right
    };

    const vector<GLuint> Cursor::OutlineIndices =
    {
        // Sides
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        // Top
        1, 2,
        3, 4,
        2, 4,
        1, 3
    };

    const vector<GLuint> Cursor::ModelIndices =
    {
        0, 1, 2,
        0, 2, 4,
        0, 4, 3,
        0, 3, 1,
        3, 4, 1,
        1, 4, 2,
    };
}
