#include "../../Window/DTWindowComponent.h"
#include "PropertiesWindow.h"
#include "../../deps/ImGui/imguifilesystem.h"
#include "../../Model/TemplatesDirectoryModel.h"

namespace DreamTool
{
    PropertiesWindow::PropertiesWindow
    (Project* proj)
        : ImGuiWidget (proj),
          mType(PROP_TYPE_NONE),
          mDefinition(nullptr),
          mRuntime(nullptr),
          mImageSize(256,256),
          mBigEditorSize(-1,-1)
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

    void PropertiesWindow::clearPropertyTargets()
    {
        mHistory.clear();
        setPropertyType(PropertyType::PROP_TYPE_NONE);
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
        auto audioDef = dynamic_cast<AudioDefinition*>(mDefinition);

        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectAudioFile,nullptr,".ogg;.wav","Select Audio File");
        if (strlen(chosenPath) > 0)
        {
            auto audioFilePath = openDlg.getChosenPath();
            log->error("Opening Audio File {}",audioFilePath);
            File audioFile(audioFilePath);
            auto audioData = audioFile.readBinary();
            ProjectDirectory projectDir(mProject);
            projectDir.writeAssetData(audioDef,audioData);
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {
            ProjectDirectory projectDir(mProject);
            projectDir.removeAssetDirectory(audioDef);
        }
    }

