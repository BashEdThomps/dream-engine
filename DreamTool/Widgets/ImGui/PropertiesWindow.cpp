// Maintain include order for GL Defines
#include "PropertiesWindow.h"

#include <glm/gtc/type_ptr.hpp>

#include "../../deps/ImGui/imguifilesystem.h"
#include "../../deps/ImGui/ImGuizmo.h"
#include "../../DTState.h"


#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Project/ProjectDefinition.h"
#include "../../../DreamCore/Project/ProjectDirectory.h"

#include "../../../DreamCore/Scene/SceneDefinition.h"
#include "../../../DreamCore/Scene/SceneRuntime.h"

#include "../../../DreamCore/Scene/SceneObject/SceneObjectDefinition.h"
#include "../../../DreamCore/Scene/SceneObject/SceneObjectRuntime.h"

#include "../../../DreamCore/Components/Animation/AnimationDefinition.h"
#include "../../../DreamCore/Components/Animation/AnimationKeyframe.h"
#include "../../../DreamCore/Components/Animation/AnimationRuntime.h"

#include "../../../DreamCore/Components/Audio/AudioDefinition.h"
#include "../../../DreamCore/Components/Graphics/GraphicsComponent.h"
#include "../../../DreamCore/Components/Graphics/Font/FontDefinition.h"
#include "../../../DreamCore/Components/Graphics/Shader/ShaderDefinition.h"
#include "../../../DreamCore/Components/Graphics/Shader/ShaderRuntime.h"
#include "../../../DreamCore/Components/Graphics/Shader/ShaderCache.h"
#include "../../../DreamCore/Components/Graphics/Model/ModelDefinition.h"
#include "../../../DreamCore/Components/Graphics/Model/ModelRuntime.h"
#include "../../../DreamCore/Components/Graphics/Model/ModelCache.h"
#include "../../../DreamCore/Components/Graphics/Light/LightDefinition.h"
#include "../../../DreamCore/Components/Graphics/Light/LightRuntime.h"
#include "../../../DreamCore/Components/Graphics/Material/MaterialDefinition.h"
#include "../../../DreamCore/Components/Graphics/Material/MaterialRuntime.h"
#include "../../../DreamCore/Components/Graphics/Texture/TextureRuntime.h"
#include "../../../DreamCore/Components/Graphics/Texture/TextureDefinition.h"
#include "../../../DreamCore/Components/Graphics/Texture/TextureCache.h"
#include "../../../DreamCore/Components/Physics/PhysicsObjectDefinition.h"
#include "../../../DreamCore/Components/Scripting/ScriptDefinition.h"
#include "../../../DreamCore/Components/Scripting/ScriptRuntime.h"

namespace DreamTool
{
    PropertiesWindow::PropertiesWindow
    (DTState* proj)
        : ImGuiWidget (proj),
          mType(None),
          mDefinition(nullptr),
          mRuntime(nullptr),
          mImageSize(256,256),
          mBigEditorSize(-1,-1),
          mGizmoUseSnap(true),
          mGizmoSnap(1.0f,1.0f,1.0f)
    {
        setLogClassName("Properties Window");
    }

    PropertiesWindow::~PropertiesWindow
    ()
    {

    }

    void
    PropertiesWindow::draw
    ()
    {
        if (mState->project)
        {
            ImGui::Begin("Properties",&mVisible);
            if(ImGui::Button("<- Back"))
            {
                popPropertyTarget();
            }
            switch (mType)
            {
                case PropertyType::None:
                    break;
                case PropertyType::Project:
                    drawProjectProperties();
                    break;
                case PropertyType::Scene:
                    drawSceneProperties();
                    break;
                case PropertyType::SceneObject:
                    drawSceneObjectProperties();
                    break;
                case PropertyType::Asset:
                    drawAssetProperties();
                    break;
            }
            ImGui::End();
        }
    }

