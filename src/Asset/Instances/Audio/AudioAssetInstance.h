#ifndef AudioAssetInstance_h
#define AudioAssetInstance_h

#include "../../AssetInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Audio     {
	
	enum AudioAssetStatus {
		PLAYING,
		PAUSED,
		STOPPED,
	};
	
	class AudioAssetInstance : public AssetInstance {
	private:
		AudioAssetStatus mStatus;
		bool             mLooping;
	public:
		AudioAssetInstance(AssetDefinition*);
		
		void             setStatus(AudioAssetStatus);
		AudioAssetStatus getStatus();
		
		void setLooping(bool);
		bool isLooping();
	};
	
} // End of Audio
} // End of Instances
} // End of Asset
} // End of Dream

#endif /* AudioAssetInstance_h */
