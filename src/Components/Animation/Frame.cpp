#include <iostream>
#include "Frame.h"
#include "../../String.h"

namespace Dream     {
  namespace Components {
    namespace Animation {

      Frame::Frame()  {
        mTranslation = std::vector<float>(3);
        mRotation    = std::vector<float>(3);
        mScale       = std::vector<float>(3);
      }

      Frame::~Frame() {}

      void Frame::applyToSceneObject(SceneObject* sceneObject) {
        sceneObject->setTranslation(mTranslation);
        sceneObject->setRotation(mRotation);
        sceneObject->setScale(mScale);
      }

      std::vector<float> Frame::getTranslation() {
        return mTranslation;
      }

      std::vector<float> Frame::getRotation() {
        return mRotation;
      }

      std::vector<float> Frame::getScale() {
        return mScale;
      }

      void Frame::setRotation(std::vector<float> rotation) {
        mRotation = rotation;
      }

      void Frame::setScale(std::vector<float> scale) {
        mScale = scale;
      }

      void Frame::setTranslation(std::vector<float> translation) {
        mTranslation = translation;
      }

      void Frame::showStatus() {
        std::cout << "Frame:" << std::endl
                  << "\tTranslation: "  << String::floatVectorToString(mTranslation) << std::endl
                  << "\t   Rotation: "  << String::floatVectorToString(mRotation)    << std::endl
                  << "\t      Scale: "  << String::floatVectorToString(mScale)       << std::endl;
      }


    } // End of Animation
  } // End of Component
} // End of Dream
