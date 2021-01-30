#pragma once

#include <cstdint>
#include <cstddef>

namespace octronic::dream
{
	class AudioDefinition;
	class ProjectRuntime;
	class AudioLoader
	{
	public:
		AudioLoader();
		virtual ~AudioLoader();
		virtual bool loadIntoBuffer(AudioDefinition* aDef, ProjectRuntime* pDef) = 0;
		uint8_t* getAudioBuffer() const;
		size_t getAudioBufferSize() const;
		uint8_t getChannels() const;
		long getSampleRate() const;
	protected:
		bool checkLoaded() const;

	protected:
		uint8_t* mAudioBuffer;
		size_t   mAudioBufferSize;
		uint8_t  mChannels;
		long     mSampleRate;
	};

}
