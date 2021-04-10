#include "WindowComponent.h"

#include "Common/Constants.h"

namespace octronic::dream
{
    WindowComponent::WindowComponent()
        : Component(),
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
