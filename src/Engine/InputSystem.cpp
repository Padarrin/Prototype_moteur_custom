#include "InputSystem.h"
#include "InputsMethods.h"

std::unordered_map<int, int> InputSystem::m_mouseMap;
std::unordered_map<int, int> InputSystem::m_keyboardMap;
HWND InputSystem::m_wndHwnd;
bool InputSystem::m_isCursorLock = false;
bool InputSystem::m_isCursorHide = false;

void InputSystem::Create()
{
    m_mouseMap = {
        { Mouse::LEFT, VK_LBUTTON },
        { Mouse::RIGHT,  VK_RBUTTON },
        { Mouse::MIDDLE, VK_MBUTTON },
        { Mouse::EXTRA_1, VK_XBUTTON1 },
        { Mouse::EXTRA_2, VK_XBUTTON2 }
    };
    m_keyboardMap = {
    { Keyboard::BACKSPACE,       VK_BACK     },
    { Keyboard::TAB,             VK_TAB      },
    { Keyboard::RETURN,          VK_RETURN   },
    { Keyboard::PAUSE,           VK_PAUSE    },
    { Keyboard::CAPSLOCK,        VK_CAPITAL  },
    { Keyboard::ESCAPE,          VK_ESCAPE   },
    { Keyboard::SPACE,           VK_SPACE    },
    { Keyboard::PAGE_UP,         VK_PRIOR    },
    { Keyboard::PAGE_DOWN,       VK_NEXT     },
    { Keyboard::END,             VK_END      },
    { Keyboard::HOME,            VK_HOME     },
    { Keyboard::LEFT,            VK_LEFT     },
    { Keyboard::UP,              VK_UP       },
    { Keyboard::RIGHT,           VK_RIGHT    },
    { Keyboard::DOWN,            VK_DOWN     },
    { Keyboard::INSERT,          VK_INSERT   },
    { Keyboard::DELETE_,         VK_DELETE   },
    { Keyboard::LWINDOW,         VK_LWIN     },
    { Keyboard::RWINDOW,         VK_RWIN     },
    { Keyboard::NUMPAD0,         VK_NUMPAD0  },
    { Keyboard::NUMPAD1,         VK_NUMPAD1  },
    { Keyboard::NUMPAD2,         VK_NUMPAD2  },
    { Keyboard::NUMPAD3,         VK_NUMPAD3  },
    { Keyboard::NUMPAD4,         VK_NUMPAD4  },
    { Keyboard::NUMPAD5,         VK_NUMPAD5  },
    { Keyboard::NUMPAD6,         VK_NUMPAD6  },
    { Keyboard::NUMPAD7,         VK_NUMPAD7  },
    { Keyboard::NUMPAD8,         VK_NUMPAD8  },
    { Keyboard::NUMPAD9,         VK_NUMPAD9  },
    { Keyboard::NUMPAD_MULTIPLY, VK_MULTIPLY },
    { Keyboard::NUMPAD_ADD,      VK_ADD      },
    { Keyboard::NUMPAD_SUBTRACT, VK_SUBTRACT },
    { Keyboard::NUMPAD_DECIMAL,  VK_DECIMAL  },
    { Keyboard::NUMPAD_DIVIDE,   VK_DIVIDE   },
    { Keyboard::F1,              VK_F1       },
    { Keyboard::F2,              VK_F2       },
    { Keyboard::F3,              VK_F3       },
    { Keyboard::F4,              VK_F4       },
    { Keyboard::F5,              VK_F5       },
    { Keyboard::F6,              VK_F6       },
    { Keyboard::F7,              VK_F7       },
    { Keyboard::F8,              VK_F8       },
    { Keyboard::F9,              VK_F9       },
    { Keyboard::F10,             VK_F10      },
    { Keyboard::F11,             VK_F11      },
    { Keyboard::F12,             VK_F12      },
    { Keyboard::NUMLOCK,         VK_NUMLOCK  },
    { Keyboard::SCROLL_LOCK,     VK_SCROLL   },
    { Keyboard::LSHIFT,          VK_LSHIFT   },
    { Keyboard::RSHIFT,          VK_RSHIFT   },
    { Keyboard::LCONTROL,        VK_LCONTROL },
    { Keyboard::RCONTROL,        VK_RCONTROL },
    { Keyboard::LALT,            VK_LMENU    },
    { Keyboard::RALT,            VK_RMENU    },
    { Keyboard::A,              'A'          },
    { Keyboard::B,              'B'          },
    { Keyboard::C,              'C'          },
    { Keyboard::D,              'D'          },
    { Keyboard::E,              'E'          },
    { Keyboard::F,              'F'          },
    { Keyboard::G,              'G'          },
    { Keyboard::H,              'H'          },
    { Keyboard::I,              'I'          },
    { Keyboard::J,              'J'          },
    { Keyboard::K,              'K'          },
    { Keyboard::L,              'L'          },
    { Keyboard::M,              'M'          },
    { Keyboard::N,              'N'          },
    { Keyboard::O,              'O'          },
    { Keyboard::P,              'P'          },
    { Keyboard::Q,              'Q'          },
    { Keyboard::R,              'R'          },
    { Keyboard::S,              'S'          },
    { Keyboard::T,              'T'          },
    { Keyboard::U,              'U'          },
    { Keyboard::V,              'V'          },
    { Keyboard::W,              'W'          },
    { Keyboard::X,              'X'          },
    { Keyboard::Y,              'Y'          },
    { Keyboard::Z,              'Z'          },
    { Keyboard::_0,             '0'          },
    { Keyboard::_1,             '1'          },
    { Keyboard::_2,             '2'          },
    { Keyboard::_3,             '3'          },
    { Keyboard::_4,             '4'          },
    { Keyboard::_5,             '5'          },
    { Keyboard::_6,             '6'          },
    { Keyboard::_7,             '7'          },
    { Keyboard::_8,             '8'          },
    { Keyboard::_9,             '9'          },
    { Keyboard::²,              VK_OEM_7     },
    };
    m_wndHwnd = GetActiveWindow();
}

