#include "MouseState.h"

#include <cstring>

namespace octronic::dream
{
    MouseState::MouseState
    () :
        mPosX(0),
		mPosY(0),
		mScrollX(0),
        mScrollY(0),
        mButtonsPressed{false}
    {

    }

    float
    MouseState::getPosX
    () const
	{
		return mPosX;
	}

	void
    MouseState::setPosX
    (float value)
	{
		mPosX = value;
	}

	float
    MouseState::getPosY
    () const
	{
		return mPosY;
	}

	void
    MouseState::setPosY
    (float value)
	{
		mPosY = value;
	}

	float
    MouseState::getScrollX
    () const
	{
		return mScrollX;
	}

	void
    MouseState::setScrollX
    (float value)
	{
		mScrollX = value;
	}

	float
    MouseState::getScrollY
    () const
	{
		return mScrollY;
	}

	void
    MouseState::setScrollY
    (float value)
	{
		mScrollY = value;
	}

    bool
    MouseState::isButtonPressed
    (unsigned int button) const
    {
        return mButtonsPressed[button];
    }


    void
    MouseState::setButtonsPressed
    (bool* b, size_t sz)
	{
        memcpy(mButtonsPressed, b, sz);
    }
}
