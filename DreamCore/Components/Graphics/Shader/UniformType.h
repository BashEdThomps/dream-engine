#pragma once

#include <string>

using std::string;

namespace octronic::dream
{
	enum UniformType
	{
		// Int
		UNIFORM_TYPE_INT1,
		UNIFORM_TYPE_INT2,
		UNIFORM_TYPE_INT3,
		UNIFORM_TYPE_INT4,
		// uInt
		UNIFORM_TYPE_UINT1,
		UNIFORM_TYPE_UINT2,
		UNIFORM_TYPE_UINT3,
		UNIFORM_TYPE_UINT4,
		// Float
		UNIFORM_TYPE_FLOAT1,
		UNIFORM_TYPE_FLOAT2,
		UNIFORM_TYPE_FLOAT3,
		UNIFORM_TYPE_FLOAT4,
		// Matrix
		UNIFORM_TYPE_MATRIX2,
		UNIFORM_TYPE_MATRIX3,
		UNIFORM_TYPE_MATRIX4,
		UNIFORM_TYPE_MATRIX2X3,
		UNIFORM_TYPE_MATRIX3X2,
		UNIFORM_TYPE_MATRIX2X4,
		UNIFORM_TYPE_MATRIX4X2,
		UNIFORM_TYPE_MATRIX3X4,
		UNIFORM_TYPE_MATRIX4X3
	};

	static string uniformTypeToString(UniformType t)
	{
		switch(t)
		{
				// Int
			case UNIFORM_TYPE_INT1: return "int1";
			case UNIFORM_TYPE_INT2: return "int2";
			case UNIFORM_TYPE_INT3: return "int3";
			case UNIFORM_TYPE_INT4: return "int4";
				// uInt
			case UNIFORM_TYPE_UINT1: return "uint1";
			case UNIFORM_TYPE_UINT2: return "uint2";
			case UNIFORM_TYPE_UINT3: return "uint3";
			case UNIFORM_TYPE_UINT4: return "uint4";
				// Float
			case UNIFORM_TYPE_FLOAT1: return "float1";
			case UNIFORM_TYPE_FLOAT2: return "float2";
			case UNIFORM_TYPE_FLOAT3: return "float3";
			case UNIFORM_TYPE_FLOAT4: return "float4";
				// Matrix
			case UNIFORM_TYPE_MATRIX2: return "matrix2";
			case UNIFORM_TYPE_MATRIX3: return "matrix3";
			case UNIFORM_TYPE_MATRIX4: return "matrix4";
			case UNIFORM_TYPE_MATRIX2X3: return "matrix2x3";
			case UNIFORM_TYPE_MATRIX3X2: return "matrix3x2";
			case UNIFORM_TYPE_MATRIX2X4: return "matrix2x4";
			case UNIFORM_TYPE_MATRIX4X2: return "matrix4x2";
			case UNIFORM_TYPE_MATRIX3X4: return "matrix3x4";
			case UNIFORM_TYPE_MATRIX4X3: return "matrix4x3";

		}
	}
}
