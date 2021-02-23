// Maintain include order for GL Defined

#include "PropertiesWindow.h"
#include "glm/gtc/type_ptr.hpp"
#include <nfd.h>
#include "DreamToolContext.h"
#include <DreamCore.h>

using glm::degrees;
using glm::radians;
using octronic::dream::FontRuntime;

namespace octronic::dream::tool
{
    PropertiesWindow::PropertiesWindow
    (DreamToolContext* proj)
        : ImGuiWidget (proj),
          mType(PropertyType_None),
          mDefinition(nullptr),
          mRuntime(nullptr),
          mImageSize(256,256),
          mBigEditorSize(-1,-1)
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
        EntityDefinition* entityDefinition = static_cast<EntityDefinition*>(mDefinition);
        EntityRuntime* entityRuntimeime = static_cast<EntityRuntime*>(mRuntime);

        if (ImGui::Button("Delete"))
        {
            if (entityDefinition)
            {
                EntityDefinition* parent = entityDefinition->getParentEntity();
                if (parent)
                {
                    parent->removeChildDefinition(entityDefinition);
                }
            }
            if (entityRuntimeime)
            {
                EntityRuntime* parent = entityRuntimeime->getParentRuntime();
                parent->removeChildRuntime(entityRuntimeime);
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
        SceneDefinition* sDef = static_cast<SceneDefinition*>(mDefinition);
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
        LOG_INFO("PropertiesWindow: Pushed target {}",mHistory.size());
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
        ProjectDefinition* projectDefinition = static_cast<ProjectDefinition*>(mDefinition);

        if (projectDefinition == nullptr)
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
        SceneDefinition* startup = projectDefinition->getStartupSceneDefinition();
        int startupScene = projectDefinition->getSceneDefinitionIndex(startup);
        vector<string> scenes;
        for(SceneDefinition* scene : projectDefinition->getSceneDefinitionsVector())
        {
            scenes.push_back(scene->getName());
        }
        if(StringCombo("Startup Scene",&startupScene,scenes,scenes.size()))
        {
            startup = projectDefinition->getSceneDefinitionAtIndex(startupScene);
            projectDefinition->setStartupSceneUuid(startup->getUuid());
        }
        ImGui::Separator();
    }

    void
    PropertiesWindow::drawSceneProperties
    ()
    {
        SceneDefinition* sceneDef = static_cast<SceneDefinition*>(mDefinition);
        SceneRuntime* sceneRuntime = static_cast<SceneRuntime*>(mRuntime);

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
            Camera* cam = sceneRuntime->getCamera();
            Transform camTransf = cam->getTransform();
            vec3 tx = camTransf.getTranslation();
            cameraTranslation[0] = tx.x;
            cameraTranslation[1] = tx.y;
            cameraTranslation[2] = tx.z;
        }
        else
        {
            Transform camTransf = sceneDef->getCameraTransform();
            vec3 tx = camTransf.getTranslation();
            cameraTranslation[0] = tx.x;
            cameraTranslation[1] = tx.y;
            cameraTranslation[2] = tx.z;
        }

        if(ImGui::CollapsingHeader("Camera"))
        {
            if(ImGui::DragFloat3("Translation",cameraTranslation,0.1f))
            {
                if (sceneRuntime)
                {
                    Camera* cam = sceneRuntime->getCamera();
                    Transform cam_tx = cam->getTransform();
                    cam_tx.setTranslation(vec3(cameraTranslation[0],cameraTranslation[1],cameraTranslation[2]));
                    cam->setTransform(cam_tx);
                }
                else
                {
                    Transform cam_tx = sceneDef->getCameraTransform();
                    cam_tx.setTranslation(vec3(cameraTranslation[0],cameraTranslation[1],cameraTranslation[2]));
                    sceneDef->setCameraTransform(cam_tx);
                }
            }

            float cam_yaw, cam_pitch, cam_roll;

            if (sceneRuntime)
            {
                Camera* cam = sceneRuntime->getCamera();
                Transform cam_tx = cam->getTransform();
                cam_yaw   = degrees(cam_tx.getYaw());
                cam_pitch = degrees(cam_tx.getPitch());
                cam_roll  = degrees(cam_tx.getRoll());
            }
            else
            {
                Transform cam_tx = sceneDef->getCameraTransform();
                cam_yaw   = degrees(cam_tx.getYaw());
                cam_pitch = degrees(cam_tx.getPitch());
                cam_roll  = degrees(cam_tx.getRoll());
            }

            bool rot_changed = false;
            rot_changed |= ImGui::DragFloat("Yaw",   &cam_yaw,   1.f);
            rot_changed |= ImGui::DragFloat("Pitch", &cam_pitch, 1.f);
            rot_changed |= ImGui::DragFloat("Roll",  &cam_roll,  1.f);

            if(rot_changed)
            {
                Transform tx;
                if (sceneDef != nullptr)
                {
                    tx = sceneDef->getCameraTransform();
                    tx.setYaw(radians(cam_yaw));
                    tx.setPitch(radians(cam_pitch));
                    tx.setRoll(radians(cam_roll));
                    sceneDef->setCameraTransform(tx);
                }

                if (sceneRuntime != nullptr)
                {
                    Camera* cam = sceneRuntime->getCamera();
                    tx = cam->getTransform();
                    tx.setYaw(radians(cam_yaw));
                    tx.setPitch(radians(cam_pitch));
                    tx.setRoll(radians(cam_roll));
                    cam->setTransform(tx);
                }
            }

            float fov;
            if (sceneRuntime)
            {
                Camera* cam = sceneRuntime->getCamera();
                fov = degrees(cam->getFieldOfView());
            }
            else
            {
                fov = degrees(sceneDef->getCameraFOV());
            }

            if (ImGui::DragFloat("FOV", &fov, 0.1f, 1.f,180.f))
            {
                sceneDef->setCameraFOV(radians(fov));
                if (sceneRuntime)
                {
                    Camera* cam = sceneRuntime->getCamera();
                    cam->setFieldOfView(radians(fov));
                }
            }

            if(ImGui::Button("Capture Camera Transform"))
            {
                if (sceneRuntime && sceneDef)
                {
                    Camera* camera = sceneRuntime->getCamera();
                    Transform camTx = camera->getTransform();
                    sceneDef->setCameraTransform(camTx);
                }
            }

            if(ImGui::Button("Restore Initial Transform"))
            {
                if (sceneRuntime)
                {
                    Camera* cam = sceneRuntime->getCamera();
                    cam->setTransform(sceneDef->getCameraTransform());
                }
            }


            if (sceneRuntime)
            {
                EntityRuntime* playerObject = sceneRuntime->getEntityRuntimeByUuid(sceneDef->getPlayerObject());
                if (playerObject)
                {
                    ImGui::Text("PlayerObject: %s",playerObject->getNameAndUuidString().c_str());
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

            vec4 clearVec = sceneDef->getClearColor();
            float clear[4] = {clearVec.x, clearVec.y, clearVec.z, clearVec.a};
            if(ImGui::ColorEdit4("Clear Color",clear))
            {
                clearVec.x = clear[0];
                clearVec.y = clear[1];
                clearVec.z = clear[2];
                clearVec.a = clear[3];

                if (sceneDef != nullptr)
                {
                    sceneDef->setClearColor(clearVec);
                }
                if (sceneRuntime)
                {
                    sceneRuntime->setClearColor(clearVec);
                }
            }

            ProjectDefinition* pDef = sceneDef->getProjectDefinition();
            vector<string> shaderList = pDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);

            // Setup ShadowPassShader
            {
                UuidType spShaderUuid = sceneDef->getShadowPassShader();
                ShaderDefinition* spShaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(spShaderUuid));
                int shadowShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,spShaderDef);

                if (StringCombo("Shadow Pass Shader", &shadowShaderIndex, shaderList, shaderList.size()))
                {
                    AssetDefinition* selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shadowShaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    sceneDef->setShadowPassShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched shadow pass shader to {} {}", name, uuid);
                }
            }

            // Setup FontShader
            {
                UuidType fontShaderUuid = sceneDef->getFontShader();
                ShaderDefinition* fontShaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(fontShaderUuid));
                int fontShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,fontShaderDef);

