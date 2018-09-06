#include "PhysicsDebugDrawer.h"

namespace Dream
{

    PhysicsDebugDrawer::PhysicsDebugDrawer
    () : DreamObject ("PhysicsDebugDrawer")
    {
        auto log = getLog();
        log->info( "Constructing Object" );

        mDebugMode = 0;
        mShaderProgram = 0;
    }

    PhysicsDebugDrawer::~PhysicsDebugDrawer
    ()
    {
        auto log = getLog();
        log->info( "Destroying Object" );
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
    }

    void
    PhysicsDebugDrawer::initShader
    ()
    {
        auto log = getLog();
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
            log->error("SHADER:VERTEX:COMPILATION_FAILED\n{}" , infoLog );
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
            log->error( "SHADER:FRAGMENT:COMPILATION_FAILED\n {}" , infoLog );
        }

        // Shader Program
        mShaderProgram = glCreateProgram();
        glAttachShader(mShaderProgram, mVertexShader);
        glAttachShader(mShaderProgram, mFragmentShader);
        glLinkProgram(mShaderProgram);

        // Print linking errors if any
        glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
            log->error ("SHADER:PROGRAM:LINKING_FAILED\n {}" , infoLog );
        }

        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);
    }

    void
    PhysicsDebugDrawer::drawLine
    (const btVector3& from,const btVector3& to,const btVector3& color, const btVector3& color2)
    {
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
    }

    void
    PhysicsDebugDrawer::setProjectionMatrix
    (glm::mat4 projMat)
    {
        mProjectionMatrix = projMat;
    }

    void
    PhysicsDebugDrawer::setViewMatrix
    (glm::mat4 viewMat)
    {
        mViewMatrix = viewMat;
    }

    void
    PhysicsDebugDrawer::drawLine
    (const btVector3& from,const btVector3& to,const btVector3& color)
    {
        auto log = getLog();
        log->trace(
            "Queuing line from {} to {} with colour {}" ,
            btVecToString(from),
            btVecToString(to),
            btVecToString(color)
        );
        drawLine(from,to,color,color);
    }

    string
    PhysicsDebugDrawer::btVecToString
    (const btVector3& vec)
    {
        stringstream str;
        str << "(" << vec.getX() << "," << vec.getY() << "," << vec.getZ();
        return str.str();
    }

    void
    PhysicsDebugDrawer::drawSphere
    (const btVector3& p, btScalar radius, const btVector3& color)
    {
        auto log = getLog();
        log->info( "Draw Sphere is not implemented" );
    }

    void
    PhysicsDebugDrawer::drawTriangle
    (const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
    {
        auto log = getLog();
        log->info( "Draw Triangle is not implemented" );
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
        auto log = getLog();
        log->info( "Draw 3DText is not implemented" );
    }

    void
    PhysicsDebugDrawer::reportErrorWarning
    (const char* warningString)
    {
        auto log = getLog();
        log->info( warningString );
    }

    void
    PhysicsDebugDrawer::drawContactPoint
    (const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
    {
        auto log = getLog();
        log->info( "Draw Contact Point is not implemented" );
    }

    void
    PhysicsDebugDrawer::drawAll
    ()
    {
        auto log = getLog();
        preRender();
        log->info( "Drawing {} lines" , mVertexBuffer.size()/2 );

        // Enable shader program
        glUseProgram(mShaderProgram);

        // Set the projection matrix
        GLint projUniform = glGetUniformLocation(mShaderProgram, "projection");
        if (projUniform == -1)
        {
            log->error( "Unable to find Uniform Location for projection" );
            checkGLError();
            return;
        }
        else
        {
            glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
        }

        // Set the view matrix
        GLint viewUniform = glGetUniformLocation(mShaderProgram, "view");
        if (viewUniform == -1)
        {
            log->error( "Unable to find Uniform Location for view" );
            checkGLError();
            return;
        }
        else
        {
            glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
        }

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(PhysicsDebugVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);

        // Vertex Array
        glBindVertexArray(mVAO);
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
        // Draw
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));
        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
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
        checkGLError();
    }

    void
    PhysicsDebugDrawer::postRender
    ()
    {
        glDisable (GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_DEPTH_TEST);
        checkGLError();
    }
} // End of Dream
