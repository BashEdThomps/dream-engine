#ifndef PhysicsDebugDrawer_h
#define PhysicsDebugDrawer_h

#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <LinearMath/btVector3.h>
#include <LinearMath/btIDebugDraw.h>
#include "../Graphics/ShaderInstance.h"
#include "../../Constants.h"

using namespace std;

namespace Dream
{

    struct PhysicsDebugVertex
    {
        glm::vec3 Position;
        glm::vec3 Color;
    };

    class PhysicsDebugDrawer : public btIDebugDraw
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
        bool checkGLError(string);
        void drawAll();

    };
} // End of Dream

#endif /* PhysicsDebugDrawer_h */
