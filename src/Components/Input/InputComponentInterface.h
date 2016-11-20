#ifndef InputInterface_hpp
#define InputInterface_hpp

#include "../ComponentInterface.h"
#include "InputEvent.h"
#include <vector>

namespace Dream {
  namespace Components {
    namespace Input {

      class InputComponentInterface : public Dream::Components::ComponentInterface {
      public:
        static std::vector<InputEvent> sInputEventsVector;
      public:
        InputComponentInterface();
        virtual ~InputComponentInterface() {}
        static void pushEvent(InputEvent);
        static std::vector<InputEvent> getInputEventVector();
        static void clearInputEvents();
      }; // End of InputInterface

    } // End of Input
  } // End of Components
} // End of Dream

#endif /* InputInterface_hpp */
