#pragma once

#include <iostream>
#include <vector>

#include "Frame.h"

#include "../../Common/Constants.h"
#include "../Transform3D.h"

namespace Dream
{

    using namespace std;

    class KeyFrame : public ILoggable
    {
    private:
        long mStartTimeMS;
        bool mWrap;
        vector<Frame*> mPlaybackFrames;
        Transform3D mTransform;
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

        Transform3D getTransform();
        void setTransform(Transform3D transform);

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

