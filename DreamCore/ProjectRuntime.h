/*
 * ProjectRuntime.h
 *
 * Created: 05 2017 by Ashley
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
#ifndef PROJECTRUNTIME_H
#define PROJECTRUNTIME_H

#include "Time.h"
#include "GameController.h"
#include "Components/Animation/AnimationComponent.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Graphics/Camera.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Window/IWindowComponent.h"
#include "Lua/LuaEngine.h"

namespace Dream
{
    class Project;
    class ProjectRuntime
    {
    private:

        bool mDone;
        Time *mTime;
        Camera *mCamera;
        AudioComponent *mAudioComponent;
        GraphicsComponent *mGraphicsComponent;
        PhysicsComponent *mPhysicsComponent;
        AnimationComponent *mAnimationComponent;
        IWindowComponent *mWindowComponent;
        GameController *mGameController;
        LuaEngine *mLuaEngine;
        Project* mProject;

    public:

        ProjectRuntime(Project*, IWindowComponent*);
        ~ProjectRuntime();

        void setProject(Project*);

        void setDone(bool);
        bool isDone();

        Camera* getCamera();
        void setCamera(Camera*);

        void setTime(Time*);
        Time* getTime();

        AnimationComponent* getAnimationComponent();
        AudioComponent* getAudioComponent();
        PhysicsComponent* getPhysicsComponent();
        GraphicsComponent* getGraphicsComponent();
        IWindowComponent* getWindowComponent();
        GameController* getGameController();
        LuaEngine* getLuaEngine();

        void setGraphicsClearColour(vector<float>);
        void setGraphicsAmbientLightColour(vector<float>);
        void setPhysicsGravity(vector<float>);
        void setPhysicsDebug(bool);
        void setCameraTranslation(glm::vec3);
        void setCameraRotation(glm::vec3);
        void setCameraMovementSpeed(float);

        bool initComponents();
        void cleanupComponents(Scene*);
    private:


        bool initAnimationComponent();
        void setAnimationComponent(AnimationComponent*);

        bool initAudioComponent();
        void setAudioComponent(AudioComponent*);

        bool initPhysicsComponent();
        void setPhysicsComponent(PhysicsComponent*);

        bool initGraphicsComponent();
        void setGraphicsComponent(GraphicsComponent*);

        bool initWindowComponent();
        void setWindowComponent(IWindowComponent*);

        bool initLuaEngine();
        void setLuaEngine(LuaEngine*);

        void setGameController(GameController*);

        void destroyComponents();
    };

} // End Dream

#endif // PROJECTRUNTIME_H
