#include "Model.h"

#include <DreamCore.h>

namespace octronic::dream::tool
{
	Model::Model(DreamToolContext* ctx)
        : mContext(ctx)
	{
        LOG_TRACE("Model: {}",__FUNCTION__);
	}
}
