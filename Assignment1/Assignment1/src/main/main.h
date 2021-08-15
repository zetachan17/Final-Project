#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <cmath>
#include <vector> //Array list as oppose to array

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./shader/Shader.h"
#include "./camera/Camera.h"
#include "./texture/Texture.h"
#include"./Lights/PointLightSource.h"

#define STB_IMAGE_IMPLEMENTATION
#include <./stb_image.h>