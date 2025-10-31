#include "tfpch.h"
#include "Input.h"
#include "Window/WindowsInput.h"
namespace Tituf
{
    Input* Input::s_Instance = new WindowsInput();
}
