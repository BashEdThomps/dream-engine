#include "PropertiesWindow.h"
#include "ImGuiHelpers.h"

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
using Dream::RGB;

namespace DreamTool
{
    PropertiesWindow::PropertiesWindow
    (Dream::Project* proj)
        : DTWidget (proj),

          mType(PROP_TYPE_NONE),
          mDefinition(nullptr),
          mRuntime(nullptr)
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
        drawNameAndIdProperties();
        switch (mType)
        {
            case DreamTool::PROP_TYPE_NONE:
                break;
            case DreamTool::PROP_TYPE_PROJECT:
                drawProjectProperties();
                break;
            case DreamTool::PROP_TYPE_SCENE:
                drawSceneProperties();
                break;
            case DreamTool::PROP_TYPE_SCENE_OBJECT:
                drawSceneObjectProperties();
                break;
            case DreamTool::PROP_TYPE_ASSET:
                drawAssetProperties();
                break;

        }
        ImGui::End();
    }

    void
    PropertiesWindow::drawNameAndIdProperties
    ()
    {
        if (mDefinition != nullptr)
        {
			ImGui::Columns(2);
            char idBuf[128] = {0};
            strncpy(&idBuf[0],mDefinition->getUuid().c_str(),mDefinition->getUuid().size());
            if(ImGui::InputText("ID", idBuf, IM_ARRAYSIZE(idBuf)))
            {
                mDefinition->setUuid(idBuf);
            }
			ImGui::NextColumn();

            char nameBuf[128] = {0};
            strncpy(&nameBuf[0],mDefinition->getName().c_str(),mDefinition->getName().size());
            if(ImGui::InputText("Name", nameBuf, IM_ARRAYSIZE(nameBuf)))
            {
               mDefinition->setName(nameBuf);
            }
			ImGui::Columns(1);
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

        if (ImGui::Checkbox("Keyboard",&usingKeyboard))
        {
            projDef->setCaptureKeyboard(usingKeyboard);
        }
        if(ImGui::Checkbox("Mouse", &usingMouse))
        {
            projDef->setCaptureMouse(usingMouse);
        }
        if(ImGui::Checkbox("Gamepad",  &usingGamepad))
        {
            projDef->setCaptureJoystick(usingGamepad);
        }
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
            auto selectedShader = mProject->getProjectDefinition()->getShaderDefinitionAtIndex(lightingShaderIndex);
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

        int selectedAudioAsset = soDef->getSelectedAssetIndex(AssetType::AUDIO);
        vector<string> audioAssets = projDef->getAssetNamesVector(AssetType::AUDIO);
        ImGui::Button("Edit");
        ImGui::SameLine();
        if(StringCombo("Audio",&selectedAudioAsset,audioAssets,audioAssets.size()))
        {

        }

        int selectedLightAsset = soDef->getSelectedAssetIndex(AssetType::LIGHT);
        vector<string> lightAssets = projDef->getAssetNamesVector(AssetType::LIGHT);
        ImGui::Button("Edit");
        ImGui::SameLine();
        if(StringCombo("Light",&selectedLightAsset,lightAssets,lightAssets.size()))
        {

        }

        int selectedModelAsset = soDef->getSelectedAssetIndex(AssetType::MODEL);
        vector<string> modelAssets = projDef->getAssetNamesVector(AssetType::MODEL);
        ImGui::Button("Edit");
        ImGui::SameLine();
        if(StringCombo("Model",&selectedModelAsset,modelAssets,modelAssets.size()))
        {

        }

        int selectedParticleEmitterAsset = soDef->getSelectedAssetIndex(AssetType::PARTICLE_EMITTER);
        vector<string> peAssets = projDef->getAssetNamesVector(AssetType::PARTICLE_EMITTER);
        ImGui::Button("Edit");
        ImGui::SameLine();
        if(StringCombo("Particle Emitter",&selectedParticleEmitterAsset,peAssets,peAssets.size()))
        {

        }

        int selectedPathAsset = soDef->getSelectedAssetIndex(AssetType::PATH);
        vector<string> pathAssets = projDef->getAssetNamesVector(AssetType::PATH);
        ImGui::Button("Edit");
        ImGui::SameLine();
        if(StringCombo("Path",&selectedPathAsset,pathAssets,pathAssets.size()))
        {

        }

        int selectedPhysicsObjectAsset = soDef->getSelectedAssetIndex(AssetType::PHYSICS_OBJECT);
        vector<string> poAssets = projDef->getAssetNamesVector(AssetType::PHYSICS_OBJECT);
        ImGui::Button("Edit");
        ImGui::SameLine();
        if(StringCombo("Physics Object",&selectedPhysicsObjectAsset,poAssets,poAssets.size()))
        {

        }

        int selectedScriptAsset = soDef->getSelectedAssetIndex(AssetType::SCRIPT);
        vector<string> scriptAssets = projDef->getAssetNamesVector(AssetType::SCRIPT);
        ImGui::Button("Edit");
        ImGui::SameLine();
        if(StringCombo("Script",&selectedScriptAsset,scriptAssets,scriptAssets.size()))
        {

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
        ImGui::Button("Audio File..."); ImGui::SameLine(); ImGui::Button("Remove File");
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
               ImGui::DragFloat("Quadratic",&f),0.001f;
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
		auto materialDef = dynamic_cast<MaterialDefinition*>(mDefinition);
		int index = 0;
		char* shader[3] = { "Shader 1","Shader 2","Shader 3" };
		ImGui::Combo("Shader", &index, shader, 3);
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

        char* tex[3] = {"Texture 1","Texture 2","Texture 3"};
		ImVec2 imgSize(256, 256);

		ImGui::Columns(2);

		// Diffuse
		ImGui::Image(0, imgSize);
		ImGui::NextColumn();
        ImGui::Combo("Diffuse",&index,tex,3);
		ImGui::NextColumn();
		ImGui::Separator();
		
		// Specular
		ImGui::Image(0, imgSize);
		ImGui::NextColumn();
        ImGui::Combo("Specular",&index,tex,3);
		ImGui::NextColumn();
		ImGui::Separator();
		
		// Normal
		ImGui::Image(0, imgSize);
		ImGui::NextColumn();
        ImGui::Combo("Normal",&index,tex,3);
		ImGui::NextColumn();
		ImGui::Separator();
		
		// Displacement
		ImGui::Image(0, imgSize);
		ImGui::NextColumn();
        ImGui::Combo("Displacement",&index,tex,3);
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
        ImGui::Columns(2);
        ImGui::Button("Texture File...");
        ImGui::NextColumn();
        ImGui::Button("Remove File");
        ImGui::Columns(1);

		ImVec2 imgSize(512, 512);
		ImGui::Image(0, imgSize);
    }
}
