#include "ScriptEditorWindow.h"
#include "DreamToolContext.h"

namespace octronic::dream::tool
{

    ScriptEditorWindow::ScriptEditorWindow
    (DreamToolContext* state)
        : ImGuiWidget (state),
          mScriptDefinition(nullptr)
    {

        mTextEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    }

    ScriptEditorWindow::~ScriptEditorWindow
    ()
    {

    }

    void
    ScriptEditorWindow::draw
    ()
    {
        if (mContext->getProject() && mScriptDefinition)
        {

            ImGui::Begin("Script Editor",&mVisible);

            auto projRunt = mContext->getProject()->getRuntime();
            ScriptRuntime* scriptInst = nullptr;

			auto scriptCache = projRunt->getScriptCache();
			if (scriptCache)
			{
				scriptInst = dynamic_cast<ScriptRuntime*>(scriptCache->getRuntime(mScriptDefinition));
			}

            if(ImGui::Button("Save"))
            {
                if(scriptInst)
                {
                    auto currentText = mTextEditor.GetText();
                    scriptInst->setSource(currentText);
                    string source = scriptInst->getSource();

                    if (mContext->getProjectDirectory()->writeAssetData(mScriptDefinition,(uint8_t*)source.c_str(),source.size(),Constants::ASSET_FORMAT_SCRIPT_LUA))
                    {

                        mContext->getMenuBar()->setMessageString("Saved Script "+scriptInst->getNameAndUuidString());
                    }
                }
            }
            ImGui::SameLine();

            vector<string> templates = mContext->getTemplatesModel()->getTemplateNames(AssetType::ASSET_TYPE_ENUM_SCRIPT);
            static int currentTemplateIndex = -1;
            if (StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
            {
                ImGui::OpenPopup("Load From Template?");
            }

            ImGui::PushFont(DreamToolWindow::MonoFont);

            TextEditor::Coordinates cpos = mTextEditor.GetCursorPosition();
            ImGui::Text(
                        "Row:%6d/%6d | Col:%-6d  | %s | %s | %s | %s",
                        cpos.mLine + 1,
                        mTextEditor.GetTotalLines(),
                        cpos.mColumn + 1,
                        mTextEditor.IsOverwrite() ? "Ovr" : "Ins",
                        mTextEditor.CanUndo() ? "*" : " ",
                        mTextEditor.GetLanguageDefinition().mName.c_str(),
                        mScriptDefinition->getNameAndUuidString().c_str());
            mTextEditor.Render("Text Editor");

            ImGui::PopFont();

            if(ImGui::BeginPopupModal("Load From Template?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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
                        LOG_ERROR("ScriptEditorWindow: Cannot load Script template at index {}",currentTemplateIndex);
                    }
                    else
                    {
                        auto templateName = templates.at(currentTemplateIndex);
                        auto templateStr = mContext->getTemplatesModel()->getTemplate(AssetType::ASSET_TYPE_ENUM_SCRIPT, templateName, Constants::ASSET_FORMAT_SCRIPT_LUA);
                        if (scriptInst)
                        {
                            scriptInst->setSource(templateStr);
                            setScriptDefinition(mScriptDefinition);
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
            ImGui::End();
        }
    }

    void
    ScriptEditorWindow::setScriptDefinition
    (ScriptDefinition* scriptDefinition)
    {
        mScriptDefinition = scriptDefinition;
        auto projRunt = mContext->getProject()->getRuntime();
        ScriptRuntime* scriptInst = nullptr;
        if (projRunt)
        {
            auto scriptCache = projRunt->getScriptCache();
            if (scriptCache)
            {
                scriptInst = static_cast<ScriptRuntime*>(scriptCache->getRuntime(mScriptDefinition));
                if (scriptInst)
                {
                    mTextEditor.SetReadOnly(false);
                    mTextEditor.SetText(scriptInst->getSource());
                }
                else
                {
                    mTextEditor.SetText("");
                }
            }
            else
            {
                mTextEditor.SetText("");
            }
        }
    }
}
