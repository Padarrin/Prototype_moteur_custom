#pragma once
#include "pch.h"
#include "unordered_map"


class InputSystem
{
public:

    enum States : int
    {
        STATE_NONE = 0b00,
        PRESSED = 0b01,
        UP = 0b10,
        DOWN = 0b11
    };

private : 
	
    static std::unordered_map<int, int> m_mouseMap;
    static std::unordered_map<int, int> m_keyboardMap;
    inline static States s_mouseStates[Mouse::AMOUNT];
    inline static States s_keyboardStates[Keyboard::AMOUNT];


    static HWND m_wndHwnd;
    static bool m_isCursorLock;
    static bool m_isCursorHide;

public:

    static void Create();

    static void HandleInputs();

    static bool GetButton(Mouse::Button const _button);
    static bool GetButtonUp(Mouse::Button const _button);
    static bool GetButtonDown(Mouse::Button const _button);
    static bool GetKey(Keyboard::Key const _key);
    static bool GetKeyUp(Keyboard::Key const _key);
    static bool GetKeyDown(Keyboard::Key const  _key);

    static void SetMousePosition(XMFLOAT2 _pos);
    static POINT GetMousePosition();
    static void LockMouseCursor();
    static void UnlockMouseCursor();
    static void ShowMouseCursor();
    static void HideMouseCursor();
private:
        friend class InputsMethods;
};

