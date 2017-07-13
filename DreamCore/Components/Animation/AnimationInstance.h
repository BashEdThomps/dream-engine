#pragma once

#include <vector>
#include <json.hpp>
#include "../IAssetInstance.h"
#include "AnimationDefinition.h"

using std::vector;
using nlohmann::json;

namespace Dream
{
    class Frame;
    class KeyFrame;
    class Transform3D;

    class AnimationInstance : public IAssetInstance
    {

    private:
        static int FramesPerSecond;
    public:
        static void setFramesPerSecond(int);
        static int  getFramesPerSecond();
    private:
        vector<KeyFrame*> mKeyFrames;
        vector<Frame*> mPlaybackFrames;
        int mCurrentPlaybackFrame;
        bool mLoop;
        bool mPlaying;
    public:
        AnimationInstance(AnimationDefinition*,SceneObjectRuntime*);
        ~AnimationInstance();
        bool load(string);
        bool isLooping();
        void setLooping(bool);
        int  getState();
        void play();
        void stop();
        void pause();
        void step(double);
        void showStatus();
        void applyTransform(Transform3D);
    private:
        void generatePlaybackFrames();
        void addKeyFrame(KeyFrame*);
        void addFrame(Frame*);
        void loadExtraAttributes(json);
        void deleteKeyFrames();
        void deletePlaybackFrames();

    }; // End of AnimationInstance

} // End of Dream
