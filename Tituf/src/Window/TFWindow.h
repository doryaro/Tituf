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
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
};
  
