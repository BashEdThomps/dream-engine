/*
 * SdlLuaApi.cpp
 *
 * Created: 12 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "SdlLuaApi.h"
#include <SDL2/SDL.h>

namespace DreamSDL
{
    SdlLuaApi::SdlLuaApi() : ILuaExposable()
    {

    }

    SdlLuaApi::~SdlLuaApi()
    {

    }

    void SdlLuaApi::exposeLuaApi(lua_State* state)
    {
        luabind::module(state)
                [
                // SDL_Event =======================================================
                luabind::class_<SDL_Event>("Event")
                .def_readonly("type", &SDL_Event::type)
                // Keyboard
                .def_readonly("key", &SDL_Event::key)
                // Controller
                .def_readonly("cbutton", &SDL_Event::cbutton)
                .def_readonly("caxis",   &SDL_Event::caxis)
                // Joystick
                .def_readonly("jbutton", &SDL_Event::jbutton)
                .def_readonly("jhat",    &SDL_Event::jhat)
                .def_readonly("jaxis",   &SDL_Event::jaxis)
                // Mouse
                .def_readonly("motion", &SDL_Event::motion)
                .def_readonly("button", &SDL_Event::button)
                .def_readonly("wheel",  &SDL_Event::wheel)
                // SDL_EventType ===============================================
                .enum_("EventType")
                [
                // Keys
                luabind::value("KEYUP",SDL_EventType::SDL_KEYUP),
                luabind::value("KEYDOWN",SDL_EventType::SDL_KEYDOWN),
                // Joystick
                luabind::value("JOY_BUTTONDOWN",SDL_JOYBUTTONDOWN),
                luabind::value("JOY_BUTTONUP",SDL_JOYBUTTONUP),
                luabind::value("JOY_HATMOTION",SDL_JOYHATMOTION),
                luabind::value("JOY_AXISMOTION",SDL_JOYAXISMOTION),
                // Controller
                luabind::value("CONTROLLER_BUTTONDOWN",SDL_CONTROLLERBUTTONDOWN),
                luabind::value("CONTROLLER_BUTTONUP",SDL_CONTROLLERBUTTONUP),
                luabind::value("CONTROLLER_AXISMOTION",SDL_CONTROLLERAXISMOTION),
                // Mouse
                luabind::value("MOUSE_MOTION",SDL_MOUSEMOTION),
                luabind::value("MOUSE_BUTTONUP",SDL_MOUSEBUTTONUP),
                luabind::value("MOUSE_BUTTONDOWN",SDL_MOUSEBUTTONDOWN)
                ]
                // SDLK_* Definitions ==========================================
                .enum_("Key")
                [
                luabind::value("KEY_RETURN",SDLK_RETURN),
                luabind::value("KEY_SPACE",SDLK_SPACE),
                luabind::value("KEY_LEFT",SDLK_LEFT),
                luabind::value("KEY_RIGHT",SDLK_RIGHT),
                luabind::value("KEY_UP",SDLK_UP),
                luabind::value("KEY_DOWN",SDLK_DOWN),
                // Alpha
                luabind::value("KEY_a",SDLK_a),
                luabind::value("KEY_b",SDLK_a),
                luabind::value("KEY_c",SDLK_c),
                luabind::value("KEY_d",SDLK_d),
                luabind::value("KEY_e",SDLK_e),
                luabind::value("KEY_f",SDLK_f),
                luabind::value("KEY_g",SDLK_g),
                luabind::value("KEY_h",SDLK_h),
                luabind::value("KEY_i",SDLK_i),
                luabind::value("KEY_j",SDLK_j),
                luabind::value("KEY_k",SDLK_k),
                luabind::value("KEY_l",SDLK_l),
                luabind::value("KEY_m",SDLK_m),
                luabind::value("KEY_n",SDLK_n),
                luabind::value("KEY_o",SDLK_o),
                luabind::value("KEY_p",SDLK_p),
                luabind::value("KEY_q",SDLK_q),
                luabind::value("KEY_r",SDLK_r),
                luabind::value("KEY_s",SDLK_s),
                luabind::value("KEY_t",SDLK_t),
                luabind::value("KEY_u",SDLK_u),
                luabind::value("KEY_v",SDLK_v),
                luabind::value("KEY_w",SDLK_w),
                luabind::value("KEY_x",SDLK_x),
                luabind::value("KEY_y",SDLK_y),
                luabind::value("KEY_z",SDLK_z),
                // Numero
                luabind::value("KEY_0",SDLK_0),
                luabind::value("KEY_1",SDLK_1),
                luabind::value("KEY_2",SDLK_2),
                luabind::value("KEY_3",SDLK_3),
                luabind::value("KEY_4",SDLK_4),
                luabind::value("KEY_5",SDLK_5),
                luabind::value("KEY_6",SDLK_6),
                luabind::value("KEY_7",SDLK_7),
                luabind::value("KEY_8",SDLK_8),
                luabind::value("KEY_9",SDLK_9)
                ],

                // SDL_ControllerButtonEvent =======================================
                luabind::class_<SDL_ControllerButtonEvent>("ControllerButtonEvent")
                .def_readonly("type",&SDL_ControllerButtonEvent::type)
                .def_readonly("timestamp",&SDL_ControllerButtonEvent::timestamp)
                .def_readonly("which",&SDL_ControllerButtonEvent::which)
                .def_readonly("button",&SDL_ControllerButtonEvent::button)
                .def_readonly("state",&SDL_ControllerButtonEvent::state)
                .enum_("GameControllerButton")
                [
                luabind::value("CBTN_INVALID",SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID),
                // Face
                luabind::value("C_BTN_A", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A),
                luabind::value("C_BTN_B", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B),
                luabind::value("C_BTN_X", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X),
                luabind::value("C_BTN_Y", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y),
                // Face.2
                luabind::value("C_BTN_BACK",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK),
                luabind::value("C_BTN_GUIDE", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_GUIDE),
                luabind::value("C_BTN_START", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START),
                // Stick
                luabind::value("C_BTN_LEFT_STICK",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK),
                luabind::value("C_BTN_RIGHT_STICK", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK),
                // Shoulder
                luabind::value("C_BTN_LEFT_SHOULDER",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
                luabind::value("C_BTN_RIGHT_SHOULDER", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),
                // DPAD
                luabind::value("C_BTN_DPAD_UP",    SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP),
                luabind::value("C_BTN_DPAD_DOWN",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN),
                luabind::value("C_BTN_DPAD_LEFT",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT),
                luabind::value("C_BTN_DPAD_RIGHT", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                ],

                // SDL_ControllerAxisEvent =========================================
                luabind::class_<SDL_ControllerAxisEvent>("ControllerAxisEvent")
                .def_readonly("type",&SDL_ControllerAxisEvent::type)
                .def_readonly("timestamp",&SDL_ControllerAxisEvent::timestamp)
                .def_readonly("axis",&SDL_ControllerAxisEvent::axis)
                .def_readonly("value",&SDL_ControllerAxisEvent::value)
                .enum_("GameControllerAxis")
                [
                luabind::value("C_AXIS_INVALID",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_INVALID),
                // Left
                luabind::value("C_AXIS_LEFT_X",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX),
                luabind::value("C_AXIS_LEFT_Y",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY),
                // Right
                luabind::value("C_AXIS_RIGHT_X",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX),
                luabind::value("C_AXIS_RIGHT_Y",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY),
                // Triggers
                luabind::value("C_AXIS_TRIGGER_LEFT",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT),
                luabind::value("C_AXIS_TRIGGER_RIGHT",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT),
                // Max
                luabind::value("C_AXIS_MAX",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX)
                ],

                // SDL_KeyboardEvent ===============================================
                luabind::class_<SDL_KeyboardEvent>("KeyboardEvent")
                .def_readonly("keysym",&SDL_KeyboardEvent::keysym),

                // SDL_Keysym ======================================================
                luabind::class_<SDL_Keysym>("Keysym")
                .def_readonly("sym",&SDL_Keysym::sym),

                // SDL_Keycode =====================================================
                luabind::class_<SDL_Keycode>("Keycode"),

                // SDL_JoyButtonEvent ==============================================
                luabind::class_<SDL_JoyButtonEvent>("JoyButtonEvent")
                .def_readonly("type",&SDL_JoyButtonEvent::type)
                .def_readonly("timestamp",&SDL_JoyButtonEvent::timestamp)
                .def_readonly("which",&SDL_JoyButtonEvent::which)
                .def_readonly("button",&SDL_JoyButtonEvent::button)
                .def_readonly("state",&SDL_JoyButtonEvent::state),

                // SDL_JoyHatEvent =================================================
                luabind::class_<SDL_JoyHatEvent>("JoyHatEvent")
                .def_readonly("type",&SDL_JoyHatEvent::type)
                .def_readonly("timestamp",&SDL_JoyHatEvent::timestamp)
                .def_readonly("which",&SDL_JoyHatEvent::which)
                .def_readonly("hat",&SDL_JoyHatEvent::hat)
                .def_readonly("value",&SDL_JoyHatEvent::value),

                // SDL_JoyAxisEvent ================================================
                luabind::class_<SDL_JoyAxisEvent>("JoyAxisEvent")
                .def_readonly("type",&SDL_JoyAxisEvent::type)
                .def_readonly("timestamp",&SDL_JoyAxisEvent::timestamp)
                .def_readonly("which",&SDL_JoyAxisEvent::which)
                .def_readonly("axis",&SDL_JoyAxisEvent::axis)
                .def_readonly("value",&SDL_JoyAxisEvent::value),

                // SDL_MouseMotionEvent ============================================
                luabind::class_<SDL_MouseMotionEvent>("MouseMotionEvent")
                .def_readonly("type",&SDL_MouseMotionEvent::type)
                .def_readonly("timestamp",&SDL_MouseMotionEvent::timestamp)
                .def_readonly("windowID",&SDL_MouseMotionEvent::windowID)
                .def_readonly("which",&SDL_MouseMotionEvent::which)
                .def_readonly("state",&SDL_MouseMotionEvent::state)
                .def_readonly("x",&SDL_MouseMotionEvent::x)
                .def_readonly("y",&SDL_MouseMotionEvent::y)
                .def_readonly("xrel",&SDL_MouseMotionEvent::xrel)
                .def_readonly("yrel",&SDL_MouseMotionEvent::yrel)

                ]; // end luabind::module
    }
}
