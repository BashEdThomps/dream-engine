#include "DreamToolContext.h"

using octronic::dream::tool::DreamToolContext;

int main(int argc,char** argv)
{
    LOG_LEVEL(LOG_LEVEL_DEBUG);
    LOG_TRACE("Main: Starting...");
    DreamToolContext state(argc,argv);
    if(!state.init())
    {
        return 1;
    }
    state.run();

    LOG_INFO("Main: Run is done. Performing stack-based clean up");
    return 0;
}
