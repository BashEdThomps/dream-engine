#include "CameraDefinition.h"

namespace octronic::dream
{
	CameraDefinition::CameraDefinition(const json& js)
        : Definition("Camera", js)
	{
		if (mJson[Constants::UUID].is_null())
        {
            setUuid(Uuid::generateUuid());
        }
	}

    CameraDefinition::~CameraDefinition()
    {

    }

    float
    CameraDefinition::getFieldOfView
    ()
    {
        if (mJson.find(Constants::CAMERA_FOV) == mJson.end())
        {
            mJson[Constants::CAMERA_FOV] = 90.f;
        }
        return mJson[Constants::CAMERA_FOV];
    }

    void
    CameraDefinition::setFieldOfView
    (float v)
    {
        mJson[Constants::CAMERA_FOV] = v;
    }


    UuidType
    CameraDefinition::getCameraEntityUuid
    ()
    {
        if (mJson.find(Constants::CAMERA_ENTITY_UUID) == mJson.end())
        {
			mJson[Constants::CAMERA_ENTITY_UUID] = Uuid::INVALID;
        }
        return mJson[Constants::CAMERA_ENTITY_UUID];
    }

    void
    CameraDefinition::setCameraEntityUuid
    (UuidType u)
    {
        mJson[Constants::CAMERA_ENTITY_UUID] = u;
    }

    void
    CameraDefinition::setFreeTransform
    (const Transform& t)
    {
        mJson[Constants::CAMERA_FREE_TRANSFORM] = t.toJson();
    }

    Transform
    CameraDefinition::getFreeTransform
    ()
    {
        if (mJson.find(Constants::CAMERA_FREE_TRANSFORM) == mJson.end())
        {
            mJson[Constants::CAMERA_FREE_TRANSFORM] = Transform().toJson();
        }
        return Transform(mJson[Constants::CAMERA_FREE_TRANSFORM]);
    }

    void
    CameraDefinition::setMeshCullDistance
    (float mcd)
    {
        mJson[Constants::CAMERA_MESH_CULL_DISTANCE] = mcd;
    }

    float
    CameraDefinition::getMeshCullDistance
    ()
    {
        if (mJson.find(Constants::CAMERA_MESH_CULL_DISTANCE) == mJson.end())
        {
            mJson[Constants::CAMERA_MESH_CULL_DISTANCE] = 100.f;
        }
        return mJson[Constants::CAMERA_MESH_CULL_DISTANCE];
    }

    void
    CameraDefinition::setMinDrawDistance
    (float f)
    {
        mJson[Constants::CAMERA_MIN_DRAW_DISTANCE] = f;
    }

    float
    CameraDefinition::getMinDrawDistance
    ()
    {
       if (mJson.find(Constants::CAMERA_MIN_DRAW_DISTANCE) == mJson.end())
       {
            mJson[Constants::CAMERA_MIN_DRAW_DISTANCE] = .1f;
       }
       return mJson[Constants::CAMERA_MIN_DRAW_DISTANCE];
    }

    float
    CameraDefinition::getMaxDrawDistance
    ()
    {
        if (mJson.find(Constants::CAMERA_MAX_DRAW_DISTANCE) == mJson.end())
        {
            mJson[Constants::CAMERA_MAX_DRAW_DISTANCE] = 100.f;
        }
        return mJson[Constants::CAMERA_MAX_DRAW_DISTANCE];
    }

    void
    CameraDefinition::setMaxDrawDistance
    (float mdd)
    {
        mJson[Constants::CAMERA_MAX_DRAW_DISTANCE] = mdd;
    }

    void
    CameraDefinition::setUseEntity
    (bool ue)
    {
        mJson[Constants::CAMERA_USE_ENTITY] = ue;
    }

    bool
    CameraDefinition::getUseEntity
    ()
    {
        if (mJson.find(Constants::CAMERA_USE_ENTITY) == mJson.end())
        {
           mJson[Constants::CAMERA_USE_ENTITY] = false;
        }
        return mJson[Constants::CAMERA_USE_ENTITY];
    }
}
