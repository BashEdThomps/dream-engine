#include "AudioDefinition.h"

#include "Common/Logger.h"
#include "Common/Constants.h"

namespace octronic::dream
{
  AudioDefinition::AudioDefinition
  (ProjectDefinition& pd,
   const json &js)
    : AssetDefinition(pd,js)
  {
    LOG_TRACE("AudioDefinition: Constructing");
  }

  bool
  AudioDefinition::isFormatWav
  ()
  const
  {
    return getFormat() == Constants::ASSET_FORMAT_AUDIO_WAV;
  }

  bool
  AudioDefinition::isFormatOgg
  ()
  const
  {
    return getFormat() == Constants::ASSET_FORMAT_AUDIO_OGG;
  }

  void
  AudioDefinition::setLoop
  (bool loop)
  {
    mJson[Constants::ASSET_ATTR_LOOP] = loop;
  }

  bool
  AudioDefinition::getLoop
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_LOOP) == mJson.end())
    {
      return false;
    }
    return mJson[Constants::ASSET_ATTR_LOOP];
  }
}
