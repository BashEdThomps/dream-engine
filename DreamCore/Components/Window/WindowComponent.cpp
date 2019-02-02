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

namespace Dream
{

    WindowComponent::WindowComponent()
        : Component(nullptr),
          mWidth(Constants::DEFAULT_SCREEN_WIDTH),
          mHeight(Constants::DEFAULT_SCREEN_HEIGHT),
          mShouldClose(false),
          mSizeHasChanged(true),
          mMouseX(0),
          mMouseY(0)
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

    bool
    WindowComponent::sizeHasChanged
    ()
    {
        bool temp = mSizeHasChanged;
        mSizeHasChanged = false;
        return temp;
    }

    double WindowComponent::getMouseX()
    const
    {
        return mMouseX;
    }

    double WindowComponent::getMouseY()
    const
    {
       return mMouseY;
    }

}
