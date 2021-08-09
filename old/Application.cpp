#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <vector> //Array list as oppose to array

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Shader.h"


//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
//Rotations use radians (convert degrees to radians)
const float toRadians = 3.14159265 / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;


//Working with Uniform variables
//Starts at 0.0, increments by 0.0005 intil it reaches 0.7 and then goes opposite direction
bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//Create shader itself: Vertex Shader (taking each point you pass)
//static const char* vShader = "Shaders/shader.vertex";


//Create Fragment Shader
//static const char* fShader = "Shaders/shader.fragment";


//Create shader itself: Vertex Shader (taking each point you pass)
static const char* vShader = "                               \n\
#version 330                                                 \n\
                                                             \n\
layout (location = 0) in vec3 pos;                           \n\
                                                              \n\
out vec4 vCol;                                                            \n\
uniform mat4 model;                                         \n\
uniform mat4 projection;                                         \n\
                                                             \n\
void main()                                                  \n\
{                                                            \n\
	gl_Position = projection * model * vec4(pos, 1.0); \n\
    vCol = vec4(clamp(pos,0.0f,1.0f), 1.0f);                                       \n\
}";
//THE 0.4 ABOVE CHANGES THE SCALE

//Create Fragment Shader
static const char* fShader = "                               \n\
#version 330                                                 \n\
in vec4 vCol;                                                             \n\
out vec4 colour;                                             \n\
                                                             \n\
void main()                                                  \n\
{                                                            \n\
	colour = vCol;                        \n\
}";

void CreateObjects()
{

	//STEP 3: CREATE A SET OF INDICES:
	unsigned int indices[]{
		0,3,1,
		1,3,2,
		2,3,0, //front facing side
		0,1,2 //base of pyramid
	};


	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, //should be black
		0.0f, -1.0f, 1.0f, //STEP 2: ADD 4TH POINT
		1.0f, -1.0f, 0.0f, //red
		0.0f, 1.0f, 0.0f //green (interpolation in fragment & vertex shader)
	};





	Mesh* obj1 = new Mesh(); //Make sure you create pointer to not accidentally delete your object
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1); //push back = adding to the end of a list

	Mesh* obj2 = new Mesh(); //Make sure you create pointer to not accidentally delete your object
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2); //push back = adding to the end of a list


}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromString(vShader, fShader);
	shaderList.push_back(*shader1);
}

/*void CreateGrid()
{

	GLfloat verticesGrid[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};


	Mesh* objGrid = new Mesh(); //Make sure you create pointer to not accidentally delete your object
	objGrid->CreateMeshGrid(verticesGrid,1000);
	meshList.push_back(objGrid); //push back = adding to the end of a list

}*/


int main()
{

	//Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return 1;
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
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	//Check if window has been constructed successfully
	if (!mainWindow)
	{
		printf("GLFW  window creation failed!");
		glfwTerminate;
		return 1;
	}

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

	//STEP 8: SET UP DEPTH BUFFER / ENABLE OUR DEPTH TEST
	glEnable(GL_DEPTH_TEST);

	//Setup viewport size 
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;



	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {

		//Get + Handle user input events
		glfwPollEvents();

		//If we are incrementing the value
		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}
		//When hits max offset-Change direction of translation

		if (abs(triOffset) >= triMaxoffset) {
			direction = !direction;
		}

		//To avoid overflow when program is running for a long time
		//Change cur angle to speed up turning (smaller = slower)
		curAngle += 0.01f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		//DO same for scale
		if (sizeDirection)
		{
			curSize += 0.0001f;
		}
		else
		{
			curSize -= 0.0001f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}


		

		//Clear window (make a purely red screen) Last Value: alpha/ Transparent vs Opaque
		//First: Red, Second: Green, Third: Blue
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//STEP 8: SET UP DEPTH BUFFER / ENABLE OUR DEPTH TEST
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		//Where you put the triOffset will decide where it translates
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.50f));
		//You can stack transformations but they must be done IN THE RIGHT ORDER
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//Here x and y axes is constantly rotating as well/ this is why the model is moving in this way
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //You can scale things in any direction
				//Assign the value to the shader itself
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();



		glm::mat4(1.0f); // will clear and we can do them seperately
		model = glm::translate(model, glm::vec3(-triOffset, 1.0f, -2.50f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //You can scale things in any direction
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Unnasign the shader
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}

	return 0;
}