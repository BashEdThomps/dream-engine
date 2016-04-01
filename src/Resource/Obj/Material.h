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

#ifndef OBJMATERIAL_H
#define OBJMATERIAL_H

#include <iostream>
#include <string>

namespace DreamFileFormats {
	namespace Obj {
		class Material {
		private:
			std::string mName;
		    float mNs;    // Specular exponent for the current material
		    float mKa[3]; // Ambient Colour RGB
		    float mKd[3]; // Diffuse Colour RGB
		    float mKs[3]; // Specular Colour RGB
		    float mNi;    // Optical Density
		    float mD;     // Dissolved 0-1000
			int mIllum;
		public:
			Material(void);
			~Material(void);
			void   setNs(float);
		    void   setKa(float, float, float);
		    void   setKd(float, float, float);
		    void   setKs(float, float, float);
		    void   setNi(float);
		    void   setD(float);
		    void   setIllum(int);
		    float  getNs();
			float* getKa();
		    void   setKa(float*);
		    float* getKd();
		    void   setKd(float*);
		    float* getKs();
		    void   setKs(float*);
		    float  getNi();
		    float  getD();
		    int    getIllum();
		    std::string getName();
			void setName(std::string);
		}; // End of Material
	} // End of Obj
} // End of DreamFileFormat

#endif // End of OBJMATERIAL_H
