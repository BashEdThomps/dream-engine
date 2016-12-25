#include "PhysicsDebugDrawer.h"

namespace Dream {

    PhysicsDebugDrawer::PhysicsDebugDrawer() {
        mDebugMode = 0;
        mShaderProgram = 0;
    }

    PhysicsDebugDrawer::~PhysicsDebugDrawer() {}

    void PhysicsDebugDrawer::initShader() {
        string mVertexShaderSource;
        string mFragmentShaderSource;
        GLuint mVertexShader = 0;
        GLuint mFragmentShader = 0;

        mVertexShaderSource = "#version 330 core\n"
                              "layout (location = 0) in vec4 position;\n"
                              "uniform mat4 view;\n"
                              "uniform mat4 projection;\n"
                              "void main () {\n"
                                "gl_Position = projection * view * position;\n"
                              "}\n";

        mFragmentShaderSource = "#version 330 core\n"
                                "out vec4 fragColor;\n"
                                "uniform highp vec3 color;\n"
                                "void main () {\n"
                                  "fragColor = vec4(color.r, color.g, color.b, 0.0);\n"
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
        if (!success) {
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
        if (!success) {
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

    void PhysicsDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color, const btVector3& color2) {
        glUseProgram(mShaderProgram);
        // Set the projection matrix
        GLint pu = glGetUniformLocation(mShaderProgram, "projection");
        GLint vu = glGetUniformLocation(mShaderProgram, "view");
        glUniformMatrix4fv(pu, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
        glUniformMatrix4fv(vu, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
        // Set the colour of the line
        GLint puc = glGetUniformLocation(mShaderProgram, "color");
        glm::vec3 colourVec = glm::vec3(color.getX(), color.getY(), color.getZ());
        glUniform3fv(puc, 1, glm::value_ptr(colourVec));
        // Set the line vertices
        float tmp[6] = {
          from.getX(), from.getY(), from.getZ(),
          to.getX(), to.getY(), to.getZ()
        };
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, &tmp[0]);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINES, 0, 2 );
        glUseProgram(0);
    }

    void PhysicsDebugDrawer::setProjectionMatrix(glm::mat4 projMat) {
      mProjectionMatrix = projMat;
    }

    void PhysicsDebugDrawer::setViewMatrix(glm::mat4 viewMat) {
      mViewMatrix = viewMat;
    }

    void PhysicsDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color) {
        if (DEBUG) {
            cout <<"PhysicsDebugDrawer: Drawing line from"
                << btVecToString(from)
                << " to " << btVecToString(to)
                << " with colour " << btVecToString(color)
                << endl;
        }
        drawLine(from,to,color,color);
    }

    string PhysicsDebugDrawer::btVecToString(const btVector3& vec) {
        stringstream str;
        str << "(" << vec.getX() << "," << vec.getY() << "," << vec.getZ() << ")";
        return str.str();
    }

    void PhysicsDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color) {
        if (DEBUG) {
            cout << "PhysicsDebugDrawer: Draw Sphere is not implemented" << endl;
        }
    }

    void PhysicsDebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha) {
        if (DEBUG) {
            cout << "PhysicsDebugDrawer: Draw Triangle is not implemented" << endl;
        }
    }

    void PhysicsDebugDrawer::setDebugMode(int debugMode) {
        mDebugMode = debugMode;
    }

    void PhysicsDebugDrawer::draw3dText(const btVector3& location,const char* textString) {
        if (DEBUG) {
            cout << "PhysicsDebugDrawer: Draw 3DText is not implemented" << endl;
        }
    }

    void PhysicsDebugDrawer::reportErrorWarning(const char* warningString) {
        if (DEBUG) {
            cout << "PhysicsDebugDrawer: " << warningString << endl;
        }
    }

    void PhysicsDebugDrawer::drawContactPoint(
            const btVector3& pointOnB, const btVector3& normalOnB,
            btScalar distance, int lifeTime, const btVector3& color) {
        if (DEBUG) {
            cout << "PhysicsDebugDrawer: Draw Contact Point is not implemented" << endl;
        }
    }

} // End of Dream
