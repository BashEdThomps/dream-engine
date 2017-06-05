/*
 * SDLWindowComponent.cpp
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

#include "SDLWindowComponent.h"

namespace DreamSDL
{
    SDLWindowComponent::SDLWindowComponent
    () : IWindowComponent()
    {

    }

    SDLWindowComponent::~SDLWindowComponent
    ()
    {
        if (mWindow != nullptr)
        {
            SDL_DestroyWindow(mWindow);
        }
        SDL_Quit();
    }

    bool
    SDLWindowComponent::init
    ()
    {
        if (!initSDL())
        {
            return false;
        }

        if (!initGL())
        {
            return false;
        }

        return true;
    }

    bool
    SDLWindowComponent::initSDL
    ()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER ) != 0)
        {
          cerr << "SDL_Init Error: " << SDL_GetError() << endl;
          return false;
        }

        mWindow = SDL_CreateWindow(
                      mName.c_str(),
                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      mWidth, mHeight,
                      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
                      );

        if (mWindow == nullptr)
        {
            if (DEBUG)
            {
                cout << "SDLWindowComponent: SDL_CreateWindow Error = " << SDL_GetError() << endl;
            }
            SDL_Quit();
            return false;
        }
        //SDL_SetRelativeMouseMode(SDL_TRUE);
        if (DEBUG)
        {
            cout << "SDLWindowComponent: Initialised SDL" << endl;
        }

        return true;
    }

    bool
    SDLWindowComponent::initGL
    ()
    {
        //Use OpenGL 3.2 core
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        //Create context
        mContext = SDL_GL_CreateContext(mWindow);
        if(mContext == nullptr)
        {
            cerr << "SDLWindowComponent: OpenGL context could not be created! "
                 << "SDL Says: " << SDL_GetError() << endl;
            return false;
        }

        if (DEBUG)
        {
            cout << "SDLWindowComponent: Initialised SDL::OpenGL" << endl;
        }
        return true;
    }

    SDL_Window* SDLWindowComponent::getWindow()
    {
        return mWindow;
    }

    void
    SDLWindowComponent::updateComponent
    (Scene* scene)
    {

        mEvents.clear();
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                setShouldClose(true);
                break;
            }
            /*else if (
                     mGameController &&
                     (
                        event.type == SDL_CONTROLLERAXISMOTION ||
                        event.type == SDL_CONTROLLERBUTTONDOWN ||
                        event.type == SDL_CONTROLLERBUTTONUP
                     )
            ){
                mGameController->updateControllerState(event);
            }*/
            else
            {
                SDL_Event e;
                memcpy(&e,&event,sizeof(SDL_Event));
                mEvents.push_back(e);
            }
            cout << "SDLWindowComponent::update GameController Implementation Disabled!" << endl;
        }
        if (VERBOSE)
        {
            cout << "SDLWindowComponent " << mEvents.size() << " Events" << endl;
        }

    }

    vector<SDL_Event> SDLWindowComponent::getSDL_Events()
    {
        return mEvents;
    }

    void SDLWindowComponent::getCurrentDimensions()
    {
        if (mWindow != nullptr)
        {
            SDL_GetWindowSize(mWindow, &mWidth, &mHeight);
        }
    }

    void SDLWindowComponent::swapBuffers()
    {
        SDL_GL_SwapWindow(mWindow);
    }

    void
    SDLWindowComponent::cleanUp
    (Dream::Scene* scene)
    {

    }

} // End of Dream
