#pragma once

#include "AudioStatus.h"
#include <glm/vec3.hpp>
#include <memory>

using glm::vec3;
using std::shared_ptr;
using std::reference_wrapper;

namespace octronic::dream
{
	class AudioLoader;
	class ProjectRuntime;
	class AudioDefinition;
	class AudioRuntime;

	class AudioRuntimeImplementation
	{
	public:
		AudioRuntimeImplementation(AudioRuntime& parent);
		virtual ~AudioRuntimeImplementation();

		virtual void play() = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;

		virtual vec3 getSourcePosition() const  = 0;
		virtual void setSourcePosition(const vec3& pos) = 0;

		virtual float getVolume() const = 0;
		virtual void  setVolume(float volume) = 0;

		virtual AudioStatus getState() = 0;

		virtual unsigned int getSampleOffset() const = 0;
		virtual void         setSampleOffset(unsigned int offset) = 0;

		virtual int getDurationInSamples() = 0;

		virtual bool loadFromDefinition() = 0;

	protected:
		reference_wrapper<AudioRuntime> mParent;
	};
}
