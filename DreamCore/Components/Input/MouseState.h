#pragma once

#include <unistd.h>

namespace octronic::dream
{
    class MouseState
    {
	public:
		MouseState();

		float getPosX() const;
		void setPosX(float value);

		float getPosY() const;
		void setPosY(float value);

		float getScrollX() const;
		void setScrollX(float value);

		float getScrollY() const;
		void setScrollY(float value);

		bool isButtonPressed(unsigned int button) const;
		void setButtonsPressed(bool* b, size_t sz);

	private:
		float mPosX;
		float mPosY;
		float mScrollX;
        float mScrollY;
        bool mButtonsPressed[5];
	};
}
