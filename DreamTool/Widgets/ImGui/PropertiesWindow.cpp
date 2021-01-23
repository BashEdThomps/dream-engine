// Maintain include order for GL Defined

#ifdef WIN32
#include <Windows.h>
#endif

#include "PropertiesWindow.h"
#include "glm/gtc/type_ptr.hpp"
#include <nfd.h>
#include <ImGuizmo.h>
#include "DreamToolContext.h"
#include <DreamCore.h>

namespace octronic::dream::tool
{
    PropertiesWindow::PropertiesWindow
    (DreamToolContext* proj)
        : ImGuiWidget (proj),
          mType(PropertyType_None),
          mDefinition(nullptr),
          mRuntime(nullptr),
          mImageSize(256,256),
          mBigEditorSize(-1,-1),
          mGizmoUseSnap(true),
          mGizmoSnap(1.0f,1.0f,1.0f)
    {
    }

    PropertiesWindow::~PropertiesWindow
    ()
    {

    }

    void
    PropertiesWindow::draw
    ()
    {
        Project* project = mContext->getProject();
        if (project)
        {
            ImGui::Begin("Properties",&mVisible);
            if(ImGui::Button("<- Back"))
            {
                popPropertyTarget();
            }
            switch (mType)
            {
                case PropertyType_None:
                    break;
                case PropertyType_Project:
                    drawProjectProperties();
                    break;
                case PropertyType_Scene:
                    drawSceneProperties();
                    break;
                case PropertyType_Entity:
                    drawEntityProperties();
                    break;
                case PropertyType_Asset:
                    drawAssetProperties();
                    break;
            }
            ImGui::End();
        }
    }

    bool
    PropertiesWindow::drawDeleteEntityButton
    ()
    {
        auto soDef = dynamic_cast<EntityDefinition*>(mDefinition);
        auto soRuntime = dynamic_cast<EntityRuntime*>(mRuntime);
        if (ImGui::Button("Delete"))
        {
            if (soDef)
            {
                auto parent = soDef->getParentEntity();
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
            mContext->getSelectionHighlighter()->clearSelection();
            removeFromHistory(mDefinition);
            mDefinition = nullptr;
            mRuntime = nullptr;
            return true;
        }
        return false;
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

        Project* project = mContext->getProject();
        SceneDefinition* sDef = dynamic_cast<SceneDefinition*>(mDefinition);
        ProjectDefinition* pDef = project->getDefinition();

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
        auto itr = std::find_if(mHistory.begin(), mHistory.end(), [&](PropertiesTarget& tgt){return tgt.definition == def;});

        if (itr != mHistory.end())
        {
            mHistory.erase(itr);
        }

        mHistory.push_back(PropertiesTarget{mType,mDefinition,mRuntime});
        LOG_ERROR("PropertiesWindow: Pushed target {}",mHistory.size());
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
        LOG_ERROR("PropertiesWindow: Popped target {}",mHistory.size());
        mHistory.pop_back();
    }

    void
    PropertiesWindow::clearPropertyTargets
    ()
    {
        mHistory.clear();
        setPropertyType(PropertyType_None);
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

            int id = mDefinition->getUuid();
            if(ImGui::InputInt("ID", &id))
            {
                mDefinition->setUuid(id);
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
        mType = PropertyType_None;
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
        Project* project = mContext->getProject();
        ProjectDefinition* projDef = dynamic_cast<ProjectDefinition*>(mDefinition);

        if (projDef == nullptr)
        {
            return;
        }

        ImGui::SameLine();
        if(ImGui::Button("New Scene"))
        {
            pushPropertyTarget(PropertyType_Scene, project->getDefinition()->createNewSceneDefinition(), nullptr);
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
        SceneDefinition* sceneDef = dynamic_cast<SceneDefinition*>(mDefinition);
        SceneRuntime* sceneRuntime = dynamic_cast<SceneRuntime*>(mRuntime);

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
            cameraTranslation[0] = tx.x();
            cameraTranslation[1] = tx.y();
            cameraTranslation[2] = tx.z();
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
                    cam->setTranslation(Vector3(
                         cameraTranslation[0],
                         cameraTranslation[1],
                         cameraTranslation[2]
                     ));
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

            if(ImGui::Button("Restore Initial Transform"))
            {
                if (sceneRuntime)
                {
                    auto cam = sceneRuntime->getCamera();
                    cam->setTranslation(sceneDef->getCameraTranslation());
                    cam->setPitch(sceneDef->getCameraPitch());
                    cam->setYaw(sceneDef->getCameraYaw());
                    cam->setMovementSpeed(sceneDef->getCameraMovementSpeed());
                }
            }


            auto focused = sceneDef->getCameraFocusedOn();
            string focusedStr = "None";
            if (sceneRuntime)
            {
                auto focusedObj = sceneRuntime->getEntityRuntimeByUuid(focused);
                if (focusedObj)
                {
                    focusedStr = focusedObj->getNameAndUuidString();
                }
            }
            if (ImGui::Button("Clear Focus"))
            {
                sceneDef->setCameraFocusedOn(0);
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

            if (sceneRuntime)
            {
                auto* po = sceneRuntime->getEntityRuntimeByUuid(sceneDef->getPlayerObject());
                if (po)
                {
                    ImGui::Text("PlayerObject: %s",po->getNameAndUuidString().c_str());
                }
            }
            else
            {
                ImGui::Text("PlayerObject: %d",sceneDef->getPlayerObject());
            }

            string nearestStr = "None";
            EntityRuntime* nearest = nullptr;
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
            float clear[3] = {clearVec.x(), clearVec.y(), clearVec.z()};
            if(ImGui::ColorEdit3("Clear Color",&clear[0]))
            {
                if (sceneDef != nullptr)
                {
                    sceneDef->setClearColourR(clear[0]);
                    sceneDef->setClearColourG(clear[1]);
                    sceneDef->setClearColourB(clear[2]);
                }
                if (sceneRuntime)
                {
                    sceneRuntime->setClearColour(
                        Vector3(clear[0],clear[1],clear[2]));
                }
            }

            ProjectDefinition* pDef = sceneDef->getProjectDefinition();
            UuidType lpShaderUuid = sceneDef->getLightingPassShader();
            ShaderDefinition* lpShaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(lpShaderUuid));
            int lightingShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,lpShaderDef);

            UuidType spShaderUuid = sceneDef->getShadowPassShader();
            ShaderDefinition* spShaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(spShaderUuid));
            int shadowShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,spShaderDef);

            auto shaderList = pDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);

