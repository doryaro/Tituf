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
		WindowResizeEvent e(1280, 720);
		TF_TRACE("12341");
		TF_TRACE(e.ToString());
		 
		while (true);
	}

}