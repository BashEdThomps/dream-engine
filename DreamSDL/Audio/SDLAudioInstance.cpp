#include "SDLAudioInstance.h"
#include "SDLAudioCache.h"

namespace Dream
{

    SDLAudioInstance::SDLAudioInstance(AssetDefinition* definition, Transform3D* transform)
        : IAudioInstance(definition, transform)
    {
        mChunk = nullptr;
        mMusic = nullptr;
        setLooping(false);
        mChannel = -1;
    }

    SDLAudioInstance::~SDLAudioInstance()
    {
        if (DEBUG)
        {
            cout << "AudioInstance: Destroying Object" << endl;
        }
    }

    void SDLAudioInstance::loadExtraAttributes(nlohmann::json jsonData)
    {
        return;
    }

    bool SDLAudioInstance::load(string path)
    {
        mAbsolutePath = path+mDefinition->getAssetPath();
        if (mDefinition->isAudioFormatSoundEffect())
        {
            mChunk = AudioCache::getChunk(mAbsolutePath.c_str());
            if (mChunk == nullptr)
            {
                return false;
            }
            if (DEBUG)
            {
                cout << "AudioInstance: Successfuly loaded Sound Effect" << endl;
            }
        }
        else if (mDefinition->isAudioFormatMusic())
        {
            mMusic = AudioCache::getMusic(mAbsolutePath.c_str());
            if (mMusic == nullptr)
            {
                return false;
            }
            if (DEBUG)
            {
                cout << "AudioInstance: Successfuly loaded Music" << endl;
            }
        }

        mLoaded = (mMusic != nullptr || mChunk != nullptr);
        return mLoaded;
    }

    AudioStatus SDLAudioInstance::getStatus()
    {
        // Music
        if (mMusic != nullptr)
        {
            if (Mix_PlayingMusic())
            {
                return PLAYING;
            }
            else
            {
                return STOPPED;
            }
        }
        // SFX
        else if (mChunk != nullptr)
        {
            if (mChannel == -1)
            {
                return STOPPED;
            }
            else if (Mix_Paused(mChannel))
            {
                return PAUSED;
            }
            else if (Mix_Playing(mChannel))
            {
                return PLAYING;
            }
        }
        return STOPPED;
    }

    bool SDLAudioInstance::play()
    {
        // Music
        if (mMusic != nullptr)
        {
            mChannel = Mix_PlayMusic(mMusic, -1);
            if (mChannel == -1)
            {
                return false;
            }
            return true;
        }
        // SFX
        else if (mChunk != nullptr)
        {
            mChannel =  Mix_PlayChannel(-1, mChunk, 0);
            if (mChannel == -1)
            {
                return false;
            }
            return true;
        }
        return false;
    }

    bool SDLAudioInstance::pause()
    {
        // Music
        if (mMusic != nullptr)
        {
            if(Mix_PausedMusic()==1)
            {
                Mix_ResumeMusic();
                return true;
            }
            else
            {
                Mix_PauseMusic();
                return true;
            }
            // SFX
        }
        else if (mChunk != nullptr && mChannel != -1)
        {
            if (Mix_Paused(mChannel))
            {
                Mix_Resume(mChannel);
                return true;
            }
            else
            {
                Mix_Pause(mChannel);
                return true;
            }
        }
        return false;
    }

    bool SDLAudioInstance::stop()
    {
        // Music
        if (mMusic != nullptr)
        {
            if (Mix_PlayingMusic())
            {
                Mix_HaltMusic();
                return true;
            }
        }
        // SFX
        else if (mChunk != nullptr)
        {
            if (Mix_Playing(mChannel))
            {
                Mix_HaltChannel(mChannel);
                return true;
            }
        }
        return false;
    }


    void SDLAudioInstance::exposeLuaApi(lua_State* state) {
        luabind::module(state) [
                luabind::class_<SDLAudioInstance>("SDLAudioInstance")
                .def("play",&SDLAudioInstance::play)
                .def("pause",&SDLAudioInstance::pause)
                .def("stop",&SDLAudioInstance::stop)
                .def("getStatus",&SDLAudioInstance::getStatus),

                luabind::class_<AudioStatus>("AudioStatus")
                .enum_("AudioStatus") [
                luabind::value("PLAYING", AudioStatus::PLAYING),
                luabind::value("PAUSED",  AudioStatus::PAUSED),
                luabind::value("STOPPED", AudioStatus::STOPPED)
                ]
                ];
    }

} // End of Dream
