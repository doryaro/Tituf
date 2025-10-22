#include "tfpch.h"
#include "TFWindow.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TF_TRACE("Window message: {0}", message);
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;	
	case WM_SIZE:
	{
		//int width = LOWORD(lParam);
		//int height = HIWORD(lParam);
		//TF_TRACE("Window resized: {0}x{1}", width, height);
		//Tituf::WindowResizeEvent event(width, height);
		//event.m_Handled = true;
		//return 0;
	}
	default:     
		//TF_ASSERT(1==1);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return LRESULT();
}

bool TFWindow::ProcessMessages()
{
	MSG msg = {};
	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return true;
}


TFWindow::TFWindow()
	: m_hInstance(GetModuleHandle(nullptr)), m_hWnd(nullptr)
{
	const wchar_t CLASS_NAME[] = L"TitufWindowClass";
	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;	
	wndClass.hInstance = m_hInstance;	
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	int width = 640;
	int height = 480;

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
	AdjustWindowRect(&rect, style, FALSE);

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Tituf Engine",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);
	ShowWindow(m_hWnd, SW_SHOW);

}
TFWindow::~TFWindow()
{
	const wchar_t CLASS_NAME[] = L"TitufWindowClass";	
	UnregisterClass(CLASS_NAME, m_hInstance);
}


