#ifndef BulletGLDebugDrawer_h
#define BulletGLDebugDrawer_h

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <LinearMath/btVector3.h>
#include <LinearMath/btIDebugDraw.h>

namespace Dream   {
namespace Plugins {
namespace Physics {
namespace Bullet  {
	
	class BulletGLDebugDrawer : public btIDebugDraw {
	private:
		int mDebugMode;
	public:
		BulletGLDebugDrawer();
		void drawLine(const btVector3&,const btVector3&,const btVector3&);
		void drawContactPoint(const btVector3&,const btVector3&, btScalar,int,const btVector3&);
		void reportErrorWarning(const char*);
		void draw3dText(const btVector3&,const char*);
		void setDebugMode(int);
		virtual int  getDebugMode() const { return mDebugMode;}
	}; // End of BulletGLDebugDrawer
	
} // End of Bullet
} // End of Physics
} // End of Plugins
} // End of Dream

#endif /* BulletGLDebugDrawer_h */
