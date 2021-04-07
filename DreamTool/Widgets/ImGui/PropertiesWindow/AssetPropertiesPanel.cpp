#include "AssetPropertiesPanel.h"

#include "PropertiesWindow.h"
#include "DreamToolContext.h"

#include <nfd.h>

using std::exception;

namespace octronic::dream::tool
{
  AssetPropertiesPanel::AssetPropertiesPanel
  (ImGuiWidget& parent)
    : ImGuiPanel(parent),
      mNameAndUuidPanel(parent)
  {

  }

  void
  AssetPropertiesPanel::draw
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    auto& pathViewer = ctx.getPathViewer();
    pathViewer.setVisible(false);

    auto target = parent.getCurrentPropertyTarget();
    auto assetDefOpt = target.getDefinition();

    if (!assetDefOpt.has_value())
    {
      return;
    }

    auto& assetDef = static_cast<AssetDefinition&>(assetDefOpt.value().get());

    ImGui::SameLine();
    if(ImGui::Button("Delete Asset"))
    {
      if (ctx.getProjectContext().has_value())
      {
        auto& pCtx = ctx.getProjectContext().value();
        if (pCtx.getProjectDirectory().has_value())
        {
          auto& pDir = pCtx.getProjectDirectory().value();
          pDir.removeAssetDirectory(assetDef);
          if (pCtx.getProjectDefinition().has_value())
          {
            auto& pDef = pCtx.getProjectDefinition().value();
            pDef.removeAssetDefinition(assetDef);
            parent.removeFromHistory(assetDef);
            return;
          }
        }
      }
    }

    ImGui::SameLine();

    if (ImGui::Button("Duplicate"))
    {
      if (ctx.getProjectContext().has_value())
      {
        auto& pCtx = ctx.getProjectContext().value();
        if (pCtx.getProjectDirectory().has_value())
        {
          auto& pDir = pCtx.getProjectDirectory().value();
          pDir.removeAssetDirectory(assetDef);
          if (pCtx.getProjectDefinition().has_value())
          {
            auto& pDef = pCtx.getProjectDefinition().value();
            auto& newDef = pDef.createAssetDefinition(assetDef.getAssetType());
            assetDef.duplicateInto(newDef);
            parent.pushPropertyTarget({PropertyType_Asset,newDef});
            return;
          }
        }
      }
    }

    mNameAndUuidPanel.draw();

    ImGui::Separator();

    char groupStr[128] = {0};
    strncpy(&groupStr[0], assetDef.getGroup().c_str(), assetDef.getGroup().size());

    if (ImGui::InputText("Group", &groupStr[0],128))
    {
      assetDef.setGroup(groupStr);
    }

    ImGui::Separator();

    AssetType type = AssetTypeHelper::GetAssetTypeEnumFromString(assetDef.getType());

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
      case AssetType::ASSET_TYPE_ENUM_PHYSICS:
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

