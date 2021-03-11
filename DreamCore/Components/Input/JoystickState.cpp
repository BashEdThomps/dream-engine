#include "JoystickState.h"

namespace octronic::dream
{
    JoystickState::JoystickState()
        : mName(""),
        mButtonCount(0),
        mButtonData{0},
        mAxisCount(0),
        mAxisData{0.0f},
        mDeadZone(0.15f)
    {

    }

    float JoystickState::getAxisData(int idx)
    {
        return mAxisData[idx];
    }

    void JoystickState::setAxisData(unsigned int index, float data)
    {
        mAxisData[index] = data;
    }

    bool JoystickState::getButtonData(int idx)
    {
        return mButtonData[idx] != 0;
    }

    void JoystickState::setButtonData(unsigned int index, bool data)
    {
        mButtonData[index] = data ? 1 : 0;
    }

    float JoystickState::getDeadZone() const
    {
        return mDeadZone;
    }

    const float* JoystickState::getAxisDataPointer() const
    {
        return &mAxisData[0];
    }

    const unsigned char* JoystickState::getButtonDataPointer() const
    {
        return &mButtonData[0];
    }

    void JoystickState::setName(const string& name)
    {
        mName = name;
    }

    string JoystickState::getName() const
    {
        return mName;
    }

    int JoystickState::getAxisCount() const
    {
        return mAxisCount;
    }

    void JoystickState::setAxisCount(int count)
    {
        mAxisCount = count;
    }

    int JoystickState::getButtonCount() const
    {
        return mButtonCount;
    }

    void JoystickState::setButtonCount(int count)
    {
        mButtonCount = count;
    }

    bool JoystickState::clearsDeadzone(float val) const
    {
        if(val > mDeadZone)
        {
            return true;
        }
        else if (val < -mDeadZone)
        {
            return true;
        }
        return false;
    }
}
