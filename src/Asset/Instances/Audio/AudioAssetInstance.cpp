#include "AudioAssetInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Audio     {
	
	AudioAssetInstance::AudioAssetInstance(AssetDefinition* definition) : AssetInstance(definition) {
			setLooping(false);
			setBuffer(0);
			setSource(0);
	}
	
	void AudioAssetInstance::setLooping(bool looping) {
		mLooping = looping;
	}
	
	bool AudioAssetInstance::isLooping() {
		return mLooping;
	}
	
	std::vector<char> AudioAssetInstance::getAudioDataBuffer() {
		return mAudioDataBuffer;
	}
	
	ALuint AudioAssetInstance::getFrequency() {
		return mFrequency;
	}
	
	ALenum AudioAssetInstance::getFormat() {
		return mFormat;
	}
	
	
	void AudioAssetInstance::setBuffer(ALuint buffer) {
		mBuffer = buffer;
	}
	
	ALuint AudioAssetInstance::getBuffer() {
		return mBuffer;
	}
		
	void AudioAssetInstance::setSource(ALuint source) {
		mSource = source;
	}
	
	ALuint AudioAssetInstance::getSource() {
		return mSource;
	}
	
} // End of Audio
} // End of Instances
} // End of Asset
} // End of Dream
