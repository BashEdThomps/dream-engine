/*
 * SpriteInstance
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include <glm/glm.hpp>

#include "SpriteInstance.h"
#include "../Model/TextureCache.h"
#include "../../../Project/AssetDefinition.h"

namespace Dream
{

  SpriteInstance::SpriteInstance
  (AssetDefinition* definition, Transform3D* transform)
      : IAssetInstance(definition,transform)
  {
    loadExtraAttributes(mDefinitionHandle->getJson());
  }

  SpriteInstance::~SpriteInstance
  ()
  {
        if (Constants::DEBUG)
        {
            cout << "SpriteInstance: Destroying Object" << endl;
        }
      return;
  }

  bool
  SpriteInstance::load
  (string projectPath)
  {
    string path = projectPath + mDefinitionHandle->getAssetPath();
    string directory = path.substr(0, path.find_last_of('/'));
    if (Constants::DEBUG)
    {
        cout << "SpriteInstance: Loading sprite from " << path << endl;
    }

    Texture tex = TextureCache::loadTextureFromFile("sprite",directory.c_str(),"sprite");
    mTexture = tex.id;
    mWidth = tex.width;
    mHeight = tex.height;
    mLoaded = (mTexture > 0);
    return mLoaded;
  }

  void
  SpriteInstance::loadExtraAttributes
  (nlohmann::json jsonData)
  {

  }

  GLuint
  SpriteInstance::getTexture
  ()
  {
      return mTexture;
  }

  int
  SpriteInstance::getWidth
  ()
  {
    return mWidth;
  }

  int
  SpriteInstance::getHeight
  ()
  {
    return mHeight;
  }
} // End Dream
