#include "InputHandler.h"
namespace Dream
{

    InputHandler::InputHandler
    (SceneRuntime* sceneRuntime)
        : DreamObject ("InputHandler"), mSceneRuntime(sceneRuntime) {}

    InputHandler::~InputHandler() {}
}
