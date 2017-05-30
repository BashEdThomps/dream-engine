#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <DreamCore.h>

using namespace Dream;

TEST_CASE("Test DreamEngine Initialisation.")
{
    DreamEngine engine;
    REQUIRE(engine.getAudioComponent()  == nullptr);
    REQUIRE(engine.getWindowComponent() == nullptr);
    REQUIRE(engine.getProject()         == nullptr);
    REQUIRE(engine.getActiveScene()     == nullptr);

}
