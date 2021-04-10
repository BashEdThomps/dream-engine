#pragma once

namespace octronic::dream
{
    enum SceneState
    {
        SCENE_STATE_TO_LOAD = 0,
        SCENE_STATE_LOADED,
        SCENE_STATE_ACTIVE,
        SCENE_STATE_TO_DESTROY,
        SCENE_STATE_DESTROYED
    };
}