void InputSystem::HandleInputs()
{
    
	for (int i = 0; i < Mouse::AMOUNT; i++)
	{
		int currKey = InputSystem::m_mouseMap[i];
        if (GetAsyncKeyState(currKey)) 
        {
            if (s_mouseStates[i] & PRESSED ) 
            {
                s_mouseStates[i] = PRESSED;
            }
            else 
            {
                s_mouseStates[i] = DOWN;
            }
        }
        else 
        {
            if (s_mouseStates[i] & PRESSED) 
            {
                s_mouseStates[i] = UP;
            }
            else 
            {
                s_mouseStates[i] = STATE_NONE;
            }
        }
	}
    for (int i = 0; i < Keyboard::AMOUNT; i++)
    {
        int currKey = m_keyboardMap[i];
        if (GetAsyncKeyState(currKey))
        {
            if (s_keyboardStates[i] & PRESSED)
            {
                s_keyboardStates[i] = PRESSED;
            }
            else
            {
                s_keyboardStates[i] = DOWN;
            }
        }
        else
        {
            if (s_keyboardStates[i] & PRESSED)
            {
                s_keyboardStates[i] = UP;
            }
            else
            {
                s_keyboardStates[i] = STATE_NONE;
            }
        }
    }

}

bool InputSystem::GetButton(Mouse::Button const _button)
{
    return InputsMethods::GetButton(_button);
}

bool InputSystem::GetButtonUp(Mouse::Button const _button)
{
    return InputsMethods::GetButtonUp(_button);
}

bool InputSystem::GetButtonDown(Mouse::Button const _button)
{
    return InputsMethods::GetButtonDown(_button);
}

bool InputSystem::GetKey(Keyboard::Key const _key)
{
    return InputsMethods::GetKey(_key);
}

bool InputSystem::GetKeyUp(Keyboard::Key const _key)
{
    return InputsMethods::GetKeyUp(_key);
}

bool InputSystem::GetKeyDown(Keyboard::Key const _key)
{
    return InputsMethods::GetKeyDown(_key);
}

void InputSystem::SetMousePosition(XMFLOAT2 _pos)
{
    InputsMethods::SetMousePosition(_pos);
}

POINT InputSystem::GetMousePosition()
{
    return InputsMethods::GetMousePosition();
}

void InputSystem::LockMouseCursor()
{
    InputsMethods::LockMouseCursor();
}

void InputSystem::UnlockMouseCursor()
{
    InputsMethods::UnlockMouseCursor();
}

void InputSystem::ShowMouseCursor()
{
    InputsMethods::ShowMouseCursor();
}

void InputSystem::HideMouseCursor()
{
    InputsMethods::HideMouseCursor();
}

