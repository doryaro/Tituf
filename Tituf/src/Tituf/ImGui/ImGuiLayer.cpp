#include "tfpch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"   // <- add this
#include "backends/imgui_impl_opengl3.h" // <- add this


// Map your engine key codes to ImGuiKey
ImGuiKey MapKey(int keycode)
{
	switch (keycode)
	{
	case TF_KEY_TAB: return ImGuiKey_Tab;
	case TF_KEY_LEFT: return ImGuiKey_LeftArrow;
	case TF_KEY_RIGHT: return ImGuiKey_RightArrow;
	case TF_KEY_UP: return ImGuiKey_UpArrow;
	case TF_KEY_DOWN: return ImGuiKey_DownArrow;
	case TF_KEY_PAGE_UP: return ImGuiKey_PageUp;
	case TF_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
	case TF_KEY_HOME: return ImGuiKey_Home;
	case TF_KEY_END: return ImGuiKey_End;
	case TF_KEY_INSERT: return ImGuiKey_Insert;
	case TF_KEY_DELETE: return ImGuiKey_Delete;
	case TF_KEY_BACKSPACE: return ImGuiKey_Backspace;
	case TF_KEY_SPACE: return ImGuiKey_Space;
	case TF_KEY_ENTER: return ImGuiKey_Enter;
	case TF_KEY_ESCAPE: return ImGuiKey_Escape;
	case TF_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
	case TF_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
	case TF_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
	case TF_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
	case TF_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
	case TF_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
	case TF_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
	case TF_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
	case TF_KEY_A: return ImGuiKey_A;
	case TF_KEY_B: return ImGuiKey_B;
	case TF_KEY_C: return ImGuiKey_C;
	case TF_KEY_D: return ImGuiKey_D;
	case TF_KEY_E: return ImGuiKey_E;
	case TF_KEY_F: return ImGuiKey_F;
	case TF_KEY_G: return ImGuiKey_G;
	case TF_KEY_H: return ImGuiKey_H;
	case TF_KEY_I: return ImGuiKey_I;
	case TF_KEY_J: return ImGuiKey_J;
	case TF_KEY_K: return ImGuiKey_K;
	case TF_KEY_L: return ImGuiKey_L;
	case TF_KEY_M: return ImGuiKey_M;
	case TF_KEY_N: return ImGuiKey_N;
	case TF_KEY_O: return ImGuiKey_O;
	case TF_KEY_P: return ImGuiKey_P;
	case TF_KEY_Q: return ImGuiKey_Q;
	case TF_KEY_R: return ImGuiKey_R;
	case TF_KEY_S: return ImGuiKey_S;
	case TF_KEY_T: return ImGuiKey_T;
	case TF_KEY_U: return ImGuiKey_U;
	case TF_KEY_V: return ImGuiKey_V;
	case TF_KEY_W: return ImGuiKey_W;
	case TF_KEY_X: return ImGuiKey_X;
	case TF_KEY_Y: return ImGuiKey_Y;
	case TF_KEY_Z: return ImGuiKey_Z;
	default: return ImGuiKey_None;
	}
}


namespace Tituf
{


	Tituf::ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

		Tituf::ImGuiLayer::~ImGuiLayer()
	{
	}

	void Tituf::ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); 
		//(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable keyboard controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		TFWindow& window = TFWindow::Get();

		ImGui_ImplWin32_Init(window.GetHwnd()); // Pass your window	
		ImGui_ImplOpenGL3_Init("#version 330"); // or your GL version
	} 

	void Tituf::ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void Tituf::ImGuiLayer::Begin()
	{
		//TF_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}	
	void Tituf::ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		TFWindow& window = TFWindow::Get();

		io.DisplaySize = ImVec2((float)window.GetData().Width, (float)window.GetData().Height);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Only if you want multi-viewports:
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			HGLRC backup_GLRC = wglGetCurrentContext();
			HDC backup_HDC = wglGetCurrentDC();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			wglMakeCurrent(backup_HDC, backup_GLRC);
		}
	}

	// to change the docking you need to remove the file Sandbox/imgui.ini
	// or run the GenerateProjects agian
	void Tituf::ImGuiLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		windowFlags |= ImGuiWindowFlags_NoBackground; // add this

		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::SetNextWindowBgAlpha(0.3f);

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// --- Here is the DockSpace itself ---
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		// ------------------------------------

		// Example: show demo window
		static bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);

		ImGui::End();
	}

	void Tituf::ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(TF_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(TF_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(TF_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(TF_BIND_EVENT_FN(ImGuiLayer::MouseScrollEvent));
		dispatcher.Dispatch<KeyPressedEvent>(TF_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(TF_BIND_EVENT_FN(ImGuiLayer::ImGuiLayer::OnKeyReleaseEvent));
		dispatcher.Dispatch<KeyTypedEvent>(TF_BIND_EVENT_FN(ImGuiLayer::ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(TF_BIND_EVENT_FN(ImGuiLayer::OnWindowsResizedEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}
	bool ImGuiLayer::MouseScrollEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();
		return false;

	}
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey key = MapKey(e.GetKeyCode());
		if (key != ImGuiKey_None)
			io.AddKeyEvent(key, true);
		return false;
	}
	bool ImGuiLayer::OnKeyReleaseEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey key = MapKey(e.GetKeyCode());
		if (key != ImGuiKey_None)
			io.AddKeyEvent(key, false);
		return false;

	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter((ImWchar)e.GetKeyCode());

		return false;
	}
	bool ImGuiLayer::OnWindowsResizedEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)e.GetWidth(), (float)e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;

	}

}