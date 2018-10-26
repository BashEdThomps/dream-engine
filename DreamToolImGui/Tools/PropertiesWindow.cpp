#include "PropertiesWindow.h"

using Dream::IAssetDefinition;
using Dream::Constants;
using Dream::AssetType;
using Dream::LightDefinition;
using Dream::LightType;

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

    PropertiesWindow::~PropertiesWindow()
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
            char idBuf[128] = {0};
            strncpy(&idBuf[0],mDefinition->getUuid().c_str(),mDefinition->getUuid().size());

            char nameBuf[128] = {0};
            strncpy(&nameBuf[0],mDefinition->getName().c_str(),mDefinition->getName().size());
            ImGui::InputText
            (
                "ID", idBuf, IM_ARRAYSIZE(idBuf),
                ImGuiInputTextFlags_CallbackAlways,
                [](ImGuiTextEditCallbackData* data)
                {
                    auto def = static_cast<IDefinition*>(data->UserData);
                    auto str = string(data->Buf,data->BufTextLen);
                    if (str.compare(def->getUuid()))
                    {
                        def->setUuid(str);
                    }
                    return 0;
                },
                mDefinition
            );

            ImGui::InputText
            (
                "Name", nameBuf, IM_ARRAYSIZE(nameBuf),
                ImGuiInputTextFlags_CallbackAlways,
                [](ImGuiTextEditCallbackData* data)
                {
                    auto def = static_cast<IDefinition*>(data->UserData);
                    auto str = string(data->Buf,data->BufTextLen);
                    if (str.compare(def->getName()))
                    {
                        def->setName(str);
                    }
                    return 0;
                },
                mDefinition
            );
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
        ImGui::Separator();
        // Startup Scene
        int startupScene = 1;
        char* scenes[3] = {"Scene 1","Scene 2","Scene 3"};
        ImGui::Combo("Startup Scene",&startupScene, scenes, 3);
        ImGui::Separator();

        // Capture mouse,keyboard,joystick
        bool usingKeyboard, usingMouse, usingGamepad;
        ImGui::Checkbox("Keyboard", &usingKeyboard);
        ImGui::Checkbox("Mouse",    &usingMouse);
        ImGui::Checkbox("Gamepad",  &usingGamepad);
        ImGui::Separator();

        // Window Dimensions
        float windowSize[2];
        ImGui::DragFloat2("Window Size",windowSize);
        ImGui::Separator();

        // Author
        char authorBuffer[512] = {"Ashley Thompson"};
        ImGui::InputText("Author",authorBuffer,512);

        // Description
        char descriptionBuffer[512] = {"Something really cool"};
        ImGui::InputTextMultiline("Description",descriptionBuffer,512);

        // Notes
        char notesBuffer[512] = {
            "Important Notes:\n"
            "\t* Do This\n"
            "\t* Do That\n"
            "\t* Do The Other\n"
        };
        ImGui::InputTextMultiline("Notes",notesBuffer,512);
    }

    void
    PropertiesWindow::drawSceneProperties
    ()
    {
        ImGui::Separator();

        // Camera
        float cameraTranslation[3];
        float cameraRotation[3];
        ImGui::Text("Camera");
        ImGui::DragFloat3("Translation",cameraTranslation);
        ImGui::DragFloat3("Rotation",cameraRotation);
        ImGui::Separator();

        // Rendering
        float meshCull;
        float drawDistance[2];
        float clearColor[3];
        float ambientColor[3];
        int lightingShader = 0;
        char* items[3] = {"Shader1","Shader2","Shader3"};

        ImGui::Text("Rendering");
        ImGui::DragFloat("Mesh Cull",&meshCull);
        ImGui::DragFloat2("Draw Distance",drawDistance);
        ImGui::DragFloat3("Clear Color",clearColor);
        ImGui::DragFloat3("Clear Color",ambientColor);
        ImGui::Combo("Lighting Pass Shader",&lightingShader,items,3);
        ImGui::Separator();

        // Physics
        bool physicsDebug;
        float gravity[3];
        ImGui::Text("Physics");
        ImGui::Checkbox("Debug",&physicsDebug);
        ImGui::DragFloat3("Gravity",gravity);
        ImGui::Separator();

        // Notes
        char notesBuffer[512] = {"Here are some notes"};
        ImGui::InputTextMultiline("Notes",notesBuffer,512);

    }

    void
    PropertiesWindow::drawSceneObjectProperties
    ()
    {
        ImGui::Separator();

        bool focus = true;
        bool followsCamera = false;
        ImGui::Checkbox("Has Focus",&focus);
        ImGui::SameLine();
        ImGui::Checkbox("Follows Camera",&followsCamera);
        ImGui::Checkbox("Always Draw",&followsCamera);
        ImGui::SameLine();
        ImGui::Checkbox("Hidden",&followsCamera);
        ImGui::Separator();

        float tx[3], rx[3], scale[3];
        ImGui::DragFloat3("Translation",tx);
        ImGui::DragFloat3("Rotation",rx);
        ImGui::DragFloat3("Scale",scale);
        ImGui::Separator();

        int selectedAsset = 1;
        char* assets[3] {"Asset 1","Asset 2","Asset 3"};
        ImGui::Text("Assets");
        ImGui::Combo("Audio",&selectedAsset,assets,3);
        ImGui::Combo("Light",&selectedAsset,assets,3);
        ImGui::Combo("Model",&selectedAsset,assets,3);
        ImGui::Combo("Particle Emitter",&selectedAsset,assets,3);
        ImGui::Combo("Path",&selectedAsset,assets,3);
        ImGui::Combo("Physics Object",&selectedAsset,assets,3);
        ImGui::Combo("Script",&selectedAsset,assets,3);
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
        ImGui::Separator();
       auto lightDef = dynamic_cast<LightDefinition*>(mDefinition);
       float color[3];
       float f;

       switch (lightDef->getType())
       {
               break;
           case LightType::LT_POINT:
               ImGui::DragFloat3("Position",color);
               ImGui::DragFloat3("Direction",color);
               ImGui::DragFloat("Constant",&f);
               ImGui::DragFloat("Linear",&f);
               ImGui::DragFloat("Quadraticl",&f);
               break;
           case LightType::LT_DIRECTIONAL:
               ImGui::DragFloat3("Direction",color);
               break;
           case LightType::LT_SPOTLIGHT:
               ImGui::DragFloat("Inner Cut Off",color);
               ImGui::DragFloat("Outer Cut Off",color);
               ImGui::DragFloat("Constant",&f);
               ImGui::DragFloat("Linear",&f);
               ImGui::DragFloat("Quadraticl",&f);

               break;
           case LightType::LT_NONE:
               break;
       }
       ImGui::Separator();
       ImGui::ColorEdit3("Ambient",color);
       ImGui::ColorEdit3("Diffuse",color);
       ImGui::ColorEdit3("Specular",color);
    }

    void
    PropertiesWindow::drawMaterialAssetProperties
    ()
    {}

    void
    PropertiesWindow::drawModelAssetProperties
    ()
    {
        ImGui::Button("Model File..."); ImGui::SameLine(); ImGui::Button("Remove File");
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
        ImGui::Button("Save"); ImGui::SameLine(); ImGui::Button("Revert");
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
        ImGui::Button("Save"); ImGui::SameLine(); ImGui::Button("Revert");
        ImGui::Combo("Template",&currentIndex,templates[0],3);
        ImGui::Separator();
        ImGui::InputTextMultiline("Vertex Shader",buf,512);
        ImGui::Separator();
        ImGui::InputTextMultiline("Fragment Shader",buf2,512);
        ImGui::Separator();
        ImGui::Text("User Uniforms");

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
        ImGui::Button("Texture File..."); ImGui::SameLine(); ImGui::Button("Remove File");
    }
}
