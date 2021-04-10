#pragma once

#include <iostream>


typedef uint32_t UuidType;

namespace octronic::dream
{
    class Uuid
    {
    public:
    	const static UuidType INVALID;
        static UuidType RandomUuid();
    };
}

