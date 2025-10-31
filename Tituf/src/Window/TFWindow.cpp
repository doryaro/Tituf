#include "tfpch.h"
#include "TFWindow.h"


void TFWindow::SetPixels(HDC& m_hdc)
{
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(m_hdc, &pfd);
    SetPixelFormat(m_hdc, pf, &pfd);
}

void TFWindow::InitGlewContext()
{
    m_hdc = GetDC(m_hWnd);
    SetPixels(m_hdc);

    m_hGLRC = wglCreateContext(m_hdc);
    wglMakeCurrent(m_hdc, m_hGLRC);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers();
}

void TFWindow::SwapBuffers()
{
    ::SwapBuffers(m_hdc);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    TFWindow& window = TFWindow::Get(); // access singleton

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
        Tituf::WindowResizeEvent event(width, height);
        if (window.GetData().EventAppCallback)
            window.GetData().EventAppCallback(event);
        return 0;
    }
    case WM_CHAR:
    {
        unsigned int character = static_cast<unsigned int>(wParam);
        Tituf::KeyTypedEvent event(character, 0);
        if (window.GetData().EventKeyCallback)
            window.GetData().EventKeyCallback(event);
        return 0;
    }
    case WM_KEYDOWN:
    {
        int key = static_cast<int>(wParam);
        bool repeat = (lParam & 0x40000000) != 0;
        int repeatCount = lParam & 0xFFFF;

        if (repeat)
        {
            Tituf::KeyRepeatEvent event(key, repeatCount);
            if (window.GetData().EventKeyCallback)
                window.GetData().EventKeyCallback(event);
        }
        else
        {
            Tituf::KeyPressedEvent event(key, 0);
            if (window.GetData().EventKeyCallback)
                window.GetData().EventKeyCallback(event);
        }
        return 0;
    }
    case WM_KEYUP:
    {
        int key = static_cast<int>(wParam);
        Tituf::KeyReleasedEvent event(key);
        if (window.GetData().EventKeyCallback)
            window.GetData().EventKeyCallback(event);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        Tituf::MouseMovedEvent event((float)x, (float)y);
        if (window.GetData().EventMouseCallback)
            window.GetData().EventMouseCallback(event);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        Tituf::MouseButtonPressedEvent event(0);
        if (window.GetData().EventMouseCallback)
            window.GetData().EventMouseCallback(event);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        Tituf::MouseButtonReleasedEvent event(0);
        if (window.GetData().EventMouseCallback)
            window.GetData().EventMouseCallback(event);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

bool TFWindow::ProcessMessages()
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}
// Event callbacks remain unchanged
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

void TFWindow::Init()
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



    if (!m_Config.Load(CONFIG_PATH))
    {
        TF_CORE_ERROR("Failed to load config.ini!");
    }
	m_Data.Width = m_Config.GetInt("Window","Width",-1);
	m_Data.Height = m_Config.GetInt("Window", "Height",-1);

    RECT rect;
    rect.left = m_Config.GetInt("Rect","left",-1);
    rect.top = m_Config.GetInt("Rect", "top", -1);
    rect.right = rect.left + m_Data.Width;  // width from Window section
    rect.bottom = rect.top + m_Data.Height; // height from Window section
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
        nullptr // no longer pass 'this'
    );

    ShowWindow(m_hWnd, SW_SHOW);
}

TFWindow::TFWindow()
    : m_hInstance(GetModuleHandle(nullptr)), m_hWnd(nullptr)
{
}

TFWindow::~TFWindow()
{
    const wchar_t CLASS_NAME[] = L"TitufWindowClass";
    UnregisterClass(CLASS_NAME, m_hInstance);
}
