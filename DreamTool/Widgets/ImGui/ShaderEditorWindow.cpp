#include "ShaderEditorWindow.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

namespace octronic::dream::tool
{

    ShaderEditorWindow::ShaderEditorWindow
    (DreamToolContext* state)
        : ImGuiWidget (state),
          mShaderDefinition(nullptr)
    {
        mVertexEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
        mFragmentEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
    }

    ShaderEditorWindow::~ShaderEditorWindow
    ()
    {

    }

    void
    ShaderEditorWindow::draw
    ()
    {
        if (mContext->getProject() && mShaderDefinition)
        {

            ImGui::Begin("Shader Editor",&mVisible);
            auto projRunt = mContext->getProject()->getRuntime();
            ShaderRuntime* shaderInst = nullptr;
            {
                auto scriptCache = projRunt->getShaderCache();
                if (scriptCache)
                {
                    shaderInst = dynamic_cast<ShaderRuntime*>(scriptCache->getRuntime(mShaderDefinition));
                }
            }

            if(ImGui::Button("Save"))
            {
                if(shaderInst)
                {
                    bool vertSuccess = false;
                    bool fragSuccess = false;
                    auto currentVertexText = mVertexEditor.GetText();
                    shaderInst->setVertexSource(currentVertexText);
                    string vSource = shaderInst->getVertexSource();
                    vertSuccess = mContext->getProjectDirectory()->writeAssetData(mShaderDefinition,(uint8_t*)vSource.c_str(), vSource.size(),Constants::SHADER_VERTEX_FILE_NAME);

                    auto currentFragmentText = mFragmentEditor.GetText();
                    shaderInst->setFragmentSource(currentFragmentText);
                    string fSource = shaderInst->getFragmentSource();
                    fragSuccess = mContext->getProjectDirectory()->writeAssetData(mShaderDefinition,(uint8_t*)fSource.c_str(),fSource.size(),Constants::SHADER_FRAGMENT_FILE_NAME);
                    if (vertSuccess && fragSuccess)
                    {
                        mContext->getMenuBar()->setMessageString("Saved Shader "+shaderInst->getNameAndUuidString());
                    }
                    else
                    {
                        mContext->getMenuBar()->setMessageString("Error saving Shader"+shaderInst->getNameAndUuidString());
                    }

                }
            }
            ImGui::SameLine();

            // Templates

            vector<string> templates = mContext->getTemplatesModel()->getTemplateNames(AssetType::ASSET_TYPE_ENUM_SHADER);
            static int currentTemplateIndex = -1;
            if(StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
            {
                ImGui::OpenPopup("Load From Template?");
            }
            // Editors
            ImGui::Columns(2);

            ImGui::PushFont(DreamToolWindow::MonoFont);

            auto cposVert = mVertexEditor.GetCursorPosition();
            ImGui::Text(
                        "%6d/%-6d %6d lines  | %s | %s | %s",
                        cposVert.mLine + 1,
                        cposVert.mColumn + 1,
                        mVertexEditor.GetTotalLines(),
                        mVertexEditor.IsOverwrite() ? "Ovr" : "Ins",
                        mVertexEditor.CanUndo() ? "*" : " ",
                        mVertexEditor.GetLanguageDefinition().mName.c_str()
                        );
            mVertexEditor.Render("Vertex Editor");

            ImGui::NextColumn();

            auto cposFrag = mFragmentEditor.GetCursorPosition();
            ImGui::Text(
                        "%6d/%-6d %6d lines  | %s | %s | %s",
                        cposFrag.mLine + 1,
                        cposFrag.mColumn + 1,
                        mFragmentEditor.GetTotalLines(),
                        mFragmentEditor.IsOverwrite() ? "Ovr" : "Ins",
                        mFragmentEditor.CanUndo() ? "*" : " ",
                        mFragmentEditor.GetLanguageDefinition().mName.c_str()
                        );
            mFragmentEditor.Render("Fragment Editor");

            ImGui::PopFont();
            ImGui::Columns(1);

            if(ImGui::BeginPopupModal("Load From Template?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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
                        LOG_ERROR("ShaderEditorWindow: Cannot load template {}",currentTemplateIndex);
                    }
                    else
                    {
                        auto  templateName = templates.at(currentTemplateIndex);
                        string vertTemplate = mContext->getTemplatesModel()->getTemplate(AssetType::ASSET_TYPE_ENUM_SHADER,templateName,Constants::SHADER_VERTEX_FILE_NAME);
                        string fragTemplate = mContext->getTemplatesModel()->getTemplate(AssetType::ASSET_TYPE_ENUM_SHADER,templateName,Constants::SHADER_FRAGMENT_FILE_NAME);
                        if (shaderInst)
                        {
                            mVertexEditor.SetText(vertTemplate);
                            mFragmentEditor.SetText(fragTemplate);
                        }
                        else
                        {
                            LOG_ERROR("ShaderEditorWindow: Cannot set template, shader is null");
                        }
                        currentTemplateIndex = -1;
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
            ImGui::End();
        }
    }

    void
    ShaderEditorWindow::setShaderDefinition
    (ShaderDefinition* scriptDefinition)
    {
        mShaderDefinition = scriptDefinition;
        auto projRunt = mContext->getProject()->getRuntime();
        ShaderRuntime* scriptInst = nullptr;
        if (projRunt)
        {
            auto scriptCache = projRunt->getShaderCache();
            if (scriptCache)
            {
                scriptInst = dynamic_cast<ShaderRuntime*>(scriptCache->getRuntime(mShaderDefinition));
                if (scriptInst)
                {
                    mVertexEditor.SetReadOnly(false);
                    mVertexEditor.SetText(scriptInst->getVertexSource());

                    mFragmentEditor.SetReadOnly(false);
                    mFragmentEditor.SetText(scriptInst->getFragmentSource());
                }
            }
        }
    }
}
