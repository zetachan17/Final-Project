// COMP 371
// Final Project finished by Team 10
// 2021-08-08 Copyright by A+ Squad
// We used https://learnopengl.com/ and labs as our code source.


#include "main.h"

#pragma comment(lib, "./irrKlang.lib")

//using namespace std;
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice();

// Handle keyboard input
void processInput(GLFWwindow* window);

//Window dimensions
const GLint WIDTH = 1024, HEIGHT = 768;
//Rotations use radians (convert degrees to radians)
const float toRadians = 3.14159265 / 180.0f;
const int numGridLines = 100;

// camera
Camera camera = Camera(glm::vec3(-6.0f, 4.0f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -40.0f, -20.0f);
Camera camera2 = Camera(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, -90.0f);
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//Create Light
glm::vec3 pointLightPosition(0.0f, 10.0f, 0.0f);

// light declarations	
PointLightSource pointLightSource = PointLightSource(
	pointLightPosition,
	glm::vec3(glm::radians(30.0f), 0, 0),
	glm::vec3(1.0f, 1.0f, 1.0f) //colour of light
);

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

float ambientStrength = 0.3f;
int second = 0;
int score = 0;

// Texture enumerator
enum textures {
	GreenMetal = 0, BlueMetal = 1, Gold = 2,
	RoseGold = 3, TealMetal = 4, Brick = 5, Tile = 6, Sky = 7
};

#pragma region Function declares

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void passThrough();
bool check();
void createCube1(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type);
void createCube5(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type);
void createCube3(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type);
void createCube4(float rootAx, float rootAy, float rootAz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type);
void createCube2(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type);
void RenderText(Shader& shader, std::string text, float i, float x, float y, float scale, glm::vec3 color);
void move();
bool isinwall();
#pragma endregion

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
int modelRendered = 0;

GLenum type = GL_TRIANGLES;

std::vector<Shader> shaderList;
std::vector<Texture> textureList;

// Vertex Shader Path
static const char* vertShader = "Assignment1/src/glsl/shader_vert.glsl";

// Fragment Shader Path
static const char* fragShader = "Assignment1/src/glsl/shader_frag.glsl";

//vertex shader for light source
static const char* LightVertShader = "Assignment1/src/glsl/LightVertShader.glsl";
static const char* LightFragShader = "Assignment1/src/glsl/LightFragShader.glsl";

static const char* DepthVertShader = "Assignment1/src/glsl/depthVertexShader.glsl";
static const char* DepthFragShader = "Assignment1/src/glsl/depthFragmentShader.glsl";

// shaders for text rendering
static const char* TextVertShader = "Assignment1/src/glsl/textVertShader.glsl";
static const char* TextFragShader = "Assignment1/src/glsl/textFragShader.glsl";

// shaders for model import
static const char* ModelVertShader = "Assignment1/src/glsl/modelVertShader.glsl";
static const char* ModelFragShader = "Assignment1/src/glsl/modelFragShader.glsl";


//Working with Uniform variables
//Starts at 0.0, increments by 0.0005 intil it reaches 0.7 and then goes opposite direction
bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;

#pragma region Roots

float rootx = 0.0f;
float rooty = 0.0f;
float rootz = 5.0f;
float anglex = 0.0f;
float anglez = 0.0f;
float angley = 0.0f;

float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

bool isMovingBackward = false;
bool isMovingForward = false;

#pragma endregion

float movingSpeed = 0.01f;

bool isShadow = true;

bool textureEn = true;
GLenum paulType = GL_TRIANGLES;
GLenum alecType = GL_TRIANGLES;
GLenum runzeType = GL_TRIANGLES;
GLenum juntingType = GL_TRIANGLES;
GLenum jennaType = GL_TRIANGLES;

float paulScale = 1.0f;
float jungtingScale = 1.0f;
float jennaScale = 1.0f;
float runzeScale = 1.0f;
float alecScale = 1.0f;

enum modelSelected { PaulModel, RunzeModel, JennaModel, JungtingModel, AlecModel };
enum cameraSelected {mainCamera, menuCamera};
enum rotateDirection { UPDIR, DOWNDIR, LEFTDIR, RIGHTDIR, NONE };

modelSelected currentModel = JungtingModel;
cameraSelected currentCamera = menuCamera;

float modelAngle = 0;
float MODANGLE = 90;
float isRotating = false;
rotateDirection currentDirection = NONE;

float cubeScale = 0.5f;
//Cube Scales
glm::vec3 xLongScale = glm::vec3(4.0f, 1.0f, 1.0f);
glm::vec3 xShortScale = glm::vec3(2.0f, 1.0f, 1.0f);
glm::vec3 yLongScale = glm::vec3(1.0f, 4.0f, 1.0f);
glm::vec3 yShortScale = glm::vec3(1.0f, 2.0f, 1.0f);
glm::vec3 zLongScale = glm::vec3(1.0f, 1.0f, 4.0f);
glm::vec3 zShortScale = glm::vec3(1.0f, 1.0f, 2.0f);
glm::vec3 BigeCube = glm::vec3(2.0f, 2.0f, 2.0f);
glm::vec3 ThiccCube = glm::vec3(4.0f, 4.0f, 4.0f);

glm::mat4 identMatrix = glm::mat4(1.0f);

GLuint vao[5];
GLuint vbo[5];

int currentShader = 0;


void CreateObjects()
{
	glGenVertexArrays(5, vao);
	glGenBuffers(5, vbo);

	//Start by Initialising the vertex arrays for red, green and blue axes lines.
	// The length of each axis line is 5 grid units.
	const float axisLineLength = 5 * (10.0f / (float)numGridLines);

	// X-axis: Red line
	glm::vec3 redLine[] = {
		// Point Vec3 X, Y, Z    	// Color Vec3 R, G, B
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(axisLineLength, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
	};

	// Y-axis: Green line
	glm::vec3 greenLine[] = {
		// Point Vec3 X, Y, Z    	// Color Vec3 R, G, B
		glm::vec3(0.0f,  0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, axisLineLength, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
	};

	// Z-axis: Blue line
	glm::vec3 blueLine[] = {
		// Point Vec3 X, Y, Z    	// Color Vec3 R, G, B
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, axisLineLength), glm::vec3(0.0f, 0.0f,1.0f),
	};

	// set up vertex data (and buffer(s)) and configure vertex attributes
	 // ------------------------------------------------------------------
	glm::vec3 vertices[] = {
		glm::vec3(-0.1f, -0.1f, -0.1f),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.1f, -0.1f, -0.1f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, -1.0f),
		 glm::vec3(0.1f,  0.1f, -0.1f),  glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, -1.0f),
		 glm::vec3(0.1f,  0.1f, -0.1f),  glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, -1.0f),
	   glm::vec3(-0.1f,  0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(-0.1f, -0.1f, -0.1f),glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, -1.0f),

		glm::vec3(-0.1f, -0.1f,  0.1f),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, 1.0f),
		glm::vec3(0.1f, -0.1f,  0.1f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, 1.0f),
	   glm::vec3(0.1f,  0.1f,  0.1f),  glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, 1.0f),
	   glm::vec3(0.1f,  0.1f,  0.1f),  glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, 1.0f),
	  glm::vec3(-0.1f,  0.1f,  0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, 1.0f),
	  glm::vec3(-0.1f, -0.1f,  0.1f),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  0.0f, 1.0f),

	   glm::vec3(-0.1f,  0.1f,  0.1f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(0.760, 0.141, 0.141), glm::vec3(-1.0f,  0.0f, .0f),
	   glm::vec3(-0.1f,  0.1f, -0.1f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(-1.0f,  0.0f, .0f),
	   glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(-1.0f,  0.0f, .0f),
	  glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(-1.0f,  0.0f, .0f),
	   glm::vec3(-0.1f, -0.1f,  0.1f),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(-1.0f,  0.0f, .0f),
	   glm::vec3(-0.1f,  0.1f,  0.1f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(-1.0f,  0.0f, .0f),

		glm::vec3(0.1f,  0.1f,  0.1f),  glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(0.760, 0.141, 0.141), glm::vec3(1.0f,  0.0f, .0f),
	   glm::vec3(0.1f,  0.1f, -0.1f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(1.0f,  0.0f, .0f),
		glm::vec3(0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(1.0f,  0.0f, .0f),
		glm::vec3(0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(1.0f,  0.0f, .0f),
		glm::vec3(0.1f, -0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(1.0f,  0.0f, .0f),
		 glm::vec3(0.1f,  0.1f,  0.1f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(1.0f,  0.0f, .0f),

	   glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  -1.0f, .0f),
		glm::vec3(0.1f, -0.1f, -0.1f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  -1.0f, .0f),
	   glm::vec3(0.1f, -0.1f,  0.1f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  -1.0f, .0f),
	   glm::vec3(0.1f, -0.1f,  0.1f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  -1.0f, .0f),
		glm::vec3(-0.1f, -0.1f,  0.1f),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  -1.0f, .0f),
	   glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  -1.0f, .0f),

	   //top
	   glm::vec3(-0.1f,  0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  1.0f, .0f),
	   glm::vec3(0.1f,  0.1f, -0.1f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  1.0f, .0f),
	   glm::vec3(0.1f,  0.1f,  0.1f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  1.0f, .0f),
		glm::vec3(0.1f,  0.1f,  0.1f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  1.0f, .0f),
	   glm::vec3(-0.1f,  0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.760, 0.141, 0.141),glm::vec3(0.0f,  1.0f, .0f),
		glm::vec3(-0.1f,  0.1f, -0.1f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.760, 0.141, 0.141), glm::vec3(0.0f,  1.0f, .0f)
	};

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	// R,G,B color value attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec3), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);
	//lighting attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec3), (void*)(3 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(3);
}

void CreateShaders()
{
	Shader* shader1 = new Shader(vertShader, fragShader);
	shaderList.push_back(*shader1);
	Shader shader2 = Shader(LightVertShader, LightFragShader);
	shaderList.push_back(shader2);
	Shader shader3 = Shader(DepthVertShader, DepthFragShader);
	shaderList.push_back(shader3);
	Shader shader4 = Shader(TextVertShader, TextFragShader);
	shaderList.push_back(shader4);
	Shader shader5 = Shader(ModelVertShader, ModelFragShader);
	shaderList.push_back(shader5);
}

void CreateAndLoadTextures() {
	Texture* gmT = new Texture("Assignment1/src/Textures/greenMetal.png");
	textureList.push_back(*gmT);

	Texture* bmT = new Texture("Assignment1/src/Textures/blueMetal.png");
	textureList.push_back(*bmT);

	Texture* gT = new Texture("Assignment1/src/Textures/gold.png");
	textureList.push_back(*gT);

	Texture* rgT = new Texture("Assignment1/src/Textures/roseGold.png");
	textureList.push_back(*rgT);

	Texture* tmT = new Texture("Assignment1/src/Textures/tealMetal.png");
	textureList.push_back(*tmT);

	Texture* bT = new Texture("Assignment1/src/Textures/brick.png");
	textureList.push_back(*bT);

	Texture* ttT = new Texture("Assignment1/src/Textures/grid.png");
	textureList.push_back(*ttT);

	Texture* sT = new Texture("Assignment1/src/Textures/Sky.png");
	textureList.push_back(*sT);

	textureList[GreenMetal].LoadTexture();
	textureList[BlueMetal].LoadTexture();
	textureList[Gold].LoadTexture();
	textureList[RoseGold].LoadTexture();
	textureList[TealMetal].LoadTexture();
	textureList[Brick].LoadTexture();
	textureList[Tile].LoadTexture();
	textureList[Sky].LoadTexture();
}

void RenderScene(int shaderIndex) {

	//Floor
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(150.0f, 0.1f, 150.0f));
	shaderList[0].setMat4("model", model); //you tell shader what matrix should be changed to
	textureList[Tile].UseTexture();
	glDrawArrays(type, 0, 36);
	model = identMatrix;
	move();
	passThrough();

	switch (modelRendered)
	{
		case 0:
			textureList[GreenMetal].UseTexture();
			createCube5(rootx, rooty, rootz, paulScale, anglex, anglez, angley, paulType);
			break;
		case 1:
			textureList[TealMetal].UseTexture();
			createCube2(rootx, rooty, rootz, runzeScale, anglex, anglez, angley, runzeType);
			break;
		case 2:
			textureList[BlueMetal].UseTexture();
			createCube3(rootx, rooty, rootz, jungtingScale, anglex, anglez, angley, juntingType);
			break;
		case 3:
			textureList[RoseGold].UseTexture();
			createCube1(rootx, rooty, rootz, jennaScale, anglex, anglez, angley, jennaType);
			break;
		case 4:
			textureList[Gold].UseTexture();
			createCube4(rootx, rooty, rootz, alecScale, anglex, anglez, angley, alecType);
			break;
	}
}

int main()
{
	//Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return -1;
	}

	//Setup GLFW window properties
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core profile is not backwards compatible
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Create Window (star makes it a pointer)
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "COMP 371 Final Project", NULL, NULL);
	//Check if window has been constructed successfully
	if (!mainWindow)
	{
		printf("GLFW  window creation failed!");
		glfwTerminate;
		return -1;
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

#pragma region freetype
	// FreeType
		// --------
		FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	// find path to font
	std::string font_name = "Assignment1/src/Font/ARLRDBD.TTF";
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return -1;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
#pragma endregion

	//Get buffer size information
	//& means with reference to
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use (tell glew this window is the one to use/should be tied to)
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extensionS features
	glewExperimental = GL_TRUE;

	//Initialize glew
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// tell GLFW to capture our mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

	//STEP 8: SET UP DEPTH BUFFER / ENABLE OUR DEPTH TEST
	glEnable(GL_DEPTH_TEST);

	//Setup viewport size 
	glViewport(0, 0, bufferWidth, bufferHeight);
	CreateShaders();
	CreateObjects();
	CreateAndLoadTextures();

#pragma region depth map

	const int SCR_WIDTH = 1024;
	const int SCR_HEIGHT = 1024;

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

	
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	if (currentCamera == menuCamera) {
		SoundEngine->play2D("Assignment1/src/Music/breakout.mp3", true);
	}

	Model UFO("Assignment1/src/Models/Low_poly_UFO.obj");

	// Implement ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {

		//Get + Handle user input events
		glfwPollEvents();

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(mainWindow);

		//Clear window (make a light blue background) Last Value: alpha/ Transparent vs Opaque
		//(R,G,B,A)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//STEP 8: SET UP DEPTH BUFFER / ENABLE OUR DEPTH TEST
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Declare new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glBindVertexArray(vao[0]);

		// activate shader
		shaderList[0].useShader();

		// create transformations
		glm::mat4 projection;
		glm::mat4 view;

		switch (currentCamera)
		{
		case mainCamera:
			projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			shaderList[0].setMat4("projection", projection);
			view = camera.GetViewMatrix();
			shaderList[0].setMat4("view", view);
			shaderList[0].setMat4("projection", projection);
			break;
		case menuCamera:
			projection = glm::perspective(glm::radians(camera2.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			shaderList[0].setMat4("projection", projection);
			view = camera2.GetViewMatrix();
			shaderList[0].setMat4("view", view);
			shaderList[0].setMat4("projection", projection);
			break;
		}

		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		

		glm::mat4 model = identMatrix; // make sure to initialize matrix to identity matrix first

#pragma region Create Models

		float near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		// 1. first render to depth map

		shaderList[0].setBool("isShadow", isShadow);
		shaderList[2].useShader();
		shaderList[2].setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//ConfigureShaderAndMatrices();
		RenderScene(2);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. then render scene as normal with shadow mapping (using depth map)
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ConfigureShaderAndMatrices();
		glBindTexture(GL_TEXTURE_2D, depthMap);

		shaderList[0].useShader();
		shaderList[currentShader].setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shaderList[currentShader].setVec3("viewPos", glm::vec3(0.0f, 5.0f, 0.0f)); //for specular lighting
		//load shader for light
		shaderList[currentShader].setFloat("ambientStrength", ambientStrength);
		shaderList[currentShader].setVec3("lightPosition", pointLightSource.position);
		shaderList[currentShader].setVec3("lightDirection", pointLightSource.direction);
		shaderList[currentShader].setVec3("lightColour", pointLightSource.color);
		shaderList[currentShader].setFloat("constant", pointLightSource.constant);
		shaderList[currentShader].setFloat("linear", pointLightSource.linear);
		shaderList[currentShader].setFloat("quadratic", pointLightSource.quadratic);
		RenderScene(0);

#pragma endregion

		
		//Unnasign the shader
		glUseProgram(0);

		shaderList[1].useShader();

		model = identMatrix;
		model = glm::translate(model, pointLightPosition);
		shaderList[1].setMat4("model", model);
		shaderList[1].setMat4("view", view);
		shaderList[1].setMat4("projection", projection);

		glDrawArrays(type, 0, 36);

		//Unnasign the shader
		glUseProgram(0);

		//glDisable(GL_DEPTH_TEST);

		// HUD
		glBindVertexArray(vao[1]);

		shaderList[3].useShader();
		glm::mat4 projection2 = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
		shaderList[3].setMat4("projection", projection2);

		//RenderText(shaderList[3], "Test1", ambientStrength, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		//RenderText(shaderList[3], "Test2", movingSpeed, 540.0f, 540.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		glUseProgram(0);

		// render the loaded model
		shaderList[4].useShader();
		glm::mat4 projectionModel;
		glm::mat4 viewModel;
		switch (currentCamera)
		{
		case mainCamera:
			projectionModel = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			viewModel = camera.GetViewMatrix();
			shaderList[4].setMat4("projection", projectionModel);
			shaderList[4].setMat4("view", viewModel);
			break;
		case menuCamera:
			projectionModel = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			viewModel = camera2.GetViewMatrix();
			shaderList[4].setMat4("projection", projectionModel);
			shaderList[4].setMat4("view", viewModel);
			break;
		}
		
		glm::mat4 modelModel = glm::mat4(1.0f);
		modelModel = glm::translate(modelModel, glm::vec3(0.5f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene
		modelModel = glm::scale(modelModel, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		shaderList[4].setMat4("model", modelModel);
		UFO.Draw(shaderList[4]);

		//Unnasign the shader
		glUseProgram(0);

		second = 40 - (int)round(glfwGetTime());

		if (second <= 0) {
			second = 0;
			ImGui::Begin("Timers Up!");
			ImGui::Text("Your final score is: %d", score);
			ImGui::Text("Thanks for playing!");
			ImGui::End();
			movingSpeed = 0.0f;
		}
		switch (currentCamera)
		{
		case mainCamera:
			ImGui::Begin("Settings");
			ImGui::Text("Diffculity");
			ImGui::SliderFloat("Speed", &movingSpeed, 0.01, 0.05);
			ImGui::Text(" ");
			ImGui::Text("General");
			ImGui::SliderFloat("Brightness", &ambientStrength, 0.0f, 1.0f);
			ImGui::End();

			ImGui::Begin("Timer");
			ImGui::Text("%d", second);
			ImGui::End();

			ImGui::Begin("Score");
			ImGui::Text("Score: %d", score);
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			break;
		case menuCamera:
			ImGui::Begin("		Welcome");
			ImGui::Text("		COMP 371 Final Project");
			ImGui::Text("	Team 10 A+ Squad");
			ImGui::Text("	Completed by:");
			ImGui::Text("	Alec Arakilyan");
			ImGui::Text("	Iphigenia Jenna Pappas");
			ImGui::Text("	Paul Grippa Vento");
			ImGui::Text("	Junting Ye");
			ImGui::Text("	Runze Zhu");
			ImGui::Text(" ");
			ImGui::Text("	Press Enter to start the Game");
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			break;
		}

		glfwSwapBuffers(mainWindow);

	}

	// Destroy ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		currentCamera = mainCamera;

	float cameraSpeed = 2.5 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		switch (currentCamera)
		{
		case mainCamera:
			camera.ProcessKeyboard(FORWARD, deltaTime);
			break;
		case menuCamera:
			camera2.ProcessKeyboard(FORWARD, deltaTime);
			break;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		switch (currentCamera)
		{
		case mainCamera:
			camera.ProcessKeyboard(BACKWARD, deltaTime);
			break;
		case menuCamera:
			camera2.ProcessKeyboard(BACKWARD, deltaTime);
			break;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		switch (currentCamera)
		{
		case mainCamera:
			camera.ProcessKeyboard(LEFT, deltaTime);
			break;
		case menuCamera:
			camera2.ProcessKeyboard(LEFT, deltaTime);
			break;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		switch (currentCamera)
		{
		case mainCamera:
			camera.ProcessKeyboard(RIGHT, deltaTime);
			break;
		case menuCamera:
			camera2.ProcessKeyboard(RIGHT, deltaTime);
			break;
		}
	}

#pragma region Model Change

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		currentModel = JungtingModel;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		currentModel = JennaModel;
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		currentModel = RunzeModel;
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		currentModel = AlecModel;
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		currentModel = PaulModel;
	}

#pragma endregion
	
#pragma region Rotation

	float FULLANGLE = 360;
	if (isRotating) {
		modelAngle += 10.0f;

		switch (currentDirection)
		{
		case UPDIR:
			anglex -= 10;
			break;
		case DOWNDIR:
			anglex += 10;
			break;
		case LEFTDIR:
			angley -= 10.0f;
			break;
		case RIGHTDIR:
			angley += 10.0f;
			break;
		}
		angley = fmod(angley, FULLANGLE);
		anglex = fmod(anglex, FULLANGLE);

		modelAngle = fmod(modelAngle, 90);
		isRotating = modelAngle != 0;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS && !isRotating)
	{
		isRotating = true;
		currentDirection = UPDIR;
		SoundEngine->play2D("Assignment1/src/Music/bleep.mp3", false);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS && !isRotating)
	{

		isRotating = true;
		currentDirection = DOWNDIR;
		SoundEngine->play2D("Assignment1/src/Music/bleep.mp3", false);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !isRotating)
	{
		isRotating = true;
		currentDirection = LEFTDIR;
		SoundEngine->play2D("Assignment1/src/Music/bleep.mp3", false);

	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !isRotating)
	{
		isRotating = true;
		currentDirection = RIGHTDIR;
		SoundEngine->play2D("Assignment1/src/Music/bleep.mp3", false);
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		switch (currentModel) {
		case JungtingModel:
			anglez -= 5.0f;
			break;
		case PaulModel:
			anglez -= 5.0f;
			break;
		case JennaModel:
			anglez -= 5.0f;
			break;
		case RunzeModel:
			anglez -= 5.0f;
			break;
		case AlecModel:
			anglez -= 5.0f;
			break;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		switch (currentModel) {
		case JungtingModel:
			anglez += 5.0f;
			break;
		case PaulModel:
			anglez += 5.0f;
			break;
		case JennaModel:
			anglez += 5.0f;
			break;
		case RunzeModel:
			anglez += 5.0f;
			break;
		case AlecModel:
			anglez += 5.0f;
			break;
		}
	}
#pragma endregion
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;


	switch (currentCamera)
	{
	case mainCamera:
		camera.ProcessMouseMovement(xoffset, yoffset);
		break;
	case menuCamera:
		camera2.ProcessMouseMovement(xoffset, yoffset);
		break;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	switch (currentCamera)
	{
	case mainCamera:
		camera.ProcessMouseScroll(yoffset);
		break;
	case menuCamera:
		camera2.ProcessMouseScroll(yoffset);
		break;
	}
}

void passThrough()
{
	if (isinwall()) {
		if (check()) {
			score += 10;
		}

		modelRendered = rand() % 5;
		rootx = 0.0f;
		rooty = 0.0f;
		rootz = 5.0f;

		anglex = 0.0f;
		anglez = 0.0f;
		angley = 0.0f;
		switch (modelRendered)
		{
		case 0:
			currentModel = PaulModel;
			break;
		case 1:
			currentModel = RunzeModel;
			break;
		case 2:
			currentModel = JungtingModel;
			break;
		case 3:
			currentModel = JennaModel;
			break;
		case 4:
			currentModel = AlecModel;
			break;
		}
	}
}

bool check() {
	return anglex == 0 && angley == 0 and anglez == 0;
}

void move() {
	if (currentCamera == mainCamera) {
		rootz -= movingSpeed;
	}
	else
	{
		rootz == 0.0f;
	}
}

bool isinwall() {
	return rootz <= 0.1;
}
#pragma region Model Creation

void createCube1(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type) {



	glm::mat4 rotationx = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglex), glm::vec3(1, 0, 0));
	glm::mat4 rotationy = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAngley), glm::vec3(0, 1, 0));
	glm::mat4 rotationz = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglez), glm::vec3(0, 0, 1));;

	//object
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(rootx, rooty, rootz)) * rotationx * rotationy * rotationz;
	glm::mat4 model = matrix * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.4f, cubeScale * 0.3f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * -0.3f, cubeScale * 0.1f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.3f, cubeScale * 0.1f, cubeScale * -0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * 0.3f, cubeScale * -0.3f)) * glm::scale(glm::mat4(1.0f), cubeScale * zShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * -0.1f, cubeScale * 0.3f)) * glm::scale(glm::mat4(1.0f), cubeScale * zShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

#pragma region Wall Jenna

	textureList[Brick].UseTexture();

	glm::vec3 wallVec = glm::vec3(0.0f, 0.0f, 0.0f);
	textureList[Brick].UseTexture();
	glm::mat4 matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y - 0.5f, wallVec.z));
	glm::mat4 modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.4f, wallVec.y - 0.5f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.5f, wallVec.y - 0.3f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.5f, wallVec.y - 0.3f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.5f, wallVec.y - 0.1f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.5f, wallVec.y - 0.1f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.9f, wallVec.y + 0.1f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.9f, wallVec.y + 0.1f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.5f, wallVec.y + 0.3f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.7f, wallVec.y + 0.3f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.2f, wallVec.y + 0.3f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y + 0.5f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.4f, wallVec.y + 0.5f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;
