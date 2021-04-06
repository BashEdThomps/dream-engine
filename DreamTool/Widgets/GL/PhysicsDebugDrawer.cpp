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
#include "PhysicsDebugDrawer.h"


#include <algorithm>

namespace octronic::dream::tool
{
    PhysicsDebugDrawer::PhysicsDebugDrawer
    (DreamToolContext& ctx)
        : GLWidget(ctx, true)
    {
        LOG_DEBUG( "PhysicaDebugDrawer: Constructing Object" );
        mDebugMode = DBG_DrawAabb;
    }

    PhysicsDebugDrawer::~PhysicsDebugDrawer
    ()
    {
        LOG_DEBUG( "PhysicaDebugDrawer: Destroying Object" );
    }

    // btIDebugDrawer Interface ================================================

    void
    PhysicsDebugDrawer::drawLine
    (const btVector3& from,const btVector3& to,const btVector3& color, const btVector3& color2)
    {
        TranslationColorVertex a, b;

        a.Translation.x = from.getX();
        a.Translation.y = from.getY();
        a.Translation.z = from.getZ();
        a.Color.r = color.getX();
        a.Color.g = color.getY();
        a.Color.b = color.getZ();
        a.Color.a = 1.f;

        b.Translation.x = to.getX();
        b.Translation.y = to.getY();
        b.Translation.z = to.getZ();
        b.Color.r = color2.getX();
        b.Color.g = color2.getY();
        b.Color.b = color2.getZ();
        b.Color.a = 1.f;
        addLineVertex(a);
        addLineVertex(b);
    }

    void
    PhysicsDebugDrawer::drawLine
    (const btVector3& from,const btVector3& to,const btVector3& color)
    {
        drawLine(from,to,color,color);
    }

    string
    PhysicsDebugDrawer::btVecToString
    (const btVector3& vec)
    {
        stringstream str;
        str << "(" << vec.getX() << "," << vec.getY() << "," << vec.getZ() << ")";
        return str.str();
    }

    void
    PhysicsDebugDrawer::drawSphere
    (const btVector3& p, btScalar radius, const btVector3& color)
    {
        LOG_DEBUG( "PhysicaDebugDrawer: Draw Sphere is not implemented" );
    }

    void
    PhysicsDebugDrawer::drawTriangle
    (const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
    {
        LOG_DEBUG( "PhysicaDebugDrawer: Draw Triangle is not implemented" );
    }

    void
    PhysicsDebugDrawer::draw3dText
    (const btVector3& location,const char* textString)
    {
        LOG_DEBUG( "PhysicaDebugDrawer: Draw 3DText is not implemented" );
    }

    void
    PhysicsDebugDrawer::reportErrorWarning
    (const char* warningString)
    {
        LOG_DEBUG( warningString );
    }

    void
    PhysicsDebugDrawer::drawContactPoint
    (const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
    {
        LOG_DEBUG( "PhysicaDebugDrawer: Draw Contact Point is not implemented" );
    }

    // Accessors ===============================================================

    void
    PhysicsDebugDrawer::setDebugMode
    (int debugMode)
    {
        mDebugMode = debugMode;
    }

    int
    PhysicsDebugDrawer::getDebugMode
    ()
    const
    {
        return mDebugMode;
    }
}
