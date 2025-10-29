#pragma once

#include "Tituf/Core.h"
#include "Tituf/Event/Event.h"
#include "../Window/TFWindow.h"

namespace Tituf
{
	class TITUF_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
	
}
