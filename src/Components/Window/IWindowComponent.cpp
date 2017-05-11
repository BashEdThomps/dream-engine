/*
 * IWindowComponent.cpp
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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
#include "IWindowComponent.h"


namespace Dream
{
    IWindowComponent::IWindowComponent() : IComponent()
    {
        setWidth(IWC_DEFAULT_SCREEN_WIDTH);
        setHeight(IWC_DEFAULT_SCREEN_HEIGHT);
        setShouldClose(false);
    }

    IWindowComponent::~IWindowComponent()
    {

    }

    void IWindowComponent::setWidth(int width)
    {
        mWidth = width;
    }

    void IWindowComponent::setHeight(int height)
    {
        mHeight = height;
    }

    int IWindowComponent::getWidth()
    {
        getCurrentDimensions();
        return mWidth;
    }

    int IWindowComponent::getHeight()
    {
        getCurrentDimensions();
        return mHeight;
    }

    void IWindowComponent::setName(string name)
    {
        mName = name;
    }

    string IWindowComponent::getName()
    {
        return mName;
    }

    void IWindowComponent::close()
    {
        mShouldClose = true;
    }

    bool IWindowComponent::shouldClose()
    {
        return mShouldClose;
    }

    void IWindowComponent::setShouldClose(bool close)
    {
        mShouldClose = close;
    }
} // End of Dream
