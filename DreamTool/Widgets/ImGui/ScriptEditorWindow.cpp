#include "ScriptEditorWindow.h"
#include "../../DTState.h"

namespace DreamTool
{

    ScriptEditorWindow::ScriptEditorWindow
    (DTState* state)
        : ImGuiWidget (state),
          mScriptDefinition(nullptr)
    {
        setLogClassName("ScriptEditorWidget");
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
        if (mScriptDefinition == nullptr)
        {
            return;
        }

        ImGui::Begin("Script Editor",&mVisible);
        auto log = getLog();
        auto projRunt = mState->project->getProjectRuntime();
        ScriptInstance* scriptInst = nullptr;
        {
            auto scriptCache = projRunt->getScriptCache();
            if (scriptCache)
            {
                scriptInst = dynamic_cast<ScriptInstance*>(scriptCache->getInstance(mScriptDefinition));
            }
        }

        if(ImGui::Button("Save"))
        {
            if(scriptInst)
            {
                ProjectDirectory pd(mState->project);
                auto currentText = mTextEditor.GetText();
                scriptInst->setSource(currentText);
                string source = scriptInst->getSource();
                vector<char> data(source.begin(),source.end());
                pd.writeAssetData(mScriptDefinition,data,Constants::ASSET_FORMAT_SCRIPT_LUA);
            }
        }
        ImGui::SameLine();

        vector<string> templates = mState->templatesModel.getTemplateNames(AssetType::SCRIPT);
        static int currentTemplateIndex = -1;
        if (StringCombo("Template",&currentTemplateIndex,templates,templates.size()))
        {
            ImGui::OpenPopup("Load From Template?");
        }

        //ImGui::PushFont(DTWindowComponent::MonoFont);

        auto cpos = mTextEditor.GetCursorPosition();
        ImGui::Text(
            "%6d/%-6d %6d lines  | %s | %s | %s | %s",
            cpos.mLine + 1,
            cpos.mColumn + 1,
            mTextEditor.GetTotalLines(),
            mTextEditor.IsOverwrite() ? "Ovr" : "Ins",
            mTextEditor.CanUndo() ? "*" : " ",
            mTextEditor.GetLanguageDefinition().mName.c_str(),
            mScriptDefinition->getNameAndUuidString().c_str());
        mTextEditor.Render("Text Editor");
        //ImGui::PopFont();

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
                   log->error("Cannot load Script template at index {}",currentTemplateIndex);
               }
               else
               {
                   auto templateName = templates.at(currentTemplateIndex);
                   auto templateStr = mState->templatesModel.getTemplate(AssetType::SCRIPT, templateName, Constants::ASSET_FORMAT_SCRIPT_LUA);
                   if (scriptInst)
                   {
                       scriptInst->setSource(templateStr);
                       setScriptDefinition(mScriptDefinition);
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
        ImGui::End();
    }

    void
    ScriptEditorWindow::setScriptDefinition
    (ScriptDefinition* scriptDefinition)
    {
        mScriptDefinition = scriptDefinition;
        auto projRunt = mState->project->getProjectRuntime();
        ScriptInstance* scriptInst = nullptr;
        if (projRunt)
        {
            auto scriptCache = projRunt->getScriptCache();
            if (scriptCache)
            {
                scriptInst = dynamic_cast<ScriptInstance*>(scriptCache->getInstance(mScriptDefinition));
                if (scriptInst)
                {
                    mTextEditor.SetReadOnly(false);
                    mTextEditor.SetText(scriptInst->getSource());
                }
            }
        }
    }
}
