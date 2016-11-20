#ifndef GLFWInput_hpp
#define GLFWInput_hpp

#include <vector>
#include <GLFW/glfw3.h>

#include "InputComponentInterface.h"
#include "InputEvent.h"

namespace Dream {
  namespace Components {
    namespace Input {

      class GLFWInput : public InputComponentInterface {
      private:
        GLFWwindow *mWindow;
      public:
        GLFWInput();
        ~GLFWInput();
        void setWindow(GLFWwindow*);
        bool init();
        void update(Dream::Scene*);
      };

    } // End of Input
  } // End of Components
} // End of Dream

#endif /* GLFWInput_hpp */
