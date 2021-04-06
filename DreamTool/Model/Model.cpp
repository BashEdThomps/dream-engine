#include "Model.h"

#include <DreamCore.h>

namespace octronic::dream::tool
{
  Model::Model(DreamToolContext& ctx)
    : mContext(ctx)
  {
  }

  DreamToolContext&
  Model::getContext()
  {
    return mContext.get();
  }
}
