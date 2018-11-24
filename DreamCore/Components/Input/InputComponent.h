/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#include "../IComponent.h"

using namespace std;

namespace Dream
{


    namespace KeyboardMapping
    {
        const static int KEY_UNKNOWN          =  -1;
        const static int KEY_SPACE            =  32;
        const static int KEY_APOSTROPHE       =  39;  /* ' */
        const static int KEY_COMMA            =  44;  /* , */
        const static int KEY_MINUS            =  45;  /* - */
        const static int KEY_PERIOD           =  46;  /* . */
        const static int KEY_SLASH            =  47;  /* / */
        const static int KEY_0                =  48;
        const static int KEY_1                =  49;
        const static int KEY_2                =  50;
        const static int KEY_3                =  51;
        const static int KEY_4                =  52;
        const static int KEY_5                =  53;
        const static int KEY_6                =  54;
        const static int KEY_7                =  55;
        const static int KEY_8                =  56;
        const static int KEY_9                =  57;
        const static int KEY_SEMICOLON        =  59;  /* ; */
        const static int KEY_EQUAL            =  61;  /* = */
        const static int KEY_A                =  65;
        const static int KEY_B                =  66;
        const static int KEY_C                =  67;
        const static int KEY_D                =  68;
        const static int KEY_E                =  69;
        const static int KEY_F                =  70;
        const static int KEY_G                =  71;
        const static int KEY_H                =  72;
        const static int KEY_I                =  73;
        const static int KEY_J                =  74;
        const static int KEY_K                =  75;
        const static int KEY_L                =  76;
        const static int KEY_M                =  77;
        const static int KEY_N                =  78;
        const static int KEY_O                =  79;
        const static int KEY_P                =  80;
        const static int KEY_Q                =  81;
        const static int KEY_R                =  82;
        const static int KEY_S                =  83;
        const static int KEY_T                =  84;
        const static int KEY_U                =  85;
        const static int KEY_V                =  86;
        const static int KEY_W                =  87;
        const static int KEY_X                =  88;
        const static int KEY_Y                =  89;
        const static int KEY_Z                =  90;
        const static int KEY_LEFT_BRACKET     =  91;  /* [ */
        const static int KEY_BACKSLASH        =  92;  /* \ */
        const static int KEY_RIGHT_BRACKET    =  93;  /* ] */
        const static int KEY_GRAVE_ACCENT     =  96;  /* ` */
        const static int KEY_WORLD_1          =  161; /* non-US #1 */
        const static int KEY_WORLD_2          =  162; /* non-US #2 */
        const static int KEY_ESCAPE           =  256;
        const static int KEY_ENTER            =  257;
        const static int KEY_TAB              =  258;
        const static int KEY_BACKSPACE        =  259;
        const static int KEY_INSERT           =  260;
        const static int KEY_DELETE           =  261;
        const static int KEY_RIGHT            =  262;
        const static int KEY_LEFT             =  263;
        const static int KEY_DOWN             =  264;
        const static int KEY_UP               =  265;
        const static int KEY_PAGE_UP          =  266;
        const static int KEY_PAGE_DOWN        =  267;
        const static int KEY_HOME             =  268;
        const static int KEY_END              =  269;
        const static int KEY_CAPS_LOCK        =  280;
        const static int KEY_SCROLL_LOCK      =  281;
        const static int KEY_NUM_LOCK         =  282;
        const static int KEY_PRINT_SCREEN     =  283;
        const static int KEY_PAUSE            =  284;
        const static int KEY_F1               =  290;
        const static int KEY_F2               =  291;
        const static int KEY_F3               =  292;
        const static int KEY_F4               =  293;
        const static int KEY_F5               =  294;
        const static int KEY_F6               =  295;
        const static int KEY_F7               =  296;
        const static int KEY_F8               =  297;
        const static int KEY_F9               =  298;
        const static int KEY_F10              =  299;
        const static int KEY_F11              =  300;
        const static int KEY_F12              =  301;
        const static int KEY_F13              =  302;
        const static int KEY_F14              =  303;
        const static int KEY_F15              =  304;
        const static int KEY_F16              =  305;
        const static int KEY_F17              =  306;
        const static int KEY_F18              =  307;
        const static int KEY_F19              =  308;
        const static int KEY_F20              =  309;
        const static int KEY_F21              =  310;
        const static int KEY_F22              =  311;
        const static int KEY_F23              =  312;
        const static int KEY_F24              =  313;
        const static int KEY_F25              =  314;
        const static int KEY_KP_0             =  320;
        const static int KEY_KP_1             =  321;
        const static int KEY_KP_2             =  322;
        const static int KEY_KP_3             =  323;
        const static int KEY_KP_4             =  324;
        const static int KEY_KP_5             =  325;
        const static int KEY_KP_6             =  326;
        const static int KEY_KP_7             =  327;
        const static int KEY_KP_8             =  328;
        const static int KEY_KP_9             =  329;
        const static int KEY_KP_DECIMAL       =  330;
        const static int KEY_KP_DIVIDE        =  331;
        const static int KEY_KP_MULTIPLY      =  332;
        const static int KEY_KP_SUBTRACT      =  333;
        const static int KEY_KP_ADD           =  334;
        const static int KEY_KP_ENTER         =  335;
        const static int KEY_KP_EQUAL         =  336;
        const static int KEY_LEFT_SHIFT       =  340;
        const static int KEY_LEFT_CONTROL     =  341;
        const static int KEY_LEFT_ALT         =  342;
        const static int KEY_LEFT_SUPER       =  343;
        const static int KEY_RIGHT_SHIFT      =  344;
        const static int KEY_RIGHT_CONTROL    =  345;
        const static int KEY_RIGHT_ALT        =  346;
        const static int KEY_RIGHT_SUPER      =  347;
        const static int KEY_MENU             =  348;
    }

