/*
 * SDLWindowComponent.h
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#ifndef SDLWINDOWCOMPONENT_H
#define SDLWINDOWCOMPONENT_H

#include "../../Components/Window/IWindowComponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


namespace Dream
{
    class SDLWindowComponent : public IWindowComponent
    {
    public:
      SDLWindowComponent();
      ~SDLWindowComponent();
      SDL_Window* getWindow();
      vector<SDL_Event> getSDL_Events();
      void update(Scene*);
      void getCurrentDimensions();
      void swapBuffers();
      bool init();
    private:
      bool initSDL();
      bool initGL();
      bool initTTF();
      SDL_Window *mWindow;
      SDL_GLContext mContext;
      vector<SDL_Event> mEvents;
    }; // End of SDLWindowComponent
} // End of Dream

#endif // SDLWINDOWCOMPONENT_H
