#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include "../../SceneObject.h"

namespace Dream {
  namespace Component {
    namespace Animation {
      class Frame {
      private:
        std::vector<float> mTranslation;
        std::vector<float> mRotation;
        std::vector<float> mScale;
      public:
        Frame();
        ~Frame();
        void applyToSceneObject(SceneObject*);
        std::vector<float> getTranslation();
        void               setTranslation(std::vector<float>);
        std::vector<float> getRotation();
        void               setRotation(std::vector<float>);
        std::vector<float> getScale();
        void               setScale(std::vector<float>);
        void showStatus();
      }; // End Frame
    } // End of Animation
  } // End of Components
} // End of Dream

#endif // FRAME_H
