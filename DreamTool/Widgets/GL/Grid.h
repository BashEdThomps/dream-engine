#pragma once

#include "GLWidget.h"

namespace octronic::dream::tool
{
    class Grid : public GLWidget
    {
    public:
        enum AxisPair
        {
            XY = 0,
            XZ,
            YZ
        };

        Grid(DreamToolContext& p);

        void init() override;

        float getMajorSpacing();
        void  setMajorSpacing(float);

        float getMinorSpacing();
        void  setMinorSpacing(float);

        float getSize();
        void setSize(float);
        void recalculateGridLines();

        vec4 getMajorColour() const;
        void setMajorColour(vec4 majorColour);

        vec4 getMinorColour() const;
        void setMinorColour(vec4 minorColour);

        void setAxisPair(Grid::AxisPair);

        Grid::AxisPair getAxisPair() const;

    protected: // Member functions
        void initMajorGridData();
        void initMinorGridData();
        void initAxisLines();

    protected: // Variables
        AxisPair mAxisPair;
        float mSize;
        float mMajorSpacing;
        float mMinorSpacing;
        vec4 mMajorColour;
        vec4 mMinorColour;
    };
}
