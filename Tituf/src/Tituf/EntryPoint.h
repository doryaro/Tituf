#pragma once
#ifdef TF_PLATFORM_WINDOWS
   
extern Tituf::Application* Tituf::CreateApplication();


int main(int argc, char** argv)
{ 
	Tituf::Log::Init();	
	TF_CORE_INFO("Tituf Engine Initialized!");
	TF_CORE_INFO("Hello from App!");
	Tituf::Application* app = Tituf::CreateApplication();
	app->Run();
	delete app;
	return 0;     
}	


#endif // TF_PLATFORM_WINDOWS

 