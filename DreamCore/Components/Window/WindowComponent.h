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

      void setWindowSizeChangedFlag(bool f);
      bool getWindowSizeChangedFlag() const;

    protected:
      int mWidth;
      int mHeight;
      string mName;
      bool mShouldClose;
      bool mWindowSizeChangedFlag;
    };
}
