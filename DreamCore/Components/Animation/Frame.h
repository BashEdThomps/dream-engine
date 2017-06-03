#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include "../../Constants.h"
#include "../../Transform3D.h"

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
        void applyToTransform(Transform3D const&);
        vector<float> getTranslation();

        void setTranslation(vector<float>);
        vector<float> getRotation();

        void setRotation(vector<float>);
        vector<float> getScale();

        void setScale(vector<float>);

        void showStatus();
    }; // End Frame
} // End of Dream

#endif // FRAME_H