    void
    PropertiesWindow::drawFontAssetProperties
    ()
    {
        auto log = getLog();
        bool selectFile = ImGui::Button("Font File...");
        auto def = dynamic_cast<FontDefinition*>(mDefinition);

        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectFile,nullptr,".ttf","Select Font File");
        if (strlen(chosenPath) > 0)
        {
            auto filePath = openDlg.getChosenPath();
            log->error("Opening Font File {}",filePath);
            File file(filePath);
            auto data = file.readBinary();
            ProjectDirectory projectDir(mProject);
            projectDir.writeAssetData(def,data);
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {
            ProjectDirectory projectDir(mProject);
            projectDir.removeAssetDirectory(def);
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
        auto log = getLog();
        auto def = dynamic_cast<ModelDefinition*>(mDefinition);

        bool selectFile = ImGui::Button("Model File...");
        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectFile,nullptr,".obj","Select Model File");
        if (strlen(chosenPath) > 0)
        {
            auto filePath = openDlg.getChosenPath();
            log->error("Opening Model File {}",filePath);
            File file(filePath);
            auto data = file.readBinary();
            ProjectDirectory projectDir(mProject);
            projectDir.writeAssetData(def,data);
        }

        ImGui::SameLine();

        bool selectAdditionalFile = ImGui::Button("Additional File...");
        static ImGuiFs::Dialog openAdditionalFileDlg;
        const char* chosenAdditionalFilePath =
                openAdditionalFileDlg.chooseFileDialog(selectAdditionalFile,nullptr,nullptr,"Select Additional File");
        if (strlen(chosenAdditionalFilePath) > 0)
        {
            auto filePath = openAdditionalFileDlg.getChosenPath();
            log->error("Opening Additional Model File {}",filePath);
            File file(filePath);
            auto data = file.readBinary();
            ProjectDirectory projectDir(mProject);
            projectDir.writeAssetData(def,data,file.name());
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File(s)"))
        {
            ProjectDirectory projectDir(mProject);
            projectDir.removeAssetDirectory(def);
        }

        // Material List --------------------------------------------------------

        vector<string> modelMaterialNames;
        auto projRunt = mProject->getProjectRuntime();
        if (projRunt)
        {
            auto modelCache = projRunt->getModelCache();
            if (modelCache)
            {
                auto modelInstance = dynamic_cast<ModelInstance*>(modelCache->getInstance(def));
                if (modelInstance)
                {
                    modelMaterialNames = modelInstance->getMaterialNames();
                }
            }
        }

        ImGui::Separator();
        ImGui::Text("Material Mappings");
        ImGui::Separator();
        ImGui::Columns(2);
        ImGui::Text("From Model");
        ImGui::NextColumn();
        ImGui::Text("In Dream");
        ImGui::NextColumn();

        vector<string> materialAssetNames;
        auto projDef =  mProject->getProjectDefinition();
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
        auto pod = dynamic_cast<PhysicsObjectDefinition*>(mDefinition);
        vector<string> poFormats = Constants::DREAM_ASSET_FORMATS_MAP[AssetType::PHYSICS_OBJECT];
        string poFormatString = pod->getFormat();
        int poFormatIndex = getStringIndexInVector(poFormatString, poFormats);
        if(StringCombo("Format",&poFormatIndex, poFormats,poFormats.size()))
        {
           pod->setFormat(poFormats.at(poFormatIndex));
        }
        ImGui::Separator();

        bool kinematic = pod->getKinematic();
        if (ImGui::Checkbox("Kinematic",&kinematic))
        {
            pod->setKinematic(kinematic);
        }

        bool controllable = pod->getControllableCharacter();
        if (ImGui::Checkbox("Controllable Character",&controllable))
        {
            pod->setControllableCharacter(controllable);
        }

        ImGui::Separator();

        float mass = pod->getMass();
        if(ImGui::InputFloat("Mass",&mass))
        {
            pod->setMass(mass);
        }

        float margin = pod->getMargin();
        if(ImGui::InputFloat("Margin",&margin))
        {
            pod->setMargin(margin);
        }

        float restitution = pod->getRestitution();
        if(ImGui::InputFloat("Restitution",&restitution))
        {
            pod->setRestitution(restitution);
        }

        float friction = pod->getFriction();
        if(ImGui::InputFloat("Friction",&friction))
        {
            pod->setFriction(friction);
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
            auto projDef = mProject->getProjectDefinition();

            string selectedModelAssetUuid = pod->getCollisionModel();
            IAssetDefinition* selectedModelAssetDef = projDef->getAssetDefinitionByUuid(selectedModelAssetUuid);
            int selectedModelAssetIndex = projDef->getAssetDefinitionIndex(AssetType::MODEL, selectedModelAssetDef);
            vector<string> modelAssets = projDef->getAssetNamesVector(AssetType::MODEL);

            if(StringCombo("Model",&selectedModelAssetIndex,modelAssets,modelAssets.size()))
            {
                IAssetDefinition* newlySelected = projDef->getAssetDefinitionAtIndex(AssetType::MODEL, selectedModelAssetIndex);
                pod->setCollisionModel(newlySelected->getUuid());
            }
        }
    }

    void
    PropertiesWindow::drawScriptProperties
    ()
    {
        auto log = getLog();
        auto scriptDef = dynamic_cast<ScriptDefinition*>(mDefinition);
        auto projRunt = mProject->getProjectRuntime();
        char buf[BigEditorBufferSize] = {0};
        ScriptInstance* scriptInst = nullptr;
        if (projRunt)
        {
            auto scriptCache = projRunt->getScriptCache();
            if (scriptCache)
            {
                scriptInst = dynamic_cast<ScriptInstance*>(scriptCache->getInstance(scriptDef));
                if (scriptInst)
                {
                    strncpy(buf,scriptInst->getSource().c_str(),BigEditorBufferSize);
                }
            }
        }

        ImGui::Columns(2);

        if(ImGui::Button("Save"))
        {
            if(scriptInst)
            {
                ProjectDirectory pd(mProject);
                string source = scriptInst->getSource();
                vector<char> data(source.begin(),source.end());
                pd.writeAssetData(scriptDef,data,Constants::ASSET_FORMAT_SCRIPT_LUA);
            }
        }

        ImGui::NextColumn();

        if(ImGui::Button("Revert"))
        {

        }

        ImGui::Columns(1);

        TemplatesDirectoryModel templateModel;
        vector<string> templates = templateModel.getTemplateNames(AssetType::SCRIPT);
        static int currentTemplateIndex = -1;
        if (StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
        {
            ImGui::OpenPopup("Load From Template?");
        }

        ImGui::Text("Script");



        ImGui::PushFont(DTWindowComponent::MonoFont);
        ImGui::PushItemWidth(-1);
        if (ImGui::InputTextMultiline("##hidelabel",buf,BigEditorBufferSize,mBigEditorSize))
        {
            if (scriptInst)
            {
                scriptInst->setSource(&buf[0]);
            }
        }
        ImGui::PopItemWidth();
        ImGui::PopFont();

        if(ImGui::BeginPopupModal("Load From Template?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Load Script from template?\n\nThis will replace the existing Script.\n\n");
            if (ImGui::Button("Cancel"))
            {
               ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button("Load Template"))
            {
               if (currentTemplateIndex < 0)
               {
                   log->error("Cannot load Script template at index {}",currentTemplateIndex);
               }
               else
               {
                   auto templateName = templates.at(currentTemplateIndex);
                   auto templateStr = templateModel.getTemplate(AssetType::SCRIPT, templateName, Constants::ASSET_FORMAT_SCRIPT_LUA);
                   if (scriptInst)
                   {
                       scriptInst->setSource(templateStr);
                   }
                   else
                   {
                       log->error("Cannot set from template, script instance is null");
                   }
               }
               ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
    }

    void
    PropertiesWindow::drawShaderAssetProperties
    ()
    {
        auto log = getLog();

        char vertBuf[BigEditorBufferSize] = {0};
        char fragBuf[BigEditorBufferSize] = {0};

        auto shaderDef = dynamic_cast<ShaderDefinition*>(mDefinition);
        auto projRunt = mProject->getProjectRuntime();
        ShaderInstance* shaderInst = nullptr;
        if (projRunt)
        {
            auto shaderCache = projRunt->getShaderCache();
            if (shaderCache)
            {
                shaderInst = dynamic_cast<ShaderInstance*>(shaderCache->getInstance(shaderDef));
                if (shaderInst)
                {
                    strncpy(vertBuf,shaderInst->getVertexSource().c_str(),BigEditorBufferSize);
                    strncpy(fragBuf,shaderInst->getFragmentSource().c_str(),BigEditorBufferSize);
                }
            }
        }
        TemplatesDirectoryModel tmd;
        vector<string> templates = tmd.getTemplateNames(AssetType::SHADER);
        static int currentTemplateIndex = -1;
        ImGui::Columns(2);
        if(ImGui::Button("Save"))
        {
           if (shaderInst)
           {
               ProjectDirectory pd(mProject);

               string vSource = shaderInst->getVertexSource();
               string fSource = shaderInst->getFragmentSource();

               vector<char> vData(vSource.begin(),vSource.end());
               vector<char> fData(fSource.begin(),fSource.end());

               pd.writeAssetData(shaderDef,vData,Constants::SHADER_VERTEX_FILE_NAME);
               pd.writeAssetData(shaderDef,fData,Constants::SHADER_FRAGMENT_FILE_NAME);
           }
        }

        ImGui::NextColumn();

        ImGui::Button("Revert");

        ImGui::Columns(1);

        if(StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
        {
            ImGui::OpenPopup("Load From Template?");
        }

        ImGui::Separator();

        ImGui::Columns(2);
        ImGui::Text("Vertex Shader");
        ImGui::PushItemWidth(-1);
        ImGui::PushFont(DTWindowComponent::MonoFont);
        if(ImGui::InputTextMultiline("##Vertex Shader",vertBuf,BigEditorBufferSize,mBigEditorSize))
        {
            if (shaderInst)
            {
                shaderInst->setVertexSource(&vertBuf[0]);
            }
        }
        ImGui::PopFont();
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        ImGui::Text("Fragment Shader");
        ImGui::PushItemWidth(-1);
        ImGui::PushFont(DTWindowComponent::MonoFont);
        if(ImGui::InputTextMultiline("##Fragment Shader",fragBuf,BigEditorBufferSize,mBigEditorSize))
        {
            if (shaderInst)
            {
                shaderInst->setFragmentSource(&fragBuf[0]);
            }
        }
        ImGui::PopFont();
        ImGui::PopItemWidth();
        ImGui::Columns(1);

        if(ImGui::BeginPopupModal("Load From Template?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Load Shader from template?\n\nThis will replace the existing Shader.\n\n");
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Template"))
            {
                if (currentTemplateIndex < 0)
                {
                    log->error("Cannot load template {}",currentTemplateIndex);
                }
                else
                {
                    auto  templateName = templates.at(currentTemplateIndex);
                    string vertTemplate = tmd.getTemplate(AssetType::SHADER,templateName,Constants::SHADER_VERTEX_FILE_NAME);
                    string fragTemplate = tmd.getTemplate(AssetType::SHADER,templateName,Constants::SHADER_FRAGMENT_FILE_NAME);
                    if (shaderInst)
                    {
                        shaderInst->setVertexSource(vertTemplate);
                        shaderInst->setFragmentSource(fragTemplate);
                    }
                    else
                    {
                        log->error("Cannot set template, shader is null");
                    }
                    currentTemplateIndex = -1;
                }
               ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
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

        auto log = getLog();
        bool selectFile = ImGui::Button("Texture File...");
        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFileDialog(selectFile,nullptr,".png","Select Texture File");
        if (strlen(chosenPath) > 0)
        {
            auto filePath = openDlg.getChosenPath();
            log->error("Opening Texture File {}",filePath);
            File file(filePath);
            auto data = file.readBinary();
            ProjectDirectory projectDir(mProject);
            projectDir.writeAssetData(textureDef,data);
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove File"))
        {
            ProjectDirectory projectDir(mProject);
            projectDir.removeAssetDirectory(textureDef);
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
}
