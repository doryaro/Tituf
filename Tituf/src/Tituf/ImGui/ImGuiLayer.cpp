#include "tfpch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"   // <- add this
#include "backends/imgui_impl_opengl3.h" // <- add this



namespace Tituf
{


	Tituf::ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(TFWindow::Get().GetData().Width, TFWindow::Get().GetData().Height);
		 


		//auto now = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration<float>(now.time_since_epoch()).count();
		//io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		//m_Time = time;
		std::chrono::time_point<std::chrono::high_resolution_clock>
			now = std::chrono::high_resolution_clock::now();
		if (m_LastFrameTime.time_since_epoch().count() != 0) {
			io.DeltaTime = std::chrono::duration<float>(now - m_LastFrameTime).count();
		}
		else {
			io.DeltaTime = 1.0f / 60.0f; // default for first frame
		}
		m_LastFrameTime = now;


		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	} 

	Tituf::ImGuiLayer::~ImGuiLayer()
	{
	}

	void Tituf::ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO(); 
		//(void)io;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
		TFWindow& window = TFWindow::Get();
		window.InitGlewContext(); // create and make current the OpenGL context

		ImGui_ImplWin32_Init(window.GetHwnd()); // Pass your window	
		ImGui_ImplOpenGL3_Init("#version 330"); // or your GL version

	}

	void Tituf::ImGuiLayer::OnDetach()
	{
	}

	void Tituf::ImGuiLayer::OnEvent(Event& event)
	{
	}


}