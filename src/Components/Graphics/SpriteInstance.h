/*
 * SpriteInstance.h
 *
 * Created: 25/11/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
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
#ifndef SPRITEINSTANCE_H
#define SPRITEINSTANCE_H

#include "../../AssetInstance.h"


namespace Dream {
  namespace Components {
    namespace Graphics {

     class SpriteInstance : public AssetInstance
     {
     public:
         SpriteInstance(AssetDefinition*,Transform3D*);
         ~SpriteInstance();
     };

    } // End of Sprite
  } // End of Components
} // End of Dream

#endif // SPRITEINSTANCE_H
