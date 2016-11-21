#ifndef InputEvent_hpp
#define InputEvent_hpp
#define INPUT_TYPE_NO_EVENT      -1
#define INPUT_TYPE_MOUSE_POSITION 0
#define INPUT_TYPE_MOUSE_SCROLL   1
#define INPUT_TYPE_MOUSE_BUTTON   2
#define INPUT_TYPE_KEYBOARD       3
#define INPUT_TYPE_JOYSTICK       4

#include <iostream>

namespace Dream {
  namespace Components {
    namespace Input {
      class InputEvent {
      private:
        int mKey;
        int mScancode;
        int mAction;
        int mMods;
        int mXScrollOffset;
        int mYScrollOffset;
        int mXPosition;
        int mYPosition;
        int mButton;
        int mSource;
      public:
        InputEvent(int source);
        ~InputEvent();

        int  getMods();
        void setMods(int);

        int  getKey();
        void setKey(int);

        int  getAction();
        void setAction(int);

        int  getScancode();
        void setScancode(int);

        int  getXScrollOffset();
        void setXScrollOffset(int);

        int  getYScrollOffset();
        void setYScrollOffset(int);

        void setXPosition(int);
        int  getXPosition();

        void setYPosition(int);
        int  getYPosition();

        int getXPositionOffset();
        int getYPositionOffset();

        int  getButton();
        void setButton(int);

        int getSource();
      };

    }
  }
}

#endif /* InputEvent_hpp */
