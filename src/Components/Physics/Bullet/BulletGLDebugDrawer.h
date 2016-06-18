#ifndef BulletGLDebugDrawer_h
#define BulletGLDebugDrawer_h

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <LinearMath/btVector3.h>
#include <LinearMath/btIDebugDraw.h>

namespace Dream   {
namespace Components {
namespace Physics {
namespace Bullet  {
	
	class GLDebugDrawer : public btIDebugDraw {
		int m_debugMode;
	public:
		GLDebugDrawer();
		virtual ~GLDebugDrawer();
		
		virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);
		virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
		virtual void	drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
		virtual void	drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
		virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
		virtual void	reportErrorWarning(const char* warningString);
		virtual void	draw3dText(const btVector3& location,const char* textString);
		virtual void	setDebugMode(int debugMode);
		virtual int		getDebugMode() const { return m_debugMode;}
		
	};
} // End of Bullet
} // End of Physics
} // End of Components
} // End of Dream

#endif /* BulletGLDebugDrawer_h */
