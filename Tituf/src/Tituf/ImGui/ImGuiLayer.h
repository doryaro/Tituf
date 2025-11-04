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
		virtual void OnImGuiRender() override {};
	
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
		std::chrono::time_point<std::chrono::high_resolution_clock>	m_LastFrameTime;
	};
}
