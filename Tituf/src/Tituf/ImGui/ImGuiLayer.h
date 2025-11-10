#pragma once
#include "Tituf/Layer.h"
#include "Tituf/Event/KeyEvent.h"
#include "Tituf/Event/MouseEvent.h"
#include "Tituf/Event/ApplicationEvent.h"
#include "Tituf/Core.h"
namespace Tituf
{

	class TITUF_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		void InitImguiWindowData();
		virtual ~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override ;
		virtual void OnEvent(Event& event) override;

		 
		void Begin();
		void End();
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool MouseScrollEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleaseEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowsResizedEvent(WindowResizeEvent& e);
		float m_Time = 0.0f;
		std::chrono::time_point<std::chrono::high_resolution_clock>	m_LastFrameTime;
		Tituf::Config m_Config;

		struct ImguiWindowData
			{
				std::wstring Title;
				unsigned int Width;
				unsigned int Height;
			};
		ImguiWindowData m_ImguiWindowData;
	};
}
