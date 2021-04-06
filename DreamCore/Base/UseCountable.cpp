#include "UseCountable.h"

namespace octronic::dream
{
  UseCountable::UseCountable()
    : mUseCount(0)
  {
  }

  bool
  UseCountable::hasUsers
  ()
  const
  {
    return mUseCount != 0;
  }

  bool
  UseCountable::hasNoUsers
  ()
  const
  {
    return mUseCount == 0;
  }

  unsigned long
  UseCountable::getUseCount
  ()
  const
  {
    return mUseCount;
  }

  void
  UseCountable::incrementUseCount
  ()
  {
    mUseCount++;
  }

  void
  UseCountable::decrementUseCount
  ()
  {
    mUseCount--;
  }
}
