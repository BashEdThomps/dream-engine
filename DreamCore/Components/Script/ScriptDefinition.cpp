#include "ScriptDefinition.h"

#include "Common/Constants.h"



namespace octronic::dream
{
  ScriptDefinition::ScriptDefinition
  (ProjectDefinition& pd, const json &js)
    : AssetDefinition(pd,js)
  {

  }

  bool
  ScriptDefinition::isFormatLua
  ()
  {
    return getFormat().compare(Constants::ASSET_FORMAT_SCRIPT_LUA) == 0;
  }
}
