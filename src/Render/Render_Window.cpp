#include "Render_Window.h"

LRESULT CALLBACK Render_Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Render_Window* pThis = nullptr;
    if (message == WM_NCCREATE)
    {
        CREATESTRUCTA* pCreate = reinterpret_cast<CREATESTRUCTA*>(lParam);
        pThis = reinterpret_cast<Render_Window*>(pCreate->lpCreateParams);
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<Render_Window*>(GetWindowLongPtrA(hWnd, GWLP_USERDATA));
    }
    switch (message)
    {
    case WM_SIZE:
        if (!pThis) return 0;

        pThis->m_wndWidth = LOWORD(lParam);  
        pThis->m_wndHeight = HIWORD(lParam);

        if (pThis->m_resizeCallback)
        {
            pThis->m_resizeCallback(pThis->m_wndWidth, pThis->m_wndHeight);
        }
		return 0;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
}

Render_Window::Render_Window()
{
}

Render_Window::~Render_Window()
{
    if (m_clientRect)
    {
        delete m_clientRect;
        m_clientRect = nullptr;
	}
}

void Render_Window::Init(HINSTANCE hInstance)
{
    const CHAR* name = "App";

    m_wndEx.cbSize = sizeof(WNDCLASSEXA);
    m_wndEx.style = CS_HREDRAW | CS_VREDRAW;
    m_wndEx.lpfnWndProc = WindowProc;
    m_wndEx.hInstance = hInstance;
    m_wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    m_wndEx.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    m_wndEx.lpszClassName = name;

    RegisterClassExA(&m_wndEx);

    Create(1280, 720);
}

bool Render_Window::Create(int _width, int _height)
{
    if (m_hWnd)
    {
        return true;
    }
    
    m_wndWidth = _width;
	m_wndHeight = _height;
	m_clientRect = new RECT{ 0, 0, m_wndWidth, m_wndHeight };

    m_hWnd = CreateWindowA(m_wndEx.lpszClassName, m_wndEx.lpszClassName
        , WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_wndWidth, m_wndHeight
        , NULL, NULL, m_wndEx.hInstance, this);
    
    if (!m_hWnd)
    {
        return false;
    }

	ShowWindow(m_hWnd, SW_SHOW);
    return true;
}

bool Render_Window::Update()
{
    if (m_hWnd == nullptr)
        return false;

    MSG msg = {};

    if (msg.message == WM_QUIT)
    {
        return false;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);
        
    return true;
}

void Render_Window::Show()
{
    ShowWindow(m_hWnd, SW_SHOW);
}

void Render_Window::Quit()
{
    if (m_hWnd)
        PostMessage(m_hWnd, WM_CLOSE, 0, 0);
}

HWND Render_Window::GetHwnd()
{
	return m_hWnd;
}

int Render_Window::GetWidth()
{
	return m_wndWidth;
}

int Render_Window::GetHeight()
{
	return m_wndHeight;
}

void Render_Window::SetResizeCallback(ResizeCallback callback)
{
    m_resizeCallback = callback;
}
