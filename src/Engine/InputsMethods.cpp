#include "InputsMethods.h"

bool InputsMethods::GetButton(Mouse::Button const _button)
{
    return InputSystem::s_mouseStates[_button] == InputSystem::States::PRESSED;
}

bool InputsMethods::GetButtonUp(Mouse::Button const _button)
{
    return InputSystem::s_mouseStates[_button] == InputSystem::States::UP;
}

bool InputsMethods::GetButtonDown(Mouse::Button const _button)
{
    return InputSystem::s_mouseStates[_button] == InputSystem::States::DOWN;
}

bool InputsMethods::GetKey(Keyboard::Key const _key)
{
    return InputSystem::s_keyboardStates[_key] == InputSystem::States::PRESSED;
}

bool InputsMethods::GetKeyUp(Keyboard::Key const _key)
{
    return InputSystem::s_keyboardStates[_key] == InputSystem::States::UP;
}

bool InputsMethods::GetKeyDown(Keyboard::Key const _key)
{
    return InputSystem::s_keyboardStates[_key] == InputSystem::States::DOWN;
}

void InputsMethods::SetMousePosition(XMFLOAT2 _pos)
{
    POINT p{ _pos.x, _pos.y };
    ClientToScreen(InputSystem::m_wndHwnd, &p);
    SetCursorPos(p.x, p.y);
}

POINT InputsMethods::GetMousePosition()
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(InputSystem::m_wndHwnd, &p);
    return { p.x, p.y };
}

void InputsMethods::LockMouseCursor()
{
    if (InputSystem::m_wndHwnd == nullptr)
    {
        return;
    }

    RECT currentClip;
    GetClipCursor(&currentClip);

    RECT rect;
    GetClientRect(InputSystem::m_wndHwnd, &rect);
    MapWindowPoints(InputSystem::m_wndHwnd, nullptr, (LPPOINT)&rect, 2);
    if (currentClip.left != rect.left || currentClip.top != rect.top) {
        ClipCursor(&rect);
    };
}

void InputsMethods::UnlockMouseCursor()
{
    ClipCursor(nullptr);
}

void InputsMethods::ShowMouseCursor()
{
    if (InputSystem::m_isCursorHide == true)
    {
        while (ShowCursor(TRUE) < 0);
        InputSystem::m_isCursorHide = false;

    }
}

void InputsMethods::HideMouseCursor()
{
    if (InputSystem::m_isCursorHide == false)
    {
        while (ShowCursor(FALSE) >= 0);
        InputSystem::m_isCursorHide = true;

    }
}
