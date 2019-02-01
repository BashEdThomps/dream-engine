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
#pragma once

#include "GLWidget.h"
#include <glm/vec3.hpp>
#include "../../../DreamCore/Components/Logic/Animation/AnimationDefinition.h"

using glm::vec3;
using Dream::AnimationDefinition;

namespace DreamTool
{
    class DTState;
    class AnimationViewer : public GLWidget
    {
    public:
        AnimationViewer(DTState* state, bool visible = false);
        ~AnimationViewer() override;

        void setAnimationDefinition(AnimationDefinition* object);
        void init();
        void updateVertexBuffer();
        void regenerate();
        void generateLines();
        void generateNode(const AnimationKeyframe& kf);

    private:
        AnimationDefinition* mAnimationDefinition;
        vec3 mSelectedColour;
        vec3 mUnselectedColour;
        vec3 mLineColour;
        int mSelectedKeyFrame;
        float mNodeSize;
    };
}
