#pragma ocne

#include <QOpenGLWidget>
#include <vector>

using std::vector;

struct MaterialPreviewVertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat tx_x;
    GLfloat tx_y;
};

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
    const static QColor ClearColour;
    const static vector<MaterialPreviewVertex> CubeVertices;
    const static vector<GLuint> CubeIndices;
};