    bool
    PropertiesWindow::drawDeleteSceneObjectButton
    ()
    {
        bool retval = false;
        auto soDef = dynamic_cast<SceneObjectDefinition*>(mDefinition);
        auto soRuntime = dynamic_cast<SceneObjectRuntime*>(mRuntime);

        if (ImGui::Button("Delete"))
        {
            ImGui::OpenPopup("Confirm Delete SceneObject");
        }

        if(ImGui::BeginPopupModal("Confirm Delete SceneObject"))
        {
            ImGui::Text("\n"
                        "Are you sure you want to delete this SceneObject?\n"
                        "\n");
            ImGui::Separator();
            if (ImGui::Button("Cancel",ImVec2(0,0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete",ImVec2(0,0)))
            {
                if (soDef)
                {
                    auto parent = soDef->getParentSceneObject();
                    if (parent)
                    {
                        parent->removeChildDefinition(soDef);
                    }
                }
                if (soRuntime)
                {
                    auto parent = soRuntime->getParentRuntime();
                    parent->removeChildRuntime(soRuntime);
                }
                mState->selectionHighlighter.clearSelection();
                removeFromHistory(mDefinition);
                mDefinition = nullptr;
                mRuntime = nullptr;
                retval = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        return retval;
    }

    bool
    PropertiesWindow::drawDeleteSceneButton
    ()
    {
        bool retval = false;
        if(ImGui::Button("Delete"))
        {
            ImGui::OpenPopup("Confirm Delete Scene");
        }

        auto sDef = dynamic_cast<SceneDefinition*>(mDefinition);
        auto pDef = mState->project->getDefinition();

        if(ImGui::BeginPopupModal("Confirm Delete Scene"))
        {
            ImGui::Text("\n"
                        "Are you sure you want to delete this Scene?\n"
                        "\n");
            ImGui::Separator();
            if (ImGui::Button("Cancel",ImVec2(0,0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete",ImVec2(0,0)))
            {
                // TODO Check is not active
                if (pDef && sDef)
                {
                    pDef->removeSceneDefinition(sDef);
                    removeFromHistory(mDefinition);
                    mDefinition = nullptr;
                    mRuntime = nullptr;
                    retval = true;
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        return retval;
    }

    void
    PropertiesWindow::pushPropertyTarget
    (PropertyType type, Definition* def, Runtime* runt)
    {
        // Remove old
        auto itr = std::find_if(
                    mHistory.begin(),
                    mHistory.end(),
                    [&](PropertiesTarget& tgt)
        {
                return tgt.definition == def;
    }
                );

        if (itr != mHistory.end())
        {
            mHistory.erase(itr);
        }

        mHistory.push_back(PropertiesTarget{mType,mDefinition,mRuntime});
        auto log = getLog();
        log->error("Pushed target {}",mHistory.size());
        setPropertyType(type);
        setDefinition(def);
        setRuntime(runt);
    }

    void
    PropertiesWindow::removeFromHistory
    (Definition* def)
    {
        auto itr = mHistory.end();
        for (itr = mHistory.begin(); itr != mHistory.end(); itr++)
        {
            if ((*itr).definition == def)
            {
                break;
            }
        }
        if (itr != mHistory.end())
        {
            mHistory.erase(itr);
        }
    }

    void
    PropertiesWindow::popPropertyTarget
    ()
    {
        if (mHistory.empty()) return;

        PropertiesTarget last = mHistory.back();
        setPropertyType(last.type);
        setDefinition(last.definition);
        setRuntime(last.runtime);
        auto log = getLog();
        mHistory.pop_back();
        log->error("Popped target {}",mHistory.size());
    }

    void
    PropertiesWindow::clearPropertyTargets
    ()
    {
        mHistory.clear();
        setPropertyType(PropertyType::None);
        setDefinition(nullptr);
        setRuntime(nullptr);
    }

    void
    PropertiesWindow::drawNameAndIdProperties
    ()
    {
        if (mDefinition != nullptr)
        {
            char nameBuf[128] = {0};
            strncpy(&nameBuf[0],mDefinition->getName().c_str(),mDefinition->getName().size());
            if(ImGui::InputText("Name", nameBuf, IM_ARRAYSIZE(nameBuf)))
            {
                mDefinition->setName(nameBuf);
            }

            char idBuf[128] = {0};
            strncpy(&idBuf[0],mDefinition->getUuid().c_str(),mDefinition->getUuid().size());
            if(ImGui::InputText("ID", idBuf, IM_ARRAYSIZE(idBuf)))
            {
                mDefinition->setUuid(idBuf);
            }
        }
    }

    void
    PropertiesWindow::setDefinition
    (Definition* def)
    {
        mDefinition = def;
    }

    void
    PropertiesWindow::setRuntime
    (Runtime* runt)
    {
        mRuntime = runt;
    }

    void
    PropertiesWindow::clear
    ()
    {
        mType = None;
        mDefinition = nullptr;
        mRuntime = nullptr;
    }

    void
    PropertiesWindow::setPropertyType
    (PropertyType t)
    {
        mType = t;
    }

    void
    PropertiesWindow::drawProjectProperties
    ()
    {
        auto projDef = dynamic_cast<ProjectDefinition*>(mDefinition);

        if (projDef == nullptr)
        {
            return;
        }

        ImGui::SameLine();
        if(ImGui::Button("New Scene"))
        {
            pushPropertyTarget
                    (
                        DreamTool::Scene,
                        mState->project->getDefinition()->createNewSceneDefinition(),
                        nullptr
                        );
            return;
        }
        drawNameAndIdProperties();

        ImGui::Separator();
        // Startup Scene
        auto startup = projDef->getStartupSceneDefinition();
        int startupScene = projDef->getSceneDefinitionIndex(startup);
        vector<string> scenes;
        for(SceneDefinition* scene : projDef->getSceneDefinitionsVector())
        {
            scenes.push_back(scene->getName());
        }
        if(StringCombo("Startup Scene",&startupScene,scenes,scenes.size()))
        {
            startup = projDef->getSceneDefinitionAtIndex(startupScene);
            projDef->setStartupSceneUuid(startup->getUuid());
        }
        ImGui::Separator();

        // Capture mouse,keyboard,joystick
        bool usingKeyboard = projDef->getCaptureKeyboard();
        bool usingMouse = projDef->getCaptureMouse();
        bool usingGamepad = projDef->getCaptureJoystick();

        ImGui::Columns(3);
        if (ImGui::Checkbox("Keyboard",&usingKeyboard))
        {
            projDef->setCaptureKeyboard(usingKeyboard);
        }
        ImGui::NextColumn();
        if(ImGui::Checkbox("Mouse", &usingMouse))
        {
            projDef->setCaptureMouse(usingMouse);
        }
        ImGui::NextColumn();
        if(ImGui::Checkbox("Gamepad",  &usingGamepad))
        {
            projDef->setCaptureJoystick(usingGamepad);
        }
        ImGui::Columns(1);
        ImGui::Separator();

        // Window Dimensions
        int windowSize[2] = {
            projDef->getWindowWidth(),
            projDef->getWindowHeight()
        };
        if (ImGui::DragInt2("Window Size",windowSize))
        {
            projDef->setWindowWidth(windowSize[0]);
            projDef->setWindowHeight(windowSize[1]);
        }
        ImGui::Separator();

        // Author
        char authorBuffer[512] = {0};
        strncpy(authorBuffer, projDef->getAuthor().c_str(), projDef->getAuthor().length());
        if(ImGui::InputText("Author",authorBuffer,512))
        {
            projDef->setAuthor(authorBuffer);
        }

        // Description
        char descriptionBuffer[512] = {0};
        strncpy(descriptionBuffer,projDef->getDescription().c_str(),projDef->getDescription().length());
        if(ImGui::InputTextMultiline("Description",descriptionBuffer,512))
        {
            projDef->setDescription(descriptionBuffer);
        }

    }

    void
    PropertiesWindow::drawSceneProperties
    ()
    {
        auto log = getLog();

        auto sceneDef = dynamic_cast<SceneDefinition*>(mDefinition);
        auto sceneRuntime = dynamic_cast<SceneRuntime*>(mRuntime);

        if (sceneDef == nullptr)
        {
            return;
        }

        ImGui::SameLine();

        if (drawDeleteSceneButton())
        {
            return;
        }
        drawNameAndIdProperties();

        ImGui::Separator();

        // Camera
        float cameraTranslation[3];

        if (sceneRuntime)
        {
            auto cam = sceneRuntime->getCamera();
            auto tx = cam->getTranslation();
            cameraTranslation[0] = tx.x;
            cameraTranslation[1] = tx.y;
            cameraTranslation[2] = tx.z;
        }
        else
        {
            cameraTranslation[0] = sceneDef->getCameraTranslationX();
            cameraTranslation[1] = sceneDef->getCameraTranslationY();
            cameraTranslation[2] = sceneDef->getCameraTranslationZ();
        }

        if(ImGui::CollapsingHeader("Camera"))
        {

            if(ImGui::DragFloat3("Translation",cameraTranslation,0.1f))
            {
                if (sceneDef != nullptr)
                {
                    sceneDef->setCameraTranslationX(cameraTranslation[0]);
                    sceneDef->setCameraTranslationY(cameraTranslation[1]);
                    sceneDef->setCameraTranslationZ(cameraTranslation[2]);
                }

                if (sceneRuntime != nullptr)
                {
                    auto cam = sceneRuntime->getCamera();
                    cam->setTranslation
                    ({
                         cameraTranslation[0],
                         cameraTranslation[1],
                         cameraTranslation[2]
                    });
                }
            }

            float cameraRotation[3] = {0.0f};

            if (sceneRuntime)
            {
                auto cam = sceneRuntime->getCamera();
                cameraRotation[0] = degrees(cam->getPitch());
                cameraRotation[1] = degrees(cam->getYaw());
            }
            else
            {
                cameraRotation[0] = degrees(sceneDef->getCameraPitch());
                cameraRotation[1] = degrees(sceneDef->getCameraYaw());
            }

            if(ImGui::DragFloat3("Pitch Yaw Roll",cameraRotation,0.1f))
            {
                if (sceneDef != nullptr)
                {
                    sceneDef->setCameraPitch(radians(cameraRotation[0]));
                    sceneDef->setCameraYaw(radians(cameraRotation[1]));
                }

                if (sceneRuntime != nullptr)
                {
                    auto cam = sceneRuntime->getCamera();
                    cam->setPitch(radians(cameraRotation[0]));
                    cam->setYaw(radians(cameraRotation[1]));
                }

            }

            float camSpeed = sceneDef->getCameraMovementSpeed();
            if(ImGui::DragFloat("Camera Speed",&camSpeed))
            {
                if (sceneDef)
                {
                    sceneDef->setCameraMovementSpeed(camSpeed);
                }
                if (sceneRuntime)
                {
                    auto cam = sceneRuntime->getCamera();
                    cam->setMovementSpeed(camSpeed);
                }
            }

            if(ImGui::Button("Capture Camera Transform"))
            {
                if (sceneDef)
                {
                    sceneDef->setCameraTranslationX(cameraTranslation[0]);
                    sceneDef->setCameraTranslationY(cameraTranslation[1]);
                    sceneDef->setCameraTranslationZ(cameraTranslation[2]);

                    sceneDef->setCameraPitch(radians(cameraRotation[0]));
                    sceneDef->setCameraYaw(radians(cameraRotation[1]));

                    sceneDef->setCameraMovementSpeed(camSpeed);
                }
            }

            auto focused = sceneDef->getCameraFocusedOn();
            string focusedStr = "None";
            if (sceneRuntime)
            {
                auto focusedObj = sceneRuntime->getSceneObjectRuntimeByUuid(focused);
                if (focusedObj)
                {
                    focusedStr = focusedObj->getNameAndUuidString();
                }
            }
            if (ImGui::Button("Clear Focus"))
            {
                sceneDef->setCameraFocusedOn("");
                if (sceneRuntime)
                {
                    auto cam = sceneRuntime->getCamera();
                    if(cam)
                    {
                        cam->setFocusedSceneObejct(nullptr);
                    }
                }
            }
            ImGui::Text("Focused on: %s",focusedStr.c_str());

            string nearestStr = "None";
            SceneObjectRuntime* nearest = nullptr;
            if (sceneRuntime)
            {
                nearest = sceneRuntime->getNearestToCamera();
                if (nearest)
                {
                    nearestStr = nearest->getNameAndUuidString();
                }
            }
            ImGui::Text("Nearest Object to Camera: %s",nearestStr.c_str());

            float theta = 0.0f;
            if (sceneRuntime)
            {
                auto cam = sceneRuntime->getCamera();
                theta = glm::degrees(cam->getFocusedObjectTheta());
            }

            ImGui::Text("Theta: %.3f",theta);
        }

        // Rendering
        float meshCull = sceneDef->getMeshCullDistance();

        if (ImGui::CollapsingHeader("Rendering"))
        {
            if (ImGui::DragFloat("Mesh Cull",&meshCull))
            {
                if (sceneDef != nullptr)
                {
                    sceneDef->setMeshCullDistance(meshCull);
                }
                if (sceneRuntime != nullptr)
                {
                    sceneRuntime->setMeshCullDistance(meshCull);
                }
            }

            float drawDistance[2] = {
                sceneDef->getMinDrawDistance(),
                sceneDef->getMaxDrawDistance()
            };

            if (ImGui::DragFloat2("Draw Distance",drawDistance))
            {
                if (sceneDef != nullptr)
                {
                    sceneDef->setMinDrawDistance(drawDistance[0]);
                    sceneDef->setMaxDrawDistance(drawDistance[1]);
                }
                if (sceneRuntime != nullptr)
                {
                    sceneRuntime->setMinDrawDistance(drawDistance[0]);
                    sceneRuntime->setMaxDrawDistance(drawDistance[1]);

                }
            }

            auto clearVec = sceneDef->getClearColour();
            if(ImGui::ColorEdit3("Clear Color",&clearVec[0]))
            {
                if (sceneDef != nullptr)
                {
                    sceneDef->setClearColourR(clearVec[0]);
                    sceneDef->setClearColourG(clearVec[1]);
                    sceneDef->setClearColourB(clearVec[2]);
                }
                if (sceneRuntime)
                {
                    sceneRuntime->setClearColour({
                                                     clearVec[0],
                                                     clearVec[1],
                                                     clearVec[2]
                                                 });
                }
            }

            auto ambientVec = sceneDef->getAmbientColour();
            if (ImGui::ColorEdit3("Ambient Color", &ambientVec[0]))
            {
                if (sceneDef)
                {
                    sceneDef->setAmbientColourR(ambientVec[0]);
                    sceneDef->setAmbientColourG(ambientVec[1]);
                    sceneDef->setAmbientColourB(ambientVec[2]);
                }
                if (sceneRuntime)
                {
                    sceneRuntime->setAmbientColour({
                                                       ambientVec[0],
                                                       ambientVec[1],
                                                       ambientVec[2]
                                                   });
                }
            }

            auto pDef = sceneDef->getProjectDefinition();
            auto lpShaderUuid = sceneDef->getLightingPassShader();
            auto lpShaderDef = pDef->getAssetDefinitionByUuid(lpShaderUuid);
            int lightingShaderIndex = pDef->getAssetDefinitionIndex(AssetType::SHADER,lpShaderDef);

            auto spShaderUuid = sceneDef->getShadowPassShader();
            auto spShaderDef = pDef->getAssetDefinitionByUuid(spShaderUuid);
            int shadowShaderIndex = pDef->getAssetDefinitionIndex(AssetType::SHADER,spShaderDef);

            auto shaderList = mState->project->getDefinition()->getAssetNamesVector(AssetType::SHADER);

            if (StringCombo("Lighting Pass Shader", &lightingShaderIndex, shaderList, shaderList.size()))
            {
                auto selectedShader = mState->project->getDefinition()->getAssetDefinitionAtIndex(AssetType::SHADER, lightingShaderIndex);
                auto uuid = selectedShader->getUuid();
                auto name = selectedShader->getName();
                sceneDef->setLightingPassShader(uuid);
                log->error("Switched lighting pass shader to {} {}", name, uuid);
            }


            if (StringCombo("Shadow Pass Shader", &shadowShaderIndex, shaderList, shaderList.size()))
            {
                auto selectedShader = mState->project->getDefinition()->getAssetDefinitionAtIndex(AssetType::SHADER, shadowShaderIndex);
                auto uuid = selectedShader->getUuid();
                auto name = selectedShader->getName();
                sceneDef->setShadowPassShader(uuid);
                log->error("Switched shadow pass shader to {} {}", name, uuid);
            }
        }

        if (ImGui::CollapsingHeader("Scripting"))
        {
            auto pDef=mState->project->getDefinition();
            vector<string> scriptAssets = mState->project->getDefinition()->getAssetNamesVector(AssetType::SCRIPT);

            string inputScriptUuid = sceneDef->getInputScript();
            auto inputScriptDef = pDef->getAssetDefinitionByUuid(inputScriptUuid);
            int inputScriptIndex = pDef->getAssetDefinitionIndex(AssetType::SCRIPT,inputScriptDef);
            if (StringCombo("Input Script",&inputScriptIndex,scriptAssets,scriptAssets.size()))
            {
                auto selected = pDef->getAssetDefinitionAtIndex(AssetType::SCRIPT,inputScriptIndex);
                auto uuid = selected->getUuid();
                sceneDef->setInputScript(uuid);
            }

            string nvgScriptUuid = sceneDef->getNanoVGScript();
            auto nvgScriptDef = pDef->getAssetDefinitionByUuid(nvgScriptUuid);
            int nvgScriptIndex = pDef->getAssetDefinitionIndex(AssetType::SCRIPT,nvgScriptDef);
            if (StringCombo("NanoVG Script",&nvgScriptIndex,scriptAssets,scriptAssets.size()))
            {
                auto selected = pDef->getAssetDefinitionAtIndex(AssetType::SCRIPT,nvgScriptIndex);
                auto uuid = selected->getUuid();
                sceneDef->setNanoVGScript(uuid);
            }
        }

        // Physics
        if(ImGui::CollapsingHeader("Physics")){
            bool physicsDebug = sceneDef->getPhysicsDebug();

            if (ImGui::Checkbox("Debug", &physicsDebug))
            {
                if (sceneDef)
                {
                    sceneDef->setPhysicsDebug(physicsDebug);
                }
            }

            auto gravityVec = sceneDef->getGravity();
            if (ImGui::DragFloat3("Gravity", &gravityVec[0],0.1f))
            {
                if (sceneDef)
                {
                    sceneDef->setGravity(gravityVec);
                }
                if (sceneRuntime)
                {
                    sceneRuntime->setGravity(gravityVec);
                }
            }
        }

        if (ImGui::CollapsingHeader("Misc"))
        {
            // Notes
            char notesBuffer[512] = { 0 };
            strncpy(notesBuffer, sceneDef->getNotes().c_str(), sceneDef->getNotes().size());

            if (ImGui::InputTextMultiline("Notes", notesBuffer, 512))
            {
                if (sceneDef)
                {
                    sceneDef->setNotes(notesBuffer);
                }
            }
        }
    }

    void
    PropertiesWindow::drawSceneObjectProperties
    ()
    {
        auto projDef = mState->project->getDefinition();
        auto soDef = dynamic_cast<SceneObjectDefinition*>(mDefinition);
        auto soRuntime = dynamic_cast<SceneObjectRuntime*>(mRuntime);
        auto log = getLog();

        if (soDef == nullptr)
        {
            return;
        }

        if (soDef->getParentSceneObject() != nullptr)
        {

            ImGui::SameLine();
            if (drawDeleteSceneObjectButton())
            {
                return;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Child"))
        {
            auto newChildDef = soDef->createNewChildDefinition();
            mat4 cursorTx = glm::translate(mat4(1.0f),mState->cursor.getPosition());
            newChildDef->setTransform(Transform(cursorTx));
            SceneObjectRuntime* newRt = nullptr;
            if (soRuntime)
            {
                newRt = soRuntime->createAndAddChildRuntime(newChildDef);
                newRt->getTransform().setMatrix(cursorTx);
            }
            pushPropertyTarget(PropertyType::SceneObject,newChildDef,newRt);
        }

        if (soDef->getParentSceneObject() != nullptr)
        {
            ImGui::SameLine();
            if (ImGui::Button("Duplicate"))
            {
                auto dup = soDef->duplicate();
                SceneObjectRuntime* newRt = nullptr;
                if (soRuntime)
                {
                    newRt = soRuntime->createAndAddChildRuntime(dup);
                }
                pushPropertyTarget(PropertyType::SceneObject,dup,newRt);
            }
        }

        drawNameAndIdProperties();
        ImGui::Separator();

        ImGui::Columns(2);

        if (ImGui::Button("Set Camera Focus"))
        {
            if(soDef)
            {
                auto sDef = soDef->getSceneDefinition();
                if (sDef)
                {
                    sDef->setCameraFocusedOn(soDef->getUuid());
                }
            }
            if (soRuntime)
            {
                auto sRunt = soRuntime->getSceneRuntime();
                if (sRunt)
                {
                    auto cam = sRunt->getCamera();
                    cam->setFocusedSceneObejct(soRuntime);
                }
            }
        }
        ImGui::NextColumn();

        bool alwaysDraw = soDef->getAlwaysDraw();
        if (ImGui::Checkbox("Always Draw", &alwaysDraw))
        {
            if(soDef) soDef->setAlwaysDraw(alwaysDraw);
            if (soRuntime) soRuntime->setAlwaysDraw(alwaysDraw);
        }
        ImGui::NextColumn();

        bool hidden = soDef->getHidden();
        if(ImGui::Checkbox("Hidden", &hidden))
        {
            if(soDef) soDef->setHidden(hidden);
            if(soRuntime) soRuntime->setHidden(hidden);
        }
        ImGui::NextColumn();

        bool isTemplate = soDef->getIsTemplate();
        if(ImGui::Checkbox("Template", &isTemplate))
        {
            if(soDef) soDef->setIsTemplate(isTemplate);
        }

        ImGui::Columns(1);

        if (soRuntime && soRuntime->hasAnimationInstance())
        {
            auto animation = soRuntime->getAnimationInstance();
            if (ImGui::CollapsingHeader("Animation"))
            {
                if (ImGui::Button("Run"))
                {
                    animation->run();
                }
                ImGui::SameLine();
                if (ImGui::Button("Pause"))
                {
                    animation->pause();
                }
                ImGui::SameLine();
                if (ImGui::Button("Reset"))
                {
                    animation->reset();
                }

                ImGui::PushItemWidth(-1);
                int animProg = animation->getCurrentTime();
                int duration = animation->getDuration();
                if(ImGui::SliderInt("#AnimationProgress", &animProg,0,duration,"%dms"))
                {
                    animation->setCurrentTime(animProg);
                    animation->seekAll(animProg);
                }
                ImGui::PopItemWidth();

            }
        }

        drawImGizmo();

        if(ImGui::CollapsingHeader("Assets"))
        {

            // Audio
            int selectedAnimationAsset = soDef->getSelectedAssetIndex(AssetType::ANIMATION);
            vector<string> animationAssets = projDef->getAssetNamesVector(AssetType::ANIMATION);
            if(ImGui::Button("-##Animation"))
            {
                soDef->setAssetDefinition(AssetType::ANIMATION,"");
                if (soRuntime)
                {
                    soRuntime->removeAnimationInstance();
                }
            }
            ImGui::SameLine();
            if(ImGui::Button(">##Animation"))
            {
                if (selectedAnimationAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::ANIMATION,selectedAnimationAsset);
                pushPropertyTarget(
                            PropertyType::Asset,
                            asset,
                            nullptr
                            );
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Animation",&selectedAnimationAsset,animationAssets,animationAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::ANIMATION, selectedAnimationAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::ANIMATION, selectedAnimationAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::ANIMATION, selectedDef->getUuid());
                    }
                }
            }

            // Audio
            int selectedAudioAsset = soDef->getSelectedAssetIndex(AssetType::AUDIO);
            vector<string> audioAssets = projDef->getAssetNamesVector(AssetType::AUDIO);
            if(ImGui::Button("-##Audio"))
            {
                soDef->setAssetDefinition(AssetType::AUDIO,"");
                if (soRuntime)
                {
                    soRuntime->removeAudioInstance();
                }
            }
            ImGui::SameLine();
            if(ImGui::Button(">##Audio"))
            {
                if (selectedAudioAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::AUDIO,selectedAudioAsset);
                pushPropertyTarget(
                            PropertyType::Asset,
                            asset,
                            nullptr
                            );
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Audio",&selectedAudioAsset,audioAssets,audioAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::AUDIO, selectedAudioAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::AUDIO, selectedAudioAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::AUDIO, selectedDef->getUuid());
                    }
                }
            }

            // Light
            int selectedLightAsset = soDef->getSelectedAssetIndex(AssetType::LIGHT);
            vector<string> lightAssets = projDef->getAssetNamesVector(AssetType::LIGHT);

            if(ImGui::Button("-##Light"))
            {
                soDef->setAssetDefinition(AssetType::LIGHT,"");
                if (soRuntime)
                {
                    soRuntime->removeLightInstance();
                }

            }
            ImGui::SameLine();

            if(ImGui::Button(">##Light"))
            {
                if (selectedLightAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::LIGHT,selectedLightAsset);
                pushPropertyTarget(
                            PropertyType::Asset,
                            asset,
                            nullptr
                            );
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Light",&selectedLightAsset,lightAssets,lightAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::LIGHT, selectedLightAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::LIGHT, selectedLightAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::LIGHT, selectedDef->getUuid());
                    }
                }
            }

            // Model
            int selectedModelAsset = soDef->getSelectedAssetIndex(AssetType::MODEL);
            vector<string> modelAssets = projDef->getAssetNamesVector(AssetType::MODEL);

            if(ImGui::Button("-##Model"))
            {
                soDef->setAssetDefinition(AssetType::MODEL,"");
                if (soRuntime)
                {
                    soRuntime->removeModelInstance();
                }

            }
            ImGui::SameLine();

            if(ImGui::Button(">##Model"))
            {
                if (selectedModelAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::MODEL,selectedModelAsset);
                pushPropertyTarget
                        (
                            PropertyType::Asset,
                            asset,
                            nullptr
                            );
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Model",&selectedModelAsset,modelAssets,modelAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::MODEL, selectedModelAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::MODEL, selectedModelAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::MODEL, selectedDef->getUuid());
                    }
                }
            }

            // Particle Emitter
            int selectedParticleEmitterAsset = soDef->getSelectedAssetIndex(AssetType::PARTICLE_EMITTER);
            vector<string> peAssets = projDef->getAssetNamesVector(AssetType::PARTICLE_EMITTER);
            if(ImGui::Button("-##ParticleEmitter"))
            {
                soDef->setSelectedAssetIndex(AssetType::PARTICLE_EMITTER,selectedParticleEmitterAsset);
                if (soRuntime)
                {
                    soRuntime->removeParticleEmitterInstance();
                }
            }
            ImGui::SameLine();

            if(ImGui::Button(">##ParticleEmitter"))
            {
                if (selectedParticleEmitterAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::PARTICLE_EMITTER,selectedParticleEmitterAsset);
                pushPropertyTarget
                        (
                            PropertyType::Asset,
                            asset,
                            nullptr
                            );
                return;
            }
            ImGui::SameLine();
            if(StringCombo("Particle Emitter",&selectedParticleEmitterAsset,peAssets,peAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::PARTICLE_EMITTER, selectedParticleEmitterAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::PARTICLE_EMITTER, selectedParticleEmitterAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::PARTICLE_EMITTER, selectedDef->getUuid());
                    }
                }
            }

            // Path
            int selectedPathAsset = soDef->getSelectedAssetIndex(AssetType::PATH);
            vector<string> pathAssets = projDef->getAssetNamesVector(AssetType::PATH);
            if(ImGui::Button("-##Path"))
            {
                soDef->setAssetDefinition(AssetType::PATH,"");
                if (soRuntime)
                {
                    soRuntime->removePathInstance();
                }

            }
            ImGui::SameLine();

            if(ImGui::Button(">##Path"))
            {
                if (selectedPathAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::PATH,selectedPathAsset);
                pushPropertyTarget(
                            PropertyType::Asset,
                            asset,
                            nullptr
                            );
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Path",&selectedPathAsset,pathAssets,pathAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::PATH, selectedPathAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(PATH, selectedPathAsset);
                    soRuntime->replaceAssetUuid(PATH, selectedDef->getUuid());
                }
            }

            //Physics Object
            int selectedPhysicsObjectAsset = soDef->getSelectedAssetIndex(AssetType::PHYSICS_OBJECT);
            vector<string> poAssets = projDef->getAssetNamesVector(AssetType::PHYSICS_OBJECT);
            if(ImGui::Button("-##PhysicsObject"))
            {
                soDef->setAssetDefinition(AssetType::PHYSICS_OBJECT,"");
                if (soRuntime)
                {
                    soRuntime->removePhysicsObjectInstance();
                }
            }
            ImGui::SameLine();

            if(ImGui::Button(">##PhysicsObject"))
            {
                if (selectedPhysicsObjectAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::PHYSICS_OBJECT,selectedPhysicsObjectAsset);
                pushPropertyTarget(
                            PropertyType::Asset,
                            asset,
                            nullptr
                            );
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Physics Object",&selectedPhysicsObjectAsset,poAssets,poAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::PHYSICS_OBJECT,selectedPhysicsObjectAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::PHYSICS_OBJECT, selectedPhysicsObjectAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::PHYSICS_OBJECT, selectedDef->getUuid());
                    }
                }
            }

            // Script
            int selectedScriptAsset = soDef->getSelectedAssetIndex(AssetType::SCRIPT);
            vector<string> scriptAssets = projDef->getAssetNamesVector(AssetType::SCRIPT);
            if(ImGui::Button("-##Script"))
            {
                soDef->setAssetDefinition(AssetType::SCRIPT,"");
                if (soRuntime)
                {
                    soRuntime->removeScriptInstance();
                }
            }
            ImGui::SameLine();

            if(ImGui::Button(">##Script"))
            {
                if (selectedScriptAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::SCRIPT,selectedScriptAsset);
                pushPropertyTarget(
                            PropertyType::Asset,
                            asset,
                            nullptr
                            );
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Script",&selectedScriptAsset,scriptAssets,scriptAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::SCRIPT, selectedScriptAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::SCRIPT, selectedScriptAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::SCRIPT, selectedDef->getUuid());
                    }
                }
            }
        }
    }

    void
    PropertiesWindow::drawImGizmo
    ()
    {
        auto log = getLog();
        float *matrix = nullptr;
        auto soRunt = dynamic_cast<SceneObjectRuntime*>(mRuntime);
        auto soDef = dynamic_cast<SceneObjectDefinition*>(mDefinition);

        if (soRunt)
        {
            matrix = soRunt->getTransform().getMatrixFloatPointer();
        }
        else
        {
            return;
        }

        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
        static bool withChildren = false;

        if(ImGui::CollapsingHeader("Transform"))
        {
            ImGui::Columns(3);
            if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            {
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            }
            ImGui::NextColumn();
            if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            {
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            }
            ImGui::NextColumn();
            if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            {
                mCurrentGizmoOperation = ImGuizmo::SCALE;
            }

            ImGui::Columns(1);

            float matrixTranslation[3], matrixRotation[3], matrixScale[3];
            ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

            ImGui::InputFloat3("Translation", matrixTranslation, 3);
            ImGui::InputFloat3("Rotation", matrixRotation, 3);
            ImGui::InputFloat3("Scale", matrixScale, 3);

            ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

            ImGui::Separator();
            ImGui::Checkbox("With Children",&withChildren);
            ImGui::Separator();

            ImGui::Checkbox("Snap to Grid", &mGizmoUseSnap);

            switch (mCurrentGizmoOperation)
            {
                case ImGuizmo::TRANSLATE:
                    ImGui::InputFloat3("Snap", &mGizmoSnap.x);
                    break;
                case ImGuizmo::ROTATE:
                    ImGui::InputFloat("Angle Snap", &mGizmoSnap.x);
                    break;
                case ImGuizmo::SCALE:
                    ImGui::InputFloat("Scale Snap", &mGizmoSnap.x);
                    break;
                default:
                    break;
            }

            ImGui::Separator();

            ImGui::Columns(2);

            if (mCurrentGizmoOperation != ImGuizmo::SCALE)
            {
                if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                {
                    mCurrentGizmoMode = ImGuizmo::LOCAL;
                }
                ImGui::NextColumn();
                if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                {
                    mCurrentGizmoMode = ImGuizmo::WORLD;
                }
                ImGui::Separator();
                ImGui::NextColumn();
            }

            if(ImGui::Button("Capture Initial"))
            {
                if (soDef && soRunt)
                {
                    soDef->setTransform(soRunt->getTransform());

                    if (withChildren)
                    {
                        soRunt->applyToAll
                        (
                            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
                            [&](SceneObjectRuntime* rt)
                            {
                                if (rt != soRunt)
                                {
                                    auto d = dynamic_cast<SceneObjectDefinition*>(rt->getDefinition());
                                    d->setTransform(rt->getTransform());
                                }
                                return static_cast<SceneObjectRuntime*>(nullptr);
                            }
                        ));
                    }
                }
            }

            ImGui::NextColumn();
            if(ImGui::Button("Restore Initial"))
            {
                if (soDef && soRunt)
                {
                    soRunt->setTransform(soDef->getTransform());
                    if (withChildren)
                    {
                        soRunt->applyToAll
                        (
                            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
                            [&](SceneObjectRuntime* rt)
                            {
                                if (rt != soRunt)
                                {
                                    auto d = dynamic_cast<SceneObjectDefinition*>(rt->getDefinition());
                                    rt->setTransform(d->getTransform());
                                }
                                return static_cast<SceneObjectRuntime*>(nullptr);
                            }
                        ));
                    }

                }

            }

            ImGui::Columns(1);
        }

        auto pRunt = mState->project->getRuntime();
        if (pRunt)
        {
            auto sRunt = pRunt->getActiveSceneRuntime();
            if (sRunt)
            {
                auto cam = sRunt->getCamera();
                if (cam)
                {
                    auto view = cam->getViewMatrix();
                    auto proj = cam->getProjectionMatrix();
                    ImGuiIO& io = ImGui::GetIO();
                    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
                    //ImGuizmo::DrawCube(glm::value_ptr(view),glm::value_ptr(proj),matrix);
                    //ImGuizmo::DrawGrid(glm::value_ptr(view),glm::value_ptr(proj),matrix,1.0f);
                    mat4 delta(1.0f);
                    ImGuizmo::Manipulate
                    (
                        glm::value_ptr(view),
                        glm::value_ptr(proj),
                        mCurrentGizmoOperation,
                        mCurrentGizmoMode,
                        matrix,
                        value_ptr(delta),
                        mGizmoUseSnap ? &mGizmoSnap.x : nullptr
                    );

                    if (soRunt && withChildren)
                    {
                        vec3 tx = vec3(delta[3]);
                        soRunt->applyToAll
                        (
                            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
                            [&](SceneObjectRuntime* rt)
                            {
                                if (rt != soRunt)
                                {
                                    rt->getTransform().preTranslate(tx);
                                }
                                return static_cast<SceneObjectRuntime*>(nullptr);
                            }
                        ));
                    }
                }
            }
        }
    }

    void
    PropertiesWindow::drawPhysicsImGizmo
    (CompoundChildDefinition ccd)
    {
        auto log = getLog();
        float* matrix = ccd.transform.getMatrixFloatPointer();

        static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

        ImGui::InputFloat3("##Translation", matrixTranslation, 3);
        ImGui::SameLine();
        if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
        {
            currentGizmoOperation = ImGuizmo::TRANSLATE;
        }

        ImGui::InputFloat3("##Rotation", matrixRotation, 3);
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
        {
            currentGizmoOperation = ImGuizmo::ROTATE;
        }

        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

        auto pRunt = mState->project->getRuntime();
        if (pRunt)
        {
            auto sRunt = pRunt->getActiveSceneRuntime();
            if (sRunt)
            {
                auto cam = sRunt->getCamera();
                if (cam)
                {
                    auto proj = cam->getProjectionMatrix();
                    auto view = cam->getViewMatrix();
                    ImGuiIO& io = ImGui::GetIO();
                    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
                    ImGuizmo::Manipulate
                    (
                        glm::value_ptr(view),
                        glm::value_ptr(proj),
                        currentGizmoOperation,
                        currentGizmoMode,
                        matrix,
                        nullptr,
                        mGizmoUseSnap ? &mGizmoSnap.x : nullptr
                    );
                    ccd.parent->updateCompoundChildTransform(ccd);
                    replaceRuntimeInstances(ccd.parent);
                }
            }
        }
    }

    void
    PropertiesWindow::drawAssetProperties
    ()
    {
        auto assetDef = dynamic_cast<AssetDefinition*>(mDefinition);
        if (assetDef == nullptr)
        {
            return;
        }

        ImGui::SameLine();
        if(ImGui::Button("Delete Asset"))
        {
            mState->projectDirectory.removeAssetDirectory(assetDef);
            auto projDef = mState->project->getDefinition();
            projDef->removeAssetDefinition(assetDef);
            removeFromHistory(mDefinition);
            mDefinition = nullptr;
            mRuntime = nullptr;
            projDef->regroupAssetDefinitions();
            return;
        }

        drawNameAndIdProperties();
        ImGui::Separator();
        char groupStr[128] = {0};
        strncpy(&groupStr[0], assetDef->getGroup().c_str(), assetDef->getGroup().size());
        if (ImGui::InputText("Group", &groupStr[0],128))
        {
           assetDef->setGroup(groupStr);
           assetDef->getProject()->regroupAssetDefinitions();
        }
        ImGui::Separator();
        auto type = Constants::getAssetTypeEnumFromString(assetDef->getType());
        switch (type)
        {
            case AssetType::ANIMATION:
                drawAnimationAssetProperties();
                break;
            case AssetType::AUDIO:
                drawAudioAssetProperties();
                break;
            case AssetType::FONT:
                drawFontAssetProperties();
                break;
            case AssetType::LIGHT:
                drawLightAssetProperties();
                break;
            case AssetType::MATERIAL:
                drawMaterialAssetProperties();
                break;
            case AssetType::MODEL:
                drawModelAssetProperties();
                break;
            case AssetType::PHYSICS_OBJECT:
                drawPhysicsObjectAssetProperties();
                break;
            case AssetType::SCRIPT:
                drawScriptProperties();
                break;
            case AssetType::SHADER:
                drawShaderAssetProperties();
                break;
            case AssetType::PATH:
                drawPathAssetProperties();
                break;
            case AssetType::PARTICLE_EMITTER:
                drawParticleEmitterAssetProperties();
                break;
            case AssetType::TEXTURE:
                drawTextureAssetProperties();
                break;
            case AssetType::NONE:
                break;
        }

        auto pRunt = mState->project->getRuntime();
        if (pRunt)
        {
            auto activeScene = pRunt->getActiveSceneRuntime();
            if (activeScene)
            {
                ImGui::Separator();
                ImGui::Columns(1);
                if(ImGui::CollapsingHeader("Active Instances"))
                {
                    auto instances = activeScene->getSceneObjectsWithInstanceOf(assetDef);
                    for (auto instance : instances)
                    {
                        ImGui::Text("%s",instance->getNameAndUuidString().c_str());
                    }
                }
            }
        }
    }

    void
    PropertiesWindow::drawAnimationAssetProperties
    ()
    {
        auto animDef = dynamic_cast<AnimationDefinition*>(mDefinition);

        int duration = animDef->getDuration();
        if (ImGui::InputInt("Duration (ms)",&duration))
        {
           animDef->setDuration(duration);
        }

        ImGui::Columns(2);

        bool looping = animDef->getLooping();
        if (ImGui::Checkbox("Looping",&looping))
        {
            animDef->setLooping(looping);
        }

        ImGui::NextColumn();

        if (ImGui::Button("Add Keyframe"))
        {
            animDef->addKeyframe(AnimationKeyframe(animDef->nextKeyframeID()));
        }

        ImGui::Separator();

        ImGui::Columns(6);
        // Table Header
        ImGui::Text("Remove");
        ImGui::NextColumn();

        ImGui::Text("Time (ms)");
        ImGui::NextColumn();

        ImGui::Text("Translation");
        ImGui::NextColumn();

        ImGui::Text("Rotation");
        ImGui::NextColumn();

        ImGui::Text("Scale");
        ImGui::NextColumn();

        ImGui::Text("Easing");
        ImGui::NextColumn();

        ImGui::Separator();

        for (auto& kf : animDef->getKeyframes())
        {
            ImGui::PushID(kf.getID());

            // Remove
            if (ImGui::Button("-##remove"))
            {
                animDef->removeKeyframe(kf);
            }
            ImGui::SameLine();
            ImGui::Text("(%d)",kf.getID());
            ImGui::NextColumn();

            // Time
            int time = kf.getTime();
            ImGui::PushItemWidth(-1);
            if(ImGui::InputInt("##time",&time))
            {
                kf.setTime(time);
                animDef->updateKeyframe(kf);
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            // Tx
            vec3 vTx = kf.getTranslation();
            float tx[3] = {vTx.x, vTx.y, vTx.z};
            ImGui::PushItemWidth(-1);
            if (ImGui::InputFloat3("##translation",&tx[0]))
            {
                vTx.x = tx[0];
                vTx.y = tx[1];
                vTx.z = tx[2];
                kf.setTranslation(vTx);
                animDef->updateKeyframe(kf);
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            // Rx
            vec3 vRx = kf.getRotation();
            float rx[3] = {degrees(vRx.x), degrees(vRx.y), degrees(vRx.z)};
            ImGui::PushItemWidth(-1);
            if (ImGui::InputFloat3("##rotation",&rx[0]))
            {
                vRx.x = radians(rx[0]);
                vRx.y = radians(rx[1]);
                vRx.z = radians(rx[2]);
                kf.setRotation(vRx);
                animDef->updateKeyframe(kf);
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            // Scale
            vec3 vScale = kf.getScale();
            float scale[3] = {vScale.x, vScale.y, vScale.z};
            ImGui::PushItemWidth(-1);
            if (ImGui::InputFloat3("##scale",&scale[0]))
            {
                vScale.x = scale[0];
                vScale.y = scale[1];
                vScale.z = scale[2];
                kf.setScale(vScale);
                animDef->updateKeyframe(kf);
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            // Easing
            auto easingTypes = AnimationEasing::EasingNames;
            int currentEasingType = kf.getEasingType();
            if (StringCombo("##EasingType", &currentEasingType, easingTypes,easingTypes.size()))
            {
               kf.setEasingType(static_cast<AnimationEasing::Type>(currentEasingType));
               animDef->updateKeyframe(kf);
            }

            ImGui::NextColumn();
            ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::Columns(1);
    }

    void
    PropertiesWindow::drawAudioAssetProperties
    ()
    {
        auto log = getLog();
        bool selectAudioFile = ImGui::Button("Audio File...");
        auto audioDef = dynamic_cast<AudioDefinition*>(mDefinition);

        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectAudioFile,mState->lastDirectory.c_str(),".ogg;.wav","Select Audio File");
        if (strlen(chosenPath) > 0)
        {
            auto audioFilePath = openDlg.getChosenPath();
            log->error("Opening Audio File {}",audioFilePath);
            File audioFile(audioFilePath);
            mState->lastDirectory = audioFile.getDirectory();
            log->error("Setting last directory {}",mState->lastDirectory);
            auto audioData = audioFile.readBinary();
            mState->projectDirectory.writeAssetData(audioDef,audioData);
            audioDef->setName(audioFile.nameWithoutExtension());
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {
            mState->projectDirectory.removeAssetDirectory(audioDef);
        }

        ImGui::Separator();

        if (ImGui::Button("Play"))
        {
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause"))
        {
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
        }

        ImGui::PushItemWidth(-1);
        int audioProg = 0;
        int duration = 0;
        if(ImGui::SliderInt("#AudioProgress", &audioProg,0,duration,"%d"))
        {
        }
        ImGui::PopItemWidth();
    }

    void
    PropertiesWindow::drawFontAssetProperties
    ()
    {
        auto log = getLog();
        bool selectFile = ImGui::Button("Font File...");
        auto def = dynamic_cast<FontDefinition*>(mDefinition);

        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectFile,mState->lastDirectory.c_str(),".ttf","Select Font File");
        if (strlen(chosenPath) > 0)
        {
            auto filePath = openDlg.getChosenPath();
            log->error("Opening Font File {}",filePath);
            File file(filePath);
            mState->lastDirectory = file.getDirectory();
            auto data = file.readBinary();
            mState->projectDirectory.writeAssetData(def,data);
            def->setName(file.nameWithoutExtension());
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {
            mState->projectDirectory.removeAssetDirectory(def);
        }
    }

    void PropertiesWindow::drawLightAssetProperties
    ()
    {
        auto lightDef = dynamic_cast<LightDefinition*>(mDefinition);

        vector<string> lightTypes = Constants::DREAM_ASSET_FORMATS_MAP[AssetType::LIGHT];
        int selectedLightType = getStringIndexInVector(lightDef->getFormat(), lightTypes);
        if(StringCombo("Type",&selectedLightType,lightTypes,lightTypes.size()))
        {
            lightDef->setFormat(lightTypes.at(selectedLightType));
        }

        ImGui::Separator();

        float ambientColor[3] = {
            lightDef->getAmbientRed(),
            lightDef->getAmbientGreen(),
            lightDef->getAmbientBlue()
        };

        float diffuseColor[3] = {
            lightDef->getDiffuseRed(),
            lightDef->getDiffuseGreen(),
            lightDef->getDiffuseBlue()
        };

        float specularColor[3] =
        {
            lightDef->getSpecularRed(),
            lightDef->getSpecularGreen(),
            lightDef->getSpecularBlue()
        };

        float constant = lightDef->getConstant();
        float linear = lightDef->getLinear();
        float quadratic = lightDef->getQuadratic();
        float innerCutOff, outerCutOff;

        switch (lightDef->getType())
        {
            case LightType::LT_SPOTLIGHT:
                innerCutOff = lightDef->getCutOff();
                outerCutOff = lightDef->getOuterCutOff();
                if(ImGui::DragFloat("Inner Cut Off", &innerCutOff))
                {
                    lightDef->setCutOff(innerCutOff);
                }
                if(ImGui::DragFloat("Outer Cut Off", &outerCutOff))
                {
                    lightDef->setOuterCutOff(outerCutOff);
                }
                break;
            case LightType::LT_POINT:
                if(ImGui::DragFloat("Constant",&constant,1.0f))
                {
                    lightDef->setConstant(constant);
                }
                if(ImGui::DragFloat("Linear",&linear,0.01f))
                {
                    lightDef->setLinear(linear);
                }
                if(ImGui::DragFloat("Quadratic",&quadratic,0.001f))
                {
                    lightDef->setQuadratic(quadratic);
                }
                break;
            case LightType::LT_DIRECTIONAL:
            case LightType::LT_NONE:
                break;
        }

        ImGui::Separator();

        if(ImGui::ColorEdit3("Ambient", ambientColor))
        {
            lightDef->setAmbientRed(ambientColor[0]);
            lightDef->setAmbientGreen(ambientColor[1]);
            lightDef->setAmbientBlue(ambientColor[2]);
        }

        if(ImGui::ColorEdit3("Diffuse", diffuseColor))
        {
            lightDef->setDiffuseRed(diffuseColor[0]);
            lightDef->setDiffuseGreen(diffuseColor[1]);
            lightDef->setDiffuseBlue(diffuseColor[2]);
        }

        if(ImGui::ColorEdit3("Specular", specularColor))
        {
            lightDef->setSpecularRed(specularColor[0]);
            lightDef->setSpecularGreen(specularColor[1]);
            lightDef->setSpecularBlue(specularColor[2]);
        }
    }

    void
    PropertiesWindow::drawMaterialAssetProperties
    ()
    {
        auto projDef = mState->project->getDefinition();
        auto projRunt = mState->project->getRuntime();
        auto materialDef = dynamic_cast<MaterialDefinition*>(mDefinition);
        int shaderIndex = 0;
        vector<string> shaderList;
        if (projDef)
        {
            auto shaderAsset = projDef->getAssetDefinitionByUuid(materialDef->getShader());
            shaderIndex = projDef->getAssetDefinitionIndex(AssetType::SHADER,shaderAsset);
            shaderList = projDef->getAssetNamesVector(AssetType::SHADER);
        }

        if(StringCombo("Shader", &shaderIndex, shaderList, shaderList.size()))
        {
            if (projDef)
            {
                auto newShader = projDef->getAssetDefinitionAtIndex(AssetType::SHADER,shaderIndex);
                if(newShader)
                {
                    materialDef->setShader(newShader->getUuid());
                }
            }
        }

        ImGui::Separator();

        float diffuseColor[3] =
        {
            materialDef->getDiffuseColour().r,
            materialDef->getDiffuseColour().g,
            materialDef->getDiffuseColour().b
        };
        if (ImGui::ColorEdit3("Diffuse", diffuseColor))
        {
            materialDef->setDiffuseColour(RGB{
                                              diffuseColor[0],
                                              diffuseColor[1],
                                              diffuseColor[2]
                                          });
        }

        float specularColor[3] = {
            materialDef->getSpecularColour().r,
            materialDef->getSpecularColour().g,
            materialDef->getSpecularColour().b
        };
        if (ImGui::ColorEdit3("Specular", specularColor))
        {
            materialDef->setSpecularColour(
                        RGB
            {
                            specularColor[0],
                            specularColor[1],
                            specularColor[2]
                        }
                        );
        }

        float ambientColor[3] = {
            materialDef->getAmbientColour().r,
            materialDef->getAmbientColour().g,
            materialDef->getAmbientColour().b
        };
        if (ImGui::ColorEdit3("Ambient", ambientColor))
        {
            materialDef->setAmbientColour(
                        RGB{
                            ambientColor[0],
                            ambientColor[1],
                            ambientColor[2]
                        }
                        );
        }

        float reflectiveColor[3] = {
            materialDef->getReflectiveColour().r,
            materialDef->getReflectiveColour().g,
            materialDef->getReflectiveColour().b
        };
        if (ImGui::ColorEdit3("Reflective", reflectiveColor))
        {
            materialDef->setReflectiveColour(
                        RGB
            {
                            reflectiveColor[0],
                            reflectiveColor[1],
                            reflectiveColor[2]
                        }
                        );
        }

        float emissiveColor[3] = {
            materialDef->getEmissiveColour().r,
            materialDef->getEmissiveColour().g,
            materialDef->getEmissiveColour().b
        };
        if(ImGui::ColorEdit3("Emissive", emissiveColor))
        {
            materialDef->setEmissiveColour(
                        RGB
            {
                            emissiveColor[0],
                            emissiveColor[1],
                            emissiveColor[2]
                        }
                        );
        }

        // Parameters
        ImGui::Separator();

        float shininess = materialDef->getShininessStrength();
        if(ImGui::InputFloat("Shininess",&shininess,0.01f,1.0f))
        {
            materialDef->setShininessStrength(shininess);
        }

        float opacity = materialDef->getOpacity();
        if(ImGui::InputFloat("Opacity",&opacity,0.01f,1.0f))
        {
            materialDef->setOpacity(opacity);
        }

        float bumpScaling = materialDef->getBumpScaling();
        if(ImGui::InputFloat("Bump Scaling",&bumpScaling,0.01f,1.0f))
        {
            materialDef->setBumpScaling(bumpScaling);
        }

        float hardness = materialDef->getHardness();
        if(ImGui::InputFloat("Hardness",&hardness,0.01f,1.0f))
        {
            materialDef->setHardness(hardness);
        }

        float reflectivity = materialDef->getReflectivity();
        if(ImGui::InputFloat("Reflectivity",&reflectivity,0.01f,1.0f))
        {
            materialDef->setReflectivity(reflectivity);
        }


        float refractionIndex = materialDef->getRefractionIndex();
        if(ImGui::InputFloat("Refraction Index",&refractionIndex,0.01f,1.0f))
        {
            materialDef->setRefractionIndex(refractionIndex);
        }

        bool ignore = materialDef->getIgnore();
        if (ImGui::Checkbox("Ignore",&ignore))
        {
            materialDef->setIgnore(ignore);
        }

        ImGui::Separator();

        string diffuseUuid = materialDef->getDiffuseTexture();
        string specularUuid = materialDef->getSpecularTexture();
        string normalUuid = materialDef->getNormalTexture();
        string displacementUuid = materialDef->getDisplacementTexture();
        void* diffuseTxId = nullptr;
        void* normalTxId = nullptr;
        void* specularTxId = nullptr;
        void* displacementTxId = nullptr;

        if (projDef && projRunt)
        {
            // Diffuse
            auto diffuseDef = projDef->getAssetDefinitionByUuid(diffuseUuid);
            if (diffuseDef)
            {
                auto txDef = dynamic_cast<TextureDefinition*>(diffuseDef);
                auto txCache = projRunt->getTextureCache();
                auto txInstance = dynamic_cast<TextureRuntime*>(txCache->getInstance(txDef));
                if (txInstance)
                {
                    diffuseTxId = (void*)(intptr_t)txInstance->getGLID();
                }
            }

            // Specular
            auto specularDef = projDef->getAssetDefinitionByUuid(specularUuid);
            if (specularDef)
            {
                auto txDef = dynamic_cast<TextureDefinition*>(specularDef);
                auto txCache = projRunt->getTextureCache();
                auto txInstance = dynamic_cast<TextureRuntime*>(txCache->getInstance(txDef));
                if (txInstance)
                {
                    specularTxId = (void*)(intptr_t)txInstance->getGLID();
                }
            }

            // Normal
            auto normalDef = projDef->getAssetDefinitionByUuid(normalUuid);
            if (normalDef)
            {
                auto txDef = dynamic_cast<TextureDefinition*>(normalDef);
                auto txCache = projRunt->getTextureCache();
                auto txInstance = dynamic_cast<TextureRuntime*>(txCache->getInstance(txDef));
                if (txInstance)
                {
                    normalTxId = (void*)(intptr_t)txInstance->getGLID();
                }
            }

            // Displacement
            auto displacementDef = projDef->getAssetDefinitionByUuid(displacementUuid);
            if (displacementDef)
            {
                auto txDef = dynamic_cast<TextureDefinition*>(displacementDef);
                auto txCache = projRunt->getTextureCache();
                auto txInstance = dynamic_cast<TextureRuntime*>(txCache->getInstance(txDef));
                if (txInstance)
                {
                    displacementTxId = (void*)(intptr_t)txInstance->getGLID();
                }
            }
        }

        vector<string> textures;
        int diffuseIndex;
        int specularIndex;
        int normalIndex;
        int displacementIndex;

        if(projDef)
        {
            textures = projDef->getAssetNamesVector(AssetType::TEXTURE);
            diffuseIndex = projDef->getAssetDefinitionIndex(
                        AssetType::TEXTURE,
                        projDef->getAssetDefinitionByUuid(diffuseUuid)
                        );
            specularIndex = projDef->getAssetDefinitionIndex(
                        AssetType::TEXTURE,
                        projDef->getAssetDefinitionByUuid(specularUuid)
                        );
            normalIndex = projDef->getAssetDefinitionIndex(
                        AssetType::TEXTURE,
                        projDef->getAssetDefinitionByUuid(normalUuid)
                        );
            displacementIndex = projDef->getAssetDefinitionIndex(
                        AssetType::TEXTURE,
                        projDef->getAssetDefinitionByUuid(displacementUuid)
                        );
        }

        // Diffuse
        if(StringCombo("Diffuse",&diffuseIndex,textures,textures.size()))
        {
            if (projDef)
            {
                auto txDef = projDef->getAssetDefinitionAtIndex(AssetType::TEXTURE, diffuseIndex);
                if (txDef)
                {
                    auto uuid = txDef->getUuid();
                    materialDef->setDiffuseTexture(uuid);
                }
            }
        }
        ImGui::Image(diffuseTxId, mImageSize);
        ImGui::Separator();

        // Specular
        if(StringCombo("Specular",&specularIndex,textures,textures.size()))
        {
            if (projDef)
            {
                auto txDef = projDef->getAssetDefinitionAtIndex(AssetType::TEXTURE, specularIndex);
                if (txDef)
                {
                    auto uuid = txDef->getUuid();
                    materialDef->setSpecularTexture(uuid);
                }
            }
        }
        ImGui::Image(specularTxId, mImageSize);
        ImGui::Separator();

        // Normal
        if(StringCombo("Normal",&normalIndex,textures,textures.size()))
        {
            if (projDef)
            {
                auto txDef = projDef->getAssetDefinitionAtIndex(AssetType::TEXTURE, normalIndex);
                if (txDef)
                {
                    auto uuid = txDef->getUuid();
                    materialDef->setNormalTexture(uuid);
                }
            }
        }
        ImGui::Image(normalTxId, mImageSize);
        ImGui::Separator();

        // Displacement
        if(StringCombo("Displacement",&displacementIndex,textures,textures.size()))
        {
            if (projDef)
            {
                auto txDef = projDef->getAssetDefinitionAtIndex(AssetType::TEXTURE, displacementIndex);
                if (txDef)
                {
                    auto uuid = txDef->getUuid();
                    materialDef->setDisplacementTexture(uuid);
                }
            }
        }
        ImGui::Image(displacementTxId, mImageSize);
        ImGui::Columns(1);

    }

    void
    PropertiesWindow::drawModelAssetProperties
    ()
    {
        auto log = getLog();
        auto def = dynamic_cast<ModelDefinition*>(mDefinition);

        bool selectFile = ImGui::Button("Model File...");
        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectFile,mState->lastDirectory.c_str(),nullptr,"Select Model File");
        if (strlen(chosenPath) > 0)
        {
            auto filePath = openDlg.getChosenPath();
            log->error("Opening Model File {}",filePath);
            File file(filePath);
            mState->lastDirectory = file.getDirectory();
            auto data = file.readBinary();
            def->setFormat(Constants::ASSET_TYPE_MODEL+"."+file.extension());
            def->setName(file.nameWithoutExtension());
            mState->projectDirectory.writeAssetData(def,data);
        }

        ImGui::SameLine();

        bool selectAdditionalFile = ImGui::Button("Additional File...");
        static ImGuiFs::Dialog openAdditionalFileDlg;
        const char* chosenAdditionalFilePath =
                openAdditionalFileDlg.chooseFileDialog(selectAdditionalFile,mState->lastDirectory.c_str(),nullptr,"Select Additional File");
        if (strlen(chosenAdditionalFilePath) > 0)
        {
            auto filePath = openAdditionalFileDlg.getChosenPath();
            log->error("Opening Additional Model File {}",filePath);
            File file(filePath);
            mState->lastDirectory = file.getDirectory();
            auto data = file.readBinary();
            mState->projectDirectory.writeAssetData(def,data,file.nameWithExtension());
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File(s)"))
        {
            mState->projectDirectory.removeAssetDirectory(def);
        }

        ImGui::Text("Model Format: %s", def->getFormat().c_str());

        vector<string> modelMaterialNames;
        map<string,Bone> bones;
        auto projRunt = mState->project->getRuntime();
        if (projRunt)
        {
            auto modelCache = projRunt->getModelCache();
            if (modelCache)
            {
                auto modelInstance = dynamic_cast<ModelRuntime*>(modelCache->getInstance(def));
                if (modelInstance)
                {
                    modelMaterialNames = modelInstance->getMaterialNames();
                    bones = modelInstance->getBones();
                }
            }
        }

        // Bones
        if (ImGui::CollapsingHeader("Bones"))
        {
            for (auto pair : bones)
            {
                ImGui::Text("%s",pair.first.c_str());
            }
        }

        // Material List --------------------------------------------------------
        ImGui::Separator();

        ImGui::Text("Material Mappings");
        ImGui::Separator();
        ImGui::Columns(2);
        ImGui::Text("From Model");
        ImGui::NextColumn();
        ImGui::Text("In Dream");
        ImGui::NextColumn();

        vector<string> materialAssetNames;
        auto projDef =  mState->project->getDefinition();
        if (projDef)
        {
            materialAssetNames = projDef->getAssetNamesVector(AssetType::MATERIAL);
        }

        for (string modelMaterial : modelMaterialNames)
        {
            ImGui::Text("%s",modelMaterial.c_str());
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            auto currentMaterialUuid = def->getDreamMaterialForModelMaterial(modelMaterial);
            auto currentMaterialDef = projDef->getAssetDefinitionByUuid(currentMaterialUuid);
            int currentMaterialIndex = projDef->getAssetDefinitionIndex(AssetType::MATERIAL,currentMaterialDef);
            string itemName = "##Material:" + modelMaterial;
            if(StringCombo(itemName.c_str(),&currentMaterialIndex,materialAssetNames,materialAssetNames.size()))
            {
                auto changedMaterial = projDef->getAssetDefinitionAtIndex(AssetType::MATERIAL, currentMaterialIndex);
                def->addModelMaterial(modelMaterial,changedMaterial->getUuid());
                log->error("Changed {} material {} to map to {}",def->getName(), modelMaterial, changedMaterial->getNameAndUuidString() );
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();
        }
    }

    void
    PropertiesWindow::drawPhysicsObjectAssetProperties
    ()
    {
        bool modified = false;
        auto pod = dynamic_cast<PhysicsObjectDefinition*>(mDefinition);
        vector<string> poFormats = Constants::DREAM_ASSET_FORMATS_MAP[AssetType::PHYSICS_OBJECT];
        string poFormatString = pod->getFormat();
        int poFormatIndex = getStringIndexInVector(poFormatString, poFormats);
        if(StringCombo("Format",&poFormatIndex, poFormats,poFormats.size()))
        {
            pod->setFormat(poFormats.at(poFormatIndex));
            modified = true;
        }
        ImGui::Separator();

        bool kinematic = pod->getKinematic();
        if (ImGui::Checkbox("Kinematic",&kinematic))
        {
            pod->setKinematic(kinematic);
            modified = true;
        }

        bool controllable = pod->getControllableCharacter();
        if (ImGui::Checkbox("Controllable Character",&controllable))
        {
            pod->setControllableCharacter(controllable);
            modified = true;
        }

        ImGui::Separator();

        float mass = pod->getMass();
        if(ImGui::InputFloat("Mass",&mass))
        {
            pod->setMass(mass);
            modified = true;
        }

        float margin = pod->getMargin();
        if(ImGui::InputFloat("Margin",&margin))
        {
            pod->setMargin(margin);
            modified = true;
        }

        float restitution = pod->getRestitution();
        if(ImGui::InputFloat("Restitution",&restitution))
        {
            pod->setRestitution(restitution);
            modified = true;
        }

        float friction = pod->getFriction();
        if(ImGui::InputFloat("Friction",&friction))
        {
            pod->setFriction(friction);
            modified = true;
        }

        float ccdspr = pod->getCcdSweptSphereRadius();
        if (ImGui::InputFloat("CCD Swept Sphere Radius",&ccdspr))
        {
            pod->setCcdSweptSphereRadius(ccdspr);
            modified = true;
        }

        ImGui::Separator();

        if (pod->getFormat().compare(Constants::COLLISION_SHAPE_BOX) == 0)
        {
            float halfExtents[3] =
            {
                pod->getHalfExtentsX(),
                pod->getHalfExtentsY(),
                pod->getHalfExtentsZ()
            };
            if(ImGui::InputFloat3("Half-Extents",&halfExtents[0]))
            {
                pod->setHalfExtentsX(halfExtents[0]);
                pod->setHalfExtentsY(halfExtents[1]);
                pod->setHalfExtentsZ(halfExtents[2]);
            modified = true;
            }
        }
        else if (pod->getFormat().compare(Constants::COLLISION_SHAPE_SPHERE) == 0)
        {
            float radius = pod->getRadius();
            if (ImGui::InputFloat("Radius",&radius))
            {
                pod->setRadius(radius);
            }
        }
        else if (pod->getFormat().compare(Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH) == 0)
        {
            auto projDef = mState->project->getDefinition();

            string selectedModelAssetUuid = pod->getCollisionModel();
            AssetDefinition* selectedModelAssetDef = projDef->getAssetDefinitionByUuid(selectedModelAssetUuid);
            int selectedModelAssetIndex = projDef->getAssetDefinitionIndex(AssetType::MODEL, selectedModelAssetDef);
            vector<string> modelAssets = projDef->getAssetNamesVector(AssetType::MODEL);

            if(StringCombo("Model",&selectedModelAssetIndex,modelAssets,modelAssets.size()))
            {
                AssetDefinition* newlySelected = projDef->getAssetDefinitionAtIndex(AssetType::MODEL, selectedModelAssetIndex);
                pod->setCollisionModel(newlySelected->getUuid());
            modified = true;
            }
        }
        else if (pod->getFormat().compare(Constants::COLLISION_SHAPE_STATIC_PLANE) == 0)
        {
            float normal[3] = {
                pod->getNormalX(),
                pod->getNormalY(),
                pod->getNormalZ()
            };

           if (ImGui::InputFloat3("Plane Normal",&normal[0], 3))
           {
                pod->setNormalX(normal[0]);
                pod->setNormalY(normal[1]);
                pod->setNormalZ(normal[2]);
                modified = true;
           }
        }
        else if (pod->getFormat().compare(Constants::COLLISION_SHAPE_COMPOUND) == 0)
        {
            ImGui::Columns(2);
            auto pDef = mState->project->getDefinition();
            if (pDef)
            {
                auto shapeNames = pDef->getAssetNamesVector(AssetType::PHYSICS_OBJECT);
                static int shapeNameIndex = -1;
                StringCombo("Shape",&shapeNameIndex,shapeNames,shapeNames.size());

                ImGui::NextColumn();
                if(ImGui::Button("Add Compound Child"))
                {
                    if (shapeNameIndex >= 0)
                    {
                        auto childDef = pDef->getAssetDefinitionAtIndex(AssetType::PHYSICS_OBJECT,shapeNameIndex);
                        pod->addCompoundChild(
                            CompoundChildDefinition
                            {
                                        pod,
                                Transform(),
                                childDef->getUuid()
                            }
                        );
                    }
                    modified = true;
                }
                ImGui::Separator();
                ImGui::Columns(1);
                ImGui::Text("Child Shapes");
                ImGui::Separator();

                auto shapes = pod->getCompoundChildren();
                static string selectedToTransform = "";
                for (auto shape : shapes)
                {
                   auto shapeDef = pDef->getAssetDefinitionByUuid(shape.uuid);

                   if (!shapeDef)
                   {
                       continue;
                   }
                   ImGui::PushID(shape.uuid.c_str());
                   ImGui::SetNextTreeNodeOpen(selectedToTransform.compare(shape.uuid) == 0);
                   if (ImGui::CollapsingHeader(shapeDef->getName().c_str()))
                   {
                        selectedToTransform = shape.uuid;
                        drawPhysicsImGizmo(shape);
                        if(ImGui::Button("Remove Shape"))
                        {
                            pod->removeCompoundChild(shape);
                            modified = true;
                        }
                   }
                   ImGui::PopID();
                }
            }
        }
        if (modified)
        {

            replaceRuntimeInstances(pod);
        }
    }

    void
    PropertiesWindow::drawScriptProperties
    ()
    {
        auto log = getLog();
        auto scriptDef = dynamic_cast<ScriptDefinition*>(mDefinition);

        ImGui::PushItemWidth(-1);
        if(ImGui::Button("Open Script Editor..."))
        {
            mState->scriptEditorWindow.setScriptDefinition(scriptDef);
            mState->scriptEditorWindow.setVisible(true);
        }
        ImGui::PopItemWidth();
    }

    void
    PropertiesWindow::drawShaderAssetProperties
    ()
    {
        auto log = getLog();
        auto shaderDef = dynamic_cast<ShaderDefinition*>(mDefinition);
        auto projRunt = mState->project->getRuntime();
        ShaderRuntime* shaderInst = nullptr;
        if (projRunt)
        {
            auto shaderCache = projRunt->getShaderCache();
            if (shaderCache)
            {
                shaderInst = dynamic_cast<ShaderRuntime*>(shaderCache->getInstance(shaderDef));
            }
        }
        ImGui::PushItemWidth(-1);
        if(ImGui::Button("Open Shader Editor..."))
        {
            mState->shaderEditorWindow.setShaderDefinition(shaderDef);
            mState->shaderEditorWindow.setVisible(true);
        }
        ImGui::PopItemWidth();
    }

    void
    PropertiesWindow::drawPathAssetProperties
    ()
    {}

    void
    PropertiesWindow::drawParticleEmitterAssetProperties
    ()
    {}

    void
    PropertiesWindow::drawTextureAssetProperties
    ()
    {
        void* textureId = nullptr;
        auto textureDef = dynamic_cast<TextureDefinition*>(mDefinition);
        auto projRunt = mState->project->getRuntime();
        if (projRunt)
        {
            auto txCache = projRunt->getTextureCache();
            auto txInstance = dynamic_cast<TextureRuntime*>(txCache->getInstance(textureDef));
            if (txInstance)
            {
                textureId = (void*)(intptr_t)txInstance->getGLID();
            }
        }

        auto log = getLog();
        bool selectFile = ImGui::Button("Texture File...");
        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectFile,mState->lastDirectory.c_str(),".png","Select Texture File");
        if (strlen(chosenPath) > 0)
        {
            auto filePath = openDlg.getChosenPath();
            log->error("Opening Texture File {}",filePath);
            File file(filePath);
            mState->lastDirectory = file.getDirectory();
            auto data = file.readBinary();
            mState->projectDirectory.writeAssetData(textureDef,data);
            textureDef->setName(file.nameWithoutExtension());
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {
            mState->projectDirectory.removeAssetDirectory(textureDef);
        }

        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::Image(textureId, mImageSize);
    }

    int
    PropertiesWindow::getStringIndexInVector
    (string str, vector<string> vec)
    {
        size_t sz = vec.size();
        for (int i=0; i<sz; i++)
        {
            if (vec.at(i).compare(str) == 0)
            {
                return i;
            }
        }
        return -1;
    }

    void
    PropertiesWindow::replaceRuntimeInstances
    (AssetDefinition* assetDef)
    {
        auto pRunt = mState->project->getRuntime();
        if (pRunt)
        {
            auto sRunt = pRunt->getActiveSceneRuntime();
            if (sRunt)
            {
                auto runts = sRunt->getSceneObjectsWithInstanceOf(assetDef);
                for (auto soRunt : runts)
                {
                    soRunt->replaceAssetUuid(assetDef->getAssetType(),assetDef->getUuid());
                }
            }
        }
    }
}
