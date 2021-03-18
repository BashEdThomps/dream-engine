#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

using std::weak_ptr;
using std::vector;

namespace octronic::dream
{
	class AudioDefinition;
	class ProjectRuntime;
	class AudioLoader
	{
	public:
		AudioLoader();
		virtual ~AudioLoader();
		virtual bool loadIntoBuffer(const weak_ptr<AudioDefinition>& aDef,
                                    const weak_ptr<ProjectRuntime>& pDef) = 0;

		vector<uint8_t> getAudioBuffer() const;
		uint8_t getChannels() const;
		long getSampleRate() const;
	protected:
		bool checkLoaded() const;

	protected:
		vector<uint8_t> mAudioBuffer;
		uint8_t  mChannels;
		long     mSampleRate;
	};

}
