#include "Uuid.h"
#include <cstdio>
#include <cstdlib>
#include <chrono>

using std::srand;
using std::chrono::high_resolution_clock;

namespace octronic::dream
{
  const UuidType Uuid::INVALID = 0;

  UuidType Uuid::RandomUuid()
  {
    srand(static_cast<unsigned int>(high_resolution_clock::now().time_since_epoch().count()));
    UuidType retval = INVALID;
    while (retval == INVALID)
    {
      retval = static_cast<UuidType>(arc4random());
    }
    return retval;
  }
}
