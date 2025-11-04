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
		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO(); 
		//(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable keyboard controls
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
		TFWindow& window = TFWindow::Get();
		window.InitGlewContext(); // create and make current the OpenGL context

		ImGui_ImplWin32_Init(window.GetHwnd()); // Pass your window	
		ImGui_ImplOpenGL3_Init("#version 330"); // or your GL version

	}

	void Tituf::ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}



}