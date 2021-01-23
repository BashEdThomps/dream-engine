#pragma once

#include "Components/SharedAssetRuntime.h"

namespace octronic::dream
{
	class FontDefinition;
	class FontRuntime : public SharedAssetRuntime
	{
	public:
		FontRuntime(FontDefinition* def, ProjectRuntime* e );
		bool useDefinition() override;
	};

}
