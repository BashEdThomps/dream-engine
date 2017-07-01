#pragma once

#include <vector>

#include "../../Common/Constants.h"
#include "../Transform3D.h"

namespace Dream
{
    using namespace std;
    class Frame
    {
    private:
        Transform3D mTransform;
    public:
        Frame();
        ~Frame();

        void applyToTransform(Transform3D&);

        Transform3D getTransform();
        void setTransform(Transform3D transform);

        void showStatus();
    }; // End Frame

} // End of Dream


