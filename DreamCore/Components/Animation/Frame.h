#pragma once

#include <vector>

#include "../../Common/Constants.h"
#include "../../Common/Transform3D.h"

namespace Dream
{
    using namespace std;
    class Frame
    {
    private:
        vector<float> mTranslation;
        vector<float> mRotation;
        vector<float> mScale;
    public:
        Frame();
        ~Frame();
        void applyToTransform(Transform3D*);
        vector<float> getTranslation();
        void setTranslation(vector<float>);
        vector<float> getRotation();
        void setRotation(vector<float>);
        vector<float> getScale();
        void setScale(vector<float>);
        void showStatus();
    }; // End Frame

} // End of Dream


