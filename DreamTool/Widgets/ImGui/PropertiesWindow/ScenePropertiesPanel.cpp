#include "ScenePropertiesPanel.h"

#include "PropertiesWindow.h"
#include "DreamToolContext.h"

#include <exception>

using std::exception;

namespace octronic::dream::tool
{

  ScenePropertiesPanel::ScenePropertiesPanel
  (ImGuiWidget& parent)
    : ImGuiPanel(parent),
      mNameAndUuidPanel(parent),
      mCameraTransformPanel(parent)
  {

  }

  void ScenePropertiesPanel::draw()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto currentTarget = parent.getCurrentPropertyTarget();
    auto sceneDefOptional = currentTarget.getDefinition();

    if (sceneDefOptional)
    {
      auto& sceneDef = static_cast<SceneDefinition&>(sceneDefOptional.value().get());
      auto& pDef = sceneDef.getProjectDefinition();

      ImGui::SameLine();

      if (drawDeleteSceneButton())
      {
        return;
      }

      mNameAndUuidPanel.draw();

      ImGui::Separator();

      // Camera


      ImGui::Separator();

      ImGui::Text("Camera");
      auto& cam = sceneDef.getCameraDefinition();
      int cameraTarget = cam.getUseEntity() ? 1 : 0;

      ImGui::RadioButton("Free Camera", &cameraTarget, 0);
      ImGui::RadioButton("Entity Camera", &cameraTarget, 1);

      auto camEntityUuid = cam.getCameraEntityUuid();
      auto camEntityOpt = sceneDef.getSceneEntityByUuid(camEntityUuid);

      if (camEntityOpt)
      {
        string camEntityName = "";
        Transform camEntityTransform;
        auto& camEntity = camEntityOpt.value().get();
        camEntityName = camEntity.getNameAndUuidString();
        camEntityTransform = camEntity.getTransform();

        ImGui::Text("Camera: %s",camEntityName.c_str());
        cam.setUseEntity(cameraTarget == 1 ? true : false);

        mCameraTransformPanel.setTransform(camEntityTransform);
        mCameraTransformPanel.draw();

        float fov;
        fov = glm::degrees(cam.getFieldOfView());

        if (ImGui::DragFloat("FOV", &fov, 0.1f, 1.f,180.f))
        {
          cam.setFieldOfView(glm::radians(fov));
        }

        ImGui::Separator();

        ImGui::Text("Rendering");

        auto& camDef = sceneDef.getCameraDefinition();

        float meshCull = camDef.getMeshCullDistance();
        if (ImGui::DragFloat("Mesh Cull",&meshCull))
        {
          camDef.setMeshCullDistance(meshCull);
        }

        float drawDistance[2] =
        {
          camDef.getMinDrawDistance(),
          camDef.getMaxDrawDistance()
        };

        if (ImGui::DragFloat2("Draw Distance",drawDistance))
        {
          camDef.setMinDrawDistance(drawDistance[0]);
          camDef.setMaxDrawDistance(drawDistance[1]);
        }
      }

      // Clear Color
      vec4 clear = sceneDef.getClearColor();
      if(ImGui::ColorEdit4("Clear Color",glm::value_ptr(clear)))
      {
        sceneDef.setClearColor(clear);
      }

      ImGui::Separator();

      ImGui::Text("Shaders");


      vector<string> shaderList = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);

      // Setup ShadowPassShader
      {
        UuidType spShaderUuid = sceneDef.getShadowPassShader();
        int shadowShaderIndex = -1;

        auto spShaderDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER,spShaderUuid);
        if (spShaderDefOpt)
        {
          auto& spShaderDef = static_cast<ShaderDefinition&>(spShaderDefOpt.value().get());
          shadowShaderIndex = pDef.getAssetDefinitionIndex(spShaderDef);
        }

