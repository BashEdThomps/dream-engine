
#include "AnimationInstance.h"
#include <algorithm>
#include "../../String.h"

namespace Dream {

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
    (AssetDefinition* definition, Transform3D* transform)
        : IAssetInstance(definition,transform)
    {
        mCurrentPlaybackFrame = 0;
        mLoop = false;
        mPlaying = false;
    }


    AnimationInstance::~AnimationInstance
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AnimationInstance: Destroying Object" << endl;
        }
        deleteKeyFrames();
        deletePlaybackFrames();
    }

    void
    AnimationInstance::showStatus
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AnimationInstance:" << endl;
            cout << "\tLoop: " << String::boolToYesNo(mLoop) << endl;
        }
    }

    bool
    AnimationInstance::load
    (string projectPath)
    {
        mLoop = mDefinition->toJson()[Constants::ASSET_ATTR_LOOP];
        loadExtraAttributes(mDefinition->toJson());
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
        if (Constants::DEBUG)
        {
            cout<<"AnimationInstance: Generating Playback Frames" << endl;
        }

        for (KeyFrame* currentKeyFrame : mKeyFrames)
        {
            // Get the next KeyFrame
            KeyFrame* nextKeyFrame = nullptr;
            // End of Vector?
            if (currentKeyFrame == mKeyFrames.back())
            {
                if (Constants::DEBUG)
                {
                    cout << "AnimationInstance: Last KeyFrame, checking for wrap..." << endl;
                }
                if (currentKeyFrame->getWrap())
                {
                    if (Constants::DEBUG)
                    {
                        cout << "AnimationInstance: KeyFrame wraps to beginning." << endl;
                    }
                    nextKeyFrame = (*mKeyFrames.begin());
                }
                else
                {
                    if (Constants::DEBUG)
                    {
                        cout << "AnimationInstance: Last KeyFrame does not wrap." << endl;
                    }
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
        if (Constants::DEBUG)
        {
            cout<<"AnimationInstance: Finished Generating Playback Frames" << endl;
        }
    }

    void
    AnimationInstance::loadExtraAttributes
    (nlohmann::json jsonObj)
    {
        showStatus();
        if (!jsonObj[Constants::ASSET_ATTR_KEYFRAMES].is_null() && jsonObj[Constants::ASSET_ATTR_KEYFRAMES].is_array())
        {
            nlohmann::json jsonKeyFrames = jsonObj[Constants::ASSET_ATTR_KEYFRAMES];
            if (Constants::DEBUG)
            {
                cout << "AnimationInstance: Loading KeyFrames" << endl;
            }
            for (nlohmann::json it : jsonKeyFrames)
            {
                KeyFrame *nextKeyFrame = new KeyFrame();
                vector<float> translation(3), rotation(3), scale(3);

                translation[0] = it[Constants::ASSET_ATTR_TRANSLATION][Constants::ASSET_ATTR_X];
                translation[1] = it[Constants::ASSET_ATTR_TRANSLATION][Constants::ASSET_ATTR_Y];
                translation[2] = it[Constants::ASSET_ATTR_TRANSLATION][Constants::ASSET_ATTR_Z];

                rotation[0] = it[Constants::ASSET_ATTR_ROTATION][Constants::ASSET_ATTR_X];
                rotation[1] = it[Constants::ASSET_ATTR_ROTATION][Constants::ASSET_ATTR_Y];
                rotation[2] = it[Constants::ASSET_ATTR_ROTATION][Constants::ASSET_ATTR_Z];

                scale[0] = it[Constants::ASSET_ATTR_SCALE][Constants::ASSET_ATTR_X];
                scale[1] = it[Constants::ASSET_ATTR_SCALE][Constants::ASSET_ATTR_Y];
                scale[2] = it[Constants::ASSET_ATTR_SCALE][Constants::ASSET_ATTR_Z];

                long startTime   = it[Constants::ASSET_ATTR_START_TIME];

                bool wrap = false;
                if (!it[Constants::ASSET_ATTR_WRAP].is_null())
                {
                    wrap = it[Constants::ASSET_ATTR_WRAP];
                }

                string interpolation = it[Constants::ASSET_ATTR_INTERPOLATION];
                string name = it[Constants::ASSET_NAME];
                string uuid = it[Constants::ASSET_UUID];

                nextKeyFrame->setName(name);
                nextKeyFrame->setInterpolationType(interpolation);
                nextKeyFrame->setUUID(uuid);
                nextKeyFrame->setStartTimeMS(startTime);
                nextKeyFrame->setTranslation(translation);
                nextKeyFrame->setRotation(rotation);
                nextKeyFrame->setScale(scale);
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
        if (mPlaying)
        {
            int advanceBy = ceil(deltaTime / (1000/getFramesPerSecond()));
            if (advanceBy > Constants::MAX_FRAME_ADVANCE) return;
            mCurrentPlaybackFrame += advanceBy;
            if (Constants::DEBUG)
            {
                cout << "AnimationInstance: Delta time: " << deltaTime << ", Advance By: " << advanceBy <<  " frames to frame: " << mCurrentPlaybackFrame << endl;
            }
            if (mCurrentPlaybackFrame > mPlaybackFrames.size())
            {
                if (!mLoop)
                {
                    if (Constants::DEBUG)
                    {
                        cout << "AnimationInstance: Playback Finished" << endl;
                    }
                    mPlaying = false;
                }
                if (Constants::DEBUG)
                {
                    cout << "AnimationInstance: Returning to Frame 0" << endl;
                }
                mCurrentPlaybackFrame = 0;
            }
        }
    }

    void
    AnimationInstance::play
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AnimationInstance: Playing Animation" << getName() << endl;
        }
        mPlaying = true;
    }

    void
    AnimationInstance::pause
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AnimationInstance: Pausing Animation" << getName() << endl;
        }
        mPlaying = false;
    }

    void
    AnimationInstance::stop
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AnimationInstance: Stopping Animation" << getName() << endl;
        }
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
    (Transform3D* transform)
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
        for (Frame* pFrame : mPlaybackFrames)
        {
            delete pFrame;
        }
        mPlaybackFrames.clear();
    }

     } // End of Dream
