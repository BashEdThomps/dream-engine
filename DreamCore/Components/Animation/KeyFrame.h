#pragma once

#include <iostream>
#include <vector>

#include "Frame.h"

#include "../../Common/Constants.h"

namespace Dream
{

    using namespace std;

    class KeyFrame
    {
    private:
        long mStartTimeMS;
        bool mWrap;
        vector<Frame*> mPlaybackFrames;
        vector<float> mTranslation;
        vector<float> mRotation;
        vector<float> mScale;
        string mName;
        string mUUID;
        string mInterpolationType;
    public:
        KeyFrame();
        ~KeyFrame();
        void showStatus();
        vector<Frame*> getPlaybackFrames();
        void generatePlaybackFrames(KeyFrame*);
        void addPlaybackFrame(Frame*);
        bool getWrap();
        void setWrap(bool);
        long getStartTimeMS();
        void setStartTimeMS(long);
        vector<float> getTranslation();
        void setTranslation( vector<float>);
        vector<float> getRotation();
        void setRotation( vector<float>);
        vector<float> getScale();
        void setScale( vector<float>);
        string getName();
        void setName( string);
        string getUUID();
        void setUUID( string);
        void setInterpolationType( string);
        string getInterpolationType();
        float getBezierPoint(float,float,float);

        bool isInterpolationTypeNone();
        void generateNoneInterpolationFrames(KeyFrame*,long);

        bool isInterpolationTypeLinear();
        void generateLinearInterpolationFrames(KeyFrame*,long);

        bool isInterpolationTypeBezier();
        void generateBezierInterpolationFrames(KeyFrame*,long);
    };

} // End of Dream

