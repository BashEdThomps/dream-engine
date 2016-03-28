/*
* ObjMaterial
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

#include "ObjMaterial.h"

namespace DreamFileFormat {

	ObjMaterial::ObjMaterial(void) {
	}

	ObjMaterial::~ObjMaterial(void) {
	}

    void ObjMaterial::setNs(float a)
    {
        mNs = a;
    }

    void ObjMaterial::setKa(float a, float b, float c)
    {
        mKa[0] = a;
        mKa[1] = b;
        mKa[2] = c;
    }

    void ObjMaterial::setKd(float a, float b, float c)
    {
        mKd[0] = a;
        mKd[1] = b;
        mKd[2] = c;
    }

    void ObjMaterial::setKs(float a, float b, float c)
    {
        mKs[0] = a;
        mKs[1] = b;
        mKs[2] = c;
    }

    void ObjMaterial::setNi(float ni)
    {
        mNi = ni;
    }

    void ObjMaterial::setD(float d)
    {
        mD = d;
    }

    void ObjMaterial::setIllum(int illum)
    {
        mIllum = illum;
    }

    float ObjMaterial::getNs()
    {
        return mNs;
    }

	    float[] ObjMaterial::getKa()
    {
        return mKa;
    }

    void ObjMaterial::setKa(float[] mKa)
    {
        this.mKa = mKa;
    }

    float[] ObjMaterial::getKd()
    {
        return mKd;
    }

    void ObjMaterial::setKd(float[] mKd)
    {
        this.mKd = mKd;
    }

    float[] ObjMaterial::getKs()
    {
        return mKs;
    }

    void ObjMaterial::setKs(float[] mKs)
    {
        this.mKs = mKs;
    }

    float ObjMaterial::getNi()
    {
        return mNi;
    }

    float ObjMaterial::getD()
    {
        return mD;
    }

    int ObjMaterial::getIllum()
    {
        return mIllum;
    }

    std::tring ObjMaterial::getName()
    {
        return mName;
    }

	void ObjMaterial::setName(String name)
	{
	    mName = name;
	}

} // End of DreamFileFormat