    struct JoystickMapping
    {
    public:
        // Axis
        int AnalogLeftXAxis;
        int AnalogLeftYAxis;
        int AnalogRightXAxis;
        int AnalogRightYAxis;
        // Buttons
        int FaceButtonNorth; // Y
        int FaceButtonEast; // B
        int FaceButtonSouth; // A
        int FaceButtonWest; // X
        int ShoulderLeft;
        int ShoulderRight;
        int TriggerLeftButton;
        int TriggerRightButton;
        int FaceButtonSelect;
        int FaceButtonStart;
        int AnalogLeftButton;
        int AnalogRightButton;
        int FaceButtonHome;
        int DPadNorth;
        int DPadWest;
        int DPadSouth;
        int DPadEast;
    };

    struct JoystickState
    {
        string Name = "";
        // Joystick
        int ButtonCount = 0;
        unsigned char ButtonData[32] = {0};
        int AxisCount = 0;
        float AxisData[32] = {0.0f};
        float DeadZone = 0.2f;
    };

    struct MouseState
    {
        float PosX = 0;
        float PosY = 0;
        float ScrollX = 0;
        float ScrollY = 0;
        bool ButtonsDown[5] = {false};
    };

    struct KeyboardState
    {
        bool KeysDown[512] = {false};
    };

    class InputComponent : public IComponent
    {
    public:

        enum InputModeFlags
        {
            None                  = 0,
            KeysOrbitCamera       = 1,
            JoystickFollowsObject = 2,
            MouseFollowsObject    = 3
        };

        InputComponent(bool useKeyboard = false, bool useMouse = false, bool useJoystick = false);
        ~InputComponent() override;

        bool init() override;
        void updateComponent(SceneRuntime*) override;

        void debugKeyboard() const;
        void debugMouse() const;
        void debugState() const;

        bool usingKeyboard() const;
        bool usingMouse() const;
        bool usingJoystick() const;

        bool isKeyDown(int key);

        KeyboardState& getKeyboardState();
        void setKeyboardState(const KeyboardState& keyboardState);

        MouseState& getMouseState();
        void setMouseState(const MouseState& mouseState);

        JoystickState& getJoystickState();
        void setJoystickState(const JoystickState& joystickState);

        int getInputModeFlags() const;
        void setInputModeFlags(const int inputMode);

        float outsizeDeadzone(int axis);
        float mouseDeltaX();
        float mouseDeltaY();

        float clearDeadzone(float val);

    private:
        int mInputModeFlags;

        bool mUseKeyboard;
        bool mUseMouse;
        bool mUseJoystick;

        KeyboardState mKeyboardState;
        KeyboardState mLastKeyboardState;

        MouseState mMouseState;
        MouseState mLastMouseState;

        JoystickState mJoystickState;
        JoystickState mLastJoystickState;
        JoystickMapping mJoystickMapping;

    public:
        const static JoystickMapping JsPsxMapping;
        const static JoystickMapping JsXboxMapping;
    };
}