#pragma endregion

}
void createCube2(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type) {

	glm::mat4 rotationx = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglex), glm::vec3(1, 0, 0));
	glm::mat4 rotationy = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAngley), glm::vec3(0, 1, 0));
	glm::mat4 rotationz = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglez), glm::vec3(0, 0, 1));;

	//object
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(rootx, rooty, rootz)) * rotationx * rotationy * rotationz;
	glm::mat4 model = matrix * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * -0.1f, cubeScale * -0.3f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;


	model = glm::translate(matrix, glm::vec3(cubeScale * -0.3f, cubeScale * 0.3f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * -0.6f, cubeScale * 0.4f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.3f, cubeScale * -0.3f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * 0.2f, cubeScale * -0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.3f, cubeScale * 0.0f, cubeScale * -0.3f)) * glm::scale(glm::mat4(1.0f), cubeScale * zShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.7f, cubeScale * -0.1f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

#pragma region Wall Runze
	textureList[Brick].UseTexture();
	glm::vec3 wallVec = glm::vec3(0.0f, 0.0f, 0.0f);
	textureList[Brick].UseTexture();
	glm::mat4 matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.8f, wallVec.y - 0.6f, wallVec.z));
	glm::mat4 modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x, wallVec.y - 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.8f, wallVec.y - 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.6f, wallVec.y - 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.8f, wallVec.y - 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.6f, wallVec.y - 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.5f, wallVec.y - 0.1f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.1f, wallVec.y - 0.3f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.8f, wallVec.y - 0.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.0f, wallVec.y - 0.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.0f, wallVec.y - 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.8f, wallVec.y + 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.0f, wallVec.y + 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);

	modelWall = identMatrix;
	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.3f, wallVec.y + 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.0f, wallVec.y + 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.2f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.8f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 1.0f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.8f, wallVec.y + 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x, wallVec.y + 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;
