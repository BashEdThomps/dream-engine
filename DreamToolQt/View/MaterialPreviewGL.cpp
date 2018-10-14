#include "MaterialPreviewGL.h"

MaterialPreviewGL::MaterialPreviewGL(QWidget* parent)
    : QOpenGLWidget (parent),
      mVAO(0),
      mVBO(0),
      mShaderProgram(0)
{
    createGLBuffers();
}

MaterialPreviewGL::~MaterialPreviewGL
()
{
    if (mVAO > 0)
    {
        glDeleteVertexArrays(1,&mVAO);
    }
    if (mVBO > 0)
    {
        glDeleteBuffers(1,&mVBO);
    }

    if (mShaderProgram > 0)
    {
        glDeleteProgram(mShaderProgram);
    }
}

void
MaterialPreviewGL::paintGL
()
{
    double r,g,b;
    ClearColour.getRgbF(&r,&g,&b);
    glClearColor(
        static_cast<float>(r),
        static_cast<float>(g),
        static_cast<float>(b),
        1.0f
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}



void
MaterialPreviewGL::createGLBuffers
()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mIBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLint>(CubeVertices.size() * sizeof(Vertex)),
        &CubeVertices[0],
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLint>(CubeIndices.size() * sizeof(GLuint)),
        &CubeIndices[0],
        GL_STATIC_DRAW
    );

    // Buffer Layout

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0, 3, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),
                static_cast<GLvoid*>(nullptr)
                );
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                1, 3, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),
                (GLvoid*)offsetof(Vertex, Normal)
                );
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
                2, 2, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),
                (GLvoid*)offsetof(Vertex, TexCoords)
                );
    // Vertex Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
                3, 3, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),
                (GLvoid*)offsetof(Vertex, Tangent)
                );
    // Vertex Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
                4, 3, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),
                (GLvoid*)offsetof(Vertex, Bitangent)
                );
    glBindVertexArray(0);

}

const QColor MaterialPreviewGL::ClearColour = QColor(35,38,41);

const vector<Vertex>  MaterialPreviewGL::CubeVertices =
{
    // Top
    Vertex{
        vec3(-1.0f,  1.0f,  1.0f),  // Position
        vec3(0.0f),                 // Normal
        vec2(0.0f, 0.0f),           // Texture Coordinates
        vec3(0.0f),                 // Tangent
        vec3(0.0f),                 // Bitangent
    }, // Front Left

    Vertex{
        vec3( 1.0f,  1.0f,  1.0f),
        vec3(0.0f),
        vec2(0.0f, 1.0f),
        vec3(0.0f),
        vec3(0.0f),
    }, // Front Right

    Vertex{
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(0.0f),
        vec2(1.0f, 0.0f),
        vec3(0.0f),
        vec3(0.0f),
    }, // Rear Left

    Vertex{
        vec3( 1.0f,  1.0f, -1.0f),
        vec3(0.0f),
        vec2(1.0f, 1.0f),
        vec3(0.0f),
        vec3(0.0f),
    }, // Rear Right

    // Bottom

    Vertex{
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(0.0f),
        vec2( 0.0f, 0.0f),
        vec3(0.0f),
        vec3(0.0f),
    }, // Front Left

    Vertex{
        vec3( 1.0f, -1.0f,  1.0f),
        vec3(0.0f),
        vec2( 0.0f, 1.0f),
        vec3(0.0f),
        vec3(0.0f),
    }, // Front Right

    Vertex{
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(0.0f),
        vec2(1.0f, 0.0f),
        vec3(0.0f),
        vec3(0.0f),
    }, // Rear Left

    Vertex{
        vec3( 1.0f, -1.0f, -1.0f),
        vec3(0.0f),
        vec2(1.0f, 1.0f),
        vec3(0.0f),
        vec3(0.0f),
    }  // Rear Right
};

const vector<GLuint>  MaterialPreviewGL::CubeIndices =
{
   // Top
   0, 2, 3,
   1, 0, 3,
   // Bottom
   4, 7, 6,
   4, 5, 7,
   // Left
   6, 2, 0,
   2, 0, 4,
   // Right
   1, 3, 7,
   1, 7, 5,
   // Front
   4, 0, 1,
   1, 5, 4,
   // Rear
   5, 3, 2,
   5, 2, 4
};
