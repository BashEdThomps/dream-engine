#include "DreamToolContext.h"

using octronic::dream::tool::DreamToolContext;

int main(int argc,char** argv)
{
    LOG_LEVEL(LOG_LEVEL_ERROR);
    LOG_TRACE("Main: Starting...");

    DreamToolContext context(argc,argv);

    if(!context.init()) exit(1);
    context.run();

    LOG_INFO("Main: Run is done. Performing stack-based clean up");
    exit(0);
}
