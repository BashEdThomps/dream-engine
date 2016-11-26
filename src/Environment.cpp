#include "Environment.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

namespace Dream {

  using namespace std;
  string Environment::getHomeDirectory() {
    char *homedir;

    if ((homedir = getenv("HOME")) == NULL) {
      homedir = getpwuid(getuid())->pw_dir;
    }

    return string(homedir);
  }

} // End of Dream
