#include "LuaDebugWindow.h"

namespace DreamTool
{

    LuaDebugWindow::LuaDebugWindow
    (Project* proj)
        : DTWidget(proj),
        LuaPrintListener()
    {
        setLogClassName("LuaDebugWindow");
    }

    LuaDebugWindow::~LuaDebugWindow
    ()
    {

    }

    void
    LuaDebugWindow::draw
    ()
    {
    }

    void
    LuaDebugWindow::onPrint
    (string)
    {

    }
}
