#pragma once

#include "AudioStatus.h"
#include <glm/vec3.hpp>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using glm::vec3;

namespace octronic::dream
{
	class AudioLoader;
	class ProjectRuntime;
	class AudioDefinition;
	class AudioRuntime;

	class AudioRuntimeImplementation
	{
	public:
		AudioRuntimeImplementation(const shared_ptr<AudioLoader>& loader)
			: mLoader(loader)
		{}

		~AudioRuntimeImplementation()
		{}

		virtual void play() = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;
		virtual void setSourcePosision(const vec3& pos) = 0;
		virtual void setVolume(float volume) = 0;
		virtual AudioStatus getState() = 0;
		virtual unsigned int getSampleOffset() const = 0;
		virtual void setSampleOffset(unsigned int offset) = 0;
		virtual int getDurationInSamples() = 0;
		virtual bool loadFromDefinition(const weak_ptr<ProjectRuntime>& pr,
                                    const weak_ptr<AudioDefinition>& ad) = 0;
		void setParent(const weak_ptr<AudioRuntime>& p) {mParent = p;};
	protected:
        /**
         * @brief mLoader - Audio Format data loader. We adopt ownership.
         */
		shared_ptr<AudioLoader> mLoader;
		weak_ptr<AudioRuntime> mParent;
	};

}
