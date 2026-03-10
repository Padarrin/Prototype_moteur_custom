#pragma once
#include "pch.h"
class InputsMethods
{

public :

	static bool GetButton(Mouse::Button const _button);
	static bool GetButtonUp(Mouse::Button const _button);
	static bool GetButtonDown(Mouse::Button const _button);

	static bool GetKey(Keyboard::Key const _key);
	static bool GetKeyUp(Keyboard::Key const _key);
	static bool GetKeyDown(Keyboard::Key const _key);

	static void SetMousePosition(XMFLOAT2 _pos);
	static POINT GetMousePosition();
	
	static void LockMouseCursor();
	static void UnlockMouseCursor();
	static void ShowMouseCursor();
	static void HideMouseCursor();


};

