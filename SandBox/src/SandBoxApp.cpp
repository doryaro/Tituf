#pragma once
#include "tfpch.h"
#include <Tituf.h>


class ExampleLayer : public Tituf::Layer
{
	public:
	ExampleLayer() : Layer("ExampleLayer") {}
	virtual ~ExampleLayer() {}
	virtual void OnAttach() override
	{   
		TF_INFO("ExampleLayer attached");
	} 
	virtual void OnDetach() override
	{
		TF_INFO("ExampleLayer detached");
	}
	virtual void OnUpdate(float deltaTime) override
	{
		//TF_INFO("ExampleLayer updated: {0} seconds elapsed", deltaTime);
		// Update logic here
	}
	virtual void OnImGuiRender() override
	{
		// ImGui rendering here
	}
	virtual void OnEvent(Tituf::Event& event) override
	{
		//TF_TRACE("ExampleLayer received event: {0}", event.ToString());
		// Event handling here
	}
};                                           

class SandBoxApp : public Tituf::Application
{
public:
	SandBoxApp() 
	{
		PushLayer(new ExampleLayer());
	};
	~SandBoxApp() {};   

};
 
Tituf::Application* Tituf::CreateApplication()
{
	return new SandBoxApp();
}	



