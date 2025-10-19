#pragma once
#ifdef TF_PLATFORM_WINDOWS
   
extern Tituf::Application* Tituf::CreateApplication();


int main(int argc, char** argv)
{ 
	Tituf::Application* app = Tituf::CreateApplication();
	app->Run();
	delete app;
	return 0;
}	




#endif // TF_PLATFORM_WINDOWS

 