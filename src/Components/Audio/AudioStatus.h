#ifndef AUDIOSTATUS_H
#define AUDIOSTATUS_H

namespace Dream {
  namespace Components {
    namespace Audio {

      enum AudioStatus {
        PLAYING,
        PAUSED,
        STOPPED,
        UNKNOWN
      };
    }
  }
}
#endif // AUDIOSTATUS_H
