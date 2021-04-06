/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OpenALImplementation.h"

namespace octronic::dream::open_al
{
  OpenALImplementation::OpenALImplementation
  (AudioRuntime& parent)
    : AudioRuntimeImplementation(parent),
      mALFormat(0),
      mALSource(0),
      mALBuffer(0),
      mALDurationInSamples(0)
  {
    LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
  }

  OpenALImplementation::~OpenALImplementation
  ()
  {
    LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
    if (mALBuffer!=0) alDeleteBuffers(1,&mALBuffer);
    if (mALSource!=0) alDeleteSources(1,&mALSource);
  }

  bool
  OpenALImplementation::loadFromDefinition
  ()
  {
    auto& parent = mParent.get();
    auto& pr = parent.getProjectRuntime();
    auto& ad = static_cast<AudioDefinition&>(parent.getDefinition());
    auto loader = parent.getAudioLoader();

    LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
    if (!loader->loadIntoBuffer(pr, ad)) return false;
    if (!loadIntoAL()) return false;
    return true;
  }

  ALenum
  OpenALImplementation::getFormat
  ()
  const
  {
    return mALFormat;
  }

  void
  OpenALImplementation::setBuffer
  (ALuint buffer)

  {
    mALBuffer = buffer;
  }

  ALuint
  OpenALImplementation::getBuffer
  ()
  const
  {
    return mALBuffer;
  }

  void
  OpenALImplementation::setSource
  (ALuint source)
  {
    mALSource = source;
  }

  ALuint
  OpenALImplementation::getSource
  ()
  const
  {
    return mALSource;
  }

  void
  OpenALImplementation::play
  ()
  {
    LOG_DEBUG(  "OpenALImplementation: Playing source {}" , mALSource);
    alSourcePlay(mALSource);
  }

  void
  OpenALImplementation::stop
  ()
  {
    LOG_DEBUG(  "OpenALImplementation: Stopping source {}" , mALSource);
    alSourceStop(mALSource);
  }

  void
  OpenALImplementation::pause
  ()
  {
    LOG_DEBUG(  "OpenALImplementation: Pausing source {}" , mALSource);
    alSourcePause(mALSource);
  }

  void
  OpenALImplementation::setSourcePosition
  (const vec3& pos)
  {
    LOG_DEBUG(  "OpenALImplementation: SetSource Position {} {},{},{}" ,
                mALSource, pos.x,pos.y,pos.z);
    alSource3f(mALSource, AL_POSITION, pos.x, pos.y, pos.z);
  }

  vec3
  OpenALImplementation::getSourcePosition
  ()
  const
  {
    LOG_DEBUG("OpenALImplementation: getSourcePosition");
    vec3 pos;

    alGetSource3f(mALSource, AL_POSITION,
                  &glm::value_ptr(pos)[0],
        &glm::value_ptr(pos)[1],
        &glm::value_ptr(pos)[2]);
    return pos;
  }

  void
  OpenALImplementation::setVolume
  (float volume)
  {
    LOG_DEBUG(  "OpenALImplementation: SetVolume src:{} vol:{},{},{}" , mALSource, volume);
    alSourcef(mALSource, AL_GAIN,volume);
  }

  float
  OpenALImplementation::getVolume
  ()
  const
  {
    float vol;
    alGetSourcef(mALSource, AL_GAIN, &vol);
    return vol;
  }

  AudioStatus
  OpenALImplementation::getState
  ()
  {
    LOG_TRACE("OpenALImplementation: {}", __FUNCTION__);
    int state = 0;
    alGetSourcei(mALSource, AL_SOURCE_STATE, &state);
    switch (state)
    {
      case AL_STOPPED:
        return AUDIO_STATUS_STOPPED;
      case AL_PLAYING:
        return AUDIO_STATUS_PLAYING;
      case AL_PAUSED:
        return AUDIO_STATUS_PAUSED;
      default:
        LOG_ERROR("OpenALImplementation: Unknown Audio State");
        return AUDIO_STATUS_UNKNOWN;
    }
  }

  int
  OpenALImplementation::getDurationInSamples
  ()
  {
    LOG_TRACE("OpenALImplementation: {}", __FUNCTION__);
    if (mALDurationInSamples == 0 && mALBuffer != 0)
    {
      ALint sizeInBytes;
      ALint channels;
      ALint bits;

      alGetBufferi(mALBuffer, AL_SIZE, &sizeInBytes);
      alGetBufferi(mALBuffer, AL_CHANNELS, &channels);
      alGetBufferi(mALBuffer, AL_BITS, &bits);

      mALDurationInSamples = sizeInBytes * 8 / (channels * bits);
    }
    return mALDurationInSamples;
  }

  unsigned int
  OpenALImplementation::getSampleOffset
  ()
  const
  {
    LOG_TRACE("OpenALImplementation: {}", __FUNCTION__);
    ALint sampleOffset;
    alGetSourcei(mALSource, AL_SAMPLE_OFFSET, &sampleOffset);
    return sampleOffset;
  }

  void
  OpenALImplementation::setSampleOffset
  (unsigned int offset)
  {
    LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
    alSourcei(mALSource,AL_SAMPLE_OFFSET, (ALint)offset);
  }

  ALuint
  OpenALImplementation::generateSource
  ()
  {
    LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
    alGetError();
    alGenSources(1, &mALSource);
    ALenum error = alGetError();
    if ( error != AL_NO_ERROR )
    {
      mALSource = static_cast<ALuint>(-1);
    }
    return mALSource;
  }

  ALuint
  OpenALImplementation::generateBuffer
  ()
  {
    LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
    alGetError();
    alGenBuffers(1, &mALBuffer);
    ALenum error = alGetError();
    if ( error != AL_NO_ERROR )
    {
      mALBuffer = static_cast<ALuint>(-1);
    }
    return mALBuffer;
  }

  bool
  OpenALImplementation::loadIntoAL
  ()
  {
    auto& parent = mParent.get();
    auto loader = parent.getAudioLoader();

    LOG_DEBUG("OpenALImplementation: Loading Into AL {}",parent.getNameAndUuidString());

    if (mALSource == 0 && mALBuffer == 0)
    {
      generateBuffer();
      generateSource();

      if (loader->getAudioBuffer().empty())
      {
        LOG_ERROR("OpenALImplementation: Unable to load audio data: Empty Buffer");
        parent.setLoadError(true);
        return false;
      }

      switch(loader->getChannels())
      {
        case 1:
          mALFormat = AL_FORMAT_MONO16;
          break;
        case 2:
        default:
          mALFormat = AL_FORMAT_STEREO16;
          break;
      }

      alBufferData(mALBuffer, mALFormat, &loader->getAudioBuffer()[0],
          static_cast<ALsizei>(loader->getAudioBuffer().size()),
          loader->getSampleRate());

      alSourcei(mALSource, AL_BUFFER, static_cast<ALint>(mALBuffer));
      alSourcei(mALSource, AL_LOOPING, parent.getLooping() ? 1 : 0 );
    }
    else
    {
      LOG_ERROR("OpenALImplementation: Unable to load audio, source or buffer is empty");
      parent.setLoadError(true);
      return false;
    }

    LOG_DEBUG("OpenALImplementation: Pushed audio asset to play queue");
    parent.setLoaded(true);
    return true;
  }
}
