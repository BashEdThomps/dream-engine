#pragma once

#include <Base/Definition.h>

namespace octronic::dream
{
	class CameraDefinition : public Definition
	{
	public:
		CameraDefinition(const json& js);
		~CameraDefinition();

		void setFieldOfView(float fov);
        float getFieldOfView();

		Transform getFreeTransform();
		void setFreeTransform(const Transform& t);

        UuidType getCameraEntityUuid();
        void setCameraEntityUuid(UuidType u);

        void setMeshCullDistance(float);
        float getMeshCullDistance();

        float getMinDrawDistance();
        void setMinDrawDistance(float);

        float getMaxDrawDistance();
        void setMaxDrawDistance(float);

        void setUseEntity(bool ue);
        bool getUseEntity();

	};
}
