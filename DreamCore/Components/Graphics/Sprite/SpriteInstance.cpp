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
#include "../Model/MaterialCache.h"
#include "SpriteDefinition.h"

namespace Dream
{

  SpriteInstance::SpriteInstance
  (
    weak_ptr<MaterialCache> cache,
    shared_ptr<SpriteDefinition> definition,
    shared_ptr<SceneObjectRuntime> transform
    )
      : IAssetInstance(definition,transform),

        mCache(cache)
  {
    setLogClassName("SpriteInstance");
    loadExtraAttributes(mDefinition->getJson());
  }

  SpriteInstance::~SpriteInstance
  ()
  {
      auto log = getLog();
            log->info( "SpriteInstance: Destroying Object" );
      return;
  }

  bool
  SpriteInstance::load
  (string projectPath)
  {
    auto log = getLog();
    string path = projectPath + mDefinition->getAssetPath();
    string directory = path.substr(0, path.find_last_of('/'));
    log->info( "SpriteInstance: Loading sprite from {}" , path );

    auto cache = mCache.lock();
    if (cache != nullptr)
    {
        shared_ptr<Texture> tex = cache->loadTextureFromFile("sprite",directory.c_str(),"sprite");
        mTexture = tex->id;
        mWidth = tex->width;
        mHeight = tex->height;
        mLoaded = (mTexture > 0);
        return mLoaded;
    }
    return false;
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
