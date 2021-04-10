#include "FontDefinition.h"

#include "Common/Constants.h"

namespace octronic::dream
{
  FontDefinition::FontDefinition
  (ProjectDefinition& pd,
   const json &js)
    : AssetDefinition(pd,js)
  {
  }

  void
  FontDefinition::setSize
  (unsigned int size)
  {
    mJson[Constants::ASSET_ATTR_FONT_SIZE] = size;
  }

  unsigned int
  FontDefinition::getSize
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_FONT_SIZE) == mJson.end())
    {
      return 1;
    }
    return mJson[Constants::ASSET_ATTR_FONT_SIZE];
  }
}