    if (ctx.getProjectContext().has_value())
    {
      auto& pCtx = ctx.getProjectContext().value();
      if (pCtx.getProjectRuntime().has_value())
      {
        auto& pRunt = pCtx.getProjectRuntime().value();
        if (pRunt.getActiveSceneRuntime().has_value())
        {
          if (pRunt.getActiveSceneRuntime().has_value())
          {
            auto& activeScene = pRunt.getActiveSceneRuntime().value().get();
            ImGui::Separator();
            ImGui::Columns(1);
            if(ImGui::CollapsingHeader("Active Runtimes"))
            {
              auto runtimes = activeScene.getEntitiesWithRuntimeOf(assetDef);
              for (auto& rtWrap : runtimes)
              {
                auto& runtime = rtWrap.get();
                ImGui::Text("%s",runtime.getNameAndUuidString().c_str());
              }
            }
          }
        }
      }
    }
  }

  void
  AssetPropertiesPanel::drawAnimationAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    //auto& ctx = parent.getContext();
    auto target = parent.getCurrentPropertyTarget();

    if (target.getDefinition().has_value())
    {
      auto& animDef = static_cast<AnimationDefinition&>(target.getDefinition().value().get());

      ImGui::Columns(2);

      bool relative = animDef.getRelative();
      if (ImGui::Checkbox("Relative to Origin",&relative))
      {
        animDef.setRelative(relative);
      }

      ImGui::NextColumn();

      if (ImGui::Button("Add Keyframe"))
      {
        animDef.addKeyframe(AnimationKeyframe(animDef.nextKeyframeID()));
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

      for (AnimationKeyframe& kf : animDef.getKeyframes())
      {
        ImGui::PushID(kf.getID());

        // Remove
        if (ImGui::Button("-##remove"))
        {
          animDef.removeKeyframe(kf);
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
          animDef.updateKeyframe(kf);
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        // Tx
        float kf_value = kf.getValue();

        ImGui::PushItemWidth(-1);

        if (ImGui::InputFloat("##translation",&kf_value))
        {
          animDef.updateKeyframe(kf);
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
        if (ImGuiWidget::StringCombo("##EasingType", &currentEasingType, easingTypes,easingTypes.size()))
        {
          kf.setEasingType(static_cast<AnimationEasing::EasingType>(currentEasingType));
          animDef.updateKeyframe(kf);
        }

        ImGui::NextColumn();
        ImGui::Separator();
        ImGui::PopID();
        if (selected == kf.getID())
        {
          //drawAnimationKeyframeImGuizmo(animDef, kf);
        }
      }
      ImGui::Columns(1);
    }
  }

  void
  AssetPropertiesPanel::drawAudioAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    auto target = parent.getCurrentPropertyTarget();

    if (target.getDefinition().has_value())
    {
      auto& audioDef = static_cast<AudioDefinition&>(target.getDefinition().value().get());
      if (ctx.getProjectContext().has_value())
      {
        optional<reference_wrapper<AudioRuntime>> audioRunt;

        auto& pCtx = ctx.getProjectContext().value();
        if (pCtx.getProjectRuntime().has_value())
        {
          auto& pRunt = pCtx.getProjectRuntime().value();
          auto& audioComp = pRunt.getAudioComponent();
          audioRunt = audioComp.getAudioRuntime(audioDef);
        }

        if (ImGui::Button("Audio File..."))
        {
          nfdchar_t *audioFilePath = NULL;
          nfdfilteritem_t filter[2] = {{"Ogg Files", "ogg"}, {"WAVE Files", "wav"}};
          nfdresult_t result = NFD_OpenDialog(&audioFilePath, filter, 2, ctx.getLastDirectory().c_str() );

          if ( result == NFD_OKAY )
          {
            LOG_INFO("AssetPropertiesPanel: Success! {}",audioFilePath);

            LOG_DEBUG("AssetPropertiesPanel: Opening Audio File {}",audioFilePath);
            auto& fm = ctx.getStorageManager();
            auto& audioFile = fm.openFile(audioFilePath);
            ctx.setLastDirectory(audioFile.getDirectory());

            LOG_DEBUG("AssetPropertiesPanel: Setting last directory {}",ctx.getLastDirectory());
            audioFile.readBinary();
            auto audioData = audioFile.getBinaryData();
            if (pCtx.getProjectDirectory().has_value())
            {
              auto& pDir = pCtx.getProjectDirectory().value();
              pDir.writeAssetData(audioDef,audioData, audioFile.getExtension());
              audioDef.setFormat(audioFile.getExtension());
              audioDef.setName(audioFile.getNameWithoutExtension());
              NFD_FreePath(audioFilePath);
              fm.closeFile(audioFile);
            }
          }
          else if ( result == NFD_CANCEL )
          {
            LOG_DEBUG("AssetPropertiesPanel: User pressed cancel.");
          }
          else
          {
            LOG_ERROR("AssetPropertiesPanel: Error: %s\n", NFD_GetError() );
          }
        }

        ImGui::Text("Format: %s", audioDef.getFormat().c_str());

        if(ImGui::Button("Remove File"))
        {
          audioDef.setFormat("");
          if (audioRunt)
          {
            auto& pRunt = audioRunt.value().get().getProjectRuntime();
            auto& audioCache = pRunt.getAudioCache();
            audioCache.removeRuntime(audioDef);
            audioRunt.reset();
          }
          if (ctx.getProjectContext().has_value())
          {
            auto& pCtx = ctx.getProjectContext().value();
            if (pCtx.getProjectDirectory().has_value())
            {
              auto& pDir = pCtx.getProjectDirectory().value();
              pDir.removeAssetDirectory(audioDef);
            }
          }
        }

        if (audioRunt.has_value())
        {
          auto& audioRuntVal = audioRunt.value().get();
          if (audioRuntVal.getLoaded())
          {
            ImGui::Separator();

            if (ImGui::Button("Play"))
            {
              audioRuntVal.play();
            }

            ImGui::SameLine();

            if (ImGui::Button("Pause"))
            {
              audioRuntVal.pause();
            }

            ImGui::SameLine();

            if (ImGui::Button("Stop"))
            {
              audioRuntVal.stop();
            }

            ImGui::PushItemWidth(-1);
            int duration = audioRuntVal.getDurationInSamples();
            if (duration > 0)
            {
              int audioProg = audioRuntVal.getSampleOffset();
              if(ImGui::SliderInt("#AudioProgress", &audioProg,0,duration,"%d"))
              {
                audioRuntVal.setSampleOffset(audioProg);
              }
            }
            ImGui::PopItemWidth();
          }
        }
      }
    }
  }

  void
  AssetPropertiesPanel::drawFontAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();

    auto target = parent.getCurrentPropertyTarget();

    if (target.getDefinition())
    {

      auto& def = static_cast<FontDefinition&>(target.getDefinition().value().get());

      bool selectFile = ImGui::Button("Font File...");

      if (selectFile)
      {
        nfdchar_t *filePath = NULL;
        nfdfilteritem_t filters[2] = {{"TrueType", "ttf"}, {"OpenType", "otf"}};
        nfdresult_t result = NFD_OpenDialog(&filePath, filters, 2, ctx.getLastDirectory().c_str());

        if ( result == NFD_OKAY )
        {
          LOG_INFO("AssetPropertiesPanel: Success! {}",filePath);
          LOG_DEBUG("AssetPropertiesPanel: Opening Font File {}",filePath);
          auto& fm = ctx.getStorageManager();
          auto& file = fm.openFile(filePath);
          ctx.setLastDirectory(file.getDirectory());
          file.readBinary();
          auto data = file.getBinaryData();
          if (ctx.getProjectContext().has_value())
          {
            auto& pCtx = ctx.getProjectContext().value();
            if (pCtx.getProjectDirectory())
            {
              auto& pDir = pCtx.getProjectDirectory().value();
              pDir.writeAssetData(def,data);
              def.setName(file.getNameWithoutExtension());
              fm.closeFile(file);
              NFD_FreePath(filePath);
            }
          }
        }
        else if ( result == NFD_CANCEL )
        {
          LOG_DEBUG("AssetPropertiesPanel: User pressed cancel.");
        }
        else
        {
          LOG_ERROR("AssetPropertiesPanel: Error: %s\n", NFD_GetError() );
        }

      }

      ImGui::SameLine();
      if (ctx.getProjectContext().has_value())
      {
        auto& pCtx = ctx.getProjectContext().value();
        if(ImGui::Button("Remove File"))
        {

          if (pCtx.getProjectDirectory().has_value())
          {
            auto& pDir = pCtx.getProjectDirectory().value();
            pDir.removeAssetDirectory(def);
          }
        }

        float fontSize = def.getSize();
        if (ImGui::SliderFloat("Size",&fontSize,1.f,100.f))
        {
          def.setSize(fontSize);
        }

        // Display the FontAtlas Texture
        if (pCtx.getProjectRuntime().has_value())
        {
          auto& pRunt = pCtx.getProjectRuntime().value();
          auto& fontCache = pRunt.getFontCache();
          auto& fontRuntime = fontCache.getRuntime(def);
          GLuint atlasTexture = fontRuntime.getAtlasTexture();
          unsigned int atlasWidth = fontRuntime.getAtlasWidth();
          unsigned int atlasHeight = fontRuntime.getAtlasHeight();
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


  void
  AssetPropertiesPanel::drawMaterialAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    if (ctx.getProjectContext().has_value())
    {
      auto& pCtx = ctx.getProjectContext().value();
      if (pCtx.getProjectDefinition().has_value())
      {
        auto& pDef = pCtx.getProjectDefinition().value();
        auto target = parent.getCurrentPropertyTarget();
        auto& materialDef = static_cast<MaterialDefinition&>(target.getDefinition().value().get());
        if (pCtx.getProjectRuntime().has_value())
        {
          auto& pRunt = pCtx.getProjectRuntime().value();

          int shaderIndex = 0;
          vector<string> shaderList;
          auto shaderAssetDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_MATERIAL,materialDef.getShaderUuid());
          if (shaderAssetDefOpt.has_value())
          {
            auto& shaderAsset = shaderAssetDefOpt.value().get();
            shaderIndex = pDef.getAssetDefinitionIndex(shaderAsset);
            shaderList = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);

            if(ImGuiWidget::StringCombo("Shader", &shaderIndex, shaderList, shaderList.size()))
            {
              auto newShaderOpt = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER,shaderIndex);
              if (newShaderOpt.has_value())
              {
                materialDef.setShaderUuid(newShaderOpt.value().get().getUuid());
              }
            }
          }

          ImGui::Separator();

          auto& txCache = pRunt.getTextureCache();

          UuidType albedoUuid = materialDef.getAlbedoTextureUuid();
          UuidType normalUuid = materialDef.getNormalTextureUuid();
          UuidType metallicUuid = materialDef.getMetallicTextureUuid();
          UuidType roughnessUuid = materialDef.getRoughnessTextureUuid();
          UuidType aoUuid = materialDef.getAoTextureUuid();

          void* albedoTxId = nullptr;
          void* normalTxId = nullptr;
          void* metallicTxId = nullptr;
          void* roughnessTxId = nullptr;
          void* aoTxId = nullptr;

          // Albedo
          auto albedoDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_TEXTURE,albedoUuid);
          if (albedoDefOpt.has_value())
          {
            auto& txDef = static_cast<TextureDefinition&>(albedoDefOpt.value().get());
            auto& txRuntime = txCache.getRuntime(txDef);
            albedoTxId = (void*)(intptr_t)txRuntime.getTextureID();
          }

          // Normal
          auto normalDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_TEXTURE,normalUuid);
          if (normalDefOpt.has_value())
          {
            auto& txDef = static_cast<TextureDefinition&>(normalDefOpt.value().get());
            try
            {
              auto& txRuntime = txCache.getRuntime(txDef);
              normalTxId = (void*)(intptr_t)txRuntime.getTextureID();
            }
            catch (exception ex)  {}
          }

          // Metallic
          auto metallicDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_TEXTURE,metallicUuid);
          if (metallicDef.has_value())
          {
            auto& txDef = static_cast<TextureDefinition&>(metallicDef.value().get());
            try
            {
              auto& txRuntime = txCache.getRuntime(txDef);
              metallicTxId = (void*)(intptr_t)txRuntime.getTextureID();
            }
            catch (exception ex) {}
          }

          // Roughness
          auto roughnessDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_TEXTURE,roughnessUuid);
          if (roughnessDef)
          {
            auto& txDef = static_cast<TextureDefinition&>(roughnessDef.value().get());
            try
            {
              auto& txRuntime = txCache.getRuntime(txDef);
              roughnessTxId = (void*)(intptr_t)txRuntime.getTextureID();
            }
            catch (exception ex) {}
          }

          // AO
          auto aoDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_TEXTURE,aoUuid);
          if (aoDef.has_value())
          {
            auto& txDef = static_cast<TextureDefinition&>(aoDef.value().get());
            try
            {
              auto& txRuntime = txCache.getRuntime(txDef);
              aoTxId = (void*)(intptr_t)txRuntime.getTextureID();
            }
            catch (exception ex) {}
          }

          vector<string> textures;
          int albedoIndex, normalIndex, metallicIndex, roughnessIndex, aoIndex;

          textures = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_TEXTURE);

          albedoIndex = pDef.getAssetDefinitionIndex(
                pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE,albedoUuid).value());

          normalIndex = pDef.getAssetDefinitionIndex(
                pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE,normalUuid).value());

          metallicIndex = pDef.getAssetDefinitionIndex(
                pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE,metallicUuid).value());

          roughnessIndex = pDef.getAssetDefinitionIndex(
                pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE,roughnessUuid).value());

          aoIndex = pDef.getAssetDefinitionIndex(
                pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE,aoUuid).value());

          // Albedo ==============================================================

          if(ImGuiWidget::StringCombo("Albedo",&albedoIndex,textures,textures.size()))
          {
            auto txDef = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, albedoIndex);

            if (txDef)
            {
              UuidType uuid = txDef.value().get().getUuid();
              materialDef.setAlbedoTextureUuid(uuid);
            }
          }

          ImGui::Image(albedoTxId, parent.getImageSize());
          ImGui::Separator();

          // Normal ==============================================================

          if(ImGuiWidget::StringCombo("Normal",&normalIndex,textures,textures.size()))
          {
            auto txDef = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, normalIndex);
            if (txDef)
            {
              UuidType uuid = txDef.value().get().getUuid();
              materialDef.setNormalTextureUuid(uuid);
            }
          }
          ImGui::Image(normalTxId, parent.getImageSize());
          ImGui::Separator();

          // Metallic ============================================================

          if(ImGuiWidget::StringCombo("Metallic",&metallicIndex,textures,textures.size()))
          {
            auto txDef = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, metallicIndex);
            if (txDef)
            {
              UuidType uuid = txDef.value().get().getUuid();
              materialDef.setMetallicTextureUuid(uuid);
            }
          }

          ImGui::Image(metallicTxId, parent.getImageSize());
          ImGui::Separator();

          // Roughness ===========================================================

          if(ImGuiWidget::StringCombo("Roughness",&roughnessIndex,textures,textures.size()))
          {
            auto txDef = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, roughnessIndex);
            if (txDef)
            {
              UuidType uuid = txDef.value().get().getUuid();
              materialDef.setRoughnessTextureUuid(uuid);
            }
          }
          ImGui::Image(roughnessTxId, parent.getImageSize());

          ImGui::Separator();

          // Ao ==================================================================

          if(ImGuiWidget::StringCombo("AO",&aoIndex,textures,textures.size()))
          {
            auto txDef = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, aoIndex);
            if (txDef)
            {
              UuidType uuid = txDef.value().get().getUuid();
              materialDef.setAoTextureUuid(uuid);
            }
          }
          ImGui::Image(aoTxId, parent.getImageSize());

          ImGui::Columns(1);
        }
      }
    }
  }

  void
  AssetPropertiesPanel::drawModelAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    auto& pCtx = ctx.getProjectContext().value();
    auto& pDir = pCtx.getProjectDirectory().value();
    auto target = parent.getCurrentPropertyTarget();
    auto& def = static_cast<ModelDefinition&>(target.getDefinition().value().get());

    bool selectFile = ImGui::Button("Model File...");

    if (selectFile)
    {
      nfdchar_t *filePath = NULL;
      nfdresult_t result = NFD_OpenDialog(&filePath, nullptr, 0, ctx.getLastDirectory().c_str());

      if ( result == NFD_OKAY )
      {
        LOG_ERROR("AssetPropertiesPanel: Opening Model File {}",filePath);
        auto& fm = ctx.getStorageManager();
        auto& file = fm.openFile(filePath);
        ctx.setLastDirectory(file.getDirectory());
        file.readBinary();
        auto data = file.getBinaryData();
        def.setFormat(Constants::ASSET_TYPE_MODEL+"."+file.getExtension());
        def.setName(file.getNameWithoutExtension());
        pDir.writeAssetData(def,data);
        fm.closeFile(file);
        NFD_FreePath(filePath);
      }
      else if ( result == NFD_CANCEL )
      {
        LOG_DEBUG("AssetPropertiesPanel: User pressed cancel.");
      }
      else
      {
        LOG_ERROR("AssetPropertiesPanel: Error: %s\n", NFD_GetError() );
      }
    }

    if(ImGui::Button("Remove File"))
    {
      pDir.removeAssetDirectory(def);
    }

    if (pCtx.getProjectRuntime().has_value())
    {
      auto& pRunt = pCtx.getProjectRuntime().value();

      auto&  modelCache = pRunt.getModelCache();

      ImGui::Text("Model Format: %s", def.getFormat().c_str());

      if (ImGui::Button("Reload Asset"))
      {
        auto& modelRuntime = modelCache.getRuntime(def);
        modelRuntime.setReloadFlag(true);
      }

      auto& modelRuntime = modelCache.getRuntime(def);
      vector<string> modelMaterialNames = modelRuntime.getMaterialNames();

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
      auto& pDef =  pCtx.getProjectDefinition().value();
      materialAssetNames = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MATERIAL);

      for (string modelMaterial : modelMaterialNames)
      {
        ImGui::Text("%s",modelMaterial.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        UuidType currentMaterialUuid = def.getDreamMaterialForModelMaterial(modelMaterial);
        auto& currentMaterialDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_MATERIAL,currentMaterialUuid).value().get();
        int currentMaterialIndex = pDef.getAssetDefinitionIndex(currentMaterialDef);
        string itemName = "##Material:" + modelMaterial;

        if(ImGuiWidget::StringCombo(itemName.c_str(),&currentMaterialIndex,materialAssetNames,materialAssetNames.size()))
        {
          auto& changedMaterial = pDef.getAssetDefinitionAtIndex(ASSET_TYPE_ENUM_MATERIAL,currentMaterialIndex).value().get();
          def.addModelMaterial(modelMaterial,changedMaterial.getUuid());
          LOG_ERROR("AssetPropertiesPanel: Changed {} material {} to map to {}",def.getName(), modelMaterial, changedMaterial.getNameAndUuidString() );
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();
      }
    }
  }

  void
  AssetPropertiesPanel::drawPhysicsObjectAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    auto& pCtx = ctx.getProjectContext().value();

    auto target = parent.getCurrentPropertyTarget();
    auto& pod = static_cast<PhysicsDefinition&>(target.getDefinition().value().get());

    vector<string> poFormats = Constants::DREAM_ASSET_FORMATS_MAP[AssetType::ASSET_TYPE_ENUM_PHYSICS];
    string poFormatString = pod.getFormat();
    int poFormatIndex = ImGuiWidget::GetStringIndexInVector(poFormatString, poFormats);

    if(ImGuiWidget::StringCombo("Format",&poFormatIndex, poFormats,poFormats.size()))
    {
      pod.setFormat(poFormats.at(poFormatIndex));
    }
    ImGui::Separator();

    bool kinematic = pod.getKinematic();
    if (ImGui::Checkbox("Kinematic",&kinematic))
    {
      pod.setKinematic(kinematic);
    }

    bool controllable = pod.getControllableCharacter();
    if (ImGui::Checkbox("Controllable Character",&controllable))
    {
      pod.setControllableCharacter(controllable);
    }

    ImGui::Separator();

    float mass = pod.getMass();
    if(ImGui::InputFloat("Mass",&mass))
    {
      pod.setMass(mass);
    }

    float margin = pod.getMargin();
    if(ImGui::InputFloat("Margin",&margin))
    {
      pod.setMargin(margin);
    }

    float restitution = pod.getRestitution();
    if(ImGui::InputFloat("Restitution",&restitution))
    {
      pod.setRestitution(restitution);
    }

    float friction = pod.getFriction();
    if(ImGui::InputFloat("Friction",&friction))
    {
      pod.setFriction(friction);
    }

    float ccdspr = pod.getCcdSweptSphereRadius();
    if (ImGui::InputFloat("CCD Swept Sphere Radius",&ccdspr))
    {
      pod.setCcdSweptSphereRadius(ccdspr);
    }

    float linearFactor[3] ={
      pod.getLinearFactor().x,
      pod.getLinearFactor().y,
      pod.getLinearFactor().z
    };

    if (ImGui::DragFloat3("Linear Factor", &linearFactor[0],0.1f))
    {
      vec3 lf(linearFactor[0],linearFactor[1],linearFactor[2]);
      pod.setLinearFactor(lf);
    }

    float angularFactor[3] ={
      pod.getAngularFactor().x,
      pod.getAngularFactor().y,
      pod.getAngularFactor().z
    };
    if (ImGui::DragFloat3("Angular Factor", &angularFactor[0],0.1f))
    {
      vec3 af(angularFactor[0], angularFactor[1],angularFactor[2]);
      pod.setAngularFactor(af);
    }

    float linearVelocity[3] ={
      pod.getLinearVelocity().x,
      pod.getLinearVelocity().y,
      pod.getLinearVelocity().z
    };
    if (ImGui::DragFloat3("Linear Velocity", &linearVelocity[0],0.1f))
    {
      vec3 lf(linearVelocity[0],linearVelocity[1],linearVelocity[2]);
      pod.setLinearVelocity(lf);
    }

    float angularVelocity[3] ={
      pod.getAngularVelocity().x,
      pod.getAngularVelocity().y,
      pod.getAngularVelocity().z
    };
    if (ImGui::DragFloat3("Angular Velocity", &angularVelocity[0],0.1f))
    {
      vec3 af(angularVelocity[0], angularVelocity[1],angularVelocity[2]);
      pod.setAngularVelocity(af);
    }


    ImGui::Separator();

    if (pod.getFormat().compare(Constants::COLLISION_SHAPE_BOX) == 0)
    {
      float halfExtents[3] = {
        pod.getHalfExtents().x,
        pod.getHalfExtents().y,
        pod.getHalfExtents().z
      };

      if(ImGui::InputFloat3("Half-Extents",&halfExtents[0]))
      {
        pod.setHalfExtents(vec3(halfExtents[0], halfExtents[1], halfExtents[2]));
      }
    }
    else if (pod.getFormat().compare(Constants::COLLISION_SHAPE_SPHERE) == 0)
    {
      float radius = pod.getRadius();
      if (ImGui::InputFloat("Radius",&radius))
      {
        pod.setRadius(radius);
      }
    }
    else if (pod.getFormat().compare(Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH) == 0)
    {
      if (pCtx.getProjectDefinition().has_value())
      {
        auto& pDef = pCtx.getProjectDefinition().value();

        UuidType selectedModelAssetUuid = pod.getCollisionModelUuid();
        auto& selectedModelAssetDef =  pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_MODEL,selectedModelAssetUuid).value().get();
        int selectedModelAssetIndex = pDef.getAssetDefinitionIndex(selectedModelAssetDef);
        vector<string> modelAssets =  pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MODEL);

        if(ImGuiWidget::StringCombo("Model",&selectedModelAssetIndex,modelAssets,modelAssets.size()))
        {
          auto newlySelected = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAssetIndex);
          pod.setCollisionModelUuid(newlySelected.value().get().getUuid());
        }
      }
    }
    else if (pod.getFormat().compare(Constants::COLLISION_SHAPE_STATIC_PLANE) == 0)
    {
      float normal[3] = {
        pod.getNormal().x,
        pod.getNormal().y,
        pod.getNormal().z
      };

      if (ImGui::InputFloat3("Plane Normal",&normal[0]))
      {
        pod.setNormal(vec3(normal[0],normal[1],normal[2]));
      }
    }
    else if (pod.getFormat().compare(Constants::COLLISION_SHAPE_COMPOUND) == 0)
    {
      ImGui::Columns(2);
      auto& pDefOpt = pCtx.getProjectDefinition();

      if (pDefOpt)
      {
        auto& pDef = pCtx.getProjectDefinition().value();

        auto shapeNames = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PHYSICS);
        static int shapeNameIndex = -1;
        ImGuiWidget::StringCombo("Shape",&shapeNameIndex,shapeNames,shapeNames.size());

        ImGui::NextColumn();
        if(ImGui::Button("Add Compound Child"))
        {
          if (shapeNameIndex >= 0)
          {
            auto childDef = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS,shapeNameIndex);
            pod.addCompoundChild({pod,Transform(),childDef.value().get().getUuid()});
          }
        }
        ImGui::Separator();
        ImGui::Columns(1);
        ImGui::Text("Child Shapes");
        ImGui::Separator();

        auto shapes = pod.getCompoundChildren();
        static UuidType selectedToTransform = 0;
        for (auto& shape : shapes)
        {
          auto shapeDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_PHYSICS,shape.uuid);

          if (!shapeDef)
          {
            continue;
          }
          ImGui::PushID(shape.uuid);
          ImGui::SetNextTreeNodeOpen(selectedToTransform == shape.uuid);
          if (ImGui::CollapsingHeader(shapeDef.value().get().getName().c_str()))
          {
            selectedToTransform = shape.uuid;
            //drawPhysicsImGizmo(shape);
            if(ImGui::Button("Remove Shape"))
            {
              pod.removeCompoundChild(shape);
            }
          }
          ImGui::PopID();
        }
      }
    }
  }

  void
  AssetPropertiesPanel::drawScriptProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    auto& pCtx = ctx.getProjectContext().value();
    auto target = parent.getCurrentPropertyTarget();
    auto& scriptDef = static_cast<ScriptDefinition&>(target.getDefinition().value().get());
    auto& pRunt = pCtx.getProjectRuntime().value();
    auto& scriptCache = pRunt.getScriptCache();

    auto& scriptInst = scriptCache.getRuntime(scriptDef);

    vector<string> templates = ctx.getTemplatesModel().getTemplateNames(AssetType::ASSET_TYPE_ENUM_SCRIPT);

    static int currentTemplateIndex = -1;

    if (ImGuiWidget::StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
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
          LOG_ERROR("AssetPropertiesPanel: Cannot load script template {}",currentTemplateIndex);
        }
        else
        {
          auto  templateName = templates.at(currentTemplateIndex);
          string scriptTemplate = ctx.getTemplatesModel().getTemplate(AssetType::ASSET_TYPE_ENUM_SCRIPT,templateName,Constants::ASSET_FORMAT_SCRIPT_LUA);
          bool scriptSuccess = false;
          auto& pDir = pCtx.getProjectDirectory().value();
          scriptSuccess = pDir.writeAssetStringData(scriptDef,
                                                    scriptTemplate, Constants::ASSET_FORMAT_SCRIPT_LUA);

          if (scriptSuccess)
          {
            ctx.getMenuBar().setMessageString(
                  "Saved Script"+scriptDef.getNameAndUuidString());
          }
          else
          {
            ctx.getMenuBar().setMessageString(
                  "Error saving Script"+scriptDef.getNameAndUuidString());
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
      ctx.openInExternalEditor(scriptDef);
    }

    if(ImGui::Button("Reload Asset"))
    {
      scriptInst.setReloadFlag(true);
    }

    ImGui::Separator();

    ImGui::Columns(2);

    ImGui::Text("Source File");

    ImGui::NextColumn();

    if(scriptInst.hasSource())
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

    if(scriptInst.getLoaded())
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

    if(scriptInst.getLoadError())
    {
      ImGui::TextColored({1.f,0.f,0.f,1.f}, "Error");
    }
    else
    {
      ImGui::TextColored({0.f,1.f,0.f,1.f}, "No Error");
    }

    ImGui::Columns(1);
  }

  void
  AssetPropertiesPanel::drawShaderAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    auto& pCtx = ctx.getProjectContext().value();
    auto& pRunt = pCtx.getProjectRuntime().value();
    auto& pDir = pCtx.getProjectDirectory().value();
    auto target = parent.getCurrentPropertyTarget();
    auto& shaderDef = static_cast<ShaderDefinition&>(target.getDefinition().value().get());
    auto& shaderCache = pRunt.getShaderCache();
    auto& shaderRuntime = shaderCache.getRuntime(shaderDef);

    // Templates

    vector<string> templates = ctx.getTemplatesModel().getTemplateNames(AssetType::ASSET_TYPE_ENUM_SHADER);
    static int currentTemplateIndex = -1;
    if(ImGuiWidget::StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
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
          string vertTemplate = ctx.getTemplatesModel().getTemplate(AssetType::ASSET_TYPE_ENUM_SHADER,templateName,Constants::SHADER_GLSL_VERTEX_FILE_NAME);
          string fragTemplate = ctx.getTemplatesModel().getTemplate(AssetType::ASSET_TYPE_ENUM_SHADER,templateName,Constants::SHADER_GLSL_FRAGMENT_FILE_NAME);

          bool vertSuccess = pDir.writeAssetStringData(shaderDef,vertTemplate, Constants::SHADER_GLSL_VERTEX_FILE_NAME);
          bool fragSuccess = pDir.writeAssetStringData(shaderDef,fragTemplate,Constants::SHADER_GLSL_FRAGMENT_FILE_NAME);

          if (vertSuccess && fragSuccess)
          {
            ctx.getMenuBar().setMessageString("Saved Shader "+shaderRuntime.getNameAndUuidString());
          }
          else
          {
            ctx.getMenuBar().setMessageString("Error saving Shader"+shaderRuntime.getNameAndUuidString());
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
        if (!ctx.getPreferencesModel().hasExternalEditorPath())
        {
          noEditorError = true;
        }
        else if (!ctx.openInExternalEditor(shaderDef, Constants::SHADER_GLSL_VERTEX_FILE_NAME))
        {
          openVertexFailed = true;
        }
      }

      if(ImGui::Button("Open Fragment Shader in Editor..."))
      {
        if (!ctx.getPreferencesModel().hasExternalEditorPath())
        {
          noEditorError = true;
        }
        else if (!ctx.openInExternalEditor(shaderDef, Constants::SHADER_GLSL_FRAGMENT_FILE_NAME))
        {
          openFragmentFailed = true;
        }
      }

      if (ImGui::Button("Reload Asset"))
      {
        shaderRuntime.setReloadFlag(true);
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
      ImGui::Text("External editor is not set.\n\nPlease set one in File.Preferences");
      if (ImGui::Button("OK",ImVec2(0,0)))
      {
        noEditorError = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("Open Vertex Shader Failed"))
    {
      string filePath = pDir.getAssetAbsolutePath(shaderDef,Constants::SHADER_GLSL_VERTEX_FILE_NAME);
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
      string filePath = pDir.getAssetAbsolutePath(shaderDef,Constants::SHADER_GLSL_FRAGMENT_FILE_NAME);
      ImGui::Text("Unable to open Fragment Shader \n\n%s\n\nFile may not exist",filePath.c_str());
      if (ImGui::Button("Cancel",ImVec2(0,0)))
      {
        openFragmentFailed = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    ImGui::Separator();

    ImGui::Text("Shader Status");
    ImGui::Columns(2);

    ImGui::Text("Loaded");
    ImGui::NextColumn();
    if (shaderRuntime.getLoaded())
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
    if (shaderRuntime.getLoadError())
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
    if (shaderRuntime.hasVertexSource())
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
    if (shaderRuntime.hasFragmentSource())
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
    if(shaderRuntime.hasVertexCompilationFailed())
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
    if(shaderRuntime.hasFragmentCompilationFailed())
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
    if(shaderRuntime.hasLinkingFailed())
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
    if(shaderRuntime.hasValidGLID())
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

    if (shaderRuntime.getLoaded())
    {
      auto& uniforms = shaderRuntime.getUniformsVector();

      ImGui::Columns(4);

      ImGui::Text("Location");
      ImGui::NextColumn();
      ImGui::Text("Type");
      ImGui::NextColumn();
      ImGui::Text("Name");
      ImGui::NextColumn();
      ImGui::Text("Status");
      ImGui::NextColumn();

      for (auto& uniform : uniforms)
      {
        ImGui::Separator();
        ImGui::Text("%d",uniform.getLocation());
        ImGui::NextColumn();
        ImGui::Text("%s",uniformTypeToString(uniform.getType()).c_str());
        ImGui::NextColumn();
        ImGui::Text("%s",uniform.getName().c_str());
        ImGui::NextColumn();

        if (shaderRuntime.checkUniformValue(uniform))
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

  void
  AssetPropertiesPanel::drawPathAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    auto target = parent.getCurrentPropertyTarget();
    auto& pathDef = static_cast<PathDefinition&>(target.getDefinition().value().get());

    ctx.getPathViewer().setPathDefinition(pathDef);
    ctx.getPathViewer().setVisible(true);

    bool needsRegen = false;
    float stepScalar = pathDef.getStepScalar();
    if (ImGui::DragFloat("Step Scalar",&stepScalar))
    {
      pathDef.setStepScalar(stepScalar >= 1.0f ? stepScalar : 1.0f);
      needsRegen = true;
    }

    int splineTypeIndex = ImGuiWidget::GetStringIndexInVector(pathDef.getSplineType(),Constants::DREAM_PATH_SPLINE_TYPES);
    if (ImGuiWidget::StringCombo("Spline Type",&splineTypeIndex, Constants::DREAM_PATH_SPLINE_TYPES,Constants::DREAM_PATH_SPLINE_TYPES.size()))
    {
      pathDef.setSplineType(Constants::DREAM_PATH_SPLINE_TYPES.at(splineTypeIndex));
      needsRegen = true;
    }

    bool wrap = pathDef.getWrap();
    if (ImGui::Checkbox("Wrap",&wrap))
    {
      pathDef.setWrap(wrap);
      needsRegen = true;
    }

    float velocity = pathDef.getVelocity();
    if (ImGui::DragFloat("Velocity",&velocity))
    {
      pathDef.setVelocity(velocity);
      needsRegen = true;
    }

    ImGui::Text("Control Points");
    if (ImGui::Button("Add"))
    {
      pathDef.addControlPoint();
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

    for (auto cp : pathDef.getControlPoints())
    {
      bool modified = false;
      ImGui::PushID(cp.id);

      // Remove
      if (ImGui::Button("-##remove"))
      {
        pathDef.deleteControlPoint(cp);
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
        pathDef.updateControlPoint(cp);
        needsRegen = true;
      }
      if (selected == cp.id)
      {
        ctx.getPathViewer().setSelectedControlPoint(cp.id);
        //drawPathControlPointImGuizmo(pathDef,cp);
        needsRegen = true;
      }
    }
    if (needsRegen)
    {
      ctx.getPathViewer().regenerate();
    }
    ImGui::Columns(1);
    if (ImGui::Button("Deselect"))
    {
      selected = -1;
    }
  }

  void
  AssetPropertiesPanel::drawTextureAssetProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();
    auto& pCtxOpt = ctx.getProjectContext();
    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();
      auto target = parent.getCurrentPropertyTarget();
      if (target.getDefinition().has_value())
      {
        auto& textureDef = static_cast<TextureDefinition&>(target.getDefinition().value().get());
        auto& pRuntOpt = pCtx.getProjectRuntime();
        auto& pDefOpt = pCtx.getProjectDefinition();
        auto& pDirOpt = pCtx.getProjectDirectory();

        if (pRuntOpt)
        {
          auto& pRunt = pRuntOpt.value();
          auto& txCache = pRunt.getTextureCache();
          auto& textureRuntime = txCache.getRuntime(textureDef);

          bool selectFile = ImGui::Button("Texture File...");

          if (selectFile)
          {
            if (pDirOpt)
            {
              auto& pDir = pDirOpt.value();

              nfdchar_t *filePath = NULL;
              nfdresult_t result = NFD_OpenDialog(&filePath, nullptr, 0,ctx.getLastDirectory().c_str());

              if ( result == NFD_OKAY )
              {
                LOG_ERROR("AssetPropertiesPanel: Opening Texture File {}",filePath);
                auto& fm = pCtx.getStorageManager();
                auto& file = fm.openFile(filePath);
                ctx.setLastDirectory(file.getDirectory());
                file.readBinary();
                auto data = file.getBinaryData();
                pDir.writeAssetData(textureDef,data);
                textureDef.setName(file.getNameWithoutExtension());
                fm.closeFile(file);
                NFD_FreePath(filePath);
              }
              else if ( result == NFD_CANCEL )
              {
                LOG_DEBUG("AssetPropertiesPanel: User pressed cancel.");
              }
              else
              {
                LOG_ERROR("AssetPropertiesPanel: Error: %s\n", NFD_GetError() );
              }
            }
          }

          if(ImGui::Button("Remove File"))
          {
            if (pDirOpt)
            {
              auto& pDir = pDirOpt.value();
              pDir.removeAssetDirectory(textureDef);
            }
          }

          if (ImGui::Button("Reload Asset"))
          {
            textureRuntime.setReloadFlag(true);
          }

          bool isHdr = textureRuntime.isHDR();
          ImGui::Text("HDR: %s", isHdr ? "Yes" : "No");

          bool isEnvironment = textureDef.getIsEnvironmentTexture();
          if (ImGui::Checkbox("Environment",&isEnvironment))
          {
            textureDef.setIsEnvironmentTexture(isEnvironment);
          }

          bool flipVertical = textureDef.getFlipVertical();
          if (ImGui::Checkbox("Flip Vertical",&flipVertical))
          {
            textureDef.setFlipVertical(flipVertical);
          }

          if (isEnvironment)
          {
            if (pDefOpt)
            {
              auto&  pDef = pDefOpt.value();
              vector<string> shaderList = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SHADER);
              // EquiToCube
              {
                UuidType shaderUuid = textureDef.getEquiToCubeMapShader();
                auto& shaderDef = static_cast<ShaderDefinition&>(pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER,shaderUuid).value().get());
                int shaderIndex = pDef.getAssetDefinitionIndex(shaderDef);

                if (ImGuiWidget::StringCombo("Cube Map Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                  auto& selectedShader = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex).value().get();
                  UuidType uuid = selectedShader.getUuid();
                  string name = selectedShader.getName();
                  textureDef.setEquiToCubeMapShader(uuid);
                  LOG_DEBUG("AssetPropertiesPanel: Switched cube map shader for texture {} to {} {}",textureDef.getNameAndUuidString(), name, uuid);
                }
              }
              // Irradiance
              {
                UuidType shaderUuid = textureDef.getIrradianceMapShader();
                auto& shaderDef = static_cast<ShaderDefinition&>(pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER,shaderUuid).value().get());
                int shaderIndex = pDef.getAssetDefinitionIndex(shaderDef);

                if (ImGuiWidget::StringCombo("Irradiance Map Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                  auto& selectedShader = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex).value().get();
                  UuidType uuid = selectedShader.getUuid();
                  string name = selectedShader.getName();
                  textureDef.setIrradianceMapShader(uuid);
                  LOG_DEBUG("AssetPropertiesPanel: Switched irradiance map shader for texture {} to {} {}",textureDef.getNameAndUuidString(), name, uuid);
                }
              }
              // PreFilter
              {
                UuidType shaderUuid = textureDef.getPreFilterShader();
                auto& shaderDef = static_cast<ShaderDefinition&>(pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER,shaderUuid).value().get());
                int shaderIndex = pDef.getAssetDefinitionIndex(shaderDef);

                if (ImGuiWidget::StringCombo("PreFilter Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                  auto& selectedShader = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex).value().get();
                  UuidType uuid = selectedShader.getUuid();
                  string name = selectedShader.getName();
                  textureDef.setPreFilterShader(uuid);
                  LOG_DEBUG("AssetPropertiesPanel: Switched PreFilter shader for texture {} to {} {}",textureDef.getNameAndUuidString(), name, uuid);
                }
              }
              // BRDF_LUT
              {
                UuidType shaderUuid = textureDef.getBrdfLutShader();
                auto& shaderDef = static_cast<ShaderDefinition&>(pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER,shaderUuid).value().get());
                int shaderIndex = pDef.getAssetDefinitionIndex(shaderDef);

                if (ImGuiWidget::StringCombo("BRDF LUT Shader", &shaderIndex, shaderList, shaderList.size()))
                {
                  auto& selectedShader = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SHADER, shaderIndex).value().get();
                  UuidType uuid = selectedShader.getUuid();
                  string name = selectedShader.getName();
                  textureDef.setBrdfLutShader(uuid);
                  LOG_DEBUG("AssetPropertiesPanel: Switched BRDF LUT shader for texture {} to {} {}",textureDef.getNameAndUuidString(), name, uuid);
                }
              }

              if (textureRuntime.getLoaded())
              {
                ImGui::Columns(2);

                ImGui::Text("Cube Map");
                ImGui::NextColumn();
                if (textureRuntime.getCubeMapTextureID() > 0) ImGui::TextColored({0.,1.f,0.f,1.f},"%d", textureRuntime.getCubeMapTextureID());
                else                                           ImGui::TextColored({1.,0.f,0.f,1.f},"%d", textureRuntime.getCubeMapTextureID());
                ImGui::NextColumn();

                ImGui::Text("Irradiance");
                ImGui::NextColumn();
                if (textureRuntime.getIrradianceTextureID() > 0) ImGui::TextColored({0.,1.f,0.f,1.f},"%d", textureRuntime.getIrradianceTextureID());
                else                                              ImGui::TextColored({1.,0.f,0.f,1.f},"%d", textureRuntime.getIrradianceTextureID());
                ImGui::NextColumn();

                ImGui::Text("PreFilter");
                ImGui::NextColumn();
                if (textureRuntime.getPreFilterTextureID() > 0) ImGui::TextColored({0.,1.f,0.f,1.f},"%d", textureRuntime.getPreFilterTextureID());
                else                                             ImGui::TextColored({1.,0.f,0.f,1.f},"%d", textureRuntime.getPreFilterTextureID());
                ImGui::NextColumn();

                ImGui::Text("BRDF LUT");
                ImGui::NextColumn();
                if (textureRuntime.getBrdfLutTextureID() > 0) ImGui::TextColored({0.,1.f,0.f,1.f},"%d", textureRuntime.getBrdfLutTextureID());
                else                                           ImGui::TextColored({1.,0.f,0.f,1.f},"%d", textureRuntime.getBrdfLutTextureID());

                ImGui::Columns(1);

              }
            }
          }

          if (textureRuntime.getLoaded())
          {
            GLuint t = textureRuntime.getTextureID();
            if (t != 0)
            {
              void* textureId = (void*)(intptr_t)t;
              if (textureId != nullptr)
              {
                ImGui::Separator();
                ImGui::Text("Size: %dx%d",textureRuntime.getWidth(),textureRuntime.getHeight());
                ImGui::Text("Channels: %d",textureRuntime.getChannels());
                ImGui::Image(textureId, parent.getImageSize());
              }

              void* brdfLutTextureId = (void*)(intptr_t)textureRuntime.getBrdfLutTextureID();
              if (brdfLutTextureId != nullptr)
              {
                ImGui::Text("BRDF Look up Table");
                ImGui::Image(brdfLutTextureId,parent.getImageSize());
              }
            }

            static vector<string> cubeDebugTypes =
            {
              "None", "Environment", "Irradiance", "PreFilter"
            };

            int debugMode = (int)textureRuntime.getCubeDebugMode();

            if (ImGuiWidget::StringCombo("Cube Debug", &debugMode, cubeDebugTypes, cubeDebugTypes.size()))
            {
              textureRuntime.setCubeDebugMode((CubeDebugMode)debugMode);
            }
          }
        }
      }
    }
  }
}
