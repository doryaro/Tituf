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

	};

	// To be defined in CLIENT
	Application* CreateApplication();

}

 
