// Maintain include order for GL Defined

#include "PropertiesWindow.h"
#include "glm/gtc/type_ptr.hpp"
#include <nfd.h>
#include "DreamToolContext.h"
#include <DreamCore.h>

using std::static_pointer_cast;
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
          mImageSize(256,256)
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
        auto project = mContext->getProject();

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
        auto entityDefinition = static_pointer_cast<EntityDefinition>(mDefinition);
        auto entityRuntimeime = static_pointer_cast<EntityRuntime>(mRuntime);

        if (ImGui::Button("Delete"))
        {
            if (entityDefinition)
            {
                auto parent = entityDefinition->getParentEntity();
                if (parent)
                {
                    parent->removeChildDefinition(entityDefinition);
                }
            }
            if (entityRuntimeime)
            {
                auto parent = entityRuntimeime->getParentEntityRuntime();
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

        auto project = mContext->getProject();
        auto sDef = static_pointer_cast<SceneDefinition>(mDefinition);
        auto pDef = project->getDefinition();

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
    (PropertyType type, const shared_ptr<Definition>& def, const shared_ptr<Runtime>& runt)
    {
        // Remove old
        auto itr = std::find_if(mHistory.begin(), mHistory.end(),
                                [&](PropertiesTarget& tgt)
        {return tgt.definition == def;});

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
    (const shared_ptr<Definition>& def)
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
    (const shared_ptr<Definition>& def)
    {
        mDefinition = def;
    }

    void
    PropertiesWindow::setRuntime
    (const shared_ptr<Runtime> runt)
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
        auto project = mContext->getProject();
        auto projectDefinition = static_pointer_cast<ProjectDefinition>(mDefinition);

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
        auto startup = projectDefinition->getStartupSceneDefinition();
        int startupScene = projectDefinition->getSceneDefinitionIndex(startup);
        vector<string> scenes;
        for(auto scene : projectDefinition->getSceneDefinitionsVector())
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
        auto sceneDef = static_pointer_cast<SceneDefinition>(mDefinition);
        auto sceneRuntime = static_pointer_cast<SceneRuntime>(mRuntime);

        if (sceneDef == nullptr) return;

        ImGui::SameLine();

        if (drawDeleteSceneButton())
        {
            return;
        }

        drawNameAndIdProperties();

        ImGui::Separator();

        // Camera

        if (sceneRuntime)
        {

            ImGui::Separator();

            ImGui::Text("Camera");
            auto cam = sceneRuntime->getCamera();
            int cameraTarget = cam->getUseEntity() ? 1 : 0;

            ImGui::RadioButton("Free Camera", &cameraTarget, 0);
            ImGui::RadioButton("Entity Camera", &cameraTarget, 1);
            if(cam->getCameraEntityRuntime())
            {
                ImGui::Text("Camera: %s",cam->getCameraEntityRuntime()->getNameAndUuidString().c_str());
            }

            cam->setUseEntity(cameraTarget == 1 ? true : false);

            Transform cam_tx = cam->getTransform();
            vec3 tx = cam_tx.getTranslation();

            if(ImGui::DragFloat3("Translation",glm::value_ptr(tx),0.1f))
            {
                cam_tx.setTranslation(tx);
                cam->setTransform(cam_tx);
            }

            float cam_yaw, cam_pitch, cam_roll;
            cam_yaw   = degrees(cam_tx.getYaw());
            cam_pitch = degrees(cam_tx.getPitch());
            cam_roll  = degrees(cam_tx.getRoll());

            bool rot_changed = false;
            rot_changed |= ImGui::DragFloat("Pitch", &cam_pitch, 1.f);
            rot_changed |= ImGui::DragFloat("Yaw",   &cam_yaw,   1.f);
            rot_changed |= ImGui::DragFloat("Roll",  &cam_roll,  1.f);

            if(rot_changed)
            {
                cam_tx.setPitch(radians(cam_pitch));
                cam_tx.setYaw(radians(cam_yaw));
                cam_tx.setRoll(radians(cam_roll));
                cam->setTransform(cam_tx);
            }

            float fov;
            fov = degrees(cam->getFieldOfView());

            if (ImGui::DragFloat("FOV", &fov, 0.1f, 1.f,180.f))
            {
                cam->setFieldOfView(radians(fov));
            }

            ImGui::Columns(2);
            if (ImGui::Button("Capture Camera"))
            {
                cam->captureDefinition();
            }

            ImGui::NextColumn();

            if (ImGui::Button("Restore Camera"))
            {
                if (sceneRuntime)
                {
                    cam->loadFromDefinition();
                }
            }
            ImGui::Columns(1);
        }

        ImGui::Separator();

        ImGui::Text("Rendering");

        // Rendering
        if (sceneRuntime)
        {
            auto camera = sceneRuntime->getCamera();
            if (camera)
            {
                float meshCull = camera->getMeshCullDistance();
                if (ImGui::DragFloat("Mesh Cull",&meshCull))
                {
                    camera->setMeshCullDistance(meshCull);
                }

                float drawDistance[2] =
                {
                    camera->getMinDrawDistance(),
                    camera->getMaxDrawDistance()
                };

                if (ImGui::DragFloat2("Draw Distance",drawDistance))
                {
                    camera->setMinDrawDistance(drawDistance[0]);
                    camera->setMaxDrawDistance(drawDistance[1]);
                }
            }
        }

        vec4 clear = sceneDef->getClearColor();
        if(ImGui::ColorEdit4("Clear Color",glm::value_ptr(clear)))
        {
            if (sceneDef != nullptr)
            {
                sceneDef->setClearColor(clear);
            }
            if (sceneRuntime)
            {
                sceneRuntime->setClearColor(clear);
            }
        }

        auto pDef = sceneDef->getProjectDefinition();
        vector<string> shaderList = pDef->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);

        // Setup ShadowPassShader
        {
            UuidType spShaderUuid = sceneDef->getShadowPassShader();
            auto spShaderDef = static_pointer_cast<ShaderDefinition>(pDef->getAssetDefinitionByUuid(spShaderUuid));
            int shadowShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,spShaderDef);

            if (StringCombo("Shadow Pass Shader", &shadowShaderIndex, shaderList, shaderList.size()))
            {
                auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shadowShaderIndex);
                UuidType uuid = selectedShader->getUuid();
                string name = selectedShader->getName();
                sceneDef->setShadowPassShader(uuid);
                LOG_DEBUG("PropertiesWindow: Switched shadow pass shader to {} {}", name, uuid);
            }
        }

        // Setup FontShader
        {
            UuidType fontShaderUuid = sceneDef->getFontShader();
            auto fontShaderDef = static_pointer_cast<ShaderDefinition>(pDef->getAssetDefinitionByUuid(fontShaderUuid));
            int fontShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,fontShaderDef);

            if (StringCombo("Font Shader", &fontShaderIndex, shaderList, shaderList.size()))
            {
                auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, fontShaderIndex);
                UuidType uuid = selectedShader->getUuid();
                string name = selectedShader->getName();
                sceneDef->setFontShader(uuid);
                LOG_DEBUG("PropertiesWindow: Switched Font shader to {} {}", name, uuid);
            }
        }

        // Setup Sprite shader
        {
            UuidType spriteShaderUuid = sceneDef->getSpriteShader();
            auto spriteShaderDef = static_pointer_cast<ShaderDefinition>(pDef->getAssetDefinitionByUuid(spriteShaderUuid));
            int spriteShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,spriteShaderDef);

            if (StringCombo("Sprite Shader", &spriteShaderIndex, shaderList, shaderList.size()))
            {
                auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, spriteShaderIndex);
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
            auto envTexDef = static_pointer_cast<TextureDefinition>(pDef->getAssetDefinitionByUuid(envTexUuid));
            int envMapTexIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, envTexDef);

            if (StringCombo("Environment Texture", &envMapTexIndex, textureList, textureList.size()))
            {
                auto selected = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, envMapTexIndex);
                UuidType uuid = selected->getUuid();
                string name = selected->getName();
                sceneDef->setEnvironmentTexture(uuid);
                if(sceneRuntime)
                {
                    auto tc = sceneRuntime->getProjectRuntime()->getTextureCache();
                    sceneRuntime->setEnvironmentTexture(tc->getRuntime(static_pointer_cast<TextureDefinition>(selected)));
                }
                LOG_DEBUG("PropertiesWindow: Switched Environment Texture to {} {}", name, uuid);
            }
        }

        // Environment Shader
        {
            UuidType envShaderUuid = sceneDef->getEnvironmentShader();
            auto envShaderDef = static_pointer_cast<ShaderDefinition>(pDef->getAssetDefinitionByUuid(envShaderUuid));
            int envShaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,envShaderDef);

            if (StringCombo("Environment Shader", &envShaderIndex, shaderList, shaderList.size()))
            {
                auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, envShaderIndex);
                UuidType uuid = selectedShader->getUuid();
                string name = selectedShader->getName();
                sceneDef->setEnvironmentShader(uuid);
                LOG_DEBUG("PropertiesWindow: Switched environment shader to {} {}", name, uuid);
            }
        }


        if (ImGui::CollapsingHeader("Scripting"))
        {
            auto pDef= mContext->getProject()->getDefinition();
            vector<string> scriptAssets = mContext->getProject()->getDefinition()->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SCRIPT);

            UuidType inputScriptUuid = sceneDef->getInputScript();
            auto inputScriptDef = pDef->getAssetDefinitionByUuid(inputScriptUuid);
            int inputScriptIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,inputScriptDef);
            if (StringCombo("Input Script",&inputScriptIndex,scriptAssets,scriptAssets.size()))
            {
                auto selected = pDef->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,inputScriptIndex);
                UuidType uuid = selected->getUuid();
                sceneDef->setInputScript(uuid);
            }
        }

        // Physics
        if(ImGui::CollapsingHeader("Physics"))
        {

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
        auto projectDefinition = mContext->getProject()->getDefinition();
        auto entityDef = static_pointer_cast<EntityDefinition>(mDefinition);
        auto entityRuntime = static_pointer_cast<EntityRuntime>(mRuntime);

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
            auto newChildDef = entityDef->createNewChildDefinition(nullptr);
            newChildDef->setTransform(mContext->getCursor()->getTransform());

            shared_ptr<EntityRuntime> newRt;
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
                auto dup = entityDef->duplicate();
                if (entityRuntime)
                {
                    auto newRt = entityRuntime->createAndAddChildRuntime(dup);
                    pushPropertyTarget(PropertyType_Entity,dup,newRt);
                }
            }
        }

        drawNameAndIdProperties();

        ImGui::Separator();

        bool isTemplate = entityDef->getIsTemplate();
        if(ImGui::Checkbox("Entity Template", &isTemplate))
        {
            if(entityDef) entityDef->setIsTemplate(isTemplate);
        }

        ImGui::SameLine();

        if (ImGui::Button("Use as Camera"))
        {
            if(entityRuntime)
            {
                auto sr = entityRuntime->getSceneRuntime();
                if (sr)
                {
                    auto camera = sr->getCamera();
                    camera->setCameraEntityUuid(entityRuntime->getUuid());
                    camera->setUseEntity(true);
                }
            }
        }

        ImGui::Separator();


        if (entityDef->getAssetDefinition(AssetType::ASSET_TYPE_ENUM_FONT) != Uuid::INVALID)
        {
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

            ImGui::Separator();
        }

        if (entityRuntime && entityRuntime->hasAnimationRuntime())
        {

            auto animation = entityRuntime->getAnimationRuntime();
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

            ImGui::Separator();
        }

        drawTransformProperties();

        ImGui::Separator();

        ImGui::Text("Assets");

        // Animation =======================================================
        {
            ImGui::Text("Animation");

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
                auto asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION,selectedAnimationAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(-1);
            if(StringCombo("##AnimationCombo",&selectedAnimationAsset,animationAssets,animationAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedAnimationAsset);
                if (entityRuntime)
                {
                    auto selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedAnimationAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedDef->getUuid());
                    }
                }
            }
            ImGui::PopItemWidth();
        }
        // Audio ===========================================================
        {
            ImGui::Text("Audio");

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
                auto asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_AUDIO,selectedAudioAsset);
                pushPropertyTarget(PropertyType_Asset,asset, nullptr);
                return;
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(-1);
            if(StringCombo("##AudioCombo",&selectedAudioAsset,audioAssets,audioAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedAudioAsset);
                if (entityRuntime)
                {
                    auto selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedAudioAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedDef->getUuid());
                    }
                }
            }
            ImGui::PopItemWidth();
        }

        // Font ============================================================
        {
            ImGui::Text("Font");

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
                auto asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_FONT,selectedFontAsset);
                pushPropertyTarget(PropertyType_Asset,asset, nullptr);
                return;
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(-1);
            if(StringCombo("##FontCombo",&selectedFontAsset,fontAssets,fontAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_FONT, selectedFontAsset);
                if (entityRuntime)
                {
                    auto selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_FONT, selectedFontAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_FONT, selectedDef->getUuid());
                    }
                }
            }
            ImGui::PopItemWidth();
        }

        // Model ===========================================================
        {
            ImGui::Text("Model");
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
                auto asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL,selectedModelAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(-1);
            if(StringCombo("##ModelCombo",&selectedModelAsset,modelAssets,modelAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAsset);
                if (entityRuntime)
                {
                    auto selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_MODEL, selectedDef->getUuid());
                    }
                }
            }
            ImGui::PopItemWidth();
        }

        // Path ============================================================
        {
            ImGui::Text("Path");
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
                auto asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PATH,selectedPathAsset);
                pushPropertyTarget(PropertyType_Asset, asset, nullptr);
                return;
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(-1);
            if(StringCombo("##PathCombo",&selectedPathAsset,pathAssets,pathAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PATH, selectedPathAsset);
                if (entityRuntime)
                {
                    auto selectedDef = projectDefinition->getAssetDefinitionAtIndex(ASSET_TYPE_ENUM_PATH, selectedPathAsset);
                    entityRuntime->replaceAssetUuid(ASSET_TYPE_ENUM_PATH, selectedDef->getUuid());
                }
            }
            ImGui::PopItemWidth();
        }

        // Physics Object ==================================================

        {
            ImGui::Text("Physics Object");
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
                auto asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,selectedPhysicsObjectAsset);
                pushPropertyTarget(PropertyType_Asset, asset, nullptr);
                return;
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(-1);
            if(StringCombo("##PhysicsObjectCombo",&selectedPhysicsObjectAsset,poAssets,poAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,selectedPhysicsObjectAsset);
                if (entityRuntime)
                {
                    auto selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT, selectedPhysicsObjectAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT, selectedDef->getUuid());
                    }
                }
            }
            ImGui::PopItemWidth();
        }

        // Script ==========================================================
        {
            ImGui::Text("Script");
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
                auto asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,selectedScriptAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(-1);
            if(StringCombo("##ScriptCombo",&selectedScriptAsset,scriptAssets,scriptAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedScriptAsset);
                if (entityRuntime)
                {
                    auto selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedScriptAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedDef->getUuid());
                    }
                }
            }
            ImGui::PopItemWidth();
        }

        // Texture (Sprite) ================================================
        {
            ImGui::Text("Sprite Texture");
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
                auto asset = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE,selectedTextureAsset);
                pushPropertyTarget(PropertyType_Asset,asset,nullptr);
                return;
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(-1);
            if(StringCombo("##TextureCombo",&selectedTextureAsset,textureAssets,textureAssets.size()))
            {
                entityDef->setSelectedAssetIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, selectedTextureAsset);
                if (entityRuntime)
                {
                    auto selectedDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, selectedTextureAsset);
                    if (selectedDef)
                    {
                        entityRuntime->replaceAssetUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE, selectedDef->getUuid());
                    }
                }
            }
            ImGui::PopItemWidth();
        }
    }

    void
    PropertiesWindow::drawTransformProperties
    ()
    {
        auto entityDefinition = static_pointer_cast<EntityDefinition>(mDefinition);
        auto entityRuntime = static_pointer_cast<EntityRuntime>(mRuntime);

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

        float yaw   = degrees(tx.getYaw());
        float pitch = degrees(tx.getPitch());
        float roll  = degrees(tx.getRoll());

        vec3 translation = tx.getTranslation();
        vec3 scale = tx.getScale();

        bool tx_changed = false;
        tx_changed |= ImGui::DragFloat3("Translation", glm::value_ptr(translation),1.f);
        tx_changed |= ImGui::DragFloat3("Scale", glm::value_ptr(scale),1.f);
        tx_changed |= ImGui::DragFloat("X (Pitch)", &pitch, 1.f);
        tx_changed |= ImGui::DragFloat("Y (Yaw)",   &yaw,   1.f);
        tx_changed |= ImGui::DragFloat("Z (Roll)",  &roll,  1.f);

        if (tx_changed)
        {
            tx.setTranslation(translation);
            tx.setScale(scale);

            tx.setPitch(radians(pitch));
            tx.setYaw(radians(yaw));
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

        if (ImGui::Button("Move to Cursor"))
        {
            if (entityDefinition && entityRuntime)
            {
                vec3 cursorPos = mContext->getCursor()->getTransform().getTranslation();
                auto current_tx = entityRuntime->getTransform();
                current_tx.setTranslation(cursorPos);
                entityRuntime->setTransform(current_tx);
            }
        }

        ImGui::SameLine();

        if(ImGui::Button("Set as Initial"))
        {
            if (entityDefinition && entityRuntime)
            {
                entityDefinition->setTransform(entityRuntime->getTransform());
            }
        }

        ImGui::SameLine();

        if(ImGui::Button("Restore to Initial"))
        {
            if (entityDefinition && entityRuntime)
            {
                entityRuntime->setTransform(entityDefinition->getTransform());
            }
        }
    }

    void
    PropertiesWindow::drawAssetProperties
    ()
    {
        mContext->getPathViewer()->setVisible(false);
        mContext->getAnimationViewer()->setVisible(false);
        auto assetDef = static_pointer_cast<AssetDefinition>(mDefinition);
        if (assetDef == nullptr)
        {
            return;
        }

        ImGui::SameLine();
        if(ImGui::Button("Delete Asset"))
        {
            mContext->getProjectDirectory()->removeAssetDirectory(assetDef);
            auto projectDefinition = mContext->getProject()->getDefinition();
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
            auto dup = assetDef->duplicate();
            auto projectDefinition = mContext->getProject()->getDefinition();
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

        auto projectRuntime = mContext->getProject()->getRuntime();
        if (projectRuntime)
        {
            auto activeScene = projectRuntime->getActiveSceneRuntime();
            if (activeScene)
            {
                ImGui::Separator();
                ImGui::Columns(1);
                if(ImGui::CollapsingHeader("Active Runtimes"))
                {
                    vector<shared_ptr<EntityRuntime>> runtimes = activeScene->getEntitiesWithRuntimeOf(assetDef);
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
        auto animDef = static_pointer_cast<AnimationDefinition>(mDefinition);
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

            ImGui::PushItemWidth(-1);

            if (ImGui::InputFloat3("##translation",glm::value_ptr(kf_tx)))
            {
                kf_transf.setTranslation(kf_tx);
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

            ImGui::PushItemWidth(-1);

            if (ImGui::InputFloat3("##scale",glm::value_ptr(kf_scale)))
            {
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
        auto audioDef = static_pointer_cast<AudioDefinition>(mDefinition);
        auto projectRuntime = mContext->getProject()->getRuntime();
        shared_ptr<AudioRuntime> audioRunt;
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
                auto fm = mContext->getStorageManager();
                auto audioFile = fm->openFile(audioFilePath);
                mContext->setLastDirectory(audioFile->getDirectory());

                LOG_DEBUG("PropertiesWindow: Setting last directory {}",mContext->getLastDirectory());
                audioFile->readBinary();
                auto audioData = audioFile->getBinaryData();
                mContext->getProjectDirectory()->writeAssetData(audioDef,audioData, audioFile->getExtension());
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
        auto def = static_pointer_cast<FontDefinition>(mDefinition);

        if (selectFile)
        {
            nfdchar_t *filePath = NULL;
            nfdfilteritem_t filters[2] = {{"TrueType", "ttf"}, {"OpenType", "otf"}};
            nfdresult_t result = NFD_OpenDialog(&filePath, filters, 2, mContext->getLastDirectory().c_str());

            if ( result == NFD_OKAY )
            {
                LOG_INFO("PropertiesWindow: Success! {}",filePath);
                LOG_DEBUG("PropertiesWindow: Opening Font File {}",filePath);
                auto fm = mContext->getStorageManager();
                auto file = fm->openFile(filePath);
                mContext->setLastDirectory(file->getDirectory());
                file->readBinary();
                auto data = file->getBinaryData();
                mContext->getProjectDirectory()->writeAssetData(def,data);
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
        auto project = mContext->getProject();
        if (project)
        {
            auto projectRuntime = project->getRuntime();
            if (projectRuntime)
            {
                auto fontCache = projectRuntime->getFontCache();
                if (fontCache)
                {
                    auto fontRuntime = fontCache->getRuntime(def);
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
        auto projectDefinition = mContext->getProject()->getDefinition();
        auto projectRuntime = mContext->getProject()->getRuntime();
        auto materialDef = static_pointer_cast<MaterialDefinition>(mDefinition);

        int shaderIndex = 0;
        vector<string> shaderList;
        if (projectDefinition)
        {
            auto shaderAsset = projectDefinition->getAssetDefinitionByUuid(materialDef->getShader());
            shaderIndex = projectDefinition->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderAsset);
            shaderList = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);
        }

        if(StringCombo("Shader", &shaderIndex, shaderList, shaderList.size()))
        {
            if (projectDefinition)
            {
                auto newShader = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderIndex);
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
            auto albedoDef = projectDefinition->getAssetDefinitionByUuid(albedoUuid);
            if (albedoDef)
            {
                auto txDef = static_pointer_cast<TextureDefinition>(albedoDef);
                auto txRuntime = txCache->getRuntime(txDef);
                if (txRuntime)
                {
                    albedoTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }

            // Normal
            auto normalDef = projectDefinition->getAssetDefinitionByUuid(normalUuid);
            if (normalDef)
            {
                auto txDef = static_pointer_cast<TextureDefinition>(normalDef);
                auto txCache = projectRuntime->getTextureCache();
                auto txRuntime = txCache->getRuntime(txDef);
                if (txRuntime)
                {
                    normalTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }

            // Metallic
            auto metallicDef = projectDefinition->getAssetDefinitionByUuid(metallicUuid);
            if (metallicDef)
            {
                auto txDef = static_pointer_cast<TextureDefinition>(metallicDef);
                auto txCache = projectRuntime->getTextureCache();
                auto txRuntime = txCache->getRuntime(txDef);
                if (txRuntime)
                {
                    metallicTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }

            // Roughness
            auto roughnessDef = projectDefinition->getAssetDefinitionByUuid(roughnessUuid);
            if (roughnessDef)
            {
                auto txDef = static_pointer_cast<TextureDefinition>(roughnessDef);
                auto txCache = projectRuntime->getTextureCache();
                auto txRuntime = txCache->getRuntime(txDef);
                if (txRuntime)
                {
                    roughnessTxId = (void*)(intptr_t)txRuntime->getTextureID();
                }
            }

            // AO
            auto aoDef = projectDefinition->getAssetDefinitionByUuid(aoUuid);
            if (aoDef)
            {
                auto txDef = static_pointer_cast<TextureDefinition>(aoDef);
                auto txCache = projectRuntime->getTextureCache();
                auto txRuntime = txCache->getRuntime(txDef);
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
                auto txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, albedoIndex);

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
                auto txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, normalIndex);
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
                auto txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, metallicIndex);
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
                auto txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, roughnessIndex);
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
                auto txDef = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, aoIndex);
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
        auto def = static_pointer_cast<ModelDefinition>(mDefinition);

        bool selectFile = ImGui::Button("Model File...");

        if (selectFile)
        {
            nfdchar_t *filePath = NULL;
            nfdresult_t result = NFD_OpenDialog(&filePath, nullptr, 0, mContext->getLastDirectory().c_str());

            if ( result == NFD_OKAY )
            {
                LOG_ERROR("PropertiesWindow: Opening Model File {}",filePath);
                auto fm = mContext->getStorageManager();
                auto file = fm->openFile(filePath);
                mContext->setLastDirectory(file->getDirectory());
                file->readBinary();
                auto data = file->getBinaryData();
                def->setFormat(Constants::ASSET_TYPE_MODEL+"."+file->getExtension());
                def->setName(file->getNameWithoutExtension());
                mContext->getProjectDirectory()->writeAssetData(def,data);
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
            mContext->getProjectDirectory()->removeAssetDirectory(def);
        }

        auto projectRuntime = mContext->getProject()->getRuntime();
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
                auto modelRuntime = modelCache->getRuntime(def);
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
                auto modelRuntime = modelCache->getRuntime(def);
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
        auto projectDefinition =  mContext->getProject()->getDefinition();
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
            auto currentMaterialDef = projectDefinition->getAssetDefinitionByUuid(currentMaterialUuid);
            int currentMaterialIndex = projectDefinition->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MATERIAL,currentMaterialDef);
            string itemName = "##Material:" + modelMaterial;

            if(StringCombo(itemName.c_str(),&currentMaterialIndex,materialAssetNames,materialAssetNames.size()))
            {
                auto changedMaterial = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MATERIAL, currentMaterialIndex);
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
        auto pod = static_pointer_cast<PhysicsObjectDefinition>(mDefinition);
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
            auto selectedModelAssetDef = projectDefinition->getAssetDefinitionByUuid(selectedModelAssetUuid);
            int selectedModelAssetIndex = projectDefinition->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAssetDef);
            vector<string> modelAssets = projectDefinition->getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MODEL);

            if(StringCombo("Model",&selectedModelAssetIndex,modelAssets,modelAssets.size()))
            {
                auto newlySelected = projectDefinition->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAssetIndex);
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
        auto scriptDef = static_pointer_cast<ScriptDefinition>(mDefinition);
        auto projRunt = mContext->getProject()->getRuntime();
        auto scriptCache = projRunt->getScriptCache();

        shared_ptr<ScriptRuntime> scriptInst;

        if (scriptCache)
        {
            scriptInst = scriptCache->getRuntime(scriptDef);
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
                    LOG_ERROR("PropertiesWindow: Cannot load script template {}",currentTemplateIndex);
                }
                else
                {
                    auto  templateName = templates.at(currentTemplateIndex);
                    string scriptTemplate = mContext->getTemplatesModel()->getTemplate(AssetType::ASSET_TYPE_ENUM_SCRIPT,templateName,Constants::ASSET_FORMAT_SCRIPT_LUA);
                    bool scriptSuccess = false;
                    scriptSuccess = mContext->getProjectDirectory()->writeAssetStringData(
                                scriptDef,
                                scriptTemplate,
                                Constants::ASSET_FORMAT_SCRIPT_LUA);

                    if (scriptSuccess)
                    {
                        mContext->getMenuBar()->setMessageString(
                                    "Saved Script"+scriptDef->getNameAndUuidString());
                    }
                    else
                    {
                        mContext->getMenuBar()->setMessageString(
                                    "Error saving Script"+scriptDef->getNameAndUuidString());
                    }

                    currentTemplateIndex = -1;
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

        if (scriptInst)
        {
            if(ImGui::Button("Reload Asset"))
            {
                scriptInst->setReloadFlag(true);
            }
        }

        if (scriptInst != nullptr)
        {
            ImGui::Separator();

            ImGui::Columns(2);

            ImGui::Text("Source File");

            ImGui::NextColumn();

            if(scriptInst->hasSource())
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f}, "Found");
            }
            else
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f}, "Not Found");
            }

            ImGui::NextColumn();

            ImGui::Text("Loaded");

            ImGui::NextColumn();

            if(scriptInst->getLoaded())
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f}, "Loaded");
            }
            else
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f}, "Not Loaded");
            }

            ImGui::NextColumn();

            ImGui::Text("Load Error");

            ImGui::NextColumn();

            if(scriptInst->getLoadError())
            {
                ImGui::TextColored({1.f,0.f,0.f,1.f}, "Error");
            }
            else
            {
                ImGui::TextColored({0.f,1.f,0.f,1.f}, "No Error");
            }

            ImGui::Columns(1);
        }
    }

    void
    PropertiesWindow::drawShaderAssetProperties
    ()
    {
        auto shaderDef = static_pointer_cast<ShaderDefinition>(mDefinition);
        auto projectRuntime = mContext->getProject()->getRuntime();
        shared_ptr<ShaderRuntime> shaderRuntime;
        auto shaderCache = projectRuntime->getShaderCache();

        if (projectRuntime)
        {
            if (shaderCache)
            {
                shaderRuntime = shaderCache->getRuntime(shaderDef);
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
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
                    vertSuccess = mContext->getProjectDirectory()->writeAssetStringData(
                                shaderDef,
                                vertTemplate,
                                Constants::SHADER_GLSL_VERTEX_FILE_NAME);

                    bool fragSuccess = false;
                    fragSuccess = mContext->getProjectDirectory()->writeAssetStringData(
                                shaderDef,
                                fragTemplate,
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
        static bool  noEditorError = false;

        ImGui::PushItemWidth(-1.f);
        {
            if(ImGui::Button("Open Vertex Shader in Editor..."))
            {
                if (!mContext->getPreferencesModel()->hasExternalEditorPath())
                {
                    noEditorError = true;
                }
                else if (!mContext->openInExternalEditor(shaderDef, Constants::SHADER_GLSL_VERTEX_FILE_NAME))
                {
                    openVertexFailed = true;
                }
            }

            if(ImGui::Button("Open Fragment Shader in Editor..."))
            {
                if (!mContext->getPreferencesModel()->hasExternalEditorPath())
                {
                    noEditorError = true;
                }
                else if (!mContext->openInExternalEditor(shaderDef, Constants::SHADER_GLSL_FRAGMENT_FILE_NAME))
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

        if (noEditorError)
        {
            ImGui::OpenPopup("No External Editor Set");
        }

        if (openVertexFailed)
        {
            ImGui::OpenPopup("Open Vertex Shader Failed");
        }

        if (openFragmentFailed)
        {
            ImGui::OpenPopup("Open Fragment Shader Failed");
        }

        if (ImGui::BeginPopup("No External Editor Set"))
        {
            ImGui::Text("External editor is not set.\n\nPlease set one in File->Preferences");
            if (ImGui::Button("OK",ImVec2(0,0)))
            {
                noEditorError = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("Open Vertex Shader Failed"))
        {
            auto pd = mContext->getProjectDirectory();
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
            auto pd = mContext->getProjectDirectory();
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

            if (shaderRuntime->getLoaded())
            {

                auto uniforms = shaderRuntime->getUniformsVector();

                ImGui::Columns(4);

                ImGui::Text("Location");
                ImGui::NextColumn();
                ImGui::Text("Type");
                ImGui::NextColumn();
                ImGui::Text("Name");
                ImGui::NextColumn();
                ImGui::Text("Status");
                ImGui::NextColumn();

                for (auto& uniform : *uniforms)
                {
                    ImGui::Separator();
                    ImGui::Text("%d",uniform->getLocation());
                    ImGui::NextColumn();
                    ImGui::Text("%s",uniformTypeToString(uniform->getType()).c_str());
                    ImGui::NextColumn();
                    ImGui::Text("%s",uniform->getName().c_str());
                    ImGui::NextColumn();

                    if (shaderRuntime->checkUniformValue(uniform.get()))
                    {
                        ImGui::TextColored({0,1,0,1},"Good");
                    }
                    else
                    {
                        ImGui::TextColored({1,0,0,1},"Bad");
                    }
                    ImGui::NextColumn();
                }

                ImGui::Columns(1);

            }
        }
    }

    void
    PropertiesWindow::drawPathAssetProperties
    ()
    {
        bool needsRegen = false;
        auto pathDef = static_pointer_cast<PathDefinition>(mDefinition);
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
        auto textureDef = static_pointer_cast<TextureDefinition>(mDefinition);
        auto projectRuntime = mContext->getProject()->getRuntime();
        auto pDef = mContext->getProject()->getDefinition();

        shared_ptr<TextureRuntime> textureRuntime;

        if (projectRuntime)
        {
            auto txCache = projectRuntime->getTextureCache();
            textureRuntime = txCache->getRuntime(textureDef);

        }

        bool selectFile = ImGui::Button("Texture File...");

        if (selectFile)
        {
            nfdchar_t *filePath = NULL;
            nfdresult_t result = NFD_OpenDialog(&filePath, nullptr, 0,mContext->getLastDirectory().c_str());

            if ( result == NFD_OKAY )
            {
                LOG_ERROR("PropertiesWindow: Opening Texture File {}",filePath);
                auto fm = mContext->getStorageManager();
                auto file = fm->openFile(filePath);
                mContext->setLastDirectory(file->getDirectory());
                file->readBinary();
                auto data = file->getBinaryData();
                mContext->getProjectDirectory()->writeAssetData(textureDef,data);
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

            bool isHdr = textureRuntime->isHDR();
            ImGui::Text("HDR: %s", isHdr ? "Yes" : "No");
        }

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
                auto shaderDef = static_pointer_cast<ShaderDefinition>(pDef->getAssetDefinitionByUuid(shaderUuid));
                int shaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderDef);

                if (StringCombo("Cube Map Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                    auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    textureDef->setEquiToCubeMapShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched cube map shader for texture {} to {} {}",textureDef->getNameAndUuidString(), name, uuid);
                }
            }
            // Irradiance
            {
                UuidType shaderUuid = textureDef->getIrradianceMapShader();
                auto shaderDef = static_pointer_cast<ShaderDefinition>(pDef->getAssetDefinitionByUuid(shaderUuid));
                int shaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderDef);

                if (StringCombo("Irradiance Map Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                    auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    textureDef->setIrradianceMapShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched irradiance map shader for texture {} to {} {}",textureDef->getNameAndUuidString(), name, uuid);
                }
            }
            // PreFilter
            {
                UuidType shaderUuid = textureDef->getPreFilterShader();
                auto shaderDef = static_pointer_cast<ShaderDefinition>(pDef->getAssetDefinitionByUuid(shaderUuid));
                int shaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderDef);

                if (StringCombo("PreFilter Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                    auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    textureDef->setPreFilterShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched PreFilter shader for texture {} to {} {}",textureDef->getNameAndUuidString(), name, uuid);
                }
            }
            // BRDF_LUT
            {
                UuidType shaderUuid = textureDef->getBrdfLutShader();
                auto shaderDef = static_pointer_cast<ShaderDefinition>(pDef->getAssetDefinitionByUuid(shaderUuid));
                int shaderIndex = pDef->getAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderDef);

                if (StringCombo("BRDF LUT Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                    auto selectedShader = mContext->getProject()->getDefinition()->getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex);
                    UuidType uuid = selectedShader->getUuid();
                    string name = selectedShader->getName();
                    textureDef->setBrdfLutShader(uuid);
                    LOG_DEBUG("PropertiesWindow: Switched BRDF LUT shader for texture {} to {} {}",textureDef->getNameAndUuidString(), name, uuid);
                }
            }

            if (textureRuntime && textureRuntime->getLoaded())
            {
                ImGui::Columns(2);

                ImGui::Text("Cube Map");
                ImGui::NextColumn();
                if (textureRuntime->getCubeMapTextureID() > 0) ImGui::TextColored({0.,1.f,0.f,1.f},"%d", textureRuntime->getCubeMapTextureID());
                else                                           ImGui::TextColored({1.,0.f,0.f,1.f},"%d", textureRuntime->getCubeMapTextureID());
                ImGui::NextColumn();

                ImGui::Text("Irradiance");
                ImGui::NextColumn();
                if (textureRuntime->getIrradianceTextureID() > 0) ImGui::TextColored({0.,1.f,0.f,1.f},"%d", textureRuntime->getIrradianceTextureID());
                else                                              ImGui::TextColored({1.,0.f,0.f,1.f},"%d", textureRuntime->getIrradianceTextureID());
                ImGui::NextColumn();

                ImGui::Text("PreFilter");
                ImGui::NextColumn();
                if (textureRuntime->getPreFilterTextureID() > 0) ImGui::TextColored({0.,1.f,0.f,1.f},"%d", textureRuntime->getPreFilterTextureID());
                else                                             ImGui::TextColored({1.,0.f,0.f,1.f},"%d", textureRuntime->getPreFilterTextureID());
                ImGui::NextColumn();

                ImGui::Text("BRDF LUT");
                ImGui::NextColumn();
                if (textureRuntime->getBrdfLutTextureID() > 0) ImGui::TextColored({0.,1.f,0.f,1.f},"%d", textureRuntime->getBrdfLutTextureID());
                else                                           ImGui::TextColored({1.,0.f,0.f,1.f},"%d", textureRuntime->getBrdfLutTextureID());

                ImGui::Columns(1);


            }
        }

        if (textureRuntime && textureRuntime->getLoaded())
        {
            GLuint t = textureRuntime->getTextureID();
            if (t != 0)
            {
                void* textureId = (void*)(intptr_t)t;
                if (textureId != nullptr)
                {
                    ImGui::Separator();
                    ImGui::Text("Size: %dx%d",textureRuntime->getWidth(),textureRuntime->getHeight());
                    ImGui::Text("Channels: %d",textureRuntime->getChannels());
                    ImGui::Image(textureId, mImageSize);
                }

                void* brdfLutTextureId = (void*)(intptr_t)textureRuntime->getBrdfLutTextureID();
                if (brdfLutTextureId != nullptr)
                {
                    ImGui::Text("BRDF Look up Table");
                    ImGui::Image(brdfLutTextureId,mImageSize);
                }
            }

            static vector<string> cubeDebugTypes =
            {
                "None",
                "Environment",
                "Irradiance",
                "PreFilter"
            };

            int debugMode = (int)textureRuntime->getCubeDebugMode();

            if (StringCombo("Cube Debug", &debugMode, cubeDebugTypes, cubeDebugTypes.size()))
            {
                textureRuntime->setCubeDebugMode((CubeDebugMode)debugMode);
            }
        }
    }

    int
    PropertiesWindow::getStringIndexInVector
    (const string& str, const vector<string>& vec)
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
    (const shared_ptr<AssetDefinition>& assetDef)
    {
        auto projectRuntime = mContext->getProject()->getRuntime();
        if (projectRuntime)
        {
            auto sceneRuntime = projectRuntime->getActiveSceneRuntime();
            if (sceneRuntime)
            {
                auto runts = sceneRuntime->getEntitiesWithRuntimeOf(assetDef);
                for (auto entityRuntime : runts)
                {
                    entityRuntime->replaceAssetUuid(assetDef->getAssetType(),assetDef->getUuid());
                }
            }
        }
    }

}