                if (StringCombo("Font Shader", &fontShaderIndex, shaderList, shaderList.size()))
                {
                    AssetDefinition* selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, fontShaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    sceneDef->setFontShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched Font shader to {} {}", name, uuid);
                }
            }

            // Setup Sprite shader
            {
                UuidType spriteShaderUuid = sceneDef->getSpriteShader();
                ShaderDefinition* spriteShaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(spriteShaderUuid));
                int spriteShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,spriteShaderDef);

                if (StringCombo("Sprite Shader", &spriteShaderIndex, shaderList, shaderList.size()))
                {
                    AssetDefinition* selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, spriteShaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    sceneDef->setSpriteShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched Sprite shader to {} {}", name, uuid);
                }
            }


            vector<string> textureList = pDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_TEXTURE);
            // Setup Environment Texture
            {
                UuidType envTexUuid = sceneDef->getEnvironmentTexture();
                TextureDefinition* envTexDef = static_cast<TextureDefinition*>(pDef->getAssetDefinitionByUuid(envTexUuid));
                int envMapTexIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, envTexDef);

                if (StringCombo("Environment Texture", &envMapTexIndex, textureList, textureList.size()))
                {
                    AssetDefinition* selected = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, envMapTexIndex);
                    UuidType uuid = selected->getUuid();
                    string name = selected->getName();
                    sceneDef->setEnvironmentTexture(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched Environment Texture to {} {}", name, uuid);
                }
            }

            // Environment Shader
            {
                UuidType envShaderUuid = sceneDef->getEnvironmentShader();
                ShaderDefinition* envShaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(envShaderUuid));
                int envShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,envShaderDef);

                if (StringCombo("Environment Shader", &envShaderIndex, shaderList, shaderList.size()))
                {
                    AssetDefinition* selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, envShaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    sceneDef->setEnvironmentShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched environment shader to {} {}", name, uuid);
                }
            }

        }

        if (ImGui::CollapsingHeader("Scripting"))
        {
            ProjectDefinition* pDef= mContext->getProject()->getDefinition();
            vector<string> scriptAssets = mContext->getProject()->getDefinition()->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SCRIPT);

            UuidType inputScriptUuid = sceneDef->getInputScript();
            AssetDefinition* inputScriptDef = pDef->getAssetDefinitionByUuid(inputScriptUuid);
            int inputScriptIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,inputScriptDef);
            if (StringCombo("Input Script",&inputScriptIndex,scriptAssets,scriptAssets.size()))
            {
                AssetDefinition* selected = pDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,inputScriptIndex);
                UuidType uuid = selected->getUuid();
                sceneDef->setInputScript(uuid);
            }
        }

        // Physics
        if(ImGui::CollapsingHeader("Physics"))
        {
            bool physicsDebug = sceneDef->getPhysicsDebug();

            if (ImGui::Checkbox("Debug", &physicsDebug))
            {
                if (sceneDef)
                {
                    sceneDef->setPhysicsDebug(physicsDebug);
                }
            }

            float gravityVec[3] =
            {
                sceneDef->getGravity().x,
                sceneDef->getGravity().y,
                sceneDef->getGravity().z
            };

            if (ImGui::DragFloat3("Gravity", &gravityVec[0],0.1f))
            {
                vec3 grav(gravityVec[0],gravityVec[1],gravityVec[2]);
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
        ProjectDefinition* projectDefinition = mContext->getProject()->getDefinition();
        EntityDefinition* entityDef = static_cast<EntityDefinition*>(mDefinition);
        EntityRuntime* entityRuntime = static_cast<EntityRuntime*>(mRuntime);

        // Valid Definition
        if (entityDef == nullptr)
        {
            return;
        }

        // Not the root
        if (entityDef->getParentEntity() != nullptr)
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
            EntityDefinition* newChildDef = entityDef->createNewChildDefinition();
            newChildDef->setTransform(mContext->getCursor()->getTransform());

            EntityRuntime* newRt = nullptr;
            if (entityRuntime)
            {
                newRt = entityRuntime->createAndAddChildRuntime(newChildDef);
                newRt->setTransform(mContext->getCursor()->getTransform());
            }
            pushPropertyTarget(PropertyType_Entity,newChildDef,newRt);
        }

        // Not root
        if (entityDef->getParentEntity() != nullptr)
        {
            ImGui::SameLine();
            if (ImGui::Button("Duplicate"))
            {
                EntityDefinition* dup = entityDef->duplicate();
                EntityRuntime* newRt = nullptr;
                if (entityRuntime)
                {
                    newRt = entityRuntime->createAndAddChildRuntime(dup);
                }
                pushPropertyTarget(PropertyType_Entity,dup,newRt);
            }
        }

        drawNameAndIdProperties();

        ImGui::Separator();

        ImGui::Columns(2);

        if (ImGui::Button("Set as Player Object"))
        {
            if(entityDef)
            {
                SceneDefinition* sDef = entityDef->getSceneDefinition();
                if (sDef)
                {
                    sDef->setPlayerObject(entityDef->getUuid());
                }
            }
            if (entityRuntime)
            {
                SceneRuntime* sceneRuntime = entityRuntime->getSceneRuntime();
                if (sceneRuntime)
                {
                    sceneRuntime->setPlayerEntity(entityRuntime);
                }
            }
        }

        ImGui::NextColumn();

        bool alwaysDraw = entityDef->getAlwaysDraw();
        if (ImGui::Checkbox("Always Draw", &alwaysDraw))
        {
            if(entityDef) entityDef->setAlwaysDraw(alwaysDraw);
            if (entityRuntime) entityRuntime->setAlwaysDraw(alwaysDraw);
        }

        ImGui::NextColumn();

        bool hidden = entityDef->getHidden();
        if(ImGui::Checkbox("Hidden", &hidden))
        {
            if(entityDef) entityDef->setHidden(hidden);
            if(entityRuntime) entityRuntime->setHidden(hidden);
        }

        ImGui::NextColumn();

        bool isTemplate = entityDef->getIsTemplate();
        if(ImGui::Checkbox("Template", &isTemplate))
        {
            if(entityDef) entityDef->setIsTemplate(isTemplate);
        }

        ImGui::Columns(1);


        if (entityDef->getAssetDefinition(AssetType::ASSET_TYPE_ENUM_FONT) != Uuid::INVALID)
        {
            ImGui::Separator();
            ImGui::Text("Text Properties");
            char fontTextBuffer[128] = {0};
            string fontText = entityDef->getFontText();
            strncpy(fontTextBuffer, fontText.c_str(), fontText.size());
            if (ImGui::InputText("Text", fontTextBuffer,128))
            {
                string txt(fontTextBuffer);
                entityDef->setFontText(txt);
                if (entityRuntime)
                {
                    entityRuntime->setFontText(txt);
                }
            }

            vec4 fontColorV = entityDef->getFontColor();
            float fontColor[4] = {fontColorV.r, fontColorV.g, fontColorV.b, fontColorV.a};

            if (ImGui::ColorPicker4("Color", &fontColor[0]))
            {
                fontColorV.r = fontColor[0];
                fontColorV.g = fontColor[1];
                fontColorV.b = fontColor[2];
                fontColorV.a = fontColor[3];
                entityDef->setFontColor(fontColorV);
                if (entityRuntime)
                {

                    entityRuntime->setFontColor(fontColorV);
                }
            }

            float fontScale = entityDef->getFontScale();
            if (ImGui::InputFloat("Font Scale",&fontScale))
            {
                entityDef->setFontScale(fontScale);
                if (entityRuntime)
                {
                    entityRuntime->setFontScale(fontScale);
                }
            }
        }

        if (entityRuntime && entityRuntime->hasAnimationRuntime())
        {
            ImGui::Separator();

            AnimationRuntime* animation = entityRuntime->getAnimationRuntime();
            ImGui::Text("Animation Transport");

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
            float animProg = animation->getAnimationSeekTime();
            float duration = animation->getDuration();
            if(ImGui::SliderFloat("#AnimationProgress", &animProg,0,duration,"%dms"))
            {
                animation->setAnimationSeekTime(animProg);
                animation->seekAll(animProg);
            }
            ImGui::PopItemWidth();
        }

        drawTransformProperties();

        ImGui::Separator();

        ImGui::Text("Assets");

        // Animation =======================================================

        int selectedAnimationAsset = entityDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION);
        vector<string> animationAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_ANIMATION);
        if(ImGui::Button("-##Animation"))
        {
            entityDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_ANIMATION,0);
            if (entityRuntime)
            {
                entityRuntime->removeAnimationRuntime();
            }
        }
        ImGui::SameLine();
        if(ImGui::Button(">##Animation"))
        {
            if (selectedAnimationAsset < 0) return;
            AssetDefinition* asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION,selectedAnimationAsset);
            pushPropertyTarget(PropertyType_Asset,asset,nullptr);
            return;
        }

        ImGui::SameLine();

        if(StringCombo("Animation",&selectedAnimationAsset,animationAssets,animationAssets.size()))
        {
            entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedAnimationAsset);
            if (entityRuntime)
            {
                AssetDefinition* selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedAnimationAsset);
                if (selectedDef)
                {
                    entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedDef->getUuid());
                }
            }
        }

        // Audio ===========================================================
        {

            int selectedAudioAsset = entityDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_AUDIO);
            vector<string> audioAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_AUDIO);
            if(ImGui::Button("-##Audio"))
            {
                entityDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_AUDIO,0);
                if (entityRuntime)
                {
                    entityRuntime->removeAudioRuntime();
                }
            }
            ImGui::SameLine();
            if(ImGui::Button(">##Audio"))
            {
                if (selectedAudioAsset < 0) return;
                AssetDefinition* asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_AUDIO,selectedAudioAsset);
                pushPropertyTarget(PropertyType_Asset,asset, nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Audio",&selectedAudioAsset,audioAssets,audioAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedAudioAsset);
                if (entityRuntime)
                {
                    AssetDefinition* selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedAudioAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedDef->getUuid());
                    }
                }
            }
        }

        // Font ============================================================
        {

            int selectedFontAsset = entityDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_FONT);
            vector<string> fontAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_FONT);
            if(ImGui::Button("-##Font"))
            {
                entityDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_FONT,0);
                if (entityRuntime)
                {
                    entityRuntime->removeFontRuntime();
                }
            }
            ImGui::SameLine();
            if(ImGui::Button(">##Font"))
            {
                if (selectedFontAsset < 0) return;
                AssetDefinition* asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_FONT,selectedFontAsset);
                pushPropertyTarget(PropertyType_Asset,asset, nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Font",&selectedFontAsset,fontAssets,fontAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_FONT, selectedFontAsset);
                if (entityRuntime)
                {
                    AssetDefinition* selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_FONT, selectedFontAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_FONT, selectedDef->getUuid());
                    }
                }
            }
        }

        // Model ===========================================================

        {
            int selectedModelAsset = entityDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_MODEL);
            vector<string> modelAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MODEL);

            if(ImGui::Button("-##Model"))
            {
                entityDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_MODEL,0);
                if (entityRuntime)
                {
                    entityRuntime->removeModelRuntime();
                }

            }
            ImGui::SameLine();

            if(ImGui::Button(">##Model"))
            {
                if (selectedModelAsset < 0) return;
                AssetDefinition* asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL,selectedModelAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Model",&selectedModelAsset,modelAssets,modelAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAsset);
                if (entityRuntime)
                {
                    AssetDefinition* selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_MODEL, selectedDef->getUuid());
                    }
                }
            }
        }

        // Path ============================================================

        {
            int selectedPathAsset = entityDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PATH);
            vector<string> pathAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PATH);
            if(ImGui::Button("-##Path"))
            {
                entityDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_PATH,0);
                if (entityRuntime)
                {
                    entityRuntime->removePathRuntime();
                }

            }
            ImGui::SameLine();

            if(ImGui::Button(">##Path"))
            {
                if (selectedPathAsset < 0) return;
                AssetDefinition* asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PATH,selectedPathAsset);
                pushPropertyTarget(PropertyType_Asset, asset, nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Path",&selectedPathAsset,pathAssets,pathAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PATH, selectedPathAsset);
                if (entityRuntime)
                {
                    AssetDefinition* selectedDef = projectDefinition->getAssetDefinitionAtIndex(ASSET_TYPE_ENUM_PATH, selectedPathAsset);
                    entityRuntime->replaceAssetUuid(ASSET_TYPE_ENUM_PATH, selectedDef->getUuid());
                }
            }
        }

        // Physics Object ==================================================

        {
            int selectedPhysicsObjectAsset = entityDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT);
            vector<string> poAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT);
            if(ImGui::Button("-##PhysicsObject"))
            {
                entityDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,0);
                if (entityRuntime)
                {
                    entityRuntime->removePhysicsObjectRuntime();
                }
            }
            ImGui::SameLine();

            if(ImGui::Button(">##PhysicsObject"))
            {
                if (selectedPhysicsObjectAsset < 0) return;
                AssetDefinition* asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,selectedPhysicsObjectAsset);
                pushPropertyTarget(PropertyType_Asset, asset, nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Physics Object",&selectedPhysicsObjectAsset,poAssets,poAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,selectedPhysicsObjectAsset);
                if (entityRuntime)
                {
                    AssetDefinition* selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT, selectedPhysicsObjectAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT, selectedDef->getUuid());
                    }
                }
            }
        }

        // Script ==========================================================
        {

            int selectedScriptAsset = entityDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT);
            vector<string> scriptAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SCRIPT);
            if(ImGui::Button("-##Script"))
            {
                entityDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_SCRIPT,0);
                if (entityRuntime)
                {
                    entityRuntime->removeScriptRuntime();
                }
            }
            ImGui::SameLine();

            if(ImGui::Button(">##Script"))
            {
                if (selectedScriptAsset < 0) return;
                AssetDefinition* asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,selectedScriptAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Script",&selectedScriptAsset,scriptAssets,scriptAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedScriptAsset);
                if (entityRuntime)
                {
                    AssetDefinition* selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedScriptAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedDef->getUuid());
                    }
                }
            }
        }

        // Texture (Sprite) ================================================
        {
            int selectedTextureAsset = entityDef->getSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE);
            vector<string> textureAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_TEXTURE);

            if(ImGui::Button("-##Texture"))
            {
                entityDef->setAssetDefinition(AssetType::ASSET_TYPE_ENUM_SCRIPT,0);
                if (entityRuntime)
                {
                    entityRuntime->removeScriptRuntime();
                }
            }

            ImGui::SameLine();

            if(ImGui::Button(">##Texture"))
            {
                if (selectedTextureAsset < 0) return;
                AssetDefinition* asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE,selectedTextureAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            if(StringCombo("Texture (Sprite)",&selectedTextureAsset,textureAssets,textureAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, selectedTextureAsset);
                if (entityRuntime)
                {
                    AssetDefinition* selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, selectedTextureAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE, selectedDef->getUuid());
                    }
                }
            }
        }
    }

    void
    PropertiesWindow::drawTransformProperties
    ()
    {
        EntityDefinition* entityDefinition = static_cast<EntityDefinition*>(mDefinition);
        EntityRuntime* entityRuntime = static_cast<EntityRuntime*>(mRuntime);

        static bool withChildren = false;

        ImGui::Separator();

        // TransformSpace ======================================================

        TransformSpace transformSpace;

        if (entityRuntime)
        {
            transformSpace = entityRuntime->getTransformSpace();
        }
        else
        {
            transformSpace = entityDefinition->getTransformSpace();
        }

        ImGui::RadioButton("World Space", (int*)&transformSpace, (int)TRANSFORM_SPACE_WORLD);
        ImGui::SameLine();
        ImGui::RadioButton("Screen Space", (int*)&transformSpace, (int)TRANSFORM_SPACE_SCREEN);

        if (entityRuntime)
        {
            entityRuntime->setTransformSpace(transformSpace);
        }
        else
        {
            entityDefinition->setTransformSpace(transformSpace);
        }

        ImGui::Separator();

        // Tx, Rotation, Scale =================================================

        Transform tx;

        if (entityRuntime)
        {
            tx = entityRuntime->getTransform();
        }
        else
        {
            tx = entityDefinition->getTransform();
        }

        ImGui::Columns(1);

        float float_translation[3] = {
            tx.getTranslation().x,
            tx.getTranslation().y,
            tx.getTranslation().z
        };

        float yaw   = degrees(tx.getYaw());
        float pitch = degrees(tx.getPitch());
        float roll  = degrees(tx.getRoll());

        float float_scale[3] =
        {
            tx.getScale().x,
            tx.getScale().y,
            tx.getScale().z
        };

        bool tx_changed = false;
        tx_changed |= ImGui::DragFloat3("Translation", float_translation,0.01f);
        tx_changed |= ImGui::DragFloat3("Scale", float_scale,0.01f);
        tx_changed |= ImGui::DragFloat("Yaw",   &yaw,   1.f);
        tx_changed |= ImGui::DragFloat("Pitch", &pitch, 1.f);
        tx_changed |= ImGui::DragFloat("Roll",  &roll,  1.f);


        if (tx_changed)
        {
            tx.setTranslation({float_translation[0], float_translation[1], float_translation[2]});
            tx.setScale(vec3{float_scale[0], float_scale[1], float_scale[2]});
            tx.setYaw(radians(yaw));
            tx.setPitch(radians(pitch));
            tx.setRoll(radians(roll));

            if (entityRuntime)
            {
                entityRuntime->setTransform(tx);
            }
            else
            {
                entityDefinition->setTransform(tx);
            }
        }

        ImGui::Separator();
        ImGui::Checkbox("With Children",&withChildren);
        ImGui::Separator();

        ImGui::Separator();

        ImGui::Columns(2);

        if(ImGui::Button("Capture as Initial"))
        {
            if (entityDefinition && entityRuntime)
            {
                entityDefinition->setTransform(entityRuntime->getTransform());

                if (withChildren)
                {
                    entityRuntime->applyToAll(
                                function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime* rt)
                                {
                                    if (rt != entityRuntime)
                                    {
                                        EntityDefinition* d = static_cast<EntityDefinition*>(rt->getDefinitionHandle());
                                        d->setTransform(rt->getTransform());
                                        d->setTransformSpace(rt->getTransformSpace());
                                    }
                                    return static_cast<EntityRuntime*>(nullptr);
                                }
                                ));
                }
            }
        }

        ImGui::NextColumn();
        if(ImGui::Button("Restore to Initial"))
        {
            if (entityDefinition && entityRuntime)
            {
                Transform tmp = entityDefinition->getTransform();
                entityRuntime->setTransform(tmp);
                if (withChildren)
                {
                    entityRuntime->applyToAll(
                                function<EntityRuntime*(EntityRuntime*)>(
                                    [&](EntityRuntime* rt){
                                    if (rt != entityRuntime)
                                    {
                                        EntityDefinition* d = static_cast<EntityDefinition*>(rt->getDefinitionHandle());
                                        Transform tmp = d->getTransform();
                                        rt->setTransform(tmp);
                                    }
                                    return static_cast<EntityRuntime*>(nullptr);
                                }));
                }
            }
        }

        ImGui::Columns(1);
    }

    void
    PropertiesWindow::drawAssetProperties
    ()
    {
        mContext->getPathViewer()->setVisible(false);
        mContext->getAnimationViewer()->setVisible(false);
        AssetDefinition* assetDef = static_cast<AssetDefinition*>(mDefinition);
        if (assetDef == nullptr)
        {
            return;
        }

        ImGui::SameLine();
        if(ImGui::Button("Delete Asset"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(assetDef);
            ProjectDefinition* projectDefinition = mContext->getProject()->getDefinition();
            projectDefinition->removeAssetDefinition(assetDef);
            removeFromHistory(mDefinition);
            mDefinition = nullptr;
            mRuntime = nullptr;
            projectDefinition->regroupAssetDefinitions();
            return;
        }

        ImGui::SameLine();

        if (ImGui::Button("Duplicate"))
        {
            AssetDefinition* dup = assetDef->duplicate();
            ProjectDefinition* projectDefinition = mContext->getProject()->getDefinition();
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

        AssetType type = Constants::getAssetTypeEnumFromString(assetDef->getType());

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

        ProjectRuntime* projectRuntime = mContext->getProject()->getRuntime();
        if (projectRuntime)
        {
            SceneRuntime* activeScene = projectRuntime->getActiveSceneRuntime();
            if (activeScene)
            {
                ImGui::Separator();
                ImGui::Columns(1);
                if(ImGui::CollapsingHeader("Active Runtimes"))
                {
                    vector<EntityRuntime*> runtimes = activeScene->getEntitysWithRuntimeOf(assetDef);
                    for (EntityRuntime* runtime : runtimes)
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
        AnimationDefinition* animDef = static_cast<AnimationDefinition*>(mDefinition);
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

        for (AnimationKeyframe& kf : animDef->getKeyframes())
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
            Transform kf_transf = kf.getTransform();
            vec3 kf_tx = kf_transf.getTranslation();

            float tx[3] = {kf_tx.x, kf_tx.y, kf_tx.z};

            ImGui::PushItemWidth(-1);

            if (ImGui::InputFloat3("##translation",&tx[0]))
            {
                kf_transf.setTranslation(vec3(tx[0],tx[1],tx[2]));
                kf.setTransform(kf_transf);
                animDef->updateKeyframe(kf);
            }
            ImGui::PopItemWidth();

            // Rx
            float yaw, pitch, roll;
            yaw   = degrees(kf_transf.getYaw());
            pitch = degrees(kf_transf.getPitch());
            roll  = degrees(kf_transf.getRoll());

            ImGui::PushItemWidth(-1);
            bool rot_changed = false;
            rot_changed |= ImGui::DragFloat("Yaw",   &yaw,   1.f);
            rot_changed |= ImGui::DragFloat("Pitch", &pitch, 1.f);
            rot_changed |= ImGui::DragFloat("Roll",  &roll,  1.f);

            if (rot_changed)
            {
                kf_transf.setYaw(radians(yaw));
                kf_transf.setPitch(radians(pitch));
                kf_transf.setRoll(radians(roll));
                kf.setTransform(kf_transf);
                animDef->updateKeyframe(kf);
            }
            ImGui::PopItemWidth();

            // Scale
            vec3 kf_scale = kf_transf.getScale();
            float scale[3] = {kf_scale.x, kf_scale.y, kf_scale.z};

            ImGui::PushItemWidth(-1);

            if (ImGui::InputFloat3("##scale",&scale[0]))
            {
                kf_scale = vec3(scale[0], scale[1], scale[2]);
                kf_transf.setScale(kf_scale);
                kf.setTransform(kf_transf);
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
            vector<string> easingTypes = AnimationEasing::EasingNames;
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
                //drawAnimationKeyframeImGuizmo(animDef, kf);
            }
            mContext->getAnimationViewer()->regenerate();
        }
        ImGui::Columns(1);
    }


    void
    PropertiesWindow::drawAudioAssetProperties
    ()
    {
        AudioDefinition* audioDef = static_cast<AudioDefinition*>(mDefinition);
        ProjectRuntime* projectRuntime = mContext->getProject()->getRuntime();
        AudioRuntime* audioRunt = nullptr;
        if (projectRuntime)
        {
            auto audioComp = projectRuntime->getAudioComponent();
            audioRunt = audioComp->getAudioRuntime(audioDef);
        }

        if (ImGui::Button("Audio File..."))
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
                uint8_t* audioData = audioFile->getBinaryData();
                size_t audioDataSize = audioFile->getBinaryDataSize();
                mContext->getProjectDirectory()->writeAssetData(audioDef,audioData, audioDataSize, audioFile->getExtension());
                audioDef->setFormat(audioFile->getExtension());
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

        ImGui::Text("Format: %s", audioDef->getFormat().c_str());

        if(ImGui::Button("Remove File"))
        {
            audioDef->setFormat("");
            if (audioRunt)
            {
                auto audioCache = projectRuntime->getAudioCache();
                audioCache->removeRuntime(audioDef);
                audioRunt = nullptr;
            }
            mContext->getProjectDirectory()->removeAssetDirectory(audioDef);
        }

        if (audioRunt != nullptr && audioRunt->getLoaded())
        {
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
            if (audioRunt)
            {
                int duration = audioRunt->getDurationInSamples();
                if (duration > 0)
                {
                    int audioProg = audioRunt->getSampleOffset();
                    if(ImGui::SliderInt("#AudioProgress", &audioProg,0,duration,"%d"))
                    {
                        audioRunt->setSampleOffset(audioProg);
                    }
                }
            }
            ImGui::PopItemWidth();
        }
    }

    void
    PropertiesWindow::drawFontAssetProperties
    ()
    {
        bool selectFile = ImGui::Button("Font File...");
        FontDefinition* def = static_cast<FontDefinition*>(mDefinition);

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
                uint8_t* data = file->getBinaryData();
                size_t dataSize = file->getBinaryDataSize();
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

        float fontSize = def->getSize();
        if (ImGui::SliderFloat("Size",&fontSize,1.f,100.f))
        {
            def->setSize(fontSize);
        }

        // Display the FontAtlas Texture
        Project* project = mContext->getProject();
        if (project)
        {
            ProjectRuntime* projectRuntime = project->getRuntime();
            if (projectRuntime)
            {
                auto fontCache = projectRuntime->getFontCache();
                if (fontCache)
                {
                    FontRuntime* fontRuntime = fontCache->getRuntimeHandle(def);
                    if (fontRuntime)
                    {
                        GLuint atlasTexture = fontRuntime->getAtlasTexture();
                        unsigned int atlasWidth = fontRuntime->getAtlasWidth();
                        unsigned int atlasHeight = fontRuntime->getAtlasHeight();
                        if (ImGui::BeginChild("Font Atlas Texture",
                                              ImVec2(atlasWidth,atlasHeight),
                                              false, ImGuiWindowFlags_HorizontalScrollbar))
                        {
                            ImGui::Image((void*)(intptr_t)atlasTexture,ImVec2(atlasWidth,atlasHeight));
                            ImGui::EndChild();
                        }
                    }
                }
            }
        }

    }

    void
    PropertiesWindow::drawMaterialAssetProperties
    ()
    {
        ProjectDefinition* projectDefinition = mContext->getProject()->getDefinition();
        ProjectRuntime* projectRuntime = mContext->getProject()->getRuntime();
        MaterialDefinition* materialDef = static_cast<MaterialDefinition*>(mDefinition);

        int shaderIndex = 0;
        vector<string> shaderList;
        if (projectDefinition)
        {
            AssetDefinition* shaderAsset = projectDefinition->getAssetDefinitionByUuid(materialDef->getShader());
            shaderIndex = projectDefinition->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderAsset);
            shaderList = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);
        }

        if(StringCombo("Shader", &shaderIndex, shaderList, shaderList.size()))
        {
            if (projectDefinition)
            {
                AssetDefinition* newShader = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderIndex);
                if(newShader)
                {
                    materialDef->setShader(newShader->getUuid());
                }
            }
        }

        ImGui::Separator();

        auto txCache = projectRuntime->getTextureCache();

        UuidType albedoUuid = materialDef->getAlbedoTexture();
        UuidType normalUuid = materialDef->getNormalTexture();
        UuidType metallicUuid = materialDef->getMetallicTexture();
        UuidType roughnessUuid = materialDef->getRoughnessTexture();
        UuidType aoUuid = materialDef->getAoTexture();

        void* albedoTxId = nullptr;
        void* normalTxId = nullptr;
        void* metallicTxId = nullptr;
        void* roughnessTxId = nullptr;
        void* aoTxId = nullptr;

        if (projectDefinition && projectRuntime)
        {
            // Albedo
            AssetDefinition* albedoDef = projectDefinition->getAssetDefinitionByUuid(albedoUuid);
            if (albedoDef)
            {
                TextureDefinition* txDef = static_cast<TextureDefinition*>(albedoDef);
                TextureRuntime* txRuntime = txCache->getRuntimeHandle(txDef);
                if (txRuntime)
                {
                    albedoTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }

            // Normal
            AssetDefinition* normalDef = projectDefinition->getAssetDefinitionByUuid(normalUuid);
            if (normalDef)
            {
                TextureDefinition* txDef = static_cast<TextureDefinition*>(normalDef);
                auto txCache = projectRuntime->getTextureCache();
                TextureRuntime* txRuntime = txCache->getRuntimeHandle(txDef);
                if (txRuntime)
                {
                    normalTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }

            // Metallic
            AssetDefinition* metallicDef = projectDefinition->getAssetDefinitionByUuid(metallicUuid);
            if (metallicDef)
            {
                TextureDefinition* txDef = static_cast<TextureDefinition*>(metallicDef);
                auto txCache = projectRuntime->getTextureCache();
                TextureRuntime* txRuntime = txCache->getRuntimeHandle(txDef);
                if (txRuntime)
                {
                    metallicTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }

            // Roughness
            auto roughnessDef = projectDefinition->getAssetDefinitionByUuid(roughnessUuid);
            if (roughnessDef)
            {
                TextureDefinition* txDef = static_cast<TextureDefinition*>(roughnessDef);
                auto txCache = projectRuntime->getTextureCache();
                TextureRuntime* txRuntime = txCache->getRuntimeHandle(txDef);
                if (txRuntime)
                {
                    roughnessTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }

            // AO
            auto aoDef = projectDefinition->getAssetDefinitionByUuid(aoUuid);
            if (aoDef)
            {
                TextureDefinition* txDef = static_cast<TextureDefinition*>(aoDef);
                auto txCache = projectRuntime->getTextureCache();
                TextureRuntime* txRuntime = txCache->getRuntimeHandle(txDef);
                if (txRuntime)
                {
                    aoTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }
        }

        vector<string> textures;
        int albedoIndex;
        int normalIndex;
        int metallicIndex;
        int roughnessIndex;
        int aoIndex;

        if(projectDefinition)
        {
            textures = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_TEXTURE);

            albedoIndex = projectDefinition->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projectDefinition->getAssetDefinitionByUuid(albedoUuid));

            normalIndex = projectDefinition->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projectDefinition->getAssetDefinitionByUuid(normalUuid));

            metallicIndex = projectDefinition->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projectDefinition->getAssetDefinitionByUuid(metallicUuid));

            roughnessIndex = projectDefinition->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projectDefinition->getAssetDefinitionByUuid(roughnessUuid));

            aoIndex = projectDefinition->getAssetDefinitionIndex(
                        AssetType::ASSET_TYPE_ENUM_TEXTURE,
                        projectDefinition->getAssetDefinitionByUuid(aoUuid));
        }

        // Albedo ==============================================================

        if(StringCombo("Albedo",&albedoIndex,textures,textures.size()))
        {
            if (projectDefinition)
            {
                AssetDefinition* txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, albedoIndex);

                if (txDef)
                {
                    UuidType uuid = txDef->getUuid();
                    materialDef->setAlbedoTexture(uuid);
                }
            }
        }

        ImGui::Image(albedoTxId, mImageSize);
        ImGui::Separator();

        // Normal ==============================================================

        if(StringCombo("Normal",&normalIndex,textures,textures.size()))
        {
            if (projectDefinition)
            {
                AssetDefinition* txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, normalIndex);
                if (txDef)
                {
                    UuidType uuid = txDef->getUuid();
                    materialDef->setNormalTexture(uuid);
                }
            }
        }
        ImGui::Image(normalTxId, mImageSize);
        ImGui::Separator();

        // Metallic ============================================================

        if(StringCombo("Metallic",&metallicIndex,textures,textures.size()))
        {
            if (projectDefinition)
            {
                AssetDefinition* txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, metallicIndex);
                if (txDef)
                {
                    UuidType uuid = txDef->getUuid();
                    materialDef->setMetallicTexture(uuid);
                }
            }
        }
        ImGui::Image(metallicTxId, mImageSize);
        ImGui::Separator();

        // Roughness ===========================================================

        if(StringCombo("Roughness",&roughnessIndex,textures,textures.size()))
        {
            if (projectDefinition)
            {
                AssetDefinition* txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, roughnessIndex);
                if (txDef)
                {
                    UuidType uuid = txDef->getUuid();
                    materialDef->setRoughnessTexture(uuid);
                }
            }
        }
        ImGui::Image(roughnessTxId, mImageSize);

        ImGui::Separator();

        // Ao ==================================================================

        if(StringCombo("AO",&aoIndex,textures,textures.size()))
        {
            if (projectDefinition)
            {
                AssetDefinition* txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, aoIndex);
                if (txDef)
                {
                    UuidType uuid = txDef->getUuid();
                    materialDef->setAoTexture(uuid);
                }
            }
        }
        ImGui::Image(aoTxId, mImageSize);

        ImGui::Columns(1);

    }

    void
    PropertiesWindow::drawModelAssetProperties
    ()
    {
        ModelDefinition* def = static_cast<ModelDefinition*>(mDefinition);

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
                uint8_t* data = file->getBinaryData();
                size_t dataSize = file->getBinaryDataSize();
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
                uint8_t* data = file->getBinaryData();
                size_t dataSize = file->getBinaryDataSize();
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

        if(ImGui::Button("Remove File(s)"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(def);
        }

        ProjectRuntime* projectRuntime = mContext->getProject()->getRuntime();
        shared_ptr<ModelCache> modelCache = nullptr;

        if (projectRuntime)
        {
            modelCache = projectRuntime->getModelCache();
        }

        ImGui::Text("Model Format: %s", def->getFormat().c_str());

        if (ImGui::Button("Reload Asset"))
        {
			if (modelCache)
			{
				ModelRuntime* modelRuntime = modelCache->getRuntimeHandle(def);
				if (modelRuntime)
				{
                    modelRuntime->setReloadFlag(true);
				}
			}
        }

        vector<string> modelMaterialNames;

        if (projectRuntime)
        {
            auto modelCache = projectRuntime->getModelCache();
            if (modelCache)
            {
                ModelRuntime* modelRuntime = modelCache->getRuntimeHandle(def);
                if (modelRuntime)
                {
                    modelMaterialNames = modelRuntime->getMaterialNames();
                }
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
        ProjectDefinition* projectDefinition =  mContext->getProject()->getDefinition();
        if (projectDefinition)
        {
            materialAssetNames = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MATERIAL);
        }

        for (string modelMaterial : modelMaterialNames)
        {
            ImGui::Text("%s",modelMaterial.c_str());
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            UuidType currentMaterialUuid = def->getDreamMaterialForModelMaterial(modelMaterial);
            AssetDefinition* currentMaterialDef = projectDefinition->getAssetDefinitionByUuid(currentMaterialUuid);
            int currentMaterialIndex = projectDefinition->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MATERIAL,currentMaterialDef);
            string itemName = "##Material:" + modelMaterial;

            if(StringCombo(itemName.c_str(),&currentMaterialIndex,materialAssetNames,materialAssetNames.size()))
            {
                AssetDefinition* changedMaterial = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MATERIAL, currentMaterialIndex);
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
        PhysicsObjectDefinition* pod = static_cast<PhysicsObjectDefinition*>(mDefinition);
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
            pod->getLinearFactor().x,
            pod->getLinearFactor().y,
            pod->getLinearFactor().z
        };
        if (ImGui::DragFloat3("Linear Factor", &linearFactor[0],0.1f))
        {
            vec3 lf(linearFactor[0],linearFactor[1],linearFactor[2]);
            if (pod)
            {
                pod->setLinearFactor(lf);
                modified = true;
            }
        }

        float angularFactor[3] ={
            pod->getAngularFactor().x,
            pod->getAngularFactor().y,
            pod->getAngularFactor().z
        };
        if (ImGui::DragFloat3("Angular Factor", &angularFactor[0],0.1f))
        {
            vec3 af(angularFactor[0], angularFactor[1],angularFactor[2]);
            if (pod)
            {
                pod->setAngularFactor(af);
                modified = true;
            }
        }

        float linearVelocity[3] ={
            pod->getLinearVelocity().x,
            pod->getLinearVelocity().y,
            pod->getLinearVelocity().z
        };
        if (ImGui::DragFloat3("Linear Velocity", &linearVelocity[0],0.1f))
        {
            vec3 lf(linearVelocity[0],linearVelocity[1],linearVelocity[2]);
            if (pod)
            {
                pod->setLinearVelocity(lf);
                modified = true;
            }
        }

        float angularVelocity[3] ={
            pod->getAngularVelocity().x,
            pod->getAngularVelocity().y,
            pod->getAngularVelocity().z
        };
        if (ImGui::DragFloat3("Angular Velocity", &angularVelocity[0],0.1f))
        {
            vec3 af(angularVelocity[0], angularVelocity[1],angularVelocity[2]);
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
                pod->getHalfExtents().x,
                pod->getHalfExtents().y,
                pod->getHalfExtents().z
            };

            if(ImGui::InputFloat3("Half-Extents",&halfExtents[0]))
            {
                pod->setHalfExtents(vec3(halfExtents[0], halfExtents[1], halfExtents[2]));
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
            auto projectDefinition = mContext->getProject()->getDefinition();

            UuidType selectedModelAssetUuid = pod->getCollisionModel();
            AssetDefinition* selectedModelAssetDef = projectDefinition->getAssetDefinitionByUuid(selectedModelAssetUuid);
            int selectedModelAssetIndex = projectDefinition->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAssetDef);
            vector<string> modelAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MODEL);

            if(StringCombo("Model",&selectedModelAssetIndex,modelAssets,modelAssets.size()))
            {
                AssetDefinition* newlySelected = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAssetIndex);
                pod->setCollisionModel(newlySelected->getUuid());
                modified = true;
            }
        }
        else if (pod->getFormat().compare(Constants::COLLISION_SHAPE_STATIC_PLANE) == 0)
        {
            float normal[3] = {
                pod->getNormal().x,
                pod->getNormal().y,
                pod->getNormal().z
            };

            if (ImGui::InputFloat3("Plane Normal",&normal[0]))
            {
                pod->setNormal(vec3(normal[0],normal[1],normal[2]));
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
                static UuidType selectedToTransform = 0;
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
                        //drawPhysicsImGizmo(shape);
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
        auto scriptDef = static_cast<ScriptDefinition*>(mDefinition);
        auto projRunt = mContext->getProject()->getRuntime();
        auto scriptCache = projRunt->getScriptCache();

        ScriptRuntime* scriptInst;

        if (scriptCache)
        {
            scriptInst = dynamic_cast<ScriptRuntime*>(scriptCache->getRuntimeHandle(scriptDef));
        }

        vector<string> templates = mContext->getTemplatesModel()->getTemplateNames(AssetType::ASSET_TYPE_ENUM_SCRIPT);

        static int currentTemplateIndex = -1;

        if (StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
        {
            ImGui::OpenPopup("Load Script From Template?");
        }

        if(ImGui::BeginPopupModal("Load Script From Template?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Load Script from template?\n\nThis will replace the existing Script.\n\n");
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button("Load from Template"))
            {
                if (currentTemplateIndex < 0)
                {
                    LOG_ERROR("ScriptEditorWindow: Cannot load Script template at index {}",currentTemplateIndex);
                }
                else
                {
                    auto templateName = templates.at(currentTemplateIndex);
                    auto templateStr = mContext->getTemplatesModel()->getTemplate(AssetType::ASSET_TYPE_ENUM_SCRIPT, templateName, Constants::ASSET_FORMAT_SCRIPT_LUA);
                    if (scriptInst)
                    {
                        scriptInst->setSource(templateStr);
                    }
                    else
                    {
                        LOG_ERROR("ScriptEditorWindow: Cannot set from template, script Runtime is null");
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        if(ImGui::Button("Open Script Editor..."))
        {
            mContext->openInExternalEditor(scriptDef);
        }
    }

    void
    PropertiesWindow::drawShaderAssetProperties
    ()
    {
        auto shaderDef = static_cast<ShaderDefinition*>(mDefinition);
        auto projectRuntime = mContext->getProject()->getRuntime();
        ShaderRuntime* shaderRuntime = nullptr;
        auto shaderCache = projectRuntime->getShaderCache();

        if (projectRuntime)
        {
            if (shaderCache)
            {
                shaderRuntime = shaderCache->getRuntimeHandle(shaderDef);
            }
        }

        // Templates

        vector<string> templates = mContext->getTemplatesModel()->getTemplateNames(AssetType::ASSET_TYPE_ENUM_SHADER);
        static int currentTemplateIndex = -1;
        if(StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
        {
            ImGui::OpenPopup("Load Shader From Template?");
        }

        if(ImGui::BeginPopupModal("Load Shader From Template?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Load Shader from template?\n\nThis will OVERWRITE the existing Shader files.\n\n");
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Load Template"))
            {
                if (currentTemplateIndex < 0)
                {
                    LOG_ERROR("ShaderEditorWindow: Cannot load template {}",currentTemplateIndex);
                }
                else
                {
                    auto  templateName = templates.at(currentTemplateIndex);
                    string vertTemplate = mContext->getTemplatesModel()->getTemplate(AssetType::ASSET_TYPE_ENUM_SHADER,templateName,Constants::SHADER_GLSL_VERTEX_FILE_NAME);
                    string fragTemplate = mContext->getTemplatesModel()->getTemplate(AssetType::ASSET_TYPE_ENUM_SHADER,templateName,Constants::SHADER_GLSL_FRAGMENT_FILE_NAME);

                    bool vertSuccess = false;
                    vertSuccess = mContext->getProjectDirectory()->writeAssetData(
                                shaderDef,
                                (uint8_t*)vertTemplate.c_str(),
                                vertTemplate.size(),
                                Constants::SHADER_GLSL_VERTEX_FILE_NAME);

                    bool fragSuccess = false;
                    fragSuccess = mContext->getProjectDirectory()->writeAssetData(
                                shaderDef,
                                (uint8_t*)fragTemplate.c_str(),
                                fragTemplate.size(),
                                Constants::SHADER_GLSL_FRAGMENT_FILE_NAME);

                    if (vertSuccess && fragSuccess)
                    {
                        mContext->getMenuBar()->setMessageString("Saved Shader "+shaderRuntime->getNameAndUuidString());
                    }
                    else
                    {
                        mContext->getMenuBar()->setMessageString("Error saving Shader"+shaderRuntime->getNameAndUuidString());
                    }

                    currentTemplateIndex = -1;
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        static bool openVertexFailed = false;
        static bool openFragmentFailed = false;

        ImGui::PushItemWidth(-1.f);
        {
            if(ImGui::Button("Open Vertex Shader in Editor..."))
            {
                if (!mContext->openInExternalEditor(shaderDef, Constants::SHADER_GLSL_VERTEX_FILE_NAME))
                {
                    openVertexFailed = true;
                }
            }

            if(ImGui::Button("Open Fragment Shader in Editor..."))
            {
                if (!mContext->openInExternalEditor(shaderDef, Constants::SHADER_GLSL_FRAGMENT_FILE_NAME))
                {
                    openFragmentFailed = true;
                }
            }

            if (shaderRuntime)
            {
                if (ImGui::Button("Reload Asset"))
                {
                    shaderRuntime->setReloadFlag(true);
                }
            }
        }
        ImGui::PopItemWidth();

        if (openVertexFailed)
        {
            ImGui::OpenPopup("Open Vertex Shader Failed");
        }

        if (openFragmentFailed)
        {
            ImGui::OpenPopup("Open Fragment Shader Failed");
        }

        if (ImGui::BeginPopup("Open Vertex Shader Failed"))
        {
            ProjectDirectory* pd = mContext->getProjectDirectory();
            string filePath = pd->getAssetAbsolutePath(shaderDef,Constants::SHADER_GLSL_VERTEX_FILE_NAME);
            ImGui::Text("Unable to open Vertex Shader \n%s\n\nFile may not exist",filePath.c_str());
            if (ImGui::Button("Cancel",ImVec2(0,0)))
            {
                openVertexFailed = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("Open Fragment Shader Failed"))
        {
            ProjectDirectory* pd = mContext->getProjectDirectory();
            string filePath = pd->getAssetAbsolutePath(shaderDef,Constants::SHADER_GLSL_FRAGMENT_FILE_NAME);
            ImGui::Text("Unable to open Fragment Shader \n\n%s\n\nFile may not exist",filePath.c_str());
            if (ImGui::Button("Cancel",ImVec2(0,0)))
            {
                openFragmentFailed = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (shaderRuntime)
        {
            ImGui::Separator();

            ImGui::Text("Shader Status");
            ImGui::Columns(2);

            ImGui::Text("Loaded");
            ImGui::NextColumn();
            if (shaderRuntime->getLoaded())
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f}, "Yes");
            }
            else
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f}, "No");
            }
            ImGui::NextColumn();

            ImGui::Text("Load Error");
            ImGui::NextColumn();
            if (shaderRuntime->getLoadError())
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f}, "Error");
            }
            else
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f}, "No Error");
            }
            ImGui::NextColumn();

            ImGui::Text("Vertex Source");
            ImGui::NextColumn();
            if (shaderRuntime->hasVertexSource())
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f}, "Found");
            }
            else
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f}, "Not Found");
            }
            ImGui::NextColumn();

            ImGui::Text("Fragment Source");
            ImGui::NextColumn();
            if (shaderRuntime->hasFragmentSource())
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f}, "Found");
            }
            else
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f}, "Not Found");
            }
            ImGui::NextColumn();

            ImGui::Text("Vertex Compilation");
            ImGui::NextColumn();
            if(shaderRuntime->hasVertexCompilationFailed())
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f},"Failed");
            }
            else
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f},"Success");
            }
            ImGui::NextColumn();

            ImGui::Text("Fragment Compilation");
            ImGui::NextColumn();
            if(shaderRuntime->hasFragmentCompilationFailed())
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f},"Failed");
            }
            else
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f},"Success");
            }
            ImGui::NextColumn();

            ImGui::Text("Linking");
            ImGui::NextColumn();
            if(shaderRuntime->hasLinkingFailed())
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f},"Failed");
            }
            else
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f},"Success");
            }
            ImGui::NextColumn();

            ImGui::Text("GL ID");
            ImGui::NextColumn();
            if(shaderRuntime->hasValidGLID())
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f}, "Valid");
            }
            else
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f}, "Invalid");
            }

            ImGui::NextColumn();

            ImGui::Columns(1);

            ImGui::Separator();
            ImGui::Text("Uniforms");

            auto uniforms = shaderRuntime->getUniformsVector();

            ImGui::Columns(3);

            ImGui::Text("Name");
            ImGui::NextColumn();
            ImGui::Text("Type");
            ImGui::NextColumn();
            ImGui::Text("Location");
            ImGui::NextColumn();


            for (auto& uniform : *uniforms)
            {
                ImGui::Separator();
                ImGui::Text("%s",uniform->getName().c_str());
                ImGui::NextColumn();
                ImGui::Text("%s",uniformTypeToString(uniform->getType()).c_str());
                ImGui::NextColumn();
                ImGui::Text("%d",uniform->getLocation());
                ImGui::NextColumn();
            }

            ImGui::Columns(1);
        }
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
            vec3 vTx = cp.position;
            float tx[3] = {vTx.x, vTx.y, vTx.z};
            if (ImGui::InputFloat3("##position",&tx[0]))
            {
                vTx.x = (tx[0]);
                vTx.y = (tx[1]);
                vTx.z = (tx[2]);
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
                //drawPathControlPointImGuizmo(pathDef,cp);
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
    PropertiesWindow::drawTextureAssetProperties
    ()
    {
        void* textureId = nullptr;

        auto textureDef = static_cast<TextureDefinition*>(mDefinition);
        auto projectRuntime = mContext->getProject()->getRuntime();
        auto pDef = mContext->getProject()->getDefinition();
        TextureRuntime* textureRuntime = nullptr;
        if (projectRuntime)
        {
            auto txCache = projectRuntime->getTextureCache();
            textureRuntime = txCache->getRuntimeHandle(textureDef);
            if (textureRuntime)
            {
                textureId = (void*)(intptr_t)textureRuntime->getTextureID();
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

        if(ImGui::Button("Remove File"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(textureDef);
        }

        if (textureRuntime)
        {
            if (ImGui::Button("Reload Asset"))
            {
                textureRuntime->setReloadFlag(true);
            }
        }

        bool isHdr = textureRuntime->isHDR();
        ImGui::Text("HDR: %s", isHdr ? "Yes" : "No");

        bool isEnvironment = textureDef->getIsEnvironmentTexture();
        if (ImGui::Checkbox("Environment",&isEnvironment))
        {
            textureDef->setIsEnvironmentTexture(isEnvironment);
        }

        bool flipVertical = textureDef->getFlipVertical();
        if (ImGui::Checkbox("Flip Vertical",&flipVertical))
        {
            textureDef->setFlipVertical(flipVertical);
        }

        if (isEnvironment)
        {
            vector<string> shaderList = pDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);
            // EquiToCube
            {
                UuidType shaderUuid = textureDef->getEquiToCubeMapShader();
                ShaderDefinition* shaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(shaderUuid));
                int shaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderDef);

                if (StringCombo("Cube Map Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                    AssetDefinition* selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    textureDef->setEquiToCubeMapShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched cube map shader for texture {} to {} {}",textureDef->getNameAndUuidString(), name, uuid);
                }
            }
            // Irradiance
            {
                UuidType shaderUuid = textureDef->getIrradianceMapShader();
                ShaderDefinition* shaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(shaderUuid));
                int shaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderDef);

                if (StringCombo("Irradiance Map Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                    AssetDefinition* selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    textureDef->setIrradianceMapShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched irradiance map shader for texture {} to {} {}",textureDef->getNameAndUuidString(), name, uuid);
                }
            }
            // PreFilter
            {
                UuidType shaderUuid = textureDef->getPreFilterShader();
                ShaderDefinition* shaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(shaderUuid));
                int shaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderDef);

                if (StringCombo("PreFilter Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                    AssetDefinition* selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    textureDef->setPreFilterShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched PreFilter shader for texture {} to {} {}",textureDef->getNameAndUuidString(), name, uuid);
                }
            }
            // BRDF_LUT
            {
                UuidType shaderUuid = textureDef->getBrdfLutShader();
                ShaderDefinition* shaderDef = static_cast<ShaderDefinition*>(pDef->getAssetDefinitionByUuid(shaderUuid));
                int shaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderDef);

                if (StringCombo("BRDF LUT Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                    AssetDefinition* selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    textureDef->setBrdfLutShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched BRDF LUT shader for texture {} to {} {}",textureDef->getNameAndUuidString(), name, uuid);
                }
            }
            ImGui::Text("CubeMap Texture ID:    %d", textureRuntime->getCubeMapTextureID());
            ImGui::Text("Irradiance Texture ID: %d", textureRuntime->getIrradianceTextureID());
            ImGui::Text("PreFilter Texture ID:  %d", textureRuntime->getPreFilterTextureID());
            ImGui::Text("BRDF LUT Texture ID:   %d", textureRuntime->getBrdfLutTextureID());
        }

        if (textureRuntime)
        {
            ImGui::Separator();
            ImGui::Text("Size: %dx%d",textureRuntime->getWidth(),textureRuntime->getHeight());
            ImGui::Text("Channels: %d",textureRuntime->getChannels());
            ImGui::Image(textureId, mImageSize);
        }
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
        auto projectRuntime = mContext->getProject()->getRuntime();
        if (projectRuntime)
        {
            auto sceneRuntime = projectRuntime->getActiveSceneRuntime();
            if (sceneRuntime)
            {
                auto runts = sceneRuntime->getEntitysWithRuntimeOf(assetDef);
                for (auto entityRuntime : runts)
                {
                    entityRuntime->replaceAssetUuid(assetDef->getAssetType(),assetDef->getUuid());
                }
            }
        }
    }

}
