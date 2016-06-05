#include "AudioAssetInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Audio     {
	
	AudioAssetInstance::AudioAssetInstance(AssetDefinition* definition)
		: AssetInstance(definition) {
			setLooping(false);
			setStatus(STOPPED);
	}
	
	void AudioAssetInstance::setStatus(AudioAssetStatus status) {
		mStatus = status;
	}

	AudioAssetStatus AudioAssetInstance::getStatus() {
		return mStatus;
	}
	
	void AudioAssetInstance::setLooping(bool looping) {
		mLooping = looping;
	}
	
	bool AudioAssetInstance::isLooping() {
		return mLooping;
	}
	
} // End of Audio
} // End of Instances
} // End of Asset
} // End of Dream
