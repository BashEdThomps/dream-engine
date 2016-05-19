/*
* Material
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

#include "Material.h"

namespace Dream {
	namespace Asset {
		namespace Model {
			namespace Wavefront {
				Material::Material(void) {
				}

				Material::~Material(void) {
				}

			    void Material::setNs(float a) {
			        mNs = a;
			    }

			    void Material::setKa(float a, float b, float c) {
			        mKa[0] = a;
			        mKa[1] = b;
			        mKa[2] = c;
			    }

			    void Material::setKd(float a, float b, float c) {
			        mKd[0] = a;
			        mKd[1] = b;
			        mKd[2] = c;
			    }

			    void Material::setKs(float a, float b, float c) {
			        mKs[0] = a;
			        mKs[1] = b;
			        mKs[2] = c;
			    }

			    void Material::setNi(float ni) {
			        mNi = ni;
			    }

			    void Material::setD(float d) {
			        mD = d;
			    }

			    void Material::setIllum(int illum) {
			        mIllum = illum;
			    }

			    float Material::getNs() {
			        return mNs;
			    }

				float* Material::getKa() {
			        return mKa;
			    }

			    float* Material::getKd() {
			        return mKd;
			    }

			    float* Material::getKs() {
			        return mKs;
			    }

			    float Material::getNi() {
			        return mNi;
			    }

			    float Material::getD() {
			        return mD;
			    }

			    int Material::getIllum() {
			        return mIllum;
			    }

			    std::string Material::getName() {
			        return mName;
			    }

				void Material::setName(std::string name) {
				    mName = name;
				}
			} // End of Wavefront
		} // End of Model
	} // End of Asset
} // End of Dream
