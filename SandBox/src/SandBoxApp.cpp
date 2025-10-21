#pragma once
#include "tfpch.h"
#include <Tituf.h>


class SandBoxApp : public Tituf::Application
{
public:
	SandBoxApp() {};
	~SandBoxApp() {};   

};
 
Tituf::Application* Tituf::CreateApplication()
{
	return new SandBoxApp();
}	



