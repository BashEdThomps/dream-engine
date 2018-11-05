//
//  GLWidget.cpp
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#include "GLWidget.h"

namespace DreamTool
{
    GLWidget::GLWidget
    (Project* project)
        : DTWidget (project),
          mVao(0),
          mVbo(0)
    {
        setLogClassName("GLWidget");
        initVaoVbo();
    }

    GLWidget::~GLWidget()
    {
        auto log=getLog();
        log->debug("Destroying");
        if (mVao > 0)
        {
            glDeleteVertexArrays(1,&mVao);
        }
        if (mVbo > 0)
        {
            glDeleteBuffers(1,&mVbo);
        }
    }

    void
    GLWidget::initVaoVbo
    ()
    {
        glGenVertexArrays(1,&mVao);
        glGenBuffers(1,&mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void
    GLWidget::setViewMatrix
    (mat4 view)
    {
        mViewMatrix = view;
    }


    void
    GLWidget::setProjectionMatrix
    (mat4 projection)
    {
        mProjectionMatrix = projection;
    }
}
