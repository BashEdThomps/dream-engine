#include "AudioInstance.h"
#include "AudioCache.h"

namespace Dream {

    AudioInstance::AudioInstance(AssetDefinition* definition, Transform3D* transform)
        : AssetInstance(definition, transform) {
        mChunk = nullptr;
        mMusic = nullptr;
        setLooping(false);
        setStatus(AudioStatus::STOPPED);
    }

    AudioInstance::~AudioInstance() {
    }

    void AudioInstance::loadExtraAttributes(nlohmann::json jsonData) {
      return;
    }

    bool AudioInstance::load(string path) {
        mAbsolutePath = path+mDefinition->getAssetPath();
        if (mDefinition->isAudioFormatSoundEffect()) {
            mChunk = AudioCache::getChunk(mAbsolutePath.c_str());
            if (mChunk == nullptr) {
                return false;
            }
            if (DEBUG) {
                cout << "AudioInstance: Successfuly loaded Sound Effect" << endl;
            }
        } else if (mDefinition->isAudioFormatMusic()) {
            mMusic = AudioCache::getMusic(mAbsolutePath.c_str());
            if (mMusic == nullptr) {
              return false;
            }
            if (DEBUG) {
              cout << "AudioInstance: Successfuly loaded Music" << endl;
            }
        }

        mLoaded = (mMusic != nullptr || mChunk != nullptr);
        return mLoaded;
    }

    void AudioInstance::setLooping(bool looping) {
        mLooping = looping;
    }

    bool AudioInstance::isLooping() {
        return mLooping;
    }

    void AudioInstance::setStatus(AudioStatus status) {
        mStatus = status;
    }

    AudioStatus AudioInstance::getStatus() {
        return mStatus;
    }

    bool AudioInstance::play() {
        if (mChunk != nullptr) {
            if (Mix_PlayChannel(-1, mChunk, 0) == -1) {
              mStatus = STOPPED;
              return false;
            }
            mStatus = PLAYING;
            return true;
        } else if (mMusic != nullptr) {
            if (Mix_PlayMusic(mMusic, -1) == -1) {
                mStatus = STOPPED;
                return false;
            }
            mStatus = PLAYING;
            return true;
        }
        mStatus = STOPPED;
        return false;
    }

    bool AudioInstance::pause() {
      mStatus = PAUSED;
        return false;
    }

    bool AudioInstance::stop() {
      mStatus = STOPPED;
        return false;
    }

} // End of Dream
