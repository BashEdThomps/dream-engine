/*
* AudioComponent
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SDLAudioComponent.h"

#include <iostream>

namespace DreamSDL
{
    SDLAudioComponent::SDLAudioComponent
    ()
        : Dream::IAudioComponent()
    {
        // Pass
    }

    SDLAudioComponent::~SDLAudioComponent
    ()
    {
        if (DEBUG)
        {
            cout << "AudioComponent: Destroying Object" << endl;
        }
        Mix_CloseAudio();
    }

    bool
    SDLAudioComponent::init
    ()
    {
        if (DEBUG)
        {
            cout << "AudioComponent: Initialising...";
        }

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        {
            if (DEBUG)
            {
                cout << endl << "AudioComponent: Error SDL_OpenAudio Failed" << endl;
            }
            return false;
        }
        if (DEBUG)
        {
            cout << "Done!" << endl;
        }
        return true;
    }

    void
    SDLAudioComponent::updateComponent
    (Scene*)
    {

    }

    Dream::IAudioInstance*
    SDLAudioComponent::newAudioInstance
    (Dream::AssetDefinition* definition, Dream::Transform3D* transform)
    {
        return new SDLAudioInstance(definition,transform);
    }
} // End of Dream