#pragma endregion
}
void createCube3(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type) {

#pragma region Model Junting
	glm::mat4 rotationx = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglex), glm::vec3(1, 0, 0));
	glm::mat4 rotationy = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAngley), glm::vec3(0, 1, 0));
	glm::mat4 rotationz = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglez), glm::vec3(0, 0, 1));;

	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(rootx, rooty, rootz)) * rotationx * rotationy * rotationz;
	glm::mat4 model = matrix * glm::scale(glm::mat4(1.0f), cubeScale * ThiccCube);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.5f, cubeScale * 0.6f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * -0.5f, cubeScale * 0.8f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.5f, cubeScale * -0.6f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * -0.5f, cubeScale * -0.8f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * -0.5f, cubeScale * 0.0f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * 0.0f, cubeScale * 0.6f)) * glm::scale(glm::mat4(1.0f), cubeScale * BigeCube);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * 0.8f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * -0.6f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;
#pragma endregion

#pragma region Wall Junting
	textureList[Brick].UseTexture();
	glm::vec3 wallVec = glm::vec3(3.5f, 0.0f, 0.0);
	textureList[Brick].UseTexture();
	glm::mat4 matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.9f, wallVec.y - 1.3f, wallVec.z));
	glm::mat4 modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.1f, wallVec.y - 1.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.3f, wallVec.y - 1.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 4.2f, wallVec.y - 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.8f, wallVec.y - 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.7f, wallVec.y - 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.2f, wallVec.y - 1.1f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.4f, wallVec.y - 1.1f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.2f, wallVec.y - 0.9f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.4f, wallVec.y - 0.9f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.2f, wallVec.y - 0.6f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.3f, wallVec.y - 0.6f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;


	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.5f, wallVec.y - 0.5f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.5f, wallVec.y - 0.7f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 4.1f, wallVec.y - 0.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.7f, wallVec.y - 0.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 4.1f, wallVec.y - 0.2f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.5f, wallVec.y - 0.1f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 4.2f, wallVec.y + 0.0f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.5f, wallVec.y + 0.1f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.7f, wallVec.y + 0.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 4.1f, wallVec.y + 0.2f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 4.1f, wallVec.y + 0.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 4.2f, wallVec.y + 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.8f, wallVec.y + 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.7f, wallVec.y + 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.3f, wallVec.y + 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.2f, wallVec.y + 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.0f, wallVec.y + 1.0f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.8f, wallVec.y + 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.6f, wallVec.y + 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.4f, wallVec.y + 0.8f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * yLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.9f, wallVec.y + 1.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 3.1f, wallVec.y + 1.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 2.3f, wallVec.y + 1.3f, wallVec.z + 0.0));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;
