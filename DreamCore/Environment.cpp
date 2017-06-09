#include "Environment.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

using namespace std;

namespace Dream
{
    string Environment::getHomeDirectory()
    {
        char *homedir;
        if ((homedir = getenv("HOME")) == nullptr)
        {
            homedir = getpwuid(getuid())->pw_dir;
        }
        return string(homedir);
    }

} // End of Dream
