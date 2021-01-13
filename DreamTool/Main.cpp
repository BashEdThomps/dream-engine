#include "DTContext.h"

using DreamTool::DTContext;

int main(int argc,char** argv)
{
    LOG_LEVEL(LOG_LEVEL_TRACE);
    LOG_TRACE("Main: Starting...");
    DTContext state(argc,argv);
    if(!state.init())
    {
        return 1;
    }
    state.run();

    LOG_INFO("Main: Run is done. Performing stack-based clean up");
    return 0;
}