#pragma endregion



}
void createCube4(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type) {
	glm::mat4 rotationx = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglex), glm::vec3(1, 0, 0));
	glm::mat4 rotationy = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAngley), glm::vec3(0, 1, 0));
	glm::mat4 rotationz = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglez), glm::vec3(0, 0, 1));;
#pragma region Model Alec
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(rootx, rooty, rootz)) * rotationx * rotationy * rotationz;
	glm::mat4 model = matrix * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * 0.2f, cubeScale * 0.1f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * -0.2f, cubeScale * -0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * 0.4f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * -0.4f, cubeScale * -0.1f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.3f, cubeScale * 0.2f, cubeScale * -0.1f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.3f, cubeScale * -0.2f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * -0.3f, cubeScale * 0.2f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * 0.4f, cubeScale * -0.1f)) * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * -0.4f, cubeScale * 0.1f)) * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * 0.2f, cubeScale * -0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * -0.2f, cubeScale * -0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * 0.6f, cubeScale * -0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * -0.6f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 1.1f, cubeScale * 0.6f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * 0.8f, cubeScale * -0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.6f, cubeScale * -0.8f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.1f, cubeScale * 0.6f, cubeScale * -0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.3f, cubeScale * 0.8f, cubeScale * 0.1f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.3f, cubeScale * -0.8f, cubeScale * -0.1f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 1.0f, cubeScale * -0.8f, cubeScale * -0.1f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.9f, cubeScale * 0.8f, cubeScale * 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

