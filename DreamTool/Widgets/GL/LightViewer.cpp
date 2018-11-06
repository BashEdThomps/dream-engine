#include "LightViewer.h"

namespace DreamTool
{
    LightViewer::LightViewer
    (Project* p)
        : GLWidget(p)
    {
        setLogClassName("LightViewer");
        for (auto index : LightModelIndices)
        {
            mVertexBuffer.push_back(LightModelVertices.at(index));
        }
    }


    LightViewer::~LightViewer
    ()
    {
    }

    void LightViewer::draw()
    {
		checkGLError();
        vector<IAssetInstance*> lightInstances;
        if (mProject)
        {
            auto pRuntime = mProject->getProjectRuntime();
            if (pRuntime)
            {
                auto gfx = pRuntime->getGraphicsComponent();
                if (gfx)
                {
                    mViewMatrix = gfx->getViewMatrix();
                    mProjectionMatrix = gfx->getProjectionMatrix();
                }
                auto activeSR = pRuntime->getActiveSceneRuntime();
                if (activeSR)
                {
                    lightInstances = activeSR->getAssetInstances(AssetType::LIGHT);
                }
            }
        }

        auto log = getLog();
        if (!mVertexBuffer.empty())
        {
#ifndef __APPLE__
            glEnable(GL_LINE_SMOOTH);
            checkGLError();
            glLineWidth(3.0f);
            checkGLError();
#endif

            // Enable shader program
            glUseProgram(mShaderProgram);
            ShaderInstance::CurrentShaderProgram = mShaderProgram;
            checkGLError();

            // Vertex Array
            glBindVertexArray(mVao);
            ShaderInstance::CurrentVAO = mVao;
            checkGLError();

            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            ShaderInstance::CurrentVBO = mVbo;
            checkGLError();

            glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(LineVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
            checkGLError();

            // Set the projection matrix
            GLint projUniform = glGetUniformLocation(mShaderProgram, "projection");
            checkGLError();
            if (projUniform == -1)
            {
                log->error("Unable to find Uniform Location for projection");
                return;
            }
            else
            {
                glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
				checkGLError();
            }

            // Set the view matrix
            GLint viewUniform = glGetUniformLocation(mShaderProgram, "view");
            checkGLError();
            if (viewUniform == -1)
            {
                log->error("Unable to find Uniform Location for view");
                return;
            }
            else
            {
                glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
				checkGLError();
            }

            GLint modelUniform = glGetUniformLocation(mShaderProgram, "model");
            checkGLError();

            GLint lightColorUniform = glGetUniformLocation(mShaderProgram, "lightColor");
            checkGLError();

            for (auto inst : lightInstances)
            {
                auto light = dynamic_cast<LightInstance*>(inst);
                mModelMatrix = glm::translate(mat4(1.0f),light->getSceneObjectRuntime()->getTransform()->getTranslation());
                vec3 lightColorVec = light->getDiffuse();
                // Set the projection matrix
                if (modelUniform == -1)
                {
                    log->error("Unable to find Uniform Location for model");
                    break;
                }
                else
                {
                    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
		            checkGLError();
                }

                if (lightColorUniform == -1)
                {
                   log->error("Unable to find uniform location for lightColor");
                   break;
                }
                else
                {
                    glUniform3fv(lightColorUniform,1,glm::value_ptr(lightColorVec));
			        checkGLError();
                }
                // Draw
                glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));
                checkGLError();
            }

            // Revert State
#ifndef __APPLE__
            glDisable(GL_LINE_SMOOTH);
			glLineWidth(1.0f);
            checkGLError();
#endif
        }
    }

    void
    LightViewer::setShader
    ()
    {
        auto log = getLog();
        log->error("Compiling LightViewer Shaders");
        mVertexShaderSource =
            "#version 330 core\n"
            "\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec3 color;\n"
            "\n"
            "out vec3 Color;\n"
            "\n"
            "uniform vec3 lightColor;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "\n"
            "void main () {\n"
            "    gl_Position = projection * view * model * vec4(position,1.0) ;\n"
            "    Color = lightColor;\n"
            "}";

        mFragmentShaderSource =
            "#version 330 core\n"
            "\n"
            "in vec3  Color;\n"
            "\n"
            "out vec4 fragColor;\n"
            "\n"
            "void main() { \n"
            "    fragColor = vec4(Color,1.0);\n"
            "}";
    }

    const vector<LineVertex> LightViewer::LightModelVertices = {
        LineVertex{vec3(-0.125f, 0.500f, 0.000f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.500f, 0.000f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.462f, -0.191f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.462f, -0.191f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.354f, -0.354f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.354f, -0.354f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.191f, -0.462f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.191f, -0.462f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.000f, -0.500f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.000f, -0.500f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.191f, -0.462f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.191f, -0.462f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.354f, -0.354f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.354f, -0.354f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.462f, -0.191f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.462f, -0.191f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.500f, -0.000f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.500f, -0.000f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.462f, 0.191f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.462f, 0.191f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.354f, 0.354f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.354f, 0.354f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.191f, 0.462f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.191f, 0.462f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.000f, 0.500f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.000f, 0.500f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.191f, 0.462f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.191f, 0.462f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.354f, 0.354f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.354f, 0.354f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.462f, 0.191f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.462f, 0.191f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.745f, -0.049f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.707f, -0.240f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.707f, -0.240f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.745f, -0.049f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.492f, -0.561f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.492f, -0.561f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.330f, -0.670f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.330f, -0.670f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.049f, -0.745f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.049f, -0.745f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.240f, -0.707f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.240f, -0.707f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.561f, -0.492f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.561f, -0.492f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.670f, -0.330f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.670f, -0.330f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.745f, 0.049f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.745f, 0.049f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.707f, 0.240f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.707f, 0.240f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.492f, 0.561f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.492f, 0.561f), vec3(1.0f)},
        LineVertex{vec3(-0.125f,-0.330f, 0.670f), vec3(1.0f)},
        LineVertex{vec3(0.125f,-0.330f, 0.670f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.049f, 0.745f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.049f, 0.745f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.240f, 0.707f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.240f, 0.707f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.561f, 0.492f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.561f, 0.492f), vec3(1.0f)},
        LineVertex{vec3(-0.125f, 0.670f, 0.330f), vec3(1.0f)},
        LineVertex{vec3(0.125f, 0.670f, 0.330f), vec3(1.0f)},
    };
    const vector<GLuint> LightViewer::LightModelIndices = {
        0, 2,
        3, 1,
        0, 1,
        3, 2,
        2, 4,
        5, 3,
        5, 4,
        4, 6,
        7, 5,
        7, 6,
        6, 8,
        9, 7,
        9, 8,
        8, 10,
        11, 9,
        11, 10,
        10, 12,
        13, 11,
        13, 12,
        12, 14,
        15, 13,
        15, 14,
        14, 16,
        17, 15,
        17, 16,
        16, 18,
        19, 17,
        19, 18,
        18, 20,
        21, 19,
        21, 20,
        20, 22,
        23, 21,
        23, 22,
        22, 24,
        25, 23,
        25, 24,
        24, 26,
        27, 25,
        27, 26,
        26, 28,
        29, 27,
        29, 28,
        28, 30,
        31, 29,
        31, 30,
        30, 0,
        1, 31,
        32, 33,
        34, 35,
        32, 35,
        34, 33,
        36, 37,
        37, 38,
        39, 36,
        39, 38,
        40, 41,
        41, 42,
        43, 40,
        43, 42,
        44, 45,
        45, 46,
        47, 44,
        47, 46,
        48, 49,
        49, 50,
        51, 48,
        51, 50,
        52, 53,
        53, 54,
        55, 52,
        55, 54,
        56, 57,
        57, 58,
        59, 56,
        59, 58,
        60, 61,
        61, 62,
        63, 60,
        63, 62,
        8, 41,
        42, 10,
        0, 32,
        33, 2,
        18, 50,
        51, 19,
        11, 43,
        40, 9,
        3, 34,
        35, 1,
        29, 60,
        61, 28,
        62, 30,
        21, 52,
        53, 20,
        31, 63,
        54, 22,
        13, 44,
        45, 12,
        23, 55,
        46, 14,
        5, 36,
        37, 4,
        15, 47,
        38, 6,
        7, 39,
        25, 56,
        57, 24,
        58, 26,
        17, 48,
        49, 16,
        27, 59,
    };
}


