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

#include <DreamCore.h>
#include "GLWidget.h"
#include <LinearMath/btIDebugDraw.h>

namespace octronic::dream::tool
{
    class PhysicsDebugDrawer : public GLWidget, btIDebugDraw
    {

    public:
        PhysicsDebugDrawer(DreamToolContext* ctx);
        ~PhysicsDebugDrawer() override;

        void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor) override;
        void drawLine(const btVector3& from,const btVector3& to,const btVector3& color) override;
        void drawSphere (const btVector3& p, btScalar radius, const btVector3& color) override;
        void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha) override;
        void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) override;
        void reportErrorWarning(const char* warningString) override;
        void draw3dText(const btVector3& location,const char* textString) override;
        void setDebugMode(int debugMode) override;
        int  getDebugMode() const override;
        string btVecToString(const btVector3&);
	private:
        int mDebugMode;
    };
}