#pragma endregion
#pragma region Wall Alec
	glm::vec3 wallVec = glm::vec3(0.0f, 0.0f, 0.0f);
	textureList[Brick].UseTexture();
	glm::mat4 matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y - 1.0f, wallVec.z));
	glm::mat4 modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.4f, wallVec.y - 1.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.2f, wallVec.y - 1.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.2f, wallVec.y - 0.8f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.4f, wallVec.y - 0.8f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.2f, wallVec.y - 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.2f, wallVec.y - 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y - 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.2f, wallVec.y - 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y - 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.2f, wallVec.y - 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.6f, wallVec.y, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.5f, wallVec.y + 0.2, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.0f, wallVec.y + 0.2, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.2f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y + 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.4f, wallVec.y + 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.2f, wallVec.y + 0.8f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.4f, wallVec.y + 0.8f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x - 0.4f, wallVec.y + 1.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 0.4f, wallVec.y + 1.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 1.2f, wallVec.y + 1.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;
#pragma endregion
}
void createCube5(float rootx, float rooty, float rootz, float cubeScale, float spinningCubeAnglex, float spinningCubeAnglez, float spinningCubeAngley, GLenum type) {

#pragma region Cube

	glm::mat4 rotationx = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglex), glm::vec3(1, 0, 0));
	glm::mat4 rotationy = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAngley), glm::vec3(0, 1, 0));
	glm::mat4 rotationz = glm::rotate(glm::mat4(1.0f), glm::radians(spinningCubeAnglez), glm::vec3(0, 0, 1));;

	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(rootx, rooty, rootz)) * rotationx * rotationy * rotationz;
	glm::mat4 model = matrix * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.4f, cubeScale * 0.5f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * yShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.2f, cubeScale * 0.2f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.0f, cubeScale * 0.0f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;
	model = glm::translate(matrix, glm::vec3(cubeScale * (-0.2f), cubeScale * 0.2f, cubeScale * -0.3f)) * glm::scale(glm::mat4(1.0f), cubeScale * zShortScale);
	shaderList[currentShader].setMat4("model", model); //updating matrix in shader
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * (-0.4f), cubeScale * 0.4f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * (-0.2f), cubeScale * -0.2f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model); //updating matrix in shader
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * (-0.5f), cubeScale * (-0.4f), cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.2f, cubeScale * -0.2f, cubeScale * 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", model); //updating matrix in shader
	glDrawArrays(type, 0, 36);
	model = identMatrix;

	model = glm::translate(matrix, glm::vec3(cubeScale * 0.4f, cubeScale * -0.4f, cubeScale * 0.1f)) * glm::scale(glm::mat4(1.0f), cubeScale * zShortScale);
	shaderList[currentShader].setMat4("model", model);
	glDrawArrays(type, 0, 36);
	model = identMatrix;

