#pragma once
#include "Tituf/Layer.h"
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
		float m_Time = 0.0f;
		std::chrono::time_point<std::chrono::high_resolution_clock>	m_LastFrameTime;
	};
}
