#pragma once

#include <memory>

using std::reference_wrapper;

namespace octronic::dream::tool
{
	class DreamToolContext;

	class Model
	{
	public:
		Model(DreamToolContext& context);
    DreamToolContext& getContext();
	protected:
		reference_wrapper<DreamToolContext> mContext;
	};
}

