#define _USE_MATH_DEFINES

#include<iostream>
#include<stdlib.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include"Mesh.h"
#include "ROML.h"
#include <chrono>
#include <cmath>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>


using namespace std;

const unsigned int width = 800;
const unsigned int height = 800;

// make the decltype slightly easier to the eye
using seconds_t = std::chrono::seconds;

// return the same type as seconds.count() below does.
// note: C++14 makes this a lot easier.
decltype(seconds_t().count()) get_millis_since_epoch()
{
	// get the current time
	const auto now = chrono::system_clock::now();

	// transform the time into a duration since the epoch
	const auto epoch = now.time_since_epoch();

	// cast the duration into seconds
	const auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

	// return the number of seconds
	return seconds.count();
}


/* 
In the background the previous frame shown is being overwritten with new information, these are called buffers
front buffer = seen/read
back buffer = written

Vertex data: Vertex Shader > Shape Assembly > Geo Shader > Rasterization (Mathematical shape into pixels) > Fragment Shader (Adds colours to pixels, depends on a lot) > tests and blending

index buffers tell open GL the oder to connect the vertices together, uses indices

#version 330 core
layout (location = 0) in vec3 aPos; uses layout with the location of 0 for the aPos. also says that there is a vec3 datatype
void main()
{
gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); asign gl_position with all needed positions
}
kind of 'outputs' gl_Position

#version 330 core
out vec4 FragColor; outputs a vec4 colour
void main()
{
   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f); what the colour is (i think its orange)
}


uniforms are like universal variables that can be accessed by other shaders and can be accessed from the main function

*/



