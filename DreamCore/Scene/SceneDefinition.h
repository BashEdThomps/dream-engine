/*
 * SceneDefinition.h
 *
 * Created: 16 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

#include <vector>
#include <memory>
#include "../Common/IDefinition.h"
#include "../Components/Transform.h"

using std::vector;
using std::unique_ptr;

namespace Dream
{
    class ProjectDefinition;
    class SceneObject;
    class SceneObjectDefinition;

    class SceneDefinition : public IDefinition
    {

    private:
        SceneObjectDefinition* mRootSceneObjectDefinition;
        ProjectDefinition* mProjectDefinition;

    public:
        SceneDefinition(ProjectDefinition* project, json data);
        ~SceneDefinition() override;

        void showStatus() override;

        void loadRootSceneObjectDefinition();

        void setCameraMovementSpeed(float speed);
        float getCameraMovementSpeed();

        void setPhysicsDebug(bool debug);
        bool getPhysicsDebug();

        void setMeshCullDistance(float mcd);
        float getMeshCullDistance();

        void setMinDrawDistance(float mdd);
        float getMinDrawDistance();

        void setMaxDrawDistance(float mdd);
        float getMaxDrawDistance();


        string getNotes();
        void setNotes(string notes);

        vec3 getCameraTranslation();
        void setCameraTranslation(vec3 trans);

        vec3 getCameraLookAt();
        void setCameraLookAt(vec3 lookAt);

        vector<float> getGravity();
        void setGravity(vector<float> gravity);
        void setGravityX(float);
        void setGravityY(float);
        void setGravityZ(float);

        vector<float> getClearColour();
        void setClearColour(vector<float> clearColour);
        void setClearColourR(float);
        void setClearColourG(float);
        void setClearColourB(float);

        vector<float> getAmbientColour();
        void setAmbientColour(vector<float> ambientColour);
        void setAmbientColourR(float);
        void setAmbientColourG(float);
        void setAmbientColourB(float);
        void setAmbientColourA(float);

        SceneObjectDefinition* getRootSceneObjectDefinition();
        ProjectDefinition* getProjectDefinition();
        SceneObjectDefinition* createNewRootSceneObjectDefinition();

        json getJson() override;

        float getCameraLookAtX();
        float getCameraLookAtY();
        float getCameraLookAtZ();

        void setCameraLookAtX(float val);
        void setCameraLookAtY(float val);
        void setCameraLookAtZ(float val);

        float getCameraTranslationX();
        float getCameraTranslationY();
        float getCameraTranslationZ();

        void setCameraTranslationX(float val);
        void setCameraTranslationY(float val);
        void setCameraTranslationZ(float val);

        void setCameraPitch(float pitch);
        void setCameraYaw(float yaw);
        float getCameraPitch();
        float getCameraYaw();

        string getLightingPassShader();
        void setLightingPassShader(string shader);
        int getCurrentLightingPassShaderIndex();
        int getCurrentShadowPassShaderIndex();
        string getShadowPassShader();
        void setShadowPassShader(string shader);
    };
}
