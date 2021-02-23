/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "AnimationViewer.h"
#include <DreamCore.h>

using octronic::dream::Constants;
using glm::quat;
using glm::mat4;
using glm::vec3;

namespace octronic::dream::tool
{
    AnimationViewer::AnimationViewer
    (DreamToolContext* state, bool visible)
        : GLWidget(state, visible),
          mSelectedColour(vec3(0.0f, 1.0f, 0.0f)),
          mUnselectedColour(vec3(0.75f, 0.75f, 0.0f)),
          mLineColour(vec3(1.0f, 0.0f, 1.0f)),
          mSelectedKeyFrame(-1),
          mNodeSize(0.25f)
    {
        LOG_TRACE("AnimationViewer: Constructing Object");
    }

    AnimationViewer::~AnimationViewer
    ()
    {
        LOG_DEBUG("AnimationViewer: Destructing Object");
    }

    void
    AnimationViewer::init
    ()
    {
        LOG_DEBUG("AnimationViewer: Initialising");
        GLWidget::init();
    }

    void
    AnimationViewer::setAnimationDefinition
    (AnimationDefinition* selected)
    {
        bool regen = (selected != mAnimationDefinition);
        mAnimationDefinition = selected;
        if (regen){ regenerate(); }
    }

    void
    AnimationViewer::regenerate
    ()
    {
        if (mAnimationDefinition == nullptr)
        {
            LOG_DEBUG("AnimationViewer: No object selected");
            return;
        }

        auto keyFrames = mAnimationDefinition->getKeyframes();

        mVertexBuffer.clear();

        for (auto keyFrame : keyFrames)
        {
            generateNode(keyFrame);
        }

        generateLines();

        updateVertexBuffer();
    }

    void
    AnimationViewer::updateVertexBuffer
    ()
    {
        LOG_DEBUG("AnimationViewer: Updating Vertex Buffer") ;

        // Vertex Array
        glBindVertexArray(mVao);
        ShaderRuntime::CurrentVAO = mVao;
        GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        ShaderRuntime::CurrentVBO = mVbo;
        GLCheckError();
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(GLWidgetVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
        GLCheckError();
        glBindVertexArray(0);
    }

    void
    AnimationViewer::generateLines
    ()
    {
        auto keyframes = mAnimationDefinition->getKeyframes();

       for (size_t i=1; i < keyframes.size(); i++)
       {
           Transform current  = keyframes.at(i-1).getTransform();
           vec3 currentTx = current.getTranslation();

           Transform next = keyframes.at(i).getTransform();
           vec3 nextTx = next.getTranslation();

           GLWidgetVertex v1,v2;
           v1.Position  = currentTx;
           v1.Color = mLineColour;
           v2.Position = nextTx;
           v2.Color = mLineColour;
           mVertexBuffer.push_back(v1);
           mVertexBuffer.push_back(v2);
       }
    }

    void
    AnimationViewer::generateNode
    (const  AnimationKeyframe& kf)
    {
        vec3 pos = kf.getTransform().getTranslation();
        int index = kf.getID();

        LOG_TRACE("AnimationViewer: Generating node cube for {} at ({},{},{})",index,pos.x,pos.y,pos.z);

        vec3 colour = (mSelectedKeyFrame == index ? mSelectedColour : mUnselectedColour);
        LOG_TRACE("AnimationViewer: Selected? {}",mSelectedKeyFrame == index);

        // Top Quad

        GLWidgetVertex topFront1, topFront2;
        topFront1.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topFront1.Color = colour;
        mVertexBuffer.push_back(topFront1);
        topFront2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topFront2.Color = colour;
        mVertexBuffer.push_back(topFront2);

        GLWidgetVertex topBack1, topBack2;
        topBack1.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topBack1.Color = colour;
        mVertexBuffer.push_back(topBack1);
        topBack2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topBack2.Color = colour;
        mVertexBuffer.push_back(topBack2);

        GLWidgetVertex topLeft1, topLeft2;
        topLeft1.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topLeft1.Color = colour;
        mVertexBuffer.push_back(topLeft1);
        topLeft2.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topLeft2.Color = colour;
        mVertexBuffer.push_back(topLeft2);

        GLWidgetVertex topRight1, topRight2;
        topRight1.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topRight1.Color = colour;
        mVertexBuffer.push_back(topRight1);
        topRight2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topRight2.Color = colour;
        mVertexBuffer.push_back(topRight2);

        // Bottom Quad

        GLWidgetVertex bottomFront1, bottomFront2;
        bottomFront1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomFront1.Color = colour;
        mVertexBuffer.push_back(bottomFront1);
        bottomFront2.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomFront2.Color = colour;
        mVertexBuffer.push_back(bottomFront2);

        GLWidgetVertex bottomBack1, bottomBack2;
        bottomBack1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomBack1.Color = colour;
        mVertexBuffer.push_back(bottomBack1);
        bottomBack2.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomBack2.Color = colour;
        mVertexBuffer.push_back(bottomBack2);

        GLWidgetVertex bottomLeft1, bottomLeft2;
        bottomLeft1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomLeft1.Color = colour;
        mVertexBuffer.push_back(bottomLeft1);
        bottomLeft2.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomLeft2.Color = colour;
        mVertexBuffer.push_back(bottomLeft2);

        GLWidgetVertex bottomRight1, bottomRight2;
        bottomRight1.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomRight1.Color = colour;
        mVertexBuffer.push_back(bottomRight1);
        bottomRight2.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomRight2.Color = colour;
        mVertexBuffer.push_back(bottomRight2);

        // Verticals

        GLWidgetVertex frontLeft1, frontLeft2;
        frontLeft1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        frontLeft1.Color = colour;
        mVertexBuffer.push_back(frontLeft1);
        frontLeft2.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        frontLeft2.Color = colour;
        mVertexBuffer.push_back(frontLeft2);

        GLWidgetVertex frontRight1, frontRight2;
        frontRight1.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        frontRight1.Color = colour;
        mVertexBuffer.push_back(frontRight1);
        frontRight2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        frontRight2.Color = colour;
        mVertexBuffer.push_back(frontRight2);

        GLWidgetVertex backLeft1, backLeft2;
        backLeft1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        backLeft1.Color = colour;
        mVertexBuffer.push_back(backLeft1);
        backLeft2.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        backLeft2.Color = colour;
        mVertexBuffer.push_back(backLeft2);

        GLWidgetVertex backRight1, backRight2;
        backRight1.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        backRight1.Color = colour;
        mVertexBuffer.push_back(backRight1);
        backRight2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        backRight2.Color = colour;
        mVertexBuffer.push_back(backRight2);
    }
}