            if (StringCombo("Lighting Pass Shader", &lightingShaderIndex, shaderList, shaderList.size()))
            {
                auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, lightingShaderIndex);
                auto uuid = selectedShader->getUuid();
                auto name = selectedShader->getName();
                sceneDef->setLightingPassShader(uuid);

                LOG_ERROR("PropertiesWindow: Switched lighting pass shader to {} {}", name, uuid);
            }


            if (StringCombo("Shadow Pass Shader", &shadowShaderIndex, shaderList, shaderList.size()))
            {
                auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shadowShaderIndex);
                auto uuid = selectedShader->getUuid();
                auto name = selectedShader->getName();
                sceneDef->setShadowPassShader(uuid);
                LOG_ERROR("PropertiesWindow: Switched shadow pass shader to {} {}", name, uuid);
            }
        }

        if (ImGui::CollapsingHeader("Scripting"))
        {
            auto pDef= mContext->getProject()->getDefinition();
            vector<string> scriptAssets = mContext->getProject()->getDefinition()->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SCRIPT);

            uint32_t inputScriptUuid = sceneDef->getInputScript();
            auto inputScriptDef = pDef->getAssetDefinitionByUuid(inputScriptUuid);
            int inputScriptIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,inputScriptDef);
            if (StringCombo("Input Script",&inputScriptIndex,scriptAssets,scriptAssets.size()))
            {
                auto selected = pDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,inputScriptIndex);
                auto uuid = selected->getUuid();
                sceneDef->setInputScript(uuid);
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

            float gravityVec[3] ={
                sceneDef->getGravity().x(),
                sceneDef->getGravity().y(),
                sceneDef->getGravity().z()
            };
            if (ImGui::DragFloat3("Gravity", &gravityVec[0],0.1f))
            {
                Vector3 grav(gravityVec[0],gravityVec[1],gravityVec[2]);
                if (sceneDef)
                {
                    sceneDef->setGravity(grav);
                }
                if (sceneRuntime)
                {
                    sceneRuntime->setGravity(grav);
                }
            }


        }
    }

    void
    PropertiesWindow::drawEntityProperties
    ()
    {
        auto projDef = mContext->getProject()->getDefinition();
        auto soDef = dynamic_cast<EntityDefinition*>(mDefinition);
        auto soRuntime = dynamic_cast<EntityRuntime*>(mRuntime);

        if (soDef == nullptr)
        {
            return;
        }

        if (soDef->getParentEntity() != nullptr)
        {

            ImGui::SameLine();
            if (drawDeleteEntityButton())
            {
                return;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Child"))
        {
            auto newChildDef = soDef->createNewChildDefinition();
            mat4 cursorTx = glm::translate(mat4(1.0f),mContext->getCursor()->getPosition());
            newChildDef->getTransform().setMatrix(cursorTx);
            EntityRuntime* newRt = nullptr;
            if (soRuntime)
            {
                newRt = soRuntime->createAndAddChildRuntime(newChildDef);
                newRt->getTransform().setMatrix(cursorTx);
            }
            pushPropertyTarget(PropertyType_Entity,newChildDef,newRt);
        }

        if (soDef->getParentEntity() != nullptr)
        {
            ImGui::SameLine();
            if (ImGui::Button("Duplicate"))
            {
                auto dup = soDef->duplicate();
                EntityRuntime* newRt = nullptr;
                if (soRuntime)
                {
                    newRt = soRuntime->createAndAddChildRuntime(dup);
                }
                pushPropertyTarget(PropertyType_Entity,dup,newRt);
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

        if (ImGui::Button("Set as Player Object"))
        {
            if(soDef)
            {
                auto sDef = soDef->getSceneDefinition();
                if (sDef)
                {
                    sDef->setPlayerObject(soDef->getUuid());
                }
            }
            if (soRuntime)
            {
                auto sRunt = soRuntime->getSceneRuntime();
                if (sRunt)
                {
                    sRunt->setPlayerObject(soRuntime);
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
        ImGui::Separator();

        ImGui::Text("Lifetime");

        int deferred = soDef->getDeferred();
        if(ImGui::InputInt("Deferred For (seconds)",&deferred))
        {
            soDef->setDeferred(deferred);
        }

        int dieAfter = soDef->getDieAfter();
        if(ImGui::InputInt("Die After (seconds)",&dieAfter))
        {
            soDef->setDieAfter(dieAfter);
        }

        ImGui::Separator();

        if (soRuntime && soRuntime->hasAnimationRuntime())
        {
            auto animation = soRuntime->getAnimationRuntime();
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
                float animProg = animation->getCurrentTime();
                float duration = animation->getDuration();
                if(ImGui::SliderFloat("#AnimationProgress", &animProg,0,duration,"%dms"))
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
            int selectedAnimationAsset = soDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION);
            vector<string> animationAssets = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_ANIMATION);
            if(ImGui::Button("-##Animation"))
            {
                soDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_ANIMATION,0);
                if (soRuntime)
                {
                    soRuntime->removeAnimationRuntime();
                }
            }
            ImGui::SameLine();
            if(ImGui::Button(">##Animation"))
            {
                if (selectedAnimationAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION,selectedAnimationAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Animation",&selectedAnimationAsset,animationAssets,animationAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedAnimationAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedAnimationAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedDef->getUuid());
                    }
                }
            }

            // Audio
            int selectedAudioAsset = soDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_AUDIO);
            vector<string> audioAssets = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_AUDIO);
            if(ImGui::Button("-##Audio"))
            {
                soDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_AUDIO,0);
                if (soRuntime)
                {
                    soRuntime->removeAudioRuntime();
                }
            }
            ImGui::SameLine();
            if(ImGui::Button(">##Audio"))
            {
                if (selectedAudioAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_AUDIO,selectedAudioAsset);
                pushPropertyTarget(PropertyType_Asset,asset, nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Audio",&selectedAudioAsset,audioAssets,audioAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedAudioAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedAudioAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedDef->getUuid());
                    }
                }
            }

            // Light
            int selectedLightAsset = soDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_LIGHT);
            vector<string> lightAssets = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_LIGHT);

            if(ImGui::Button("-##Light"))
            {
                soDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_LIGHT,0);
                if (soRuntime)
                {
                    soRuntime->removeLightRuntime();
                }

            }
            ImGui::SameLine();

            if(ImGui::Button(">##Light"))
            {
                if (selectedLightAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_LIGHT,selectedLightAsset);
                pushPropertyTarget(PropertyType_Asset, asset, nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Light",&selectedLightAsset,lightAssets,lightAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_LIGHT, selectedLightAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_LIGHT, selectedLightAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_LIGHT, selectedDef->getUuid());
                    }
                }
            }

            // Model
            int selectedModelAsset = soDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_MODEL);
            vector<string> modelAssets = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MODEL);

            if(ImGui::Button("-##Model"))
            {
                soDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_MODEL,0);
                if (soRuntime)
                {
                    soRuntime->removeModelRuntime();
                }

            }
            ImGui::SameLine();

            if(ImGui::Button(">##Model"))
            {
                if (selectedModelAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL,selectedModelAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Model",&selectedModelAsset,modelAssets,modelAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_MODEL, selectedDef->getUuid());
                    }
                }
            }

            // Path
            int selectedPathAsset = soDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PATH);
            vector<string> pathAssets = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PATH);
            if(ImGui::Button("-##Path"))
            {
                soDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_PATH,0);
                if (soRuntime)
                {
                    soRuntime->removePathRuntime();
                }

            }
            ImGui::SameLine();

            if(ImGui::Button(">##Path"))
            {
                if (selectedPathAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PATH,selectedPathAsset);
                pushPropertyTarget(PropertyType_Asset, asset, nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Path",&selectedPathAsset,pathAssets,pathAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PATH, selectedPathAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(ASSET_TYPE_ENUM_PATH, selectedPathAsset);
                    soRuntime->replaceAssetUuid(ASSET_TYPE_ENUM_PATH, selectedDef->getUuid());
                }
            }

            //Physics Object
            int selectedPhysicsObjectAsset = soDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT);
            vector<string> poAssets = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT);
            if(ImGui::Button("-##PhysicsObject"))
            {
                soDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,0);
                if (soRuntime)
                {
                    soRuntime->removePhysicsObjectRuntime();
                }
            }
            ImGui::SameLine();

            if(ImGui::Button(">##PhysicsObject"))
            {
                if (selectedPhysicsObjectAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,selectedPhysicsObjectAsset);
                pushPropertyTarget(PropertyType_Asset, asset, nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Physics Object",&selectedPhysicsObjectAsset,poAssets,poAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,selectedPhysicsObjectAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT, selectedPhysicsObjectAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT, selectedDef->getUuid());
                    }
                }
            }

            // Script
            int selectedScriptAsset = soDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT);
            vector<string> scriptAssets = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SCRIPT);
            if(ImGui::Button("-##Script"))
            {
                soDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_SCRIPT,0);
                if (soRuntime)
                {
                    soRuntime->removeScriptRuntime();
                }
            }
            ImGui::SameLine();

            if(ImGui::Button(">##Script"))
            {
                if (selectedScriptAsset < 0) return;
                auto asset = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,selectedScriptAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Script",&selectedScriptAsset,scriptAssets,scriptAssets.size()))
            {
                soDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedScriptAsset);
                if (soRuntime)
                {
                    auto selectedDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedScriptAsset);
                    if (selectedDef)
                    {
                        soRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedDef->getUuid());
                    }
                }
            }

            ImGui::SameLine();

        }
    }

    void
    PropertiesWindow::drawImGizmo
    ()
    {
        float *matrix = nullptr;
        auto soRunt = dynamic_cast<EntityRuntime*>(mRuntime);
        auto soDef = dynamic_cast<EntityDefinition*>(mDefinition);

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

            ImGui::InputFloat3("Translation", matrixTranslation);
            ImGui::InputFloat3("Rotation", matrixRotation);
            ImGui::InputFloat3("Scale", matrixScale);

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
                        soRunt->applyToAll(
                        	function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime* rt){
								if (rt != soRunt){
									auto d = dynamic_cast<EntityDefinition*>(rt->getDefinition());
									d->setTransform(rt->getTransform());
								}
								return static_cast<EntityRuntime*>(nullptr);
							}));
                    }
                }
            }

            ImGui::NextColumn();
            if(ImGui::Button("Restore Initial"))
            {
                if (soDef && soRunt)
                {
                    auto tmp = soDef->getTransform();
                    soRunt->setTransform(&tmp);
                    if (withChildren)
                    {
                        soRunt->applyToAll
                                (
                                    function<EntityRuntime*(EntityRuntime*)>(
                                        [&](EntityRuntime* rt)
                        {
                                        if (rt != soRunt)
                                        {
                                            auto d = dynamic_cast<EntityDefinition*>(rt->getDefinition());
                                            auto tmp = d->getTransform();
                                            rt->setTransform(&tmp);
                                        }
                                        return static_cast<EntityRuntime*>(nullptr);
                                    }
                                    ));
                    }

                }

            }

            ImGui::Columns(1);
        }

        auto pRunt = mContext->getProject()->getRuntime();
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
                        Vector3 tx(delta[3][0],delta[3][1],delta[3][2]);
                        soRunt->applyToAll
                                (
                                    function<EntityRuntime*(EntityRuntime*)>(
                                        [&](EntityRuntime* rt)
                        {
                                        if (rt != soRunt)
                                        {
                                            rt->getTransform().preTranslate(tx);
                                        }
                                        return static_cast<EntityRuntime*>(nullptr);
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
        float* matrix = ccd.transform.getMatrixFloatPointer();

        static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

        ImGui::InputFloat3("##Translation", matrixTranslation);
        ImGui::SameLine();
        if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
        {
            currentGizmoOperation = ImGuizmo::TRANSLATE;
        }

        ImGui::InputFloat3("##Rotation", matrixRotation);
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
        {
            currentGizmoOperation = ImGuizmo::ROTATE;
        }

        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

        auto pRunt = mContext->getProject()->getRuntime();
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
                    replaceRuntimes(ccd.parent);
                }
            }
        }
    }

    void
    PropertiesWindow::drawAssetProperties
    ()
    {
        mContext->getPathViewer()->setVisible(false);
        mContext->getAnimationViewer()->setVisible(false);
        auto assetDef = dynamic_cast<AssetDefinition*>(mDefinition);
        if (assetDef == nullptr)
        {
            return;
        }

        ImGui::SameLine();
        if(ImGui::Button("Delete Asset"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(assetDef);
            auto projDef = mContext->getProject()->getDefinition();
            projDef->removeAssetDefinition(assetDef);
            removeFromHistory(mDefinition);
            mDefinition = nullptr;
            mRuntime = nullptr;
            projDef->regroupAssetDefinitions();
            return;
        }
        ImGui::SameLine();
        if (ImGui::Button("Duplicate"))
        {
            auto dup = assetDef->duplicate();
            auto projDef = mContext->getProject()->getDefinition();
            pushPropertyTarget(PropertyType_Entity,dup,nullptr);
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
            case AssetType::ASSET_TYPE_ENUM_ANIMATION:
                drawAnimationAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_AUDIO:
                drawAudioAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_FONT:
                drawFontAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_LIGHT:
                drawLightAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_MATERIAL:
                drawMaterialAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_MODEL:
                drawModelAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT:
                drawPhysicsObjectAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_SCRIPT:
                drawScriptProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_SHADER:
                drawShaderAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_PATH:
                drawPathAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_TEXTURE:
                drawTextureAssetProperties();
                break;
            case AssetType::ASSET_TYPE_ENUM_NONE:
                break;
        }

        auto pRunt = mContext->getProject()->getRuntime();
        if (pRunt)
        {
            auto activeScene = pRunt->getActiveSceneRuntime();
            if (activeScene)
            {
                ImGui::Separator();
                ImGui::Columns(1);
                if(ImGui::CollapsingHeader("Active Runtimes"))
                {
                    auto runtimes = activeScene->getEntitysWithRuntimeOf(assetDef);
                    for (auto runtime : runtimes)
                    {
                        ImGui::Text("%s",runtime->getNameAndUuidString().c_str());
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
        mContext->getAnimationViewer()->setAnimationDefinition(animDef);
        mContext->getAnimationViewer()->setVisible(true);

        ImGui::Columns(2);

        bool relative = animDef->getRelative();
        if (ImGui::Checkbox("Relative to Origin",&relative))
        {
            animDef->setRelative(relative);
        }

        ImGui::NextColumn();

        if (ImGui::Button("Add Keyframe"))
        {
            animDef->addKeyframe(AnimationKeyframe(animDef->nextKeyframeID()));
        }

        ImGui::Separator();

        ImGui::Columns(5);
        // Table Header
        ImGui::Text("Remove");
        ImGui::NextColumn();

        ImGui::Text("Time (ms)");
        ImGui::NextColumn();

        ImGui::Text("Transform");
        ImGui::NextColumn();

        ImGui::Text("Edit");
        ImGui::NextColumn();

        ImGui::Text("Easing");
        ImGui::NextColumn();

        ImGui::Separator();

        static int selected = -1;

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

            if(ImGui::Button("Edit"))
            {
               selected = kf.getID();
            }
            ImGui::NextColumn();

            // Easing
            auto easingTypes = AnimationEasing::EasingNames;
            int currentEasingType = kf.getEasingType();
            if (StringCombo("##EasingType", &currentEasingType, easingTypes,easingTypes.size()))
            {
                kf.setEasingType(static_cast<AnimationEasing::EasingType>(currentEasingType));
                animDef->updateKeyframe(kf);
            }

            ImGui::NextColumn();
            ImGui::Separator();
            ImGui::PopID();
            if (selected == kf.getID())
            {
                drawAnimationKeyframeImGuizmo(animDef, kf);
            }
            mContext->getAnimationViewer()->regenerate();
        }
        ImGui::Columns(1);
    }

    void
    PropertiesWindow::drawAnimationKeyframeImGuizmo
    (AnimationDefinition* def, AnimationKeyframe kf)
    {
        mat4 mtx(1.0f);
        mtx = glm::translate(mtx, kf.getTranslation());
        float* matrix = glm::value_ptr(mtx);

        static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);

        auto pRunt = mContext->getProject()->getRuntime();
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
                        nullptr
                    );
                    kf.setTranslation(vec3(mtx[3]));
                    def->updateKeyframe(kf);
                }
            }
        }
    }

    void
    PropertiesWindow::drawAudioAssetProperties
    ()
    {
        bool selectAudioFile = ImGui::Button("Audio File...");
        auto audioDef = dynamic_cast<AudioDefinition*>(mDefinition);
        if (selectAudioFile)
        {
            nfdchar_t *audioFilePath = NULL;
            nfdfilteritem_t filter[2] = {{"Ogg Files", "ogg"}, {"WAVE Files", "wav"}};
			nfdresult_t result = NFD_OpenDialog(&audioFilePath, filter, 2, mContext->getLastDirectory().c_str() );

			if ( result == NFD_OKAY )
			{
				LOG_INFO("PropertiesWindow: Success! {}",audioFilePath);

				LOG_DEBUG("PropertiesWindow: Opening Audio File {}",audioFilePath);
                StorageManager* fm = mContext->getStorageManager();
				File* audioFile = fm->openFile(audioFilePath);
				mContext->setLastDirectory(audioFile->getDirectory());

				LOG_DEBUG("PropertiesWindow: Setting last directory {}",mContext->getLastDirectory());
				audioFile->readBinary();
				auto audioData = audioFile->getBinaryData();
                auto audioDataSize = audioFile->getBinaryDataSize();
				mContext->getProjectDirectory()->writeAssetData(audioDef,audioData, audioDataSize);
				audioDef->setName(audioFile->getNameWithoutExtension());
				NFD_FreePath(audioFilePath);
                fm->closeFile(audioFile);
			}
			else if ( result == NFD_CANCEL )
			{
				LOG_DEBUG("PropertiesWindow: User pressed cancel.");
			}
			else
			{
				LOG_ERROR("PropertiesWindow: Error: %s\n", NFD_GetError() );
			}
        }

        auto projRunt = mContext->getProject()->getRuntime();

        AudioRuntime* audioRunt = nullptr;
        if (projRunt)
        {
            auto audioCache = projRunt->getAudioCache();
            audioRunt = dynamic_cast<AudioRuntime*>(audioCache->getRuntime(audioDef));
        }


        ImGui::SameLine();


        if(ImGui::Button("Remove File"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(audioDef);
        }

        ImGui::Separator();

        if (ImGui::Button("Play"))
        {
            if (audioRunt)
            {
                audioRunt->play();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause"))
        {
            if (audioRunt)
            {
                audioRunt->pause();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            if (audioRunt)
            {
                audioRunt->stop();
            }
        }

        ImGui::PushItemWidth(-1);
        int audioProg = audioRunt->getSampleOffset();
        int duration = audioRunt->getDurationInSamples();
        if(ImGui::SliderInt("#AudioProgress", &audioProg,0,duration,"%d"))
        {
            audioRunt->setSampleOffset(audioProg);
        }
        ImGui::PopItemWidth();
    }

    void
    PropertiesWindow::drawFontAssetProperties
    ()
    {
        bool selectFile = ImGui::Button("Font File...");
        auto def = dynamic_cast<FontDefinition*>(mDefinition);

        if (selectFile)
        {
            nfdchar_t *filePath = NULL;
            nfdfilteritem_t filters[2] = {{"TrueType", "ttf"}, {"OpenType", "otf"}};
			nfdresult_t result = NFD_OpenDialog(&filePath, filters, 2, mContext->getLastDirectory().c_str());

			if ( result == NFD_OKAY )
			{
				LOG_INFO("PropertiesWindow: Success! {}",filePath);
				LOG_DEBUG("PropertiesWindow: Opening Font File {}",filePath);
                StorageManager* fm = mContext->getStorageManager();
				File* file = fm->openFile(filePath);
				mContext->setLastDirectory(file->getDirectory());
				file->readBinary();
				auto data = file->getBinaryData();
				auto dataSize = file->getBinaryDataSize();
				mContext->getProjectDirectory()->writeAssetData(def,data,dataSize);
				def->setName(file->getNameWithoutExtension());
                fm->closeFile(file);
				NFD_FreePath(filePath);
			}
			else if ( result == NFD_CANCEL )
			{
				LOG_DEBUG("PropertiesWindow: User pressed cancel.");
			}
			else
			{
				LOG_ERROR("PropertiesWindow: Error: %s\n", NFD_GetError() );
			}
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(def);
        }
    }

    void PropertiesWindow::drawLightAssetProperties
    ()
    {
        auto lightDef = dynamic_cast<LightDefinition*>(mDefinition);

        vector<string> lightTypes = Constants::DREAM_ASSET_FORMATS_MAP[AssetType::ASSET_TYPE_ENUM_LIGHT];
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
        auto projDef = mContext->getProject()->getDefinition();
        auto projRunt = mContext->getProject()->getRuntime();
        auto materialDef = dynamic_cast<MaterialDefinition*>(mDefinition);

        int shaderIndex = 0;
        vector<string> shaderList;
        if (projDef)
        {
            auto shaderAsset = projDef->getAssetDefinitionByUuid(materialDef->getShader());
            shaderIndex = projDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderAsset);
            shaderList = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);
        }

        if(StringCombo("Shader", &shaderIndex, shaderList, shaderList.size()))
        {
            if (projDef)
            {
                auto newShader = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderIndex);
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

        uint32_t diffuseUuid = materialDef->getDiffuseTexture();
        uint32_t specularUuid = materialDef->getSpecularTexture();
        uint32_t normalUuid = materialDef->getNormalTexture();
        uint32_t displacementUuid = materialDef->getDisplacementTexture();
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
                auto txRuntime = dynamic_cast<TextureRuntime*>(txCache->getRuntime(txDef));
                if (txRuntime)
                {
                    diffuseTxId = (void*)(intptr_t)txRuntime->getGLID();
                }
            }

            // Specular
            auto specularDef = projDef->getAssetDefinitionByUuid(specularUuid);
            if (specularDef)
            {
                auto txDef = dynamic_cast<TextureDefinition*>(specularDef);
                auto txCache = projRunt->getTextureCache();
                auto txRuntime = dynamic_cast<TextureRuntime*>(txCache->getRuntime(txDef));
                if (txRuntime)
                {
                    specularTxId = (void*)(intptr_t)txRuntime->getGLID();
                }
            }

            // Normal
            auto normalDef = projDef->getAssetDefinitionByUuid(normalUuid);
            if (normalDef)
            {
                auto txDef = dynamic_cast<TextureDefinition*>(normalDef);
                auto txCache = projRunt->getTextureCache();
                auto txRuntime = dynamic_cast<TextureRuntime*>(txCache->getRuntime(txDef));
                if (txRuntime)
                {
                    normalTxId = (void*)(intptr_t)txRuntime->getGLID();
                }
            }

            // Displacement
            auto displacementDef = projDef->getAssetDefinitionByUuid(displacementUuid);
            if (displacementDef)
            {
                auto txDef = dynamic_cast<TextureDefinition*>(displacementDef);
                auto txCache = projRunt->getTextureCache();
                auto txRuntime = dynamic_cast<TextureRuntime*>(txCache->getRuntime(txDef));
                if (txRuntime)
                {
                    displacementTxId = (void*)(intptr_t)txRuntime->getGLID();
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
            textures = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_TEXTURE);
            diffuseIndex = projDef->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projDef->getAssetDefinitionByUuid(diffuseUuid)
                        );
            specularIndex = projDef->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projDef->getAssetDefinitionByUuid(specularUuid)
                        );
            normalIndex = projDef->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projDef->getAssetDefinitionByUuid(normalUuid)
                        );
            displacementIndex = projDef->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projDef->getAssetDefinitionByUuid(displacementUuid)
                        );
        }

        // Diffuse
        if(StringCombo("Diffuse",&diffuseIndex,textures,textures.size()))
        {
            if (projDef)
            {
                auto txDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, diffuseIndex);
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
                auto txDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, specularIndex);
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
                auto txDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, normalIndex);
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
                auto txDef = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, displacementIndex);
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
        auto def = dynamic_cast<ModelDefinition*>(mDefinition);

        bool selectFile = ImGui::Button("Model File...");

        if (selectFile)
        {
            nfdchar_t *filePath = NULL;
			nfdresult_t result = NFD_OpenDialog(&filePath, nullptr, 0, mContext->getLastDirectory().c_str());

			if ( result == NFD_OKAY )
			{
				LOG_ERROR("PropertiesWindow: Opening Model File {}",filePath);
                StorageManager* fm = mContext->getStorageManager();
				File* file = fm->openFile(filePath);
				mContext->setLastDirectory(file->getDirectory());
				file->readBinary();
				auto data = file->getBinaryData();
				auto dataSize = file->getBinaryDataSize();
				def->setFormat(Constants::ASSET_TYPE_MODEL+"."+file->getExtension());
				def->setName(file->getNameWithoutExtension());
				mContext->getProjectDirectory()->writeAssetData(def,data,dataSize);
                fm->closeFile(file);
				NFD_FreePath(filePath);
			}
			else if ( result == NFD_CANCEL )
			{
				LOG_DEBUG("PropertiesWindow: User pressed cancel.");
			}
			else
			{
				LOG_ERROR("PropertiesWindow: Error: %s\n", NFD_GetError() );
			}
        }

        ImGui::SameLine();

        bool selectAdditionalFile = ImGui::Button("Additional File...");

        if (selectAdditionalFile)
        {
            nfdchar_t *filePath = NULL;
			nfdresult_t result = NFD_OpenDialog(&filePath, nullptr, 0,mContext->getLastDirectory().c_str());

			if ( result == NFD_OKAY )
			{
				LOG_ERROR("PropertiesWindow: Opening Model File {}",filePath);
				LOG_ERROR("PropertiesWindow: Opening Additional Model File {}",filePath);
                StorageManager* fm = mContext->getStorageManager();
				File* file = fm->openFile(filePath);
				mContext->setLastDirectory(file->getDirectory());
				file->readBinary();
				auto data = file->getBinaryData();
				auto dataSize = file->getBinaryDataSize();
				mContext->getProjectDirectory()->writeAssetData(def,data,dataSize,file->getNameWithExtension());
                fm->closeFile(file);
				NFD_FreePath(filePath);
			}
			else if ( result == NFD_CANCEL )
			{
				LOG_DEBUG("PropertiesWindow: User pressed cancel.");
			}
			else
			{
				LOG_ERROR("PropertiesWindow: Error: %s\n", NFD_GetError() );
			}
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File(s)"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(def);
        }

        ImGui::Text("Model Format: %s", def->getFormat().c_str());

        vector<string> modelMaterialNames;
        map<string,Bone> bones;
        auto projRunt = mContext->getProject()->getRuntime();
        if (projRunt)
        {
            auto modelCache = projRunt->getModelCache();
            if (modelCache)
            {
                auto modelRuntime = dynamic_cast<ModelRuntime*>(modelCache->getRuntime(def));
                if (modelRuntime)
                {
                    modelMaterialNames = modelRuntime->getMaterialNames();
                    bones = modelRuntime->getBones();
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
        auto projDef =  mContext->getProject()->getDefinition();
        if (projDef)
        {
            materialAssetNames = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MATERIAL);
        }

        for (string modelMaterial : modelMaterialNames)
        {
            ImGui::Text("%s",modelMaterial.c_str());
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            auto currentMaterialUuid = def->getDreamMaterialForModelMaterial(modelMaterial);
            auto currentMaterialDef = projDef->getAssetDefinitionByUuid(currentMaterialUuid);
            int currentMaterialIndex = projDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MATERIAL,currentMaterialDef);
            string itemName = "##Material:" + modelMaterial;
            if(StringCombo(itemName.c_str(),&currentMaterialIndex,materialAssetNames,materialAssetNames.size()))
            {
                auto changedMaterial = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MATERIAL, currentMaterialIndex);
                def->addModelMaterial(modelMaterial,changedMaterial->getUuid());

                LOG_ERROR("PropertiesWindow: Changed {} material {} to map to {}",def->getName(), modelMaterial, changedMaterial->getNameAndUuidString() );
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
        vector<string> poFormats = Constants::DREAM_ASSET_FORMATS_MAP[AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT];
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

        float linearFactor[3] ={
            pod->getLinearFactor().x(),
            pod->getLinearFactor().y(),
            pod->getLinearFactor().z()
        };
        if (ImGui::DragFloat3("Linear Factor", &linearFactor[0],0.1f))
        {
            Vector3 lf(linearFactor[0],linearFactor[1],linearFactor[2]);
            if (pod)
            {
                pod->setLinearFactor(lf);
                modified = true;
            }
        }

        float angularFactor[3] ={
            pod->getAngularFactor().x(),
            pod->getAngularFactor().y(),
            pod->getAngularFactor().z()
        };
        if (ImGui::DragFloat3("Angular Factor", &angularFactor[0],0.1f))
        {
            Vector3 af(angularFactor[0], angularFactor[1],angularFactor[2]);
            if (pod)
            {
                pod->setAngularFactor(af);
                modified = true;
            }
        }

        float linearVelocity[3] ={
            pod->getLinearVelocity().x(),
            pod->getLinearVelocity().y(),
            pod->getLinearVelocity().z()
        };
        if (ImGui::DragFloat3("Linear Velocity", &linearVelocity[0],0.1f))
        {
            Vector3 lf(linearVelocity[0],linearVelocity[1],linearVelocity[2]);
            if (pod)
            {
                pod->setLinearVelocity(lf);
                modified = true;
            }
        }

        float angularVelocity[3] ={
            pod->getAngularVelocity().x(),
            pod->getAngularVelocity().y(),
            pod->getAngularVelocity().z()
        };
        if (ImGui::DragFloat3("Angular Velocity", &angularVelocity[0],0.1f))
        {
            Vector3 af(angularVelocity[0], angularVelocity[1],angularVelocity[2]);
            if (pod)
            {
                pod->setAngularVelocity(af);
                modified = true;
            }
        }


        ImGui::Separator();

        if (pod->getFormat().compare(Constants::COLLISION_SHAPE_BOX) == 0)
        {
            float halfExtents[3] = {
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
            auto projDef = mContext->getProject()->getDefinition();

            uint32_t selectedModelAssetUuid = pod->getCollisionModel();
            AssetDefinition* selectedModelAssetDef = projDef->getAssetDefinitionByUuid(selectedModelAssetUuid);
            int selectedModelAssetIndex = projDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAssetDef);
            vector<string> modelAssets = projDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MODEL);

            if(StringCombo("Model",&selectedModelAssetIndex,modelAssets,modelAssets.size()))
            {
                AssetDefinition* newlySelected = projDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAssetIndex);
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

            if (ImGui::InputFloat3("Plane Normal",&normal[0]))
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
            auto pDef = mContext->getProject()->getDefinition();
            if (pDef)
            {
                auto shapeNames = pDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT);
                static int shapeNameIndex = -1;
                StringCombo("Shape",&shapeNameIndex,shapeNames,shapeNames.size());

                ImGui::NextColumn();
                if(ImGui::Button("Add Compound Child"))
                {
                    if (shapeNameIndex >= 0)
                    {
                        auto childDef = pDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,shapeNameIndex);
                        pod->addCompoundChild(CompoundChildDefinition
                        {
                            pod,Transform(),childDef->getUuid()
                        });
                    }
                    modified = true;
                }
                ImGui::Separator();
                ImGui::Columns(1);
                ImGui::Text("Child Shapes");
                ImGui::Separator();

                auto shapes = pod->getCompoundChildren();
                static uint32_t selectedToTransform = 0;
                for (auto shape : shapes)
                {
                    auto shapeDef = pDef->getAssetDefinitionByUuid(shape.uuid);

                    if (!shapeDef)
                    {
                        continue;
                    }
                    ImGui::PushID(shape.uuid);
                    ImGui::SetNextTreeNodeOpen(selectedToTransform == shape.uuid);
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

            replaceRuntimes(pod);
        }
    }

    void
    PropertiesWindow::drawScriptProperties
    ()
    {
        auto scriptDef = dynamic_cast<ScriptDefinition*>(mDefinition);

        ImGui::PushItemWidth(-1);
        if(ImGui::Button("Open Script Editor..."))
        {
            mContext->getScriptEditorWindow()->setScriptDefinition(scriptDef);
            mContext->getScriptEditorWindow()->setVisible(true);
        }
        ImGui::PopItemWidth();
    }

    void
    PropertiesWindow::drawShaderAssetProperties
    ()
    {
        auto shaderDef = dynamic_cast<ShaderDefinition*>(mDefinition);
        auto projRunt = mContext->getProject()->getRuntime();
        ShaderRuntime* shaderInst = nullptr;
        if (projRunt)
        {
            auto shaderCache = projRunt->getShaderCache();
            if (shaderCache)
            {
                shaderInst = dynamic_cast<ShaderRuntime*>(shaderCache->getRuntime(shaderDef));
            }
        }
        ImGui::PushItemWidth(-1);
        if(ImGui::Button("Open Shader Editor..."))
        {
            mContext->getShaderEditorWindow()->setShaderDefinition(shaderDef);
            mContext->getShaderEditorWindow()->setVisible(true);
        }
        ImGui::PopItemWidth();
    }

    void
    PropertiesWindow::drawPathAssetProperties
    ()
    {
        bool needsRegen = false;
        auto* pathDef = static_cast<PathDefinition*>(mDefinition);
        mContext->getPathViewer()->setPathDefinition(pathDef);
        mContext->getPathViewer()->setVisible(true);

        float stepScalar = pathDef->getStepScalar();
        if (ImGui::DragFloat("Step Scalar",&stepScalar))
        {
            pathDef->setStepScalar(stepScalar >= 1.0f ? stepScalar : 1.0f);
            needsRegen = true;
        }

        int splineTypeIndex = getStringIndexInVector(pathDef->getSplineType(),Constants::DREAM_PATH_SPLINE_TYPES);
        if (StringCombo("Spline Type",&splineTypeIndex, Constants::DREAM_PATH_SPLINE_TYPES,Constants::DREAM_PATH_SPLINE_TYPES.size()))
        {
            pathDef->setSplineType(Constants::DREAM_PATH_SPLINE_TYPES.at(splineTypeIndex));
            needsRegen = true;
        }

        bool wrap = pathDef->getWrap();
        if (ImGui::Checkbox("Wrap",&wrap))
        {
           pathDef->setWrap(wrap);
            needsRegen = true;
        }

        float velocity = pathDef->getVelocity();
        if (ImGui::DragFloat("Velocity",&velocity))
        {
           pathDef->setVelocity(velocity);
            needsRegen = true;
        }

        ImGui::Text("Control Points");
        if (ImGui::Button("Add"))
        {
            pathDef->addControlPoint();
            needsRegen = true;
        }
        ImGui::Separator();

        ImGui::Columns(3);
        ImGui::Text("Remove (id)");
        ImGui::NextColumn();
        ImGui::Text("Index");
        ImGui::NextColumn();
        ImGui::Text("Translation");
        ImGui::NextColumn();

        static int selected = -1;

        for (auto cp : pathDef->getControlPoints())
        {
            bool modified = false;
            ImGui::PushID(cp.id);

            // Remove
            if (ImGui::Button("-##remove"))
            {
                pathDef->deleteControlPoint(cp);
            }
            ImGui::SameLine();
            ImGui::Text("(%d)",cp.id);
            ImGui::NextColumn();

            // Index
            int index = cp.index;
            ImGui::PushItemWidth(-1);
            if(ImGui::InputInt("##index",&index))
            {
                cp.index = index;
                modified = true;
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            // Tx
            if (ImGui::Button("Edit"))
            {
                selected = cp.id;
            }
            ImGui::SameLine();
            Vector3 vTx = cp.position;
            float tx[3] = {vTx.x(), vTx.y(), vTx.z()};
            if (ImGui::InputFloat3("##position",&tx[0]))
            {
                vTx.setX(tx[0]);
                vTx.setY(tx[1]);
                vTx.setZ(tx[2]);
                cp.position = vTx;
                modified = true;
            }
            ImGui::NextColumn();
            ImGui::PopID();
            if (modified)
            {
                pathDef->updateControlPoint(cp);
                needsRegen = true;
            }
            if (selected == cp.id)
            {
                mContext->getPathViewer()->setSelectedControlPoint(cp.id);
                drawPathControlPointImGuizmo(pathDef,cp);
                needsRegen = true;
            }
        }
        if (needsRegen)
        {
            replaceRuntimes(pathDef);
            mContext->getPathViewer()->regenerate();
        }
        ImGui::Columns(1);
        if (ImGui::Button("Deselect"))
        {
            selected = -1;
        }
    }

   void
   PropertiesWindow::drawPathControlPointImGuizmo
   (PathDefinition* pDef, PathControlPoint cp)
   {
        mat4 mtx(1.0f);
        mtx = glm::translate(mtx, cp.position.toGLM());
        float* matrix = glm::value_ptr(mtx);

        static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);

        auto pRunt = mContext->getProject()->getRuntime();
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
                        nullptr
                    );
                    cp.position = Vector3(mtx[3][0],mtx[3][1],mtx[3][2]);
                    pDef->updateControlPoint(cp);
                }
            }
        }
   }

    void
    PropertiesWindow::drawTextureAssetProperties
    ()
    {
        void* textureId = nullptr;

        auto textureDef = dynamic_cast<TextureDefinition*>(mDefinition);
        auto projRunt = mContext->getProject()->getRuntime();
        if (projRunt)
        {
            auto txCache = projRunt->getTextureCache();
            auto txRuntime = dynamic_cast<TextureRuntime*>(txCache->getRuntime(textureDef));
            if (txRuntime)
            {
                textureId = (void*)(intptr_t)txRuntime->getGLID();
            }
        }

        bool selectFile = ImGui::Button("Texture File...");
        if (selectFile)
        {
            nfdchar_t *filePath = NULL;
			nfdresult_t result = NFD_OpenDialog(&filePath, nullptr, 0,mContext->getLastDirectory().c_str());

			if ( result == NFD_OKAY )
			{
				LOG_ERROR("PropertiesWindow: Opening Texture File {}",filePath);
                StorageManager* fm = mContext->getStorageManager();
				File* file = fm->openFile(filePath);
				mContext->setLastDirectory(file->getDirectory());
				file->readBinary();
				auto data = file->getBinaryData();
				auto dataSize = file->getBinaryDataSize();
				mContext->getProjectDirectory()->writeAssetData(textureDef,data,dataSize);
				textureDef->setName(file->getNameWithoutExtension());
                fm->closeFile(file);
				NFD_FreePath(filePath);
			}
			else if ( result == NFD_CANCEL )
			{
				LOG_DEBUG("PropertiesWindow: User pressed cancel.");
			}
			else
			{
				LOG_ERROR("PropertiesWindow: Error: %s\n", NFD_GetError() );
			}
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(textureDef);
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
    PropertiesWindow::replaceRuntimes
    (AssetDefinition* assetDef)
    {
        auto pRunt = mContext->getProject()->getRuntime();
        if (pRunt)
        {
            auto sRunt = pRunt->getActiveSceneRuntime();
            if (sRunt)
            {
                auto runts = sRunt->getEntitysWithRuntimeOf(assetDef);
                for (auto soRunt : runts)
                {
                    soRunt->replaceAssetUuid(assetDef->getAssetType(),assetDef->getUuid());
                }
            }
        }
    }
}
