#pragma once

namespace octronic::dream::tool
{
	class DreamToolContext;

	class Model
	{
	public:
		Model(DreamToolContext* context);
	protected:
		DreamToolContext* mContext;
	};
}

