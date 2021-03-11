#include "KeyboardState.h"

namespace octronic::dream
{
    KeyboardState::KeyboardState()
        : mKeysPressed{false}
    {

    }
    bool KeyboardState::isKeyPressed(unsigned int key)
    {
        if (key > KeysMax)
        {
            return false;
        }
        else
        {
            return mKeysPressed[key];
        }
    }

    void KeyboardState::setKeysPressed(bool* b, size_t sz)
    {
        memcpy(mKeysPressed, b, sz);
    }
}