        if (ImGuiWidget::StringCombo("Shadow Pass Shader", &shadowShaderIndex, shaderList, shaderList.size()))
        {
          auto selectedShaderOpt = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shadowShaderIndex);
          if (selectedShaderOpt)
          {
            auto& selectedShader = selectedShaderOpt.value().get();
            UuidType uuid = selectedShader.getUuid();
            string name = selectedShader.getName();
            sceneDef.setShadowPassShader(uuid);
            LOG_DEBUG("PropertiesWindow: Switched shadow pass shader to {} {}", name, uuid);
          }
        }
      }

      // Setup FontShader
      {
        UuidType fontShaderUuid = sceneDef.getFontShader();
        auto fontShaderDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER,fontShaderUuid);
        int fontShaderIndex = -1;

        if (fontShaderDefOpt)
        {
          auto & fontShaderDef = fontShaderDefOpt.value().get();
          fontShaderIndex = pDef.getAssetDefinitionIndex(fontShaderDef);
        }

        if (ImGuiWidget::StringCombo("Font Shader", &fontShaderIndex, shaderList, shaderList.size()))
        {
          auto& selectedShader = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, fontShaderIndex).value().get();
          UuidType uuid = selectedShader.getUuid();
          string name = selectedShader.getName();
          sceneDef.setFontShader(uuid);
          LOG_DEBUG("PropertiesWindow: Switched Font shader to {} {}", name, uuid);
        }
      }

      // Setup Sprite shader
      {
        UuidType spriteShaderUuid = sceneDef.getSpriteShader();

        auto spriteShaderDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER,spriteShaderUuid);
        int spriteShaderIndex = -1;

        if (spriteShaderDefOpt)
        {
          auto& spriteShaderDef = spriteShaderDefOpt.value().get();
          spriteShaderIndex = pDef.getAssetDefinitionIndex(spriteShaderDef);
        }

        if (ImGuiWidget::StringCombo("Sprite Shader", &spriteShaderIndex, shaderList, shaderList.size()))
        {
          auto& selectedShader = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, spriteShaderIndex).value().get();
          UuidType uuid = selectedShader.getUuid();
          string name = selectedShader.getName();
          sceneDef.setSpriteShader(uuid);
          LOG_DEBUG("PropertiesWindow: Switched Sprite shader to {} {}", name, uuid);
        }
      }

      ImGui::Separator();
      ImGui::Text("Environment");

      vector<string> textureList = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_TEXTURE);
      // Setup Environment Texture
      {
        UuidType envTexUuid = sceneDef.getEnvironmentTexture();
        auto envTexDefOpt = pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE, envTexUuid);
        int envMapTexIndex = -1;

        if (envTexDefOpt)
        {
          auto& envTexDef = envTexDefOpt.value().get();
          envMapTexIndex = pDef.getAssetDefinitionIndex( envTexDef);
        }


        if (ImGuiWidget::StringCombo("Environment Texture", &envMapTexIndex, textureList, textureList.size()))
        {
          auto& selected = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, envMapTexIndex).value().get();
          UuidType uuid = selected.getUuid();
          string name = selected.getName();
          sceneDef.setEnvironmentTexture(uuid);
          LOG_DEBUG("PropertiesWindow: Switched Environment Texture to {} {}", name, uuid);
        }
      }

      // Environment Shader
      {
        UuidType envShaderUuid = sceneDef.getEnvironmentShader();
        auto envShaderDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER,envShaderUuid);
        int envShaderIndex = -1;
        if (envShaderDefOpt)
        {
          auto& envShaderDef = envShaderDefOpt.value().get();
          envShaderIndex = pDef.getAssetDefinitionIndex(envShaderDef);
        }

        if (ImGuiWidget::StringCombo("Environment Shader", &envShaderIndex, shaderList, shaderList.size()))
        {
          auto& selectedShader = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, envShaderIndex).value().get();
          UuidType uuid = selectedShader.getUuid();
          string name = selectedShader.getName();
          sceneDef.setEnvironmentShader(uuid);
          LOG_DEBUG("PropertiesWindow: Switched environment shader to {} {}", name, uuid);
        }
      }


      if (ImGui::CollapsingHeader("Scripting"))
      {
        vector<string> scriptAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SCRIPT);

        UuidType inputScriptUuid = sceneDef.getInputScript();
        auto inputScriptDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SCRIPT, inputScriptUuid);
        int inputScriptIndex = -1;

        if (inputScriptDefOpt)
        {
          auto& inputScriptDef = inputScriptDefOpt.value().get();
          inputScriptIndex = pDef.getAssetDefinitionIndex(inputScriptDef);
        }

        if (ImGuiWidget::StringCombo("Input Script",&inputScriptIndex,scriptAssets,scriptAssets.size()))
        {
          auto& selected = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,inputScriptIndex).value().get();
          UuidType uuid = selected.getUuid();
          sceneDef.setInputScript(uuid);
        }
      }

      // Physics
      if(ImGui::CollapsingHeader("Physics"))
      {

        float gravityVec[3] =
        {
          sceneDef.getGravity().x,
          sceneDef.getGravity().y,
          sceneDef.getGravity().z
        };

        if (ImGui::DragFloat3("Gravity", &gravityVec[0],0.1f))
        {
          vec3 grav(gravityVec[0],gravityVec[1],gravityVec[2]);
          sceneDef.setGravity(grav);
        }
      }
    }
  }

  bool
  ScenePropertiesPanel::drawDeleteSceneButton
  ()
  {
    bool retval = false;
    auto& parent = static_cast<PropertiesWindow&>(getParent());

    if(ImGui::Button("Delete"))
    {
      ImGui::OpenPopup("Confirm Delete Scene");
    }
    auto& dtCtx = parent.getContext();
    auto& pCtxOpt = dtCtx.getProjectContext();
    if(pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();
      auto target = parent.getCurrentPropertyTarget();
      auto sDefOpt = target.getDefinition();
      auto& pDefOpt = pCtx.getProjectDefinition();

      if (pDefOpt && sDefOpt)
      {
        auto& pDef = pDefOpt.value();
        auto& sDef = static_cast<SceneDefinition&>(sDefOpt.value().get());

        if(ImGui::BeginPopupModal("Confirm Delete Scene"))
        {
          ImGui::Text("\nAre you sure you want to delete this Scene?\n\n");
          ImGui::Separator();

          if (ImGui::Button("Cancel",ImVec2(0,0)))
          {
            ImGui::CloseCurrentPopup();
          }

          ImGui::SameLine();

          if (ImGui::Button("Delete",ImVec2(0,0)))
          {
            // TODO Check is not active
            pDef.removeSceneDefinition(sDef);
            parent.removeFromHistory(sDef);
            retval = true;
            ImGui::CloseCurrentPopup();
          }
          ImGui::EndPopup();
        }
      }
    }
    return retval;
  }
}
