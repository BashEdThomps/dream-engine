#include "PropertiesWindow.h"
#include "ImGuiHelpers.h"
#include "../deps/ImGui/imguifilesystem.h"

using Dream::IAssetDefinition;
using Dream::Constants;
using Dream::AssetType;
using Dream::LightDefinition;
using Dream::LightType;
using Dream::ProjectDefinition;
using Dream::SceneDefinition;
using Dream::SceneRuntime;
using Dream::SceneObjectDefinition;
using Dream::SceneObjectRuntime;
using Dream::MaterialDefinition;
using Dream::ProjectRuntime;
using Dream::TextureDefinition;
using Dream::TextureInstance;
using Dream::RGB;

namespace DreamTool
{
    PropertiesWindow::PropertiesWindow
    (Dream::Project* proj)
        : DTWidget (proj),
          mType(PROP_TYPE_NONE),
          mDefinition(nullptr),
          mRuntime(nullptr),
          mImageSize(128,128)
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
        ImGui::Begin("Properties");
        if(ImGui::Button("<- Back"))
        {
            popPropertyTarget();
        }
        switch (mType)
        {
            case DreamTool::PROP_TYPE_NONE:
                break;
            case DreamTool::PROP_TYPE_PROJECT:
                ImGui::SameLine();
                if(ImGui::Button("New Scene"))
                {
                    pushPropertyTarget
                    (
                        DreamTool::PROP_TYPE_SCENE,
                        mProject->getProjectDefinition()->createNewSceneDefinition(),
                        nullptr
                    );
                    break;

                }
                drawNameAndIdProperties();
                drawProjectProperties();
                break;
            case DreamTool::PROP_TYPE_SCENE:
                ImGui::SameLine();
                if(ImGui::Button("Delete Scene"))
                {

                }
                drawNameAndIdProperties();
                drawSceneProperties();
                break;
            case DreamTool::PROP_TYPE_SCENE_OBJECT:
                ImGui::SameLine();
                ImGui::Button("Add Child");
                ImGui::SameLine();
                ImGui::Button("Delete Scene Object");
                drawNameAndIdProperties();
                drawSceneObjectProperties();
                break;
            case DreamTool::PROP_TYPE_ASSET:
                ImGui::SameLine();
                ImGui::Button("Delete Asset");
                drawNameAndIdProperties();
                drawAssetProperties();
                break;

        }
        ImGui::End();
    }

    void
    PropertiesWindow::pushPropertyTarget
    (PropertyType type, IDefinition* def, IRuntime* runt)
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
    (IDefinition* def)
    {
        mDefinition = def;
    }

    void
    PropertiesWindow::setRuntime
    (IRuntime* runt)
    {
        mRuntime = runt;
    }

    void
    PropertiesWindow::clear
    ()
    {
        mType = PROP_TYPE_NONE;
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
        ImGui::Separator();
        // Startup Scene
        int startupScene = static_cast<int>(
            projDef->getSceneDefinitionIndex(
                projDef->getStartupSceneDefinition()
            )
        );
        vector<string> scenes;
        for(SceneDefinition* scene : projDef->getSceneDefinitionsVector())
        {
           scenes.push_back(scene->getName());
        }
        if(StringCombo(
           "Startup Scene",
            &startupScene,
            scenes,
            static_cast<int>(scenes.size()
        )))
        {

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


        if (ProjectRuntime::CurrentSceneRuntime)
        {
            mRuntime = ProjectRuntime::CurrentSceneRuntime;
        }

        auto sceneDef = dynamic_cast<SceneDefinition*>(mDefinition);
        auto sceneRuntime = dynamic_cast<SceneRuntime*>(mRuntime);

        ImGui::Separator();

        // Camera
        float cameraTranslation[3] =
        {
            sceneDef->getCameraTranslationX(),
            sceneDef->getCameraTranslationY(),
            sceneDef->getCameraTranslationZ()
        };

        ImGui::Text("Camera");
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
                sceneRuntime->setCameraTranslation
                ({
                    cameraTranslation[0],
                    cameraTranslation[1],
                    cameraTranslation[2]
                });
            }
        }

        float cameraRotation[3] =
        {
            sceneDef->getCameraPitch(),
            sceneDef->getCameraYaw(),
            0.0f
        };

        if(ImGui::DragFloat3("Pitch Yaw Roll",cameraRotation,0.1f))
        {
            if (sceneDef != nullptr)
            {
                sceneDef->setCameraPitch(cameraRotation[0]);
                sceneDef->setCameraYaw(cameraRotation[1]);

            }

            if (sceneRuntime != nullptr)
            {
                sceneRuntime->setCameraPitch(cameraTranslation[0]);
                sceneRuntime->setCameraYaw(cameraTranslation[1]);
            }

        }
        ImGui::Separator();

        // Rendering
        float meshCull = sceneDef->getMeshCullDistance();

        ImGui::Text("Rendering");
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

        int lightingShaderIndex = sceneDef->getCurrentLightingShaderIndex();
        auto shaderList = mProject->getProjectDefinition()->getAssetNamesVector(AssetType::SHADER);

        if (StringCombo("Lighting Pass Shader", &lightingShaderIndex, shaderList, shaderList.size()))
        {
            auto selectedShader = mProject->getProjectDefinition()->getAssetDefinitionAtIndex(AssetType::SHADER, lightingShaderIndex);
            auto uuid = selectedShader->getUuid();
            auto name = selectedShader->getName();
            sceneDef->setLightingShader(uuid);
            log->error("Switched lighting shader to {} {}", name, uuid);
        }
        ImGui::Separator();

        // Physics
        bool physicsDebug = sceneDef->getPhysicsDebug();
        ImGui::Text("Physics");

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
        ImGui::Separator();

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

    void
    PropertiesWindow::drawSceneObjectProperties
    ()
    {
        auto soDef = dynamic_cast<SceneObjectDefinition*>(mDefinition);
        auto soRuntime = dynamic_cast<SceneObjectRuntime*>(mRuntime);
        auto log = getLog();

        ImGui::Separator();

        ImGui::Columns(2);
        bool hasFocus = soDef->getHasFocus();
        if (ImGui::Checkbox("Has Focus", &hasFocus))
        {
            if (soDef) soDef->setHasFocus(hasFocus);
            if (soRuntime) soRuntime->setHasFocus(hasFocus);
        }
        ImGui::NextColumn();

        bool followsCamera = soDef->getFollowsCamera();
        if (ImGui::Checkbox("Follows Camera", &followsCamera))
        {
            if(soDef) soDef->setFollowsCamera(followsCamera);
            if (soRuntime) soRuntime->setFollowsCamera(followsCamera);
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

        ImGui::Columns(1);

        ImGui::Separator();
        ImGui::Text("Transform");

        ImGui::Columns(2);

        bool transformAbsolute = soDef->getTransform()->getTransformType().compare(Constants::TRANSFORM_TYPE_ABSOLUTE) == 0;
        if(ImGui::RadioButton("Absolute", transformAbsolute))
        {
            soDef->getTransform()->setTransformType(Constants::TRANSFORM_TYPE_ABSOLUTE);
        }
        ImGui::NextColumn();
        if(ImGui::RadioButton("Offset Parent", !transformAbsolute))
        {
            soDef->getTransform()->setTransformType(Constants::TRANSFORM_TYPE_OFFSET);
        }

        ImGui::Columns(1);

        float tx[3] = {
            soDef->getTransform()->getTranslationX(),
            soDef->getTransform()->getTranslationY(),
            soDef->getTransform()->getTranslationZ()
        };
        if (ImGui::DragFloat3("Translation", tx,0.1f))
        {
            if (soDef)
            {
                soDef->getTransform()->setTranslationX(tx[0]);
                soDef->getTransform()->setTranslationY(tx[1]);
                soDef->getTransform()->setTranslationZ(tx[2]);
            }
            if(soRuntime)
            {
                soRuntime->getTransform()->setTranslationX(tx[0]);
                soRuntime->getTransform()->setTranslationY(tx[1]);
                soRuntime->getTransform()->setTranslationZ(tx[2]);
            }
        }

        float rx[3] = {
            soDef->getTransform()->getRotationX(),
            soDef->getTransform()->getRotationY(),
            soDef->getTransform()->getRotationZ()
        };
        if (ImGui::DragFloat3("Rotation",rx,0.1f))
        {
            if(soDef)
            {
                soDef->getTransform()->setRotationX(rx[0]);
                soDef->getTransform()->setRotationX(rx[1]);
                soDef->getTransform()->setRotationX(rx[2]);
            }
            if (soRuntime)
            {
                soRuntime->getTransform()->setRotationX(rx[0]);
                soRuntime->getTransform()->setRotationY(rx[1]);
                soRuntime->getTransform()->setRotationZ(rx[2]);
            }
        }

        float scale[3] = {
            soDef->getTransform()->getScaleX(),
            soDef->getTransform()->getScaleY(),
            soDef->getTransform()->getScaleZ()
        };
        if(ImGui::DragFloat3("Scale",scale,0.1f))
        {
            if(soDef)
            {
                soDef->getTransform()->setScaleX(scale[0]);
                soDef->getTransform()->setScaleY(scale[1]);
                soDef->getTransform()->setScaleZ(scale[2]);
            }
            if(soRuntime)
            {
                soRuntime->getTransform()->setScaleX(scale[0]);
                soRuntime->getTransform()->setScaleY(scale[1]);
                soRuntime->getTransform()->setScaleZ(scale[2]);
            }

        }

        ImGui::Separator();

        ImGui::Text("Assets");
        auto projDef = mProject->getProjectDefinition();

        // Audio
        int selectedAudioAsset = soDef->getSelectedAssetIndex(AssetType::AUDIO);
        vector<string> audioAssets = projDef->getAssetNamesVector(AssetType::AUDIO);
        if(ImGui::Button("-##Audio"))
        {
            soDef->setAssetDefinition(AssetType::AUDIO,"");
        }
        ImGui::SameLine();
        if(ImGui::Button(">##Audio"))
        {
            if (selectedAudioAsset < 0) return;
            auto asset = projDef->getAssetDefinitionAtIndex(AssetType::AUDIO,selectedAudioAsset);
            pushPropertyTarget(
                PropertyType::PROP_TYPE_ASSET,
                asset,
                nullptr
            );
            return;
        }

        ImGui::SameLine();

        if(StringCombo("Audio",&selectedAudioAsset,audioAssets,audioAssets.size()))
        {
            soDef->setSelectedAssetIndex(AssetType::AUDIO, selectedAudioAsset);
        }

        // Light
        int selectedLightAsset = soDef->getSelectedAssetIndex(AssetType::LIGHT);
        vector<string> lightAssets = projDef->getAssetNamesVector(AssetType::LIGHT);

        if(ImGui::Button("-##Light"))
        {
            soDef->setAssetDefinition(AssetType::LIGHT,"");
        }
        ImGui::SameLine();

        if(ImGui::Button(">##Light"))
        {
            if (selectedLightAsset < 0) return;
            auto asset = projDef->getAssetDefinitionAtIndex(AssetType::LIGHT,selectedLightAsset);
            pushPropertyTarget(
                PropertyType::PROP_TYPE_ASSET,
                asset,
                nullptr
            );
            return;
        }

        ImGui::SameLine();

        if(StringCombo("Light",&selectedLightAsset,lightAssets,lightAssets.size()))
        {
            soDef->setSelectedAssetIndex(AssetType::LIGHT, selectedLightAsset);
        }

        // Model
        int selectedModelAsset = soDef->getSelectedAssetIndex(AssetType::MODEL);
        vector<string> modelAssets = projDef->getAssetNamesVector(AssetType::MODEL);

        if(ImGui::Button("-##Model"))
        {

            soDef->setAssetDefinition(AssetType::MODEL,"");
        }
        ImGui::SameLine();

        if(ImGui::Button(">##Model"))
        {
            if (selectedModelAsset < 0) return;
            auto asset = projDef->getAssetDefinitionAtIndex(AssetType::MODEL,selectedModelAsset);
            pushPropertyTarget
            (
                PropertyType::PROP_TYPE_ASSET,
                asset,
                nullptr
            );
            return;
        }

        ImGui::SameLine();

        if(StringCombo("Model",&selectedModelAsset,modelAssets,modelAssets.size()))
        {
            soDef->setSelectedAssetIndex(AssetType::MODEL, selectedModelAsset);
        }

        // Particle Emitter
        int selectedParticleEmitterAsset = soDef->getSelectedAssetIndex(AssetType::PARTICLE_EMITTER);
        vector<string> peAssets = projDef->getAssetNamesVector(AssetType::PARTICLE_EMITTER);
        if(ImGui::Button("-##ParticleEmitter"))
        {

            soDef->setAssetDefinition(AssetType::PARTICLE_EMITTER,"");
        }
        ImGui::SameLine();

        if(ImGui::Button(">##ParticleEmitter"))
        {
            if (selectedParticleEmitterAsset < 0) return;
            auto asset = projDef->getAssetDefinitionAtIndex(AssetType::PARTICLE_EMITTER,selectedParticleEmitterAsset);
            pushPropertyTarget
            (
                PropertyType::PROP_TYPE_ASSET,
                asset,
                nullptr
            );
            return;
        }
        ImGui::SameLine();
        if(StringCombo("Particle Emitter",&selectedParticleEmitterAsset,peAssets,peAssets.size()))
        {
            soDef->setSelectedAssetIndex(AssetType::PARTICLE_EMITTER, selectedParticleEmitterAsset);
        }

        // Path
        int selectedPathAsset = soDef->getSelectedAssetIndex(AssetType::PATH);
        vector<string> pathAssets = projDef->getAssetNamesVector(AssetType::PATH);
        if(ImGui::Button("-##Path"))
        {

            soDef->setAssetDefinition(AssetType::PATH,"");
        }
        ImGui::SameLine();

        if(ImGui::Button(">##Path"))
        {
            if (selectedPathAsset < 0) return;
            auto asset = projDef->getAssetDefinitionAtIndex(AssetType::PATH,selectedPathAsset);
            pushPropertyTarget(
                PropertyType::PROP_TYPE_ASSET,
                asset,
                nullptr
            );
            return;
        }

        ImGui::SameLine();

        if(StringCombo("Path",&selectedPathAsset,pathAssets,pathAssets.size()))
        {
            soDef->setSelectedAssetIndex(AssetType::PATH, selectedPathAsset);
        }

        //Physics Object
        int selectedPhysicsObjectAsset = soDef->getSelectedAssetIndex(AssetType::PHYSICS_OBJECT);
        vector<string> poAssets = projDef->getAssetNamesVector(AssetType::PHYSICS_OBJECT);
        if(ImGui::Button("-##PhysicsObject"))
        {

            soDef->setAssetDefinition(AssetType::PHYSICS_OBJECT,"");
        }
        ImGui::SameLine();

        if(ImGui::Button(">##PhysicsObject"))
        {
            if (selectedPhysicsObjectAsset < 0) return;
            auto asset = projDef->getAssetDefinitionAtIndex(AssetType::PHYSICS_OBJECT,selectedPhysicsObjectAsset);
            pushPropertyTarget(
                PropertyType::PROP_TYPE_ASSET,
                asset,
                nullptr
            );
            return;
        }

        ImGui::SameLine();

        if(StringCombo("Physics Object",&selectedPhysicsObjectAsset,poAssets,poAssets.size()))
        {
            soDef->setSelectedAssetIndex(AssetType::PHYSICS_OBJECT,selectedPhysicsObjectAsset);
        }

        // Script
        int selectedScriptAsset = soDef->getSelectedAssetIndex(AssetType::SCRIPT);
        vector<string> scriptAssets = projDef->getAssetNamesVector(AssetType::SCRIPT);
        if(ImGui::Button("-##Script"))
        {
            soDef->setAssetDefinition(AssetType::SCRIPT,"");
        }
        ImGui::SameLine();

        if(ImGui::Button(">##Script"))
        {
            if (selectedScriptAsset < 0) return;
            auto asset = projDef->getAssetDefinitionAtIndex(AssetType::SCRIPT,selectedScriptAsset);
            pushPropertyTarget(
                PropertyType::PROP_TYPE_ASSET,
                asset,
                nullptr
            );
            return;
        }

        ImGui::SameLine();

        if(StringCombo("Script",&selectedScriptAsset,scriptAssets,scriptAssets.size()))
        {
            soDef->setSelectedAssetIndex(AssetType::SCRIPT, selectedScriptAsset);
        }
    }

    void
    PropertiesWindow::drawAssetProperties
    ()
    {
        ImGui::Separator();
        auto assetDef = dynamic_cast<IAssetDefinition*>(mDefinition);
        auto type = Constants::getAssetTypeEnumFromString(assetDef->getType());
        switch (type)
        {
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
    }

    void
    PropertiesWindow::drawAudioAssetProperties
    ()
    {
        auto log = getLog();
        bool selectAudioFile = ImGui::Button("Audio File...");

        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectAudioFile,nullptr,".ogg;.wav","Select Audio File");
        if (strlen(chosenPath) > 0)
        {
            auto projectDir = openDlg.getChosenPath();
            log->error("Opening Audio File {}",projectDir);
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {

        }
    }

    void
    PropertiesWindow::drawFontAssetProperties
    ()
    {

        ImGui::Button("Font File..."); ImGui::SameLine(); ImGui::Button("Remove File");
    }

    void PropertiesWindow::drawLightAssetProperties
    ()
    {
       auto lightDef = dynamic_cast<LightDefinition*>(mDefinition);
       float color[3];
       float f;

       switch (lightDef->getType())
       {
               break;
           case LightType::LT_POINT:
               ImGui::DragFloat3("Position",color,0.1f);
               ImGui::DragFloat3("Direction",color,0.1f);
               ImGui::DragFloat("Constant",&f,1.0f);
               ImGui::DragFloat("Linear",&f,0.01f);
               ImGui::DragFloat("Quadratic",&f,0.001f);
               break;
           case LightType::LT_DIRECTIONAL:
                ImGui::DragFloat3("Direction", color, 0.1f);
                break;
           case LightType::LT_SPOTLIGHT:
               ImGui::DragFloat("Inner Cut Off", color);
               ImGui::DragFloat("Outer Cut Off", color);
               ImGui::DragFloat("Constant", &f);
               ImGui::DragFloat("Linear", &f);
               ImGui::DragFloat("Quadratic", &f);

               break;
           case LightType::LT_NONE:
               break;
       }
       ImGui::Separator();
       ImGui::ColorEdit3("Ambient", color);
       ImGui::ColorEdit3("Diffuse", color);
       ImGui::ColorEdit3("Specular", color);
    }

    void
    PropertiesWindow::drawMaterialAssetProperties
    ()
    {
        auto projDef = mProject->getProjectDefinition();
        auto projRunt = mProject->getProjectRuntime();
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
                auto txInstance = dynamic_cast<TextureInstance*>(txCache->getInstance(txDef));
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
                auto txInstance = dynamic_cast<TextureInstance*>(txCache->getInstance(txDef));
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
                auto txInstance = dynamic_cast<TextureInstance*>(txCache->getInstance(txDef));
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
                auto txInstance = dynamic_cast<TextureInstance*>(txCache->getInstance(txDef));
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
        ImGui::Columns(2);
        ImGui::Button("Model File...");
        ImGui::NextColumn();
        ImGui::Button("Remove File");
        ImGui::Columns(1);

        ImGui::Separator();
        ImGui::Text("Material Map");
        ImGui::Separator();
        ImGui::Columns(2);
        ImGui::Text("From Model");
        ImGui::NextColumn();
        ImGui::Text("In Dream");
        ImGui::NextColumn();

        int index = 1;
        char* materials[3] = {"Material 1", "Material 2", "Material 3" };
        for (int i=0; i<5; i++)
        {
            ImGui::Text("Material %d",i);
            ImGui::NextColumn();
            ImGui::Combo("##hidelabel",&index, &materials[0],3);
            ImGui::NextColumn();
        }
    }

    void
    PropertiesWindow::drawPhysicsObjectAssetProperties
    ()
    {

    }

    void
    PropertiesWindow::drawScriptProperties
    ()
    {
        char buf[512] = {0};
        char* templates[3] = {"Template 1","Template 2","Template 3"};
        int currentIndex = 2;
        ImGui::Columns(2);
        ImGui::Button("Save");
        ImGui::NextColumn();
        ImGui::Button("Revert");
        ImGui::Columns(1);
        ImGui::Combo("Template",&currentIndex,templates[0],3);
        ImGui::InputTextMultiline("##hidelabel",buf,512);
    }

    void
    PropertiesWindow::drawShaderAssetProperties
    ()
    {
        char buf[512] = {0};
        char buf2[512] = {0};
        char* templates[3] = {"Template 1","Template 2","Template 3"};
        int currentIndex = 2;
        ImGui::Columns(2);
        ImGui::Button("Save");
        ImGui::NextColumn();
        ImGui::Button("Revert");
        ImGui::Columns(1);
        ImGui::Combo("Template",&currentIndex,templates[0],3);
        ImGui::Separator();
        ImGui::InputTextMultiline("Vertex Shader",buf,512);
        ImGui::Separator();
        ImGui::InputTextMultiline("Fragment Shader",buf2,512);
        ImGui::Separator();
        ImGui::Text("Uniforms");

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
        auto projRunt = mProject->getProjectRuntime();
        if (projRunt)
        {
           auto txCache = projRunt->getTextureCache();
           auto txInstance = dynamic_cast<TextureInstance*>(txCache->getInstance(textureDef));
           if (txInstance)
           {
                textureId = (void*)(intptr_t)txInstance->getGLID();
           }
        }
        ImGui::Columns(2);
        ImGui::Button("Texture File...");
        ImGui::NextColumn();
        ImGui::Button("Remove File");
        ImGui::Columns(1);
        ImGui::Image(textureId, mImageSize);
    }
}
