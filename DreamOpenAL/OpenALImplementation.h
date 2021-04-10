#pragma once

#include "ALHeader.h"
#include <DreamCore.h>
#include <deque>

using std::deque;
using glm::vec3;
using octronic::dream::AudioComponent;
using octronic::dream::AudioDefinition;
using octronic::dream::AudioComponent;
using octronic::dream::AudioLoader;
using octronic::dream::AudioStatus;

namespace octronic::dream::open_al
{
    /**
     * @brief AudioRuntime data for an OpenAL based Audio Clip.
     */
    class OpenALImplementation : public AudioRuntimeImplementation
    {
    public:
        OpenALImplementation(AudioRuntime& parent);
        ~OpenALImplementation();

        bool loadFromDefinition() override;
        void setSampleOffset(unsigned int offset) override;
        int getDurationInSamples() override;

        void setSourcePosition(const vec3& pos) override;
        vec3 getSourcePosition() const override;

        void setVolume(float volume) override;
        float getVolume() const override;

        void play() override;
        void pause() override;
        void stop() override;

        AudioStatus getState() override;
        unsigned int getSampleOffset() const override;

        ALsizei getFrequency()const ;
        ALenum  getFormat()const ;

        void setBuffer(ALuint buffer);
        ALuint getBuffer()const ;

        void setSource(ALuint source);
        ALuint getSource() const;

    protected:
        bool loadIntoAL();
        ALuint generateSource();
        ALuint generateBuffer();

    protected:
        ALenum mALFormat;
        ALuint mALSource;
        ALuint mALBuffer;
        ALint mALDurationInSamples;
    };
}
