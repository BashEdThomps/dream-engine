#ifndef VulkanVideo_h
#define VulkanVideo_h

#include "VideoComponentInterface.h"

namespace Dream {
  namespace Components {
    namespace Video {
      class VulkanVideo : public VideoComponentInterface {
      public:
        VulkanVideo();
        ~VulkanVideo();
        bool init();
      };
    }
  }
}
#endif /* VulkanVideo_h */
