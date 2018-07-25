#include <iostream>

#include "Frame.h"

#include "../../Utilities/String.h"

namespace Dream
{
    Frame::Frame
    ()
    {
        cout << "Frame: Constructing Object" << endl;
    }


    Frame::~Frame
    ()
    {
        cout << "Frame: Destroying Object" << endl;
    }

    void
    Frame::applyToTransform
    (Transform3D& transform)
    {
        //transform->setTranslation(mTranslation);
        //transform->setRotation(mRotation);
        //transform->setScale(mScale);
    }

    Transform3D
    Frame::getTransform
    ()
    {
        return mTransform;
    }

    void
    Frame::setTransform
    (Transform3D transform)
    {
        mTransform = transform;
    }

    void
    Frame::showStatus
    ()
    {
        cout << "Frame " << mTransform.getJson().dump() << endl;
    }

} // End of Dream
