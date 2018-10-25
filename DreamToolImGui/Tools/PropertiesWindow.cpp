#include "PropertiesWindow.h"

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

            ImGui::Begin("Properties");
            {
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
            ImGui::End();
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
        mDefinition = nullptr;
        mRuntime = nullptr;
    }

    void
    PropertiesWindow::setPropertyType
    (PropertyType t)
    {
       mType = t;
    }

    void PropertiesWindow::drawProjectProperties()
    {
        // Description
        // Notes
        // Author
        // Window Dimensions
        // Startup Scene
        // Capture mouse,keyboard,joystick
    }

    void PropertiesWindow::drawSceneProperties()
    {
        // Physics Debug
        // Mesh Cull Distance
        // min/max draw
        // Notes
        // Camera Transform
        // Camera Look at?
        // Gravity
        // Clear Colour
        // Ambient Colour
        // Lighting Shader

    }

    void PropertiesWindow::drawSceneObjectProperties()
    {}

    void PropertiesWindow::drawAssetProperties()
    {}
}
