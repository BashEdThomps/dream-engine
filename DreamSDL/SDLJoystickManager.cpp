/*
 * JoystickManager.cpp
 *
 * Created: 17/12/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
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
#include "SDLJoystickManager.h"

namespace DreamSDL
{

    bool SDLJoystickManager::IsInitialised = false;

    vector<SDL_GameController*> SDLJoystickManager::ControllerHandles = vector<SDL_GameController*>();
    vector<SDL_Joystick*> SDLJoystickManager::JoystickHandles = vector<SDL_Joystick*>();

    void SDLJoystickManager::init()
    {

      if (IsInitialised)
      {
          cerr << "JoystickManager: All ready initialised" << endl;
          return;
      }

      int numJoysticks = SDL_NumJoysticks();

      {
        cout << "JoystickManager: Initialising "<< numJoysticks << " Joysticks" << endl;
      }

      for(int joystickIndex=0; joystickIndex < numJoysticks; joystickIndex++)
      {
        // Push JS Handle
        JoystickHandles.push_back(SDL_JoystickOpen(joystickIndex));
        {
          cout << "JoystickManager: Opened joystick at index " << joystickIndex << endl;
        }
        // Push Controller Handle
        if (!SDL_IsGameController(joystickIndex))
        {
          {
            cout << "JoystickManager: js at index " << joystickIndex
                 << " is not a GameController." << endl;
          }
          continue;
        }
        ControllerHandles.push_back(SDL_GameControllerOpen(joystickIndex));
        {
          cout << "JoystickManager: Opened GameController from js index " << joystickIndex << endl;
        }
      }
      SDL_JoystickEventState(SDL_ENABLE);
    }

    void SDLJoystickManager::destroy()
    {
      {
        cout << "JoystickManager: destroy called" << endl;
      }
      if (IsInitialised)
      {
        // Close GameControllers
        vector<SDL_GameController*>::iterator gcIt;
        for(gcIt=ControllerHandles.begin(); gcIt!=ControllerHandles.end(); gcIt++)
        {
           SDL_GameControllerClose(*gcIt);
        }
        ControllerHandles.clear();
        // Close Joysticks
        vector<SDL_Joystick*>::iterator jsIt;
        for(jsIt=JoystickHandles.begin(); jsIt!=JoystickHandles.end(); jsIt++)
        {
          if (SDL_JoystickGetAttached(*jsIt))
          {
            SDL_JoystickClose(*jsIt);
          }
        }
      }
      else
      {
        cerr << "JoystickManager: Cannot destroy, not initialised." << endl;
      }
      SDL_JoystickEventState(SDL_IGNORE);
    }
} // End of Dream
