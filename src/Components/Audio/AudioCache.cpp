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

#include "AudioCache.h"

namespace Dream {

    map<string, Mix_Chunk*> AudioCache::ChunkCache = map<string,Mix_Chunk*>();
    map<string, Mix_Music*> AudioCache::MusicCache = map<string,Mix_Music*>();

    Mix_Chunk* AudioCache::getChunk(string path) {
        map<string,Mix_Chunk*>::iterator it;
        for (it=ChunkCache.begin(); it!=ChunkCache.end(); it++) {
            if (it->first.compare(path) == 0) {
              return it->second;
            }
        }
        Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
        ChunkCache.insert(pair<string,Mix_Chunk*>(path,chunk));
        return chunk;
    }

    Mix_Music* AudioCache::getMusic(string path) {
        map<string,Mix_Music*>::iterator it;
        for (it=MusicCache.begin(); it!=MusicCache.end(); it++) {
            if (it->first.compare(path) == 0) {
              return it->second;
            }
        }
        Mix_Music* music = Mix_LoadMUS(path.c_str());
        MusicCache.insert(pair<string,Mix_Music*>(path,music));
        return music;
    }
}
