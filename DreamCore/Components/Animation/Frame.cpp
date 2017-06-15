#include <iostream>

#include "Frame.h"

#include "../../Utilities/String.h"

namespace Dream
{
    Frame::Frame
    ()
    {
        mTranslation = vector<float>(3);
        mRotation = vector<float>(3);
        mScale = vector<float>(3);
    }


    Frame::~Frame
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Frame: Destroying Object" << endl;
        }
    }

    void
    Frame::applyToTransform
    (Transform3D* transform)
    {
        //transform->setTranslation(mTranslation);
        //transform->setRotation(mRotation);
        //transform->setScale(mScale);
    }

    vector<float>
    Frame::getTranslation
    ()
    {
        return mTranslation;
    }

    vector<float>
    Frame::getRotation
    ()
    {
        return mRotation;
    }

    vector<float>
    Frame::getScale
    ()
    {
        return mScale;
    }

    void
    Frame::setRotation
    (vector<float> rotation)
    {
        mRotation = rotation;
    }

    void
    Frame::setScale
    (vector<float> scale)
    {
        mScale = scale;
    }

    void
    Frame::setTranslation
    (vector<float> translation)
    {
        mTranslation = translation;
    }

    void
    Frame::showStatus
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Frame"
                 << "{" << endl
                 << "\tTranslation: " << String::floatVectorToString(mTranslation) << endl
                 << "\tRotation: " << String::floatVectorToString(mRotation) << endl
                 << "\tScale: " << String::floatVectorToString(mScale) << endl
                 << "}" << endl;
        }
    }

} // End of Dream
