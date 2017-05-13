/*
 * AudioCache
 *
 * Created: 15/12/2016 2016 by Ashley
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

#ifndef AUDIOCACHE_H
#define AUDIOCACHE_H

#include <iostream>
#include <map>
#include <SDL2/SDL_mixer.h>

using namespace std;

namespace DreamSDL
{
    class AudioCache
    {
    public:
      static AudioCache* instance;
      static map<string,Mix_Chunk*> ChunkCache;
      static map<string,Mix_Music*> MusicCache;
      static Mix_Music* getMusic(string);
      static Mix_Chunk* getChunk(string);
    }; // End of AudioCache
} // End of Dream

#endif // AUDIOCACHE_H
