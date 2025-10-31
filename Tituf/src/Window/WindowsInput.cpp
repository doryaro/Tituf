#include "tfpch.h"
#include "WindowsInput.h"
#include "Tituf/Application.h"

namespace Tituf
{
	//Input* Input::s_Instance = new WindowsInput();
         
    static int MapToVirtualKey(int key)
    {
        switch (key)
        {
            // Letters
        case TF_KEY_A: return 'A';
        case TF_KEY_B: return 'B';
        case TF_KEY_C: return 'C';
        case TF_KEY_D: return 'D';
        case TF_KEY_E: return 'E';
        case TF_KEY_F: return 'F';
        case TF_KEY_G: return 'G';
        case TF_KEY_H: return 'H';
        case TF_KEY_I: return 'I';
        case TF_KEY_J: return 'J';
        case TF_KEY_K: return 'K';
        case TF_KEY_L: return 'L';
        case TF_KEY_M: return 'M';
        case TF_KEY_N: return 'N';
        case TF_KEY_O: return 'O';
        case TF_KEY_P: return 'P';
        case TF_KEY_Q: return 'Q';
        case TF_KEY_R: return 'R';
        case TF_KEY_S: return 'S';
        case TF_KEY_T: return 'T';
        case TF_KEY_U: return 'U';
        case TF_KEY_V: return 'V';
        case TF_KEY_W: return 'W';
        case TF_KEY_X: return 'X';
        case TF_KEY_Y: return 'Y';
        case TF_KEY_Z: return 'Z';

            // Numbers
        case TF_KEY_0: return '0';
        case TF_KEY_1: return '1';
        case TF_KEY_2: return '2';
        case TF_KEY_3: return '3';
        case TF_KEY_4: return '4';
        case TF_KEY_5: return '5';
        case TF_KEY_6: return '6';
        case TF_KEY_7: return '7';
        case TF_KEY_8: return '8';
        case TF_KEY_9: return '9';

            // Punctuation
        case TF_KEY_SPACE: return VK_SPACE;
        case TF_KEY_APOSTROPHE: return VK_OEM_7;   // '
        case TF_KEY_COMMA: return VK_OEM_COMMA;    // ,
        case TF_KEY_MINUS: return VK_OEM_MINUS;    // -
        case TF_KEY_PERIOD: return VK_OEM_PERIOD;  // .
        case TF_KEY_SLASH: return VK_OEM_2;        // /
        case TF_KEY_SEMICOLON: return VK_OEM_1;    // ;
        case TF_KEY_EQUAL: return VK_OEM_PLUS;     // =

            // Function / control keys
        case TF_KEY_ESCAPE: return VK_ESCAPE;
        case TF_KEY_ENTER: return VK_RETURN;
        case TF_KEY_TAB: return VK_TAB;
        case TF_KEY_BACKSPACE: return VK_BACK;
        case TF_KEY_INSERT: return VK_INSERT;
        case TF_KEY_DELETE: return VK_DELETE;
        case TF_KEY_RIGHT: return VK_RIGHT;
        case TF_KEY_LEFT: return VK_LEFT;
        case TF_KEY_DOWN: return VK_DOWN;
        case TF_KEY_UP: return VK_UP;
        case TF_KEY_PAGE_UP: return VK_PRIOR;
        case TF_KEY_PAGE_DOWN: return VK_NEXT;
        case TF_KEY_HOME: return VK_HOME;
        case TF_KEY_END: return VK_END;

            // Modifiers
        case TF_KEY_LEFT_SHIFT: return VK_LSHIFT;
        case TF_KEY_RIGHT_SHIFT: return VK_RSHIFT;
        case TF_KEY_LEFT_CONTROL: return VK_LCONTROL;
        case TF_KEY_RIGHT_CONTROL: return VK_RCONTROL;
        case TF_KEY_LEFT_ALT: return VK_LMENU;
        case TF_KEY_RIGHT_ALT: return VK_RMENU;
        case TF_KEY_LEFT_SUPER: return VK_LWIN;
        case TF_KEY_RIGHT_SUPER: return VK_RWIN;

        default: return 0;
        }
    }


    bool WindowsInput::IsKeyPressedImpl(int keycode)
    {
        int vk = MapToVirtualKey(keycode);
        if (vk == 0) return false;

        SHORT state = GetAsyncKeyState(vk);
        return (state & 0x8000) != 0;
    }
    static int MapMouseButton(int button)
    {
        switch (button)
        {
        case 0: return VK_LBUTTON;
        case 1: return VK_RBUTTON;
        case 2: return VK_MBUTTON;
        default: return 0;
        }
    }


    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        int vk = MapMouseButton(button);
        if (vk == 0) return false;

        SHORT state = GetAsyncKeyState(vk);
        return (state & 0x8000) != 0;
    }

    std::pair<float, float> WindowsInput::GetMousePositionImpl()
    {
        POINT point;
        GetCursorPos(&point);

        // Convert to window client coordinates
        HWND hwnd = TFWindow::Get().GetHwnd();
        ScreenToClient(hwnd, &point);

        return { static_cast<float>(point.x), static_cast<float>(point.y) };
    }

    float WindowsInput::GetMouseXImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return x;
    }

    float WindowsInput::GetMouseYImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }
}