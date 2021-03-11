#pragma once

#include <string>

using std::string;

namespace octronic::dream
{
	class JoystickState
    {
    public:
        JoystickState();
        float getAxisData(int idx);
        void setAxisData(unsigned int index, float data);
        bool getButtonData(int idx);
        void setButtonData(unsigned int index, bool data);
        float getDeadZone() const;
        const float* getAxisDataPointer() const;
        const unsigned char* getButtonDataPointer() const;
        void setName(const string& name);
        string getName() const;
        int getAxisCount() const;
        void setAxisCount(int count);
        int getButtonCount() const;
        void setButtonCount(int count);
        bool clearsDeadzone(float val) const;
    private:
        string mName;
        int mButtonCount;
        unsigned char mButtonData[32];
        int mAxisCount;
        float mAxisData[32];
        float mDeadZone;
    };
}
