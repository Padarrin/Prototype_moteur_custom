#pragma once

#include <windows.h>
#include <functional>

class Render_Window
{
private :
	WNDCLASSEXA m_wndEx = {};
	HWND m_hWnd;
	LPRECT m_clientRect;
	int m_wndWidth;
	int m_wndHeight;

	
	

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:

	Render_Window();
	~Render_Window();
	 
	void Init(HINSTANCE hInstance);

	bool Create(int _width, int _height);
	bool Update();
	void Show();
	void Quit();

	HWND GetHwnd();
	int GetWidth();
	int GetHeight();

	using ResizeCallback = std::function<void(int, int)>;
	void SetResizeCallback(ResizeCallback callback);

private:
	ResizeCallback m_resizeCallback;
};

