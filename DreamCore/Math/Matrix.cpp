#include "Matrix.h"

#include <glm/gtc/type_ptr.hpp>

namespace octronic::dream
{
	json Matrix::toJson(const mat4& m)
	{
        json j = json::array();
		for (int i=0; i<16; i++)
        {
            j.push_back(glm::value_ptr(m)[i]);
        }
        return j;
	}

	mat4 Matrix::fromJson(const json& js)
	{
        mat4 m(1.f);

        if (!js.is_array()) return m;

		for (int i=0; i<16; i++)
        {
            glm::value_ptr(m)[i] = js[i];
        }
        return m;
	}
}
