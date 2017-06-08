#include "PhysicsDebugDrawer.h"

namespace Dream
{

    PhysicsDebugDrawer::PhysicsDebugDrawer
    ()
    {
        mDebugMode = 0;
        mShaderProgram = 0;
    }

    PhysicsDebugDrawer::~PhysicsDebugDrawer
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsDebugDrawer: Destroying Object" << endl;
        }
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
                              "void main () {\n"
                              "    gl_Position = projection * view * vec4(position,1.0);\n"
                              "    Color = color;\n"
                              "}";

        mFragmentShaderSource = "#version 330 core\n"
                                "\n"
                                "in vec3  Color;\n"
                                "\n"
                                "out vec4 fragColor;\n"
                                "\n"
                                "void main() { \n"
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
            cerr << "PhysicsDebugDrawer: SHADER:VERTEX:COMPILATION_FAILED\n" << infoLog << endl;
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
            cerr << "PhysicsDebugDrawer: SHADER:FRAGMENT:COMPILATION_FAILED\n" << infoLog << endl;
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
            cerr << "PhysicsDebugDrawer: SHADER:PROGRAM:LINKING_FAILED\n" << infoLog << endl;
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
        if (Constants::VERBOSE)
        {
            cout << "PhysicsDebugDrawer: Drawing line from " << btVecToString(from)
                 << " to " << btVecToString(to)
                 << " with colour " << btVecToString(color) << endl;
        }
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
        if (Constants::DEBUG)
        {
            cout << "PhysicsDebugDrawer: Draw Sphere is not implemented" << endl;
        }
    }

    void
    PhysicsDebugDrawer::drawTriangle
    (const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsDebugDrawer: Draw Triangle is not implemented" << endl;
        }
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
        if (Constants::DEBUG)
        {
            cout << "PhysicsDebugDrawer: Draw 3DText is not implemented" << endl;
        }
    }

    void
    PhysicsDebugDrawer::reportErrorWarning
    (const char* warningString)
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsDebugDrawer: " << warningString << endl;
        }
    }

    void
    PhysicsDebugDrawer::drawContactPoint
    (const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsDebugDrawer: Draw Contact Point is not implemented" << endl;
        }
    }

    bool
    PhysicsDebugDrawer::checkGLError
    (string marker)
    {
        GLenum errorCode = 0;
        bool wasError = false;
        do
        {
            errorCode = glGetError();
            if (errorCode!=0)
            {
                cerr << "PhysicsDebugDrawer: Error Check " << marker << ": " << endl;
                switch (errorCode)
                {
                    case GL_NO_ERROR:
                        cerr << "\tGL_NO_ERROR" << endl;
                        break;
                    case GL_INVALID_ENUM:
                        cerr << "\tGL_INVALID_ENUM" << endl;
                        break;
                    case GL_INVALID_VALUE:
                        cerr << "\tGL_INVALID_VALUE" << endl;
                        break;
                    case GL_INVALID_OPERATION:
                        cerr << "\tGL_INVALID_OPERATION" << endl;
                        break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        cerr << "\tGL_INVALID_FRAMEBUFFER_OPERATION" << endl;
                        break;
                    case GL_OUT_OF_MEMORY:
                        cerr << "\tGL_OUT_OF_MEMORY" << endl;
                        break;
                }
                cerr << "\tName: " << glewGetErrorString(errorCode) << endl;
                cerr << "\tCode: " << errorCode << endl;
                wasError = true;
            }
        }
        while(errorCode != 0);
        return wasError;
    }

    void
    PhysicsDebugDrawer::drawAll
    ()
    {
        glEnable(GL_LINE_SMOOTH);
        if (Constants::VERBOSE)
        {
            cout << "PhysicsDebugDrawer: Drawing all - " << mVertexBuffer.size()/2 << " lines." << endl;
        }
        // Enable shader program
        glUseProgram(mShaderProgram);


        // Set the projection matrix
        GLint projUniform = glGetUniformLocation(mShaderProgram, "projection");
        if (projUniform == -1)
        {
            cerr << "PhysicsDebugDrawer: Unable to find Uniform Location for projection" << endl;
            checkGLError("set proj");
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
            cerr << "PhysicsDebugDrawer: Unable to find Uniform Location for view" << endl;
            checkGLError("set view");
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
        glVertexAttribPointer(
                    0, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(PhysicsDebugVertex)),
                    static_cast<GLvoid*>(0)
                    );
        // Vertex Colors
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
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
        glDisable(GL_LINE_SMOOTH);
        // Clear old buffer
        mVertexBuffer.clear();
    }

} // End of Dream
