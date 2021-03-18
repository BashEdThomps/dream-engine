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
          mSelectedColour(0.0f, 1.0f, 0.0f,1.f),
          mUnselectedColour(0.75f, 0.75f, 0.0f,1.f),
          mLineColour(1.0f, 0.0f, 1.0f,1.f),
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
    (const shared_ptr<AnimationDefinition>& selected)
    {
        bool regen = (selected != mAnimationDefinition);
        mAnimationDefinition = selected;
        if (regen)
        {
            regenerate();
        }
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

        clearLineVertexBuffer();

        for (auto keyFrame : keyFrames)
        {
            generateNode(keyFrame);
        }

        generateLines();
        submitLineVertexBuffer();
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

           TranslationColorVertex v1,v2;
           v1.Translation  = currentTx;
           v1.Color = mLineColour;
           v2.Translation = nextTx;
           v2.Color = mLineColour;
           addLineVertex(v1);
           addLineVertex(v2);
       }
    }

    void
    AnimationViewer::generateNode
    (const  AnimationKeyframe& kf)
    {
        vec3 pos = kf.getTransform().getTranslation();
        int index = kf.getID();

        LOG_TRACE("AnimationViewer: Generating node cube for {} at ({},{},{})",index,pos.x,pos.y,pos.z);

        vec4 colour = (mSelectedKeyFrame == index ? mSelectedColour : mUnselectedColour);
        LOG_TRACE("AnimationViewer: Selected? {}",mSelectedKeyFrame == index);

        // Top Quad

        TranslationColorVertex topFront1, topFront2;
        topFront1.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topFront1.Color = colour;
        addLineVertex(topFront1);
        topFront2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topFront2.Color = colour;
        addLineVertex(topFront2);

        TranslationColorVertex topBack1, topBack2;
        topBack1.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topBack1.Color = colour;
        addLineVertex(topBack1);
        topBack2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topBack2.Color = colour;
        addLineVertex(topBack2);

        TranslationColorVertex topLeft1, topLeft2;
        topLeft1.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topLeft1.Color = colour;
        addLineVertex(topLeft1);
        topLeft2.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topLeft2.Color = colour;
        addLineVertex(topLeft2);

        TranslationColorVertex topRight1, topRight2;
        topRight1.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topRight1.Color = colour;
        addLineVertex(topRight1);
        topRight2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topRight2.Color = colour;
        addLineVertex(topRight2);

        // Bottom Quad

        TranslationColorVertex bottomFront1, bottomFront2;
        bottomFront1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomFront1.Color = colour;
        addLineVertex(bottomFront1);
        bottomFront2.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomFront2.Color = colour;
        addLineVertex(bottomFront2);

        TranslationColorVertex bottomBack1, bottomBack2;
        bottomBack1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomBack1.Color = colour;
        addLineVertex(bottomBack1);
        bottomBack2.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomBack2.Color = colour;
        addLineVertex(bottomBack2);

        TranslationColorVertex bottomLeft1, bottomLeft2;
        bottomLeft1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomLeft1.Color = colour;
        addLineVertex(bottomLeft1);
        bottomLeft2.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomLeft2.Color = colour;
        addLineVertex(bottomLeft2);

        TranslationColorVertex bottomRight1, bottomRight2;
        bottomRight1.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomRight1.Color = colour;
        addLineVertex(bottomRight1);
        bottomRight2.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomRight2.Color = colour;
        addLineVertex(bottomRight2);

        // Verticals

        TranslationColorVertex frontLeft1, frontLeft2;
        frontLeft1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        frontLeft1.Color = colour;
        addLineVertex(frontLeft1);
        frontLeft2.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        frontLeft2.Color = colour;
        addLineVertex(frontLeft2);

        TranslationColorVertex frontRight1, frontRight2;
        frontRight1.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        frontRight1.Color = colour;
        addLineVertex(frontRight1);
        frontRight2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        frontRight2.Color = colour;
        addLineVertex(frontRight2);

        TranslationColorVertex backLeft1, backLeft2;
        backLeft1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        backLeft1.Color = colour;
        addLineVertex(backLeft1);
        backLeft2.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        backLeft2.Color = colour;
        addLineVertex(backLeft2);

        TranslationColorVertex backRight1, backRight2;
        backRight1.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        backRight1.Color = colour;
        addLineVertex(backRight1);
        backRight2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        backRight2.Color = colour;
        addLineVertex(backRight2);
    }
}