#pragma endregion

#pragma region Wall Paul

	textureList[Brick].UseTexture();

	glm::vec3 wallVec = glm::vec3(-3.5f, 0.0f, 0.0f);
	textureList[Brick].UseTexture();
	glm::mat4 matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.0f, wallVec.y - 0.6f, wallVec.z));
	glm::mat4 modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.8f, wallVec.y - 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 2.7f, wallVec.y - 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.4f, wallVec.y - 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.7f, wallVec.y - 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 4.1f, wallVec.y - 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.1f, wallVec.y - 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 2.8f, wallVec.y - 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.5f, wallVec.y - 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 4.0f, wallVec.y - 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.0f, wallVec.y - 0.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 4.0f, wallVec.y - 0.0f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.1f, wallVec.y + 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 2.8f, wallVec.y + 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.5f, wallVec.y + 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 4.0f, wallVec.y + 0.2f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 2.8f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.4f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.7f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 4.1f, wallVec.y + 0.4f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.0f, wallVec.y + 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.6f, wallVec.y + 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xShortScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 4.1f, wallVec.y + 0.6f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), glm::vec3(cubeScale, cubeScale, cubeScale));
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.0f, wallVec.y + 0.8f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

	matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(wallVec.x + 3.8f, wallVec.y + 0.8f, wallVec.z));
	modelWall = matrix2 * glm::scale(glm::mat4(1.0f), cubeScale * xLongScale);
	shaderList[currentShader].setMat4("model", modelWall);
	glDrawArrays(type, 0, 36);
	modelWall = identMatrix;

#pragma endregion

}

#pragma endregion

// render line of text
// -------------------
void RenderText(Shader& shader, std::string text, float i, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.useShader();
	glUniform3f(glGetUniformLocation(shader.shader_ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao[1]);

	std::string alltext = text + std::to_string(i);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = alltext.begin(); c != alltext.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void showHUD() {

}
//useless
/*
static void ShowExampleAppSimpleOverlay(bool* p_open)
{
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
	{
		ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void drawText(const char* text, int length, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2d(x, y);
	//for (int i = 0; i < length, i++) {
		
	//}
}*/