int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Specifying OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Using core porfile (just modern libraries)

	//Face normals are perpendicular to a surface of one triangle while vertex normals are projected from each vertex
	//Face normals = flat shading, vertex normals = smooth shading. we're gonna use face normals

	//Had to remake this whole thing 
	Vertex vertices[] =
	{//     CO-ORDINATES                     /        COLOURS               /         TEXTURE CO-ORDS  /                NORMALS   //
		Vertex{glm::vec3(-0.5f, 0.0f, 0.5f),  glm::vec3(0.83f, 0.70f, 0.44f),  glm::vec3(0.0f, -1.0f, 0.0f),          glm::vec2(0.0f, 0.0f)},  //Bottom Side
		Vertex{glm::vec3(-0.5f, 0.0f, -0.5f),  glm::vec3(0.83f, 0.70f, 0.44f),   glm::vec3(0.0f, -1.0f, 0.0f),        glm::vec2(0.0f, 5.0f)},  //Bottom Side   
		Vertex{glm::vec3(0.5f, 0.0f, -0.5f),   glm::vec3(0.83f, 0.70f, 0.44f),   glm::vec3(0.0f, -1.0f, 0.0f),        glm::vec2(5.0f, 5.0f)},  //Bottom Side 
		Vertex{glm::vec3(0.5f, 0.0f, 0.5f),    glm::vec3(0.83f, 0.70f, 0.44f),    glm::vec3(0.0f, -1.0f, 0.0f),       glm::vec2(5.0f, 0.0f)},  //Bottom Side   
																												      
		Vertex{glm::vec3(-0.5f, 0.0f, 0.5f),   glm::vec3(0.83f, 0.70f, 0.44f),    glm::vec3(-0.8f, 0.5f, 0.0f),       glm::vec2(0.0f, 0.0f)},     //Left Side
		Vertex{glm::vec3(-0.5f, 0.0f, -0.5f),   glm::vec3(0.83f, 0.70f, 0.44f),    glm::vec3(-0.8f, 0.5f, 0.0f),      glm::vec2(5.0f, 0.0f)},  //Left Side
		Vertex{glm::vec3(0.0f, 0.8f, 0.0f),     glm::vec3(0.92f, 0.86f, 0.76f),      glm::vec3(-0.8f, 0.5f, 0.0f),    glm::vec2(2.5f, 5.0f)},   //Left Side
																												      
		Vertex{glm::vec3(-0.5f, 0.0f, -0.5f),   glm::vec3(0.83f, 0.70f, 0.44f),     glm::vec3(0.0f, 0.5f, -0.8f),     glm::vec2(5.0f, 0.0f)},  //Non-facing side
		Vertex{glm::vec3(0.5f, 0.0f, -0.5f),     glm::vec3(0.83f, 0.70f, 0.44f),    glm::vec3(0.0f, 0.5f, -0.8f),     glm::vec2(0.0f, 0.0f)},  //Non-facing side  
		Vertex{glm::vec3(0.0f, 0.8f, 0.0f),      glm::vec3(0.92f, 0.86f, 0.76f),      glm::vec3(0.0f, 0.5f, -0.8f),   glm::vec2(2.5f, 5.0f)},    //Non-facing side 

		Vertex{glm::vec3(0.5f, 0.0f, -0.5f),     glm::vec3(0.83f, 0.70f, 0.44),      glm::vec3(0.8f, 0.5f, 0.0f),     glm::vec2(0.0f, 0.0f)},   //Right Side
		Vertex{glm::vec3(0.5f, 0.0f, 0.5f),      glm::vec3(0.83f, 0.70f, 0.44),      glm::vec3(0.8f, 0.5f, 0.0f),     glm::vec2(5.0f, 0.0f)},  //Right Side   
		Vertex{glm::vec3(0.0f, 0.8f, 0.0f),      glm::vec3(0.92f, 0.86f, 0.76),      glm::vec3(0.8f, 0.5f, 0.0f),     glm::vec2(2.5f, 5.0f)},  //Right Side  
																											
																			
		Vertex{glm::vec3(0.5f, 0.0f, 0.5f),      glm::vec3(0.83f, 0.70f, 0.44f),       glm::vec3(0.0f, 0.5f, 0.8f),     glm::vec2(5.0f, 0.0f)},  //Facing Side   
		Vertex{glm::vec3(-0.5f, 0.0f, 0.5f),     glm::vec3(0.83f, 0.70f, 0.44f),       glm::vec3(0.0f, 0.5f, 0.8f),     glm::vec2(0.0f, 0.0f)},  //Facing Side     
		Vertex{glm::vec3(0.0f, 0.8f, 0.0f),      glm::vec3(0.92f, 0.86f, 0.76f),       glm::vec3(0.0f, 0.5f, 0.8f),     glm::vec2(2.5f, 5.0f)}  //Facing Side    

		/*-1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,             0.0f, 0.0f,              0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,    0.0f, 0.0f, 0.0f,             0.0f, 1.0f,              0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, -1.0f,    0.0f, 0.0f, 0.0f,             1.0f, 1.0f,              0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,             1.0f, 0.0f,              0.0f, 1.0f, 0.0f*/
	};

	//index buffer
	GLuint indices[] = {
		0, 1, 2, //Bottom Side
		0, 2, 3, //Bottom Side
		4, 6, 5, //Left Side
		7, 9, 8, //Non-facing side
		10, 12, 11, //Right side
		13, 15, 14 //Facing side

		/*0, 1, 2,
		0, 2, 3*/

	};

	//Making a cube that will be our light source
	Vertex lightVertices[] = {
		//   COORDINATES  //
		Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
		Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
		Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}

	};

	GLuint lightIndices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

	//------------------------------------------------------------------------------------------------

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Testing", NULL, NULL); //Width, Height, Name, Fullscreen, ~
	if (window == NULL) {
		cout << "Failed to create a window" << endl; //Just checking if the window failed to create
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //tells glfw to make the window to hold 'all' of opengl

	gladLoadGL(); //load the config for opengl

	glViewport(0, 0, width, height); //where we want the opengl to show stuff

	Texture textures[]{
		//----------TEXTURES-------------------------------
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
		//-------------------------------------------------
	};


	//Creates shadeprogram from default.vert and default.frag
	Shader shaderProgram("Default.vert", "default.frag");
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh pyramid(verts, ind, tex);

	//GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale"); //get uniform reference value (stored in uniID)
	GLuint timeID = glGetUniformLocation(shaderProgram.ID, "time");


	auto startTime = get_millis_since_epoch();



	Shader lightShader("light.vert", "light.frag");

	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex); //The tex is just a placeholder

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = roml::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = roml::translate(pyramidModel, pyramidPos);

	//Exporting data
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //clears the colour of current background and replaces it. also this is navy blue
	glClear(GL_COLOR_BUFFER_BIT); //le execute with the color buffer
	glfwSwapBuffers(window);

	float rotation = 0.0f;
	double prevTime = get_millis_since_epoch();
	glEnable(GL_DEPTH_TEST); //fixes depth issues

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	while (!glfwWindowShouldClose(window)) {
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//-------Stinky 3D (Note: I am using GLM's types. Not functions)-----------
		double crntTime = get_millis_since_epoch();
		if (crntTime - prevTime >= 1 / 60) { //change rotation half a degree every 1/60 of a millisecond
			rotation += 0.1f;
			prevTime = crntTime;
		}
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		
		//Tells opengl what shader program we're gonna use
		shaderProgram.Activate();

		camera.Inputs(window); //Yay inputs!
		//Updates and exports camera matrix to the vert shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		pyramid.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);


		glfwSwapBuffers(window);
		glfwPollEvents(); //this is to make the window actually, respond.
	}

	//------Delete-----------------
	shaderProgram.Delete();
	lightShader.Delete();
	//-----------------------------

	
	glfwDestroyWindow(window); //killing the window after we finish

	glfwTerminate(); //terminating glfw after function
	return 0;
}