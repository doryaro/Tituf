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
		virtual ~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate() override ;	
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool MouseScrollEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleaseEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowsResizedEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;
		std::chrono::time_point<std::chrono::high_resolution_clock>	m_LastFrameTime;
	};
}
