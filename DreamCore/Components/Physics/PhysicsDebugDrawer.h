/*
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


#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <LinearMath/btVector3.h>
#include <LinearMath/btIDebugDraw.h>

#include "../Graphics/Shader/ShaderInstance.h"

using namespace std;

namespace Dream
{

    struct PhysicsDebugVertex
    {
        glm::vec3 Position;
        glm::vec3 Color;
    };

    class PhysicsDebugDrawer : public btIDebugDraw, DreamObject
    {
        glm::mat4 mProjectionMatrix;
        glm::mat4 mViewMatrix;
        GLuint mShaderProgram;
        GLuint mVAO, mVBO;
        vector<PhysicsDebugVertex> mVertexBuffer;
        int mDebugMode;

    public:
        PhysicsDebugDrawer();
        ~PhysicsDebugDrawer();

        void init();
        void initShader();
        void initVaoVbo();
        void setProjectionMatrix(glm::mat4);
        void setViewMatrix(glm::mat4);
        void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);
        void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
        void drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
        void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
        void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
        void reportErrorWarning(const char* warningString);
        void draw3dText(const btVector3& location,const char* textString);
        void setDebugMode(int debugMode);
        int  getDebugMode() const { return mDebugMode;}
        string btVecToString(const btVector3&);
        void drawAll();
        void preRender();
        void postRender();

    };

} // End of Dream
