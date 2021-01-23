#include "FontRuntime.h"

#include "Common/Logger.h"
#include "FontDefinition.h"
#include "Scene/Entity/EntityRuntime.h"

namespace octronic::dream
{
	FontRuntime::FontRuntime(FontDefinition* fd, ProjectRuntime* er)
        : SharedAssetRuntime(fd, er)
	{
		LOG_TRACE("FontRuntime: {}", __FUNCTION__);
	}

	bool FontRuntime::useDefinition()
	{
		LOG_TRACE("FontRuntime: {}", __FUNCTION__);
        return false;
	}

}
