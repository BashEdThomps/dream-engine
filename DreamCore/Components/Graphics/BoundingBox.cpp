#include "BoundingBox.h"

#include <limits>
#include <assimp/types.h>
#include <assimp/mesh.h>

using std::numeric_limits;

namespace Dream
{
    BoundingBox::BoundingBox(vec3 min, vec3 max)
        : DreamObject("BoundingBox"),
          maxDimension(max.x*2),
          minimum(min),
          maximum(max)
    {
        init();
    }

    BoundingBox::~BoundingBox
    ()
    {

    }

    vec3
    BoundingBox::getCenter
    ()
    {
       return vec3((minimum.x+maximum.x)/2, (minimum.y+maximum.y)/2, (minimum.z+maximum.z)/2);
    }

    void
    BoundingBox::init
    ()
    {
        maximum = vec3(numeric_limits<float>::min());
        minimum = vec3(numeric_limits<float>::max());
    }

    void
    BoundingBox::updateFromMesh
    (aiMesh* mesh)
    {
        auto log = getLog();
        log->debug( "Updating bounding box");

        for (unsigned int i=0; i < mesh->mNumVertices; i++)
        {
            aiVector3D vertex = mesh->mVertices[i];

            // Maximum
            if (maximum.x < vertex.x)
            {
                maximum.x = vertex.x;
            }

            if (maximum.y < vertex.y)
            {
                maximum.y = vertex.y;
            }

            if (maximum.z < vertex.z)
            {
                maximum.z = vertex.z;
            }

            // Maximum
            if (minimum.x > vertex.x)
            {
                minimum.x = vertex.x;
            }

            if (minimum.y > vertex.y)
            {
                minimum.y = vertex.y;
            }

            if (minimum.z > vertex.z)
            {
                minimum.z = vertex.z;
            }
        }

        float maxBound;
        maxBound = (
                    maximum.x > maximum.y ?
                        maximum.x :
                        maximum.y
                        );

        maxBound = (
                    maxBound > maximum.z ?
                        maxBound :
                        maximum.z
                        );

        maxDimension = maxBound;
    }
}
