
#include <algorithm>

#include "AnimationInstance.h"
#include "KeyFrame.h"

#include "../../Common/Constants.h"
#include "../../Utilities/String.h"

namespace Dream
{

    int AnimationInstance::FramesPerSecond = 60;

    void
    AnimationInstance::setFramesPerSecond
    (int fps)
    {
        FramesPerSecond = fps;
    }

    int
    AnimationInstance::getFramesPerSecond
    ()
    {
        return FramesPerSecond;
    }


    AnimationInstance::AnimationInstance
    (AnimationDefinition* definition, SceneObjectRuntime* runtime)
        : IAssetInstance(definition,runtime), ILoggable ("AnimationInstance")
    {
        mCurrentPlaybackFrame = 0;
        mLoop = false;
        mPlaying = false;
    }


    AnimationInstance::~AnimationInstance
    ()
    {
        auto log = getLog();
        log->info("Destroying Object");
        deleteKeyFrames();
        deletePlaybackFrames();
    }

    void
    AnimationInstance::showStatus
    ()
    {
        auto log = getLog();
        log->info("\tLoop: {}",String::boolToYesNo(mLoop));
    }

    bool
    AnimationInstance::load
    (string projectPath)
    {
        mLoop = mDefinitionHandle->getJson()[Constants::ASSET_ATTR_LOOP];
        loadExtraAttributes(mDefinitionHandle->getJson());
        mLoaded = false;
        return mLoaded;
    }

    void
    AnimationInstance::addKeyFrame
    (KeyFrame *kf)
    {
        mKeyFrames.push_back(kf);
    }

    void
    AnimationInstance::generatePlaybackFrames
    ()
    {
        auto log = getLog();
        log->info("Generating Playback Frames");

        for (KeyFrame* currentKeyFrame : mKeyFrames)
        {
            // Get the next KeyFrame
            KeyFrame* nextKeyFrame = nullptr;
            // End of Vector?
            if (currentKeyFrame == mKeyFrames.back())
            {
                log->info("Last KeyFrame, checking for wrap..." );
                if (currentKeyFrame->getWrap())
                {
                    log->info("KeyFrame wraps to beginning." );
                    nextKeyFrame = (*mKeyFrames.begin());
                }
                else
                {
                    log->info("Last KeyFrame does not wrap." );
                    break;
                }
            }
            else
            {
                nextKeyFrame = currentKeyFrame;
            }

            currentKeyFrame->generatePlaybackFrames(nextKeyFrame);
            vector<Frame*> frames = currentKeyFrame->getPlaybackFrames();
            mPlaybackFrames.insert(mPlaybackFrames.end(),frames.begin(),frames.end());
        }
        log->info("Finished Generating Playback Frames" );
    }

    void
    AnimationInstance::loadExtraAttributes
    (nlohmann::json jsonObj)
    {
        auto log = getLog();
        showStatus();
        if (!jsonObj[Constants::ASSET_ATTR_KEYFRAMES].is_null() && jsonObj[Constants::ASSET_ATTR_KEYFRAMES].is_array())
        {
            nlohmann::json jsonKeyFrames = jsonObj[Constants::ASSET_ATTR_KEYFRAMES];
            log->info( "Loading KeyFrames" );
            for (nlohmann::json it : jsonKeyFrames)
            {
                KeyFrame *nextKeyFrame = new KeyFrame();

                Transform3D transform(it[Constants::TRANSFORM]);

                long startTime   = it[Constants::ASSET_ATTR_START_TIME];

                bool wrap = false;
                if (!it[Constants::ASSET_ATTR_WRAP].is_null())
                {
                    wrap = it[Constants::ASSET_ATTR_WRAP];
                }

                string interpolation = it[Constants::ASSET_ATTR_INTERPOLATION];
                string name = it[Constants::NAME];
                string uuid = it[Constants::UUID];

                nextKeyFrame->setName(name);
                nextKeyFrame->setInterpolationType(interpolation);
                nextKeyFrame->setUUID(uuid);
                nextKeyFrame->setStartTimeMS(startTime);
                nextKeyFrame->setTransform(transform);
                nextKeyFrame->setWrap(wrap);

                nextKeyFrame->showStatus();

                addKeyFrame(nextKeyFrame);
            }
            generatePlaybackFrames();
        }
    }

    void
    AnimationInstance::step
    (double deltaTime)
    {
        auto log = getLog();
        if (mPlaying)
        {
            int advanceBy = ceil(deltaTime / (1000/getFramesPerSecond()));
            if (advanceBy > Constants::MAX_FRAME_ADVANCE) return;
            mCurrentPlaybackFrame += advanceBy;
            log->info(
                "Delta time: {}, Advance By: {} frames to frame: {}",
                deltaTime, advanceBy, mCurrentPlaybackFrame
            );

            if (mCurrentPlaybackFrame > mPlaybackFrames.size())
            {
                if (!mLoop)
                {
                    log->info("Playback Finished" );
                    mPlaying = false;
                }
                log->info("Returning to Frame 0" );
                mCurrentPlaybackFrame = 0;
            }
        }
    }

    void
    AnimationInstance::play
    ()
    {
        auto log = getLog();
        log->info("Playing Animation", getName() );
        mPlaying = true;
    }

    void
    AnimationInstance::pause
    ()
    {
        auto log = getLog();
        log->info("AnimationInstance: Pausing Animation", getName() );
        mPlaying = false;
    }

    void
    AnimationInstance::stop
    ()
    {
        auto log = getLog();
        log->info( "AnimationInstance: Stopping Animation" , getName() );
        mPlaying = false;
        mCurrentPlaybackFrame = 0;
    }

    bool
    AnimationInstance::isLooping()
    {
        return mLoop;
    }

    void
    AnimationInstance::setLooping
    (bool looping)
    {
        mLoop = looping;
    }

    void
    AnimationInstance::applyTransform
    (Transform3D transform)
    {
        if (mPlaying && mCurrentPlaybackFrame < mPlaybackFrames.size())
        {
            Frame *currentFrame = mPlaybackFrames[mCurrentPlaybackFrame];
            currentFrame->applyToTransform(transform);
        }
    }

    void
    AnimationInstance::deleteKeyFrames
    ()
    {
        auto log = getLog();
        log->info("Deleting Key Frames" );
        for (KeyFrame* pKeyFrame : mKeyFrames)
        {
            delete pKeyFrame;
        }
        mKeyFrames.clear();
    }

    void
    AnimationInstance::deletePlaybackFrames
    ()
    {
        auto log = getLog();
        log->info( "AnimationInstance: Playback Frames" );
        for (Frame* pFrame : mPlaybackFrames)
        {
            delete pFrame;
        }
        mPlaybackFrames.clear();
    }

 } // End of Dream
