#pragma once


#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>	
#include <functional>
#include <iostream>
#include <filesystem>  

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include <chrono>

#ifdef TF_PLATFORM_WINDOWS
	#include <Windows.h>
	#include "Window/TFWindow.h"
#endif


#include <GL/glew.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi