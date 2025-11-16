#pragma once
#include "Tituf/Core.h"
#include "Tituf/Log.h"
#include "Tituf/Event/ApplicationEvent.h"
#include "Tituf/Event/KeyEvent.h"
#include "Tituf/Event/MouseEvent.h"
#include <Config.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class TFWindow
{
public:
    // Get the single instance   
    static TFWindow& Get()
    {
        static TFWindow instance; // created once, guaranteed to be destroyed
        return instance;
    }

    void Init();
    HWND CreateAdditionalWindow(const wchar_t* title, int width, int height);
    // Delete copy and move constructors
    TFWindow(const TFWindow& other) = delete;
    TFWindow& operator=(const TFWindow& other) = delete;
    TFWindow(TFWindow&& other) = delete;
    TFWindow& operator=(TFWindow&& other) = delete;

    ~TFWindow();

    bool ProcessMessages();
    void SetPixels();
    void InitImguiContext();
    void GlMakeCurrentImgui();
    void GlMakeCurrent();
    void InitGlewContext();
    void SwapBuffers();
    void SwapBuffersImgui();


    using EventAppCallbackFn = std::function<void(Tituf::Event&)>;
    using EventKeyCallbackFn = std::function<void(Tituf::Event&)>;
    using EventMouseCallbackFn = std::function<void(Tituf::Event&)>;

    void SetAppEventCallback(const EventAppCallbackFn& callback);
    void SetKeyEventCallback(const EventKeyCallbackFn& callback);
    void SetMouseEventCallback(const EventMouseCallbackFn& callback);


    HDC GetHdc() const { return m_hdc; }
    HWND GetHwnd() const { return m_hWnd; } // for Window
    HWND GetImGuiHwnd() const { return m_hImGuiWnd; } // for ImGui

	//virtual void* GetNativeWindow() const { return m_hWnd; }   for cross-platform compatibility 
     
private:
    TFWindow(); // private constructor

    HINSTANCE m_hInstance;
    
    //main window
    HWND m_hWnd;
    HDC m_hdc = nullptr;
    HGLRC m_hGLRC = nullptr;
    //imgui window
    HWND m_hImGuiWnd = nullptr;
    HDC m_hImGuiDC = nullptr;
    HGLRC m_hImGuiRC = nullptr;

    Tituf::Config m_Config;
    struct WindowData
    {
        std::wstring Title;
        unsigned int Width = 0;
        unsigned int Height = 0;
        EventAppCallbackFn EventAppCallback;
        EventMouseCallbackFn EventMouseCallback;
        EventKeyCallbackFn EventKeyCallback;
    };   

    WindowData m_Data;
public:
    inline WindowData& GetData() { return m_Data; }
};
