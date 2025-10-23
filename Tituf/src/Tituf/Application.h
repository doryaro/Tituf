#pragma once
#include "Core.h"
#include "Event/Event.h"

namespace Tituf	
{

	class TITUF_API Application
	{
		public:
			Application();
			virtual ~Application();
		
			void Run();

			void OnAppEvent(Event& e);
			bool OnWindowResize(WindowResizeEvent& e);
			bool OnWindowClose(WindowCloseEvent& e);

			void OnKeyEvent(Event& e);
			bool OnKeyPressEvent(Event& e);
			bool OnKeyRepeatEvent(Event& e);
			bool OnKeyReleasedEvent(Event& e);

			void OnMouseEvent(Event& e);
			bool OnMouseMovedEvent(Event& e);
			bool OnMouseButtonPressedEvent(Event& e);
			bool OnMouseButtonReleasedEvent(Event& e);

		private:
			std::unique_ptr<TFWindow> m_Window;
			bool running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}

 
