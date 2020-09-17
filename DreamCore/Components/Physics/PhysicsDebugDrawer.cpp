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


#include <algorithm>
#include "PhysicsDebugDrawer.h"
#include "../../Scene/Actor/BoundingBox.h"
#include "../Graphics/Camera.h"

namespace Dream
{

    void PhysicsDebugDrawer::setCamera(Camera* camera)
    {
        mCamera = camera;
    }

    PhysicsDebugDrawer::PhysicsDebugDrawer
    () : DreamObject ("PhysicsDebugDrawer"),
        mCamera(nullptr),
        mVAO(0),mVBO(0)

    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Constructing Object" );
        #endif

        mDebugMode = DBG_DrawAabb;
        mShaderProgram = 0;
    }

    PhysicsDebugDrawer::~PhysicsDebugDrawer
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Destroying Object" );
        #endif
    }

    void
    PhysicsDebugDrawer::init
    ()
    {
        initShader();
        initVaoVbo();
    }

    void
    PhysicsDebugDrawer::initVaoVbo
    ()
    {
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        // Vertex Array
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer
                (
                    0, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(PhysicsDebugVertex)),
                    static_cast<GLvoid*>(nullptr)
                    );
        // Vertex Colors
        glEnableVertexAttribArray(1);
        glVertexAttribPointer
                (
                    1, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(PhysicsDebugVertex)),
                    (GLvoid*)offsetof(PhysicsDebugVertex, Color)
                    );
    }

    void
    PhysicsDebugDrawer::initShader
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif
        string mVertexShaderSource;
        string mFragmentShaderSource;
        GLuint mVertexShader = 0;
        GLuint mFragmentShader = 0;

        mVertexShaderSource = "#version 330 core\n"
                              "\n"
                              "layout (location = 0) in vec3 position;\n"
                              "layout (location = 1) in vec3 color;\n"
                              "\n"
                              "out vec3 Color;\n"
                              "\n"
                              "uniform mat4 view;\n"
                              "uniform mat4 projection;\n"
                              "\n"
                              "void main ()\n"
                              "{\n"
                              "    gl_Position = projection * view * vec4(position,1.0);\n"
                              "    Color = color;\n"
                              "}";

        mFragmentShaderSource = "#version 330 core\n"
                                "\n"
                                "in vec3  Color;\n"
                                "\n"
                                "out vec4 fragColor;\n"
                                "\n"
                                "void main()\n"
                                "{\n"
                                "    fragColor = vec4(Color,1.0);\n"
                                "}";
        // Compile shaders
        GLint success;
        GLchar infoLog[512];

        // Vertex Shader
        mVertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vSource = mVertexShaderSource.c_str();
        glShaderSource(mVertexShader, 1, &vSource, nullptr);
        glCompileShader(mVertexShader);

        // Print compile errors if any
        glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(mVertexShader, 512, nullptr, infoLog);

            #ifdef DREAM_LOG
            log->error("SHADER:VERTEX:COMPILATION_FAILED\n{}" , infoLog );
            #endif
        }

        // Fragment Shader
        mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fSource = mFragmentShaderSource.c_str();
        glShaderSource(mFragmentShader, 1, &fSource, nullptr);
        glCompileShader(mFragmentShader);

        // Print compile errors if any
        glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(mFragmentShader, 512, nullptr, infoLog);
            #ifdef DREAM_LOG
            log->error( "SHADER:FRAGMENT:COMPILATION_FAILED\n {}" , infoLog );
            #endif
        }

        // Shader Program
        mShaderProgram = glCreateProgram();
        glAttachShader(mShaderProgram, mVertexShader);
        glAttachShader(mShaderProgram, mFragmentShader);
        glLinkProgram(mShaderProgram);

        // Print linking errors if any
        glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
            #ifdef DREAM_LOG
            log->error ("SHADER:PROGRAM:LINKING_FAILED\n {}" , infoLog );
            #endif
        }

        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);
    }

    void
    PhysicsDebugDrawer::drawLine
    (const btVector3& from,const btVector3& to,const btVector3& color, const btVector3& color2)
    {
        // Too many lines to be effective?
        /*
        BoundingBox bb;
        bb.minimum.x = std::min(from.x(),to.x());
        bb.minimum.y = std::min(from.y(),to.y());
        bb.minimum.z = std::min(from.z(),to.z());

        bb.maximum.x = std::max(from.x(),to.x());
        bb.maximum.y = std::max(from.y(),to.y());
        bb.maximum.z = std::max(from.z(),to.z());

        if (mCamera->inFrustum(bb))
        {
        */
            PhysicsDebugVertex a, b;

            a.Position.x = from.getX();
            a.Position.y = from.getY();
            a.Position.z = from.getZ();
            a.Color.r = color.getX();
            a.Color.g = color.getY();
            a.Color.b = color.getZ();
            mVertexBuffer.push_back(a);

            b.Position.x = to.getX();
            b.Position.y = to.getY();
            b.Position.z = to.getZ();
            b.Color.r = color2.getX();
            b.Color.g = color2.getY();
            b.Color.b = color2.getZ();
            mVertexBuffer.push_back(b);
            /*
        }
        else
        {
            auto log = getLog();
            log->error("Skipping debug point");
        }
        */
    }

    void
    PhysicsDebugDrawer::drawLine
    (const btVector3& from,const btVector3& to,const btVector3& color)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace(
            "Queuing line from {} to {} with colour {}" ,
            btVecToString(from),
            btVecToString(to),
            btVecToString(color)
        );
        #endif
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
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Draw Sphere is not implemented" );
        #endif
    }

    void
    PhysicsDebugDrawer::drawTriangle
    (const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Draw Triangle is not implemented" );
        #endif
    }

    void
    PhysicsDebugDrawer::setDebugMode
    (int debugMode)
    {
        mDebugMode = debugMode;
    }

    void
    PhysicsDebugDrawer::draw3dText
    (const btVector3& location,const char* textString)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Draw 3DText is not implemented" );
        #endif
    }

    void
    PhysicsDebugDrawer::reportErrorWarning
    (const char* warningString)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( warningString );
        #endif
    }

    void
    PhysicsDebugDrawer::drawContactPoint
    (const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Draw Contact Point is not implemented" );
        #endif
    }

    void
    PhysicsDebugDrawer::drawAll
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Drawing {} lines" , mVertexBuffer.size()/2 );
        #endif
        preRender();

        // Enable shader program
        glUseProgram(mShaderProgram);

        glBindVertexArray(mVAO);
        ShaderRuntime::CurrentShaderProgram = mShaderProgram;

        // Set the projection matrix
        GLint projUniform = glGetUniformLocation(mShaderProgram, "projection");
        if (projUniform == -1)
        {
            #ifdef DREAM_LOG
            log->error( "Unable to find Uniform Location for projection" );
            checkGLError();
            #endif
            return;
        }
        else
        {
            mat4 projectionMatrix = mCamera->getProjectionMatrix();
            glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        }

        // Set the view matrix
        GLint viewUniform = glGetUniformLocation(mShaderProgram, "view");
        if (viewUniform == -1)
        {
            #ifdef DREAM_LOG
            log->error( "Unable to find Uniform Location for view" );
            checkGLError();
            #endif
            return;
        }
        else
        {
            mat4 viewMatrix = mCamera->getViewMatrix();
            glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        }

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(PhysicsDebugVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);

        // Draw
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));
        // Unbind
        postRender();
        // Clear old buffer
        mVertexBuffer.clear();
    }
    void
    PhysicsDebugDrawer::preRender
    ()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_DEPTH_TEST);

        #ifdef DREAM_LOG
        checkGLError();
        #endif
    }

    void
    PhysicsDebugDrawer::postRender
    ()
    {
        glDisable (GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_DEPTH_TEST);
        #ifdef DREAM_LOG
        checkGLError();
        #endif
    }
} // End of Dream
