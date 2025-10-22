#include "tfpch.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"

namespace Tituf
{
	Application::Application()
	{ 
	}
	Application::~Application()
	{ 
	}
	void Application::Run()
	{
		TFWindow* pWindow = new TFWindow();
		bool running = true;

		while (true)
		{
			if (!pWindow->ProcessMessages())
			{
				TF_TRACE("Window closed");
				break;
			}

			Sleep(16);
			//Render
		}
		WindowResizeEvent e(1280, 720);
		TF_TRACE("12341");
		TF_TRACE(e.ToString());
		TF_ASSERT(1 == 1);
		 
		while (true);
	}

}