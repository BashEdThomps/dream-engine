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

using Dream::Constants;
using Dream::SceneRuntime;
using Dream::SceneState;

namespace DreamSDL
{
    SDLWindowComponent::SDLWindowComponent
    () : IWindowComponent()
    {
        auto log = spdlog::get("SDLWindowComponent");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("SDLWindowComponent");
        }
        log->info("Constructing" );
        mName = "Dream";
    }

    SDLWindowComponent::~SDLWindowComponent
    ()
    {
        auto log = spdlog::get("SDLWindowComponent");
        {
            log->info("Destructing" );
        }
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
        auto log = spdlog::get("SDLWindowComponent");
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0)
        {
            log->error("SDL_Init Error: {}" ,SDL_GetError() );
            return false;
        }

        mWindow = SDL_CreateWindow(
                    mName.c_str(),
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    mWidth, mHeight,
                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                    );

        if (mWindow == nullptr)
        {
            log->info("SDL_CreateWindow Error = {}" ,SDL_GetError() );
            SDL_Quit();
            return false;
        }
        log->info("Initialised SDL" );

        return true;
    }

    bool
    SDLWindowComponent::initGL
    ()
    {
        auto log = spdlog::get("SDLWindowComponent");
        //Use OpenGL 3.2 core
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        //Create context
        mContext = SDL_GL_CreateContext(mWindow);
        if(mContext == nullptr)
        {
            log->error("OpenGL context could not be created! {}", SDL_GetError() );
            return false;
        }
        log->info("Initialised SDL::OpenGL");
        return true;
    }

    SDL_Window* SDLWindowComponent::getWindow()
    {
        return mWindow;
    }

    void
    SDLWindowComponent::updateComponent
    ()
    {
        auto log = spdlog::get("SDLWindowComponent");

        mEvents.clear();
        SDL_Event event;
        auto asr = mActiveSceneRuntime.lock();

        if (asr == nullptr)
        {
            log->error("No active scene runtime");
            return;
        }

        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                log->info("SDL_QUIT Event");
                asr->setState(Dream::SCENE_STATE_STOPPED);
                break;
            }
            else if (event.type == SDL_WINDOWEVENT)
            {
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        getCurrentDimensions();
                        break;
                }
            }
            else
            {
                SDL_Event e;
                memcpy(&e,&event,sizeof(SDL_Event));
                mEvents.push_back(e);
            }
        }
        log->info("{} events", mEvents.size());

    }

    vector<SDL_Event> SDLWindowComponent::getSDL_Events()
    {
        return mEvents;
    }

    void SDLWindowComponent::getCurrentDimensions()
    {
        auto log = spdlog::get("SDLWindowComponent");

        if (mWindow != nullptr)
        {
            SDL_GetWindowSize(mWindow, &mWidth, &mHeight);
        }
        log->info("SDLWindowComponent: Window size changed to {},{}", mWidth , mHeight );
        mSizeHasChanged = true;
    }

    void SDLWindowComponent::swapBuffers()
    {
        SDL_GL_SwapWindow(mWindow);
    }

} // End of Dream
