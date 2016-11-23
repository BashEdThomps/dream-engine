#ifndef InputComponent_h
#define InputComponent_h

#include <GLFW/glfw3.h>
#include "../ComponentInterface.h"
#include "InputEvent.h"
#include <vector>

namespace Dream {
  namespace Components {
    namespace Input {

      class InputComponent : public Dream::Components::ComponentInterface {
      private:
        GLFWwindow *mWindow;
        std::vector<InputEvent> mInputEventsVector;
      public:
        InputComponent();
        ~InputComponent();
        void pushEvent(InputEvent);
        std::vector<InputEvent> getInputEventVector();
        void clearInputEvents();
        void setWindow(GLFWwindow*);
        bool init();
        void update(Dream::Scene*);
        void onKeyboardEvent(GLFWwindow*, int, int, int, int);
        void onMouseButtonEvent(GLFWwindow*, int, int, int);
        void onMouseScrollEvent(GLFWwindow*, double, double);
        void onMousePositionEvent(GLFWwindow*, double, double);
      }; // End of InputComponent

    } // End of Input
  } // End of Components
} // End of Dream

#endif /* InputInterface_hpp */
