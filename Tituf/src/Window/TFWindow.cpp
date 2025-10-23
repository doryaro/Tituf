#include "tfpch.h"
#include "TFWindow.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    TFWindow* window = nullptr;

    // Retrieve pointer to our TFWindow instance
    if (message == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<TFWindow*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
    }
    else
    {
        window = reinterpret_cast<TFWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

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
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        if (window)
        {
            // Dispatch a resize event to the engine/application
            Tituf::WindowResizeEvent event(width, height);
            auto& data = window->GetData();
            if (data.EventAppCallback)
                data.EventAppCallback(event);
        }
        return 0;
    }
    case WM_KEYDOWN:
    {
        int key = static_cast<int>(wParam);
        bool repeat = (lParam & 0x40000000) != 0; // check bit 30 for repeat
        int repeatCount = lParam & 0xFFFF;

        if (window)
        {
            if (repeat)
            {
				Tituf::KeyRepeatEvent event(key, repeatCount);  //only print 1 repeats
                auto& data = window->GetData();
                if (data.EventKeyCallback)
                    data.EventKeyCallback(event);
            }
            else
            {
                Tituf::KeyPressedEvent event(key, 0);
                auto& data = window->GetData();
                if (data.EventKeyCallback)
                    data.EventKeyCallback(event);
            }
        }
        return 0;
    }

    case WM_KEYUP:
    {
        int key = static_cast<int>(wParam);
        if (window)
        {
            Tituf::KeyReleasedEvent event(key);
            auto& data = window->GetData();
            if (data.EventKeyCallback)
                data.EventKeyCallback(event);
        }
        return 0;
	}  
    case WM_MOUSEMOVE:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        if (window)
        {
            Tituf::MouseMovedEvent event((float)x, (float)y);
            auto& data = window->GetData();
            if (data.EventMouseCallback)
                data.EventMouseCallback(event);
        }
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        int button = 0; // Left button
        if (window)
        {
            Tituf::MouseButtonPressedEvent event(button);
            auto& data = window->GetData();
            if (data.EventMouseCallback)
                data.EventMouseCallback(event);
        }
        return 0;
    }

    case WM_LBUTTONUP:
    {
        int button = 0; // Left button
        if (window)
        {
            Tituf::MouseButtonReleasedEvent event(button);
            auto& data = window->GetData();
            if (data.EventMouseCallback)
                data.EventMouseCallback(event);
        }
        return 0;
    }

    //case WM_RBUTTONDOWN:
    //{
    //    int button = 1; // Right button
    //    if (window)
    //    {
    //        Tituf::MouseButtonPressedEvent event(button);
    //        auto& data = window->GetData();
    //        if (data.EventMouseCallback)
    //            data.EventMouseCallback(event);
    //    }
    //    return 0;
    //}

    //case WM_RBUTTONUP:
    //{
    //    int button = 1; // Right button
    //    if (window)
    //    {
    //        Tituf::MouseButtonReleasedEvent event(button);
    //        auto& data = window->GetData();
    //        if (data.EventMouseCallback)
    //            data.EventMouseCallback(event);
    //    }
    //    return 0;
    //}


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
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
 
void TFWindow::SetAppEventCallback(const EventAppCallbackFn& callback)
{
	m_Data.EventAppCallback = callback;
}

void TFWindow::SetKeyEventCallback(const EventKeyCallbackFn& callback)
{
    m_Data.EventKeyCallback = callback;
}

void TFWindow::SetMouseEventCallback(const EventMouseCallbackFn& callback)
{
    m_Data.EventMouseCallback = callback;
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

    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

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
		this
	);
	ShowWindow(m_hWnd, SW_SHOW);

}
TFWindow::~TFWindow()
{
	const wchar_t CLASS_NAME[] = L"TitufWindowClass";	
	UnregisterClass(CLASS_NAME, m_hInstance);
}


