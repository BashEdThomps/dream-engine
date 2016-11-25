#include "AudioInstance.h"

namespace Dream {
    namespace Components {
        namespace Audio {

            AudioInstance::AudioInstance(
                    AssetDefinition* definition, Transform3D* transform
                    ) : AssetInstance(definition, transform) {
                setLooping(false);
                setStatus(AudioStatus::STOPPED);
            }

            AudioInstance::~AudioInstance() {

            }

            void AudioInstance::loadExtraAttributes(nlohmann::json json) {
                return;
            }

            bool AudioInstance::load(std::string path) {
                return false;
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

        } // End of Audio
    } // End of Asset
} // End of Dream
