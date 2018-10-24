#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <QOpenGLWidget>

namespace Dream
{
    struct Vertex;
}

using std::vector;
using glm::mat4;
using glm::vec3;
using glm::vec2;
using Dream::Vertex;

class MaterialPreviewGL : public QOpenGLWidget
{
public:
    MaterialPreviewGL(QWidget* parent = nullptr);
    ~MaterialPreviewGL() override;

protected:
    void paintGL() override;
    void createGLBuffers();
private:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mIBO;
    GLuint mShaderProgram;
    //mat4 mViewMatrix;
    //mat4 mModelMatrix;
    //mat4 mProjectionMatrix;
    //vec3 mViewerPos;
    const static QColor ClearColour;
    const static vector<Vertex> CubeVertices;
    const static vector<GLuint> CubeIndices;
};
