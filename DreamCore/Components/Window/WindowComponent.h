/*
 * IWindowComponent.h
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
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

#pragma once

#include "Components/Component.h"
#include "Common/GLHeader.h"

#include <string>

using std::string;

namespace octronic::dream
{

    class WindowComponent : public Component
    {
    public:
      WindowComponent();
      virtual ~WindowComponent();

      bool init() = 0;
      virtual void getCurrentDimensions() = 0;
      virtual void swapBuffers() = 0;
      virtual void updateWindow() = 0;
      virtual void bindFrameBuffer() = 0;
      virtual GLuint getFrameBuffer() const = 0;
      virtual GLuint getDepthBuffer() const = 0;

      void setWidth(int);
      void setHeight(int);
      int getWidth() const;
      int getHeight() const;

      void setName(string);
      string getName() const;

      void close();
      bool shouldClose() const;
      void setShouldClose(bool);
      double getMouseX() const;
      double getMouseY() const;

      void setWindowSizeChangedFlag(bool f);
      bool getWindowSizeChangedFlag() const;

    protected:
      int mWidth;
      int mHeight;
      string mName;
      bool mShouldClose;
      double mMouseX;
      double mMouseY;
      bool mWindowSizeChangedFlag;
    };
}
