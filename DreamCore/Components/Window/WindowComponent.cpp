/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "WindowComponent.h"

#include "Common/Constants.h"

namespace octronic::dream
{
    WindowComponent::WindowComponent()
        : Component(nullptr),
          mWidth(0),
          mHeight(0),
          mShouldClose(false),
          mWindowSizeChangedFlag(false)
    {
    }

    WindowComponent::~WindowComponent
    ()
    {
    }

    void
    WindowComponent::setWidth
    (int width)
    {
        mWidth = width;
    }

    void
    WindowComponent::setHeight
    (int height)
    {
        mHeight = height;
    }

    int
    WindowComponent::getWidth
    ()
    const
    {
        return mWidth;
    }

    int
    WindowComponent::getHeight
    ()
    const
    {
        return mHeight;
    }

    void
    WindowComponent::setName
    (string name)
    {
        mName = name;
    }

    string
    WindowComponent::getName
    ()
    const
    {
        return mName;
    }

    void
    WindowComponent::close
    ()
    {
        mShouldClose = true;
    }

    bool
    WindowComponent::shouldClose
    ()
    const
    {
        return mShouldClose;
    }

    void
    WindowComponent::setShouldClose
    (bool close)
    {
        mShouldClose = close;
    }

    void WindowComponent::setWindowSizeChangedFlag(bool f)
    {
        mWindowSizeChangedFlag =  f;
    }

    bool WindowComponent::getWindowSizeChangedFlag() const
    {
        return mWindowSizeChangedFlag;
    }
}
