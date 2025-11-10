#include "tfpch.h"
#include "TFWindow.h"

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
    m_Data.Width = m_Config.GetInt("Window", "Width", -1);
    m_Data.Height = m_Config.GetInt("Window", "Height", -1);
    m_Data.Title = m_Config.GetWString("Window", "Title", L"Null");


    RECT rect;
    rect.left = m_Config.GetInt("Rect", "left", -1);
    rect.top = m_Config.GetInt("Rect", "top", -1);
    rect.right = rect.left + m_Data.Width;  // width from Window section
    rect.bottom = rect.top + m_Data.Height; // height from Window section
    AdjustWindowRect(&rect, style, FALSE);

    m_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        m_Data.Title.c_str(),
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

void TFWindow::SetPixels()
{
    if (!m_hWnd)
    {
        TF_CORE_ERROR("Window handle is null!");
        return;
    }

    m_hdc = GetDC(m_hWnd);  // get HDC from the created window
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(m_hdc, &pfd);
    if (!SetPixelFormat(m_hdc, pf, &pfd))
    {
        TF_CORE_ERROR("Failed toSetPixelFormat");
    }
}

void TFWindow::InitImguiContext()
{
    // -----------------------
    // Create ImGui separate window
    // ----------------------- 
    std::wstring Title = m_Config.GetWString("ImGuiWindow", "Title", L"Null");
    m_hImGuiWnd = CreateAdditionalWindow(Title.c_str(), 800, 600);
    m_hImGuiDC = GetDC(m_hImGuiWnd);

    // Set pixel format for ImGui window
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int format = ChoosePixelFormat(m_hImGuiDC, &pfd);
    SetPixelFormat(m_hImGuiDC, format, &pfd);

    // Create OpenGL context for ImGui window
    m_hImGuiRC = wglCreateContext(m_hImGuiDC);
    if (!m_hImGuiRC)
        TF_CORE_ERROR("Failed to create ImGui OpenGL context");

    // Share resources with main context
    if (!wglShareLists(m_hGLRC, m_hImGuiRC))
        TF_CORE_ERROR("wglShareLists failed!"); 
}

void TFWindow::GlMakeCurrentImgui()
{
    if (!wglMakeCurrent(m_hImGuiDC, m_hImGuiRC))
    {
        TF_CORE_ERROR("GlMakeCurrentImgui failed");
    }
} 
void TFWindow::GlMakeCurrent()
{
    if (!wglMakeCurrent(m_hdc, m_hGLRC))
    {
        TF_CORE_ERROR("GlMakeCurrent failed");
    }
}


void TFWindow::InitGlewContext()
{
    SetPixels();

    m_hGLRC = wglCreateContext(m_hdc);
    if (!m_hGLRC)
    { 
        TF_CORE_ERROR("Failed to create OpenGL context");
    }  
     
    if (!wglMakeCurrent(m_hdc, m_hGLRC)) 
    {
        TF_CORE_ERROR("wglMakeCurrent failed");
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    glViewport(0, 0, m_Data.Width, m_Data.Height);

    if (err != GLEW_OK)
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;

    TF_TRACE("OpenGl Info");
    std::cout << "   Vendor:" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "   Renderer:" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "   Version:" << glGetString(GL_VERSION) << std::endl;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers();
}

void TFWindow::SwapBuffers()
{
    ::SwapBuffers(m_hdc);
}
void TFWindow::SwapBuffersImgui()
{
    ::SwapBuffers(m_hImGuiDC);
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



HWND TFWindow::CreateAdditionalWindow(const wchar_t* title, int width, int height)
{
    const wchar_t CLASS_NAME[] = L"TitufImGuiWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL, m_hInstance, NULL
    );

    ShowWindow(hWnd, SW_SHOW);
    return hWnd;
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
