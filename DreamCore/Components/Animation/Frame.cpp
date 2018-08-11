#include <iostream>

#include "Frame.h"

#include "../../Utilities/String.h"

namespace Dream
{
    Frame::Frame
    ()
        : ILoggable ("Frame")
    {
        auto log = getLog();
        log->trace("Constructing Object");
    }


    Frame::~Frame
    ()
    {
        auto log = getLog();
        log->trace("Destroying Object");
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
        auto log = getLog();
        log->info(mTransform.getJson().dump());
    }

} // End of Dream
