/*
 * Transform3D.h
 *
 * Created: 22/11/2016 2016 by Ashley
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
#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H


#define TRANSFORM_X 0
#define TRANSFORM_Y 1
#define TRANSFORM_Z 2

#include <vector>

namespace Dream {
    class Transform3D {

    private:
      std::vector<float> mTranslation;
      std::vector<float> mRotation;
      std::vector<float> mScale;

    public:
      Transform3D();

      std::vector<float> getTranslation();
      void setTranslation(std::vector<float>);
      void setTranslation(float, float, float);

      std::vector<float> getRotation();
      void setRotation(std::vector<float>);
      void setRotation(float, float, float);

      std::vector<float> getScale();
      void setScale(std::vector<float>);
      void setScale(float, float, float);

    };
} // End of Dream

#endif // TRANSFORM3D_H
