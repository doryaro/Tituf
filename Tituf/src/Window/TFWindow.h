#pragma once
#include "Tituf/Core.h"
#include "Tituf/Log.h"
#include "Tituf/Event/ApplicationEvent.h"
#include "Tituf/Event/KeyEvent.h"
#include "Tituf/Event/MouseEvent.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class TFWindow
{
	public: 
	TFWindow();
	TFWindow(const TFWindow& other) = delete;
	TFWindow& operator=(const TFWindow& other) = delete;
	~TFWindow();
	  
	bool ProcessMessages();
	   
	void SetPixels(HDC& hdc);

	void InitGlewContext();

	void SwapBuffers();
	  
	using EventAppCallbackFn = std::function<void(Tituf::Event&)>;
	using EventKeyCallbackFn = std::function<void(Tituf::Event&)>;
	using EventMouseCallbackFn = std::function<void(Tituf::Event&)>;

	void SetAppEventCallback(const EventAppCallbackFn& callback);
	void SetKeyEventCallback(const EventKeyCallbackFn& callback);
	void SetMouseEventCallback(const EventMouseCallbackFn& callback);


private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	HDC m_hdc = nullptr;   
	HGLRC m_hGLRC = nullptr;
	struct WindowData
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		EventAppCallbackFn EventAppCallback;
		EventMouseCallbackFn EventMouseCallback;
		EventKeyCallbackFn EventKeyCallback;
	};
	WindowData m_Data;
public:
	inline WindowData& GetData() {return m_Data;}
	HDC GetHdc() const { return m_hdc; }
};
  
