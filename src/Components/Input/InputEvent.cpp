#include "InputEvent.h"

namespace Dream {
  namespace Components {
    namespace Input {

      InputEvent::InputEvent(int src) {
        mSource        = src;
        mKey           = 0;
        mScancode      = 0;
        mAction        = 0;
        mMods          = 0;
        mXScrollOffset = 0;
        mYScrollOffset = 0;
        mXPosition     = 0;
        mYPosition     = 0;
        mButton        = 0;
      }

      InputEvent::~InputEvent() {

      }

      int InputEvent::getMods() {
        return mMods;
      }

      void InputEvent::setMods(int mods) {
        mMods = mods;
      }

      int InputEvent::getKey() {
        return mKey;
      }

      void InputEvent::setKey(int key) {
        mKey = key;
      }

      int InputEvent::getAction() {
        return mAction;
      }

      void InputEvent::setAction(int action) {
        mAction = action;
      }

      int InputEvent::getScancode() {
        return mScancode;
      }

      void InputEvent::setScancode(int scancode) {
        mScancode = scancode;
      }

      int  InputEvent::getXScrollOffset() {
        return mXScrollOffset;
      }

      void InputEvent::setXScrollOffset(int xOffset) {
        mXScrollOffset = xOffset;
      }

      int  InputEvent::getYScrollOffset() {
        return mYScrollOffset;
      }

      void InputEvent::setYScrollOffset(int yOffset) {
        mYScrollOffset = yOffset;
      }

      void InputEvent::setXPosition(int xPosition) {
        mXPosition = xPosition;
      }

      int InputEvent::getXPosition() {
        return mXPosition;
      }

      void InputEvent::setYPosition(int yPosition) {
        mYPosition = yPosition;
      }

      int InputEvent::getYPosition() {
        return mYPosition;
      }

      int InputEvent::getButton() {
        return mButton;
      }

      void InputEvent::setButton(int button) {
        mButton = button;
      }

      int InputEvent::getSource() {
        return mSource;
      }

      int InputEvent::getXPositionOffset() {
        //if (LastEvent.getSource() == INPUT_TYPE_MOUSE_POSITION) {
        //  return getXPosition() - LastEvent.getXPosition();
        //} else {
          return 0;
        //}
      }

      int InputEvent::getYPositionOffset() {
        //if (LastEvent.getSource() == INPUT_TYPE_MOUSE_POSITION) {
        //  return getYPosition() - LastEvent.getYPosition();
        //} else {
          return 0;
        //}
      }

    } // End of Input
  } // End of Components
} // End of Dream
