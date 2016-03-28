/*
* ObjFace
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>
#include "ObjFace.h"

namespace DreamFileFormat {

	ObjFace::ObjFace(int* verts, int* norms, Material material) {
        mVerts = verts;
        mNormals = norms;
        mMaterial = material;
    }

	ObjFace::~ObjFace(void) {
	}

    int* ObjFace::getVerts() {
        return mVerts;
    }

    int* ObjFace::getNormals() {
        return mNormals;
    }

    Material ObjFace::getMaterial() {
        return mMaterial;
    }

    std::string ObjFace::toString() {
        std::stringstream ss;
        ss << "Face:" << std::endl;;
        ss << "\tVerts: (" << mVerts[0] << "," << mVerts[1] << "," << mVerts[2] << ")" << std::endl;
        ss << "\tNormals: (" << mNormals[0] << "," << mNormals[1] << "," << mNormals[2] << ")"<< std::endl;
        ss << "\tMaterial: " << (mMaterial==NULL?"None":mMaterial.getName() << std::endl;
        return ss.str();
    }

} // End of DreamFileFormat
