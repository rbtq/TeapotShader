// TeapotAD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "gl_core_4_3.hpp"

#include <glfw3.h>

#include "scene.h"

#include "scenediffuse.h"


//#include <string>
//using std::cout;


#include "QuatCamera.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define MOVE_VELOCITY 0.01f
#define ROTATE_VELOCITY 0.001f

using namespace imat2908;

//The GLFW Window
GLFWwindow *window;

//The Scene
SceneDiffuse *scene;

//The camera
QuatCamera camera;

ThingToChange thingToChange;

//To keep track of cursor location
double lastCursorPositionX, lastCursorPositionY, cursorPositionX, cursorPositionY;

//printing the result of a change using the keypad
void printResultOfChange(bool isIncreasing, WhatToChange changed, float changeAmount, float newValue) {
	//is it increasing?
	if (isIncreasing) {
		std::cout << "Increasing ";
	}
	else {
		std::cout << "Decreasing ";
	}
	//what is the change?
	switch (changed)
	{
		case WhatToChange::AMBIENT_INTENSITY: std::cout << "ambient light intensity "; break;
		case WhatToChange::DIFFUSE_INTENSITY: std::cout << "diffuse light intensity "; break;
		case WhatToChange::SPECULAR_INTENSITY: std::cout << "specular light intensity "; break;
		case WhatToChange::POWER_KS_TEAPOT: std::cout << "teapot shinyness value "; break;
		case WhatToChange::POWER_KS_BACKGROUND: std::cout << "plane shinyness value "; break;
		case WhatToChange::BACKGROUND_KA_R: std::cout << "background ambient red value "; break;
		case WhatToChange::BACKGROUND_KA_G: std::cout << "background ambient green value "; break;
		case WhatToChange::BACKGROUND_KA_B: std::cout << "background ambient blue value "; break;
		case WhatToChange::BACKGROUND_KD_R: std::cout << "background diffuse red value "; break;
		case WhatToChange::BACKGROUND_KD_G: std::cout << "background diffuse green value "; break;
		case WhatToChange::BACKGROUND_KD_B: std::cout << "background diffuse blue value "; break;
		case WhatToChange::BACKGROUND_KS_R: std::cout << "background specular red value "; break;
		case WhatToChange::BACKGROUND_KS_G: std::cout << "background specular green value "; break;
		case WhatToChange::BACKGROUND_KS_B: std::cout << "background specular blue value "; break;
		case WhatToChange::TEAPOT_KA_R: std::cout << "teapot ambient red value "; break;
		case WhatToChange::TEAPOT_KA_G: std::cout << "teapot ambient green value "; break;
		case WhatToChange::TEAPOT_KA_B: std::cout << "teapot ambient blue value "; break;
		case WhatToChange::TEAPOT_KD_R: std::cout << "teapot diffuse red value "; break;
		case WhatToChange::TEAPOT_KD_G: std::cout << "teapot diffuse green value "; break;
		case WhatToChange::TEAPOT_KD_B: std::cout << "teapot diffuse blue value "; break;
		case WhatToChange::TEAPOT_KS_R: std::cout << "teapot specular red value "; break;
		case WhatToChange::TEAPOT_KS_G: std::cout << "teapot specular green value "; break;
		case WhatToChange::TEAPOT_KS_B: std::cout << "teapot specular blue value "; break;
		case WhatToChange::LIGHT_POSITION: std::cout << "light position to origin "; break;
		default: std::cout << "null "; break;
	}

	//what is the amount and new value
	std::cout << "by " << changeAmount << " to " << newValue << std::endl;

}

//printing what is currently selected to be changed using the keypad
void printWhatCouldBeChanged(WhatToChange change) {
	std::cout << "Current value selected: ";
	//what is selected
	switch (change)
	{
		case WhatToChange::AMBIENT_INTENSITY: std::cout << "ambient light intensity "; break;
		case WhatToChange::DIFFUSE_INTENSITY: std::cout << "diffuse light intensity "; break;
		case WhatToChange::SPECULAR_INTENSITY: std::cout << "specular light intensity "; break;
		case WhatToChange::POWER_KS_TEAPOT: std::cout << "teapot shinyness value "; break;
		case WhatToChange::POWER_KS_BACKGROUND: std::cout << "plane shinyness value "; break;
		case WhatToChange::BACKGROUND_KA_R: std::cout << "background ambient red value "; break;
		case WhatToChange::BACKGROUND_KA_G: std::cout << "background ambient green value "; break;
		case WhatToChange::BACKGROUND_KA_B: std::cout << "background ambient blue value "; break;
		case WhatToChange::BACKGROUND_KD_R: std::cout << "background diffuse red value "; break;
		case WhatToChange::BACKGROUND_KD_G: std::cout << "background diffuse green value "; break;
		case WhatToChange::BACKGROUND_KD_B: std::cout << "background diffuse blue value "; break;
		case WhatToChange::BACKGROUND_KS_R: std::cout << "background specular red value "; break;
		case WhatToChange::BACKGROUND_KS_G: std::cout << "background specular green value "; break;
		case WhatToChange::BACKGROUND_KS_B: std::cout << "background specular blue value "; break;
		case WhatToChange::TEAPOT_KA_R: std::cout << "teapot ambient red value "; break;
		case WhatToChange::TEAPOT_KA_G: std::cout << "teapot ambient green value "; break;
		case WhatToChange::TEAPOT_KA_B: std::cout << "teapot ambient blue value "; break;
		case WhatToChange::TEAPOT_KD_R: std::cout << "teapot diffuse red value "; break;
		case WhatToChange::TEAPOT_KD_G: std::cout << "teapot diffuse green value "; break;
		case WhatToChange::TEAPOT_KD_B: std::cout << "teapot diffuse blue value "; break;
		case WhatToChange::TEAPOT_KS_R: std::cout << "teapot specular red value "; break;
		case WhatToChange::TEAPOT_KS_G: std::cout << "teapot specular green value "; break;
		case WhatToChange::TEAPOT_KS_B: std::cout << "teapot specular blue value "; break;
		case WhatToChange::TOON_SHADE_COUNT: std::cout << "toon shade count "; break;
		case WhatToChange::LIGHT_POSITION: std::cout << "light position to origin "; break;
		default: std::cout << "null "; break;
	}
	std::cout << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Callback function for keypress use to toggle animate (not used at the moment)
// and to check for R to reset camera
/////////////////////////////////////////////////////////////////////////////////////////////
static void key_callback(GLFWwindow* window, int key, int cancode, int action, int mods)
{
	float initialValue = scene->getValue(thingToChange.getWhatToChange());
	//std::cout << "a" << std::endl;
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		if (scene)
			scene->animate(!(scene->animating()));
	if (key == 'R' && action == GLFW_RELEASE)
			camera.reset();
	//increase value of selected component by a set amount
	if (key == GLFW_KEY_KP_ADD) {
		scene->changeValue(thingToChange.getWhatToChange(), 0.01f);
		printResultOfChange(true, thingToChange.getWhatToChange(), scene->getValue(thingToChange.getWhatToChange()) - initialValue, scene->getValue(thingToChange.getWhatToChange()));
	}
	//decrease value of selected component by a set amount as long as it doesnt result in a negative value
	else if (key == GLFW_KEY_KP_SUBTRACT && scene->getValue(thingToChange.getWhatToChange()) >= 0.0f) {
		scene->changeValue(thingToChange.getWhatToChange(), -0.01f);
		printResultOfChange(false, thingToChange.getWhatToChange(), scene->getValue(thingToChange.getWhatToChange()) - initialValue, scene->getValue(thingToChange.getWhatToChange()));
	}
	//multiply value of selected component by a set amount 
	else if (key == GLFW_KEY_KP_MULTIPLY && scene->getValue(thingToChange.getWhatToChange()) >= 0.0f) {
		scene->changeValue(thingToChange.getWhatToChange(), initialValue * 0.1f);
		printResultOfChange(true, thingToChange.getWhatToChange(), scene->getValue(thingToChange.getWhatToChange()) - initialValue, scene->getValue(thingToChange.getWhatToChange()));
	}
	//divide value of selected component by a set amount 
	else if (key == GLFW_KEY_KP_DIVIDE && scene->getValue(thingToChange.getWhatToChange()) >= 0.0f) {
		scene->changeValue(thingToChange.getWhatToChange(), initialValue * -0.1f);
		printResultOfChange(false, thingToChange.getWhatToChange(), scene->getValue(thingToChange.getWhatToChange()) - initialValue, scene->getValue(thingToChange.getWhatToChange()));
	}
	//change the selected value to be changed
	else if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS) {
		thingToChange++;
		printWhatCouldBeChanged(thingToChange.getWhatToChange());
	}
	//change the selected value to be changed
	else if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) {
		thingToChange--;
		printWhatCouldBeChanged(thingToChange.getWhatToChange());
	}
	//print the current values being used
	else if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) {
		system("CLS");
		scene->printSceneValues();
	}
	//switch between the 2 shader types
	else if (key == GLFW_KEY_KP_0 && action == GLFW_PRESS) {
		scene->togglePhong();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Callback function when mouse wheel is scrolled
/////////////////////////////////////////////////////////////////////////////////////////////
void scroll_callback(GLFWwindow *window, double x, double y)
{
			camera.zoom((float)y*0.5f);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Initialise 
/////////////////////////////////////////////////////////////////////////////////////////////
void initializeGL() {


    gl::ClearColor(0.5f,0.5f,0.5f,1.0f);
    
	lastCursorPositionX=0.0;
	lastCursorPositionY=0.0;
	cursorPositionX=0.0;
	cursorPositionY=0.0;
	

	// Create the scene class and initialise it for the camera
	scene = new SceneDiffuse();
    scene->initScene(camera);
	glfwSetKeyCallback(window, key_callback);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Update
/////////////////////////////////////////////////////////////////////////////////////////////
void update( float t ) 
{ 
	
	//Get the current cursor position
	glfwGetCursorPos(window,&cursorPositionX,&cursorPositionY);

	//See how much the cursor has moved
	float deltaX = (float)(lastCursorPositionX - cursorPositionX);
	float deltaY = (float)(lastCursorPositionY - cursorPositionY);

	//Using a different way (i.e. instead of callback) to check for LEFT mouse button
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) )
	{
		//std::cout << "Left button \n";
		//Rotate the camera. The 0.001f is a velocity mofifier to make the speed sensible

	//	std::cout <<"deltaX " << deltaX << " deltaY " << deltaY << "\n";

		camera.rotate(deltaX*ROTATE_VELOCITY, deltaY*ROTATE_VELOCITY);
		
	}
	
	//Using a different way (i.e. instead of callback) to check for RIGHT mouse button
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) )
	{
		//std::cout << "Right button \n";
		//Rotate the camera. The 0.01f is a velocity mofifier to make the speed sensible
		camera.pan(deltaX*MOVE_VELOCITY, deltaY*MOVE_VELOCITY);

	}
	//To adjust Roll with MIDDLE mouse button
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_MIDDLE) )
	{
	
		camera.roll(deltaX*ROTATE_VELOCITY);
		
	}
		
	//Store the current cursor position
	lastCursorPositionX = cursorPositionX;
	lastCursorPositionY = cursorPositionY;
	
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Main loop updates scene and renders until we quit
/////////////////////////////////////////////////////////////////////////////////////////////
void mainLoop() {
	while( ! glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
		//GLUtils::checkForOpenGLError(__FILE__,__LINE__);
		update((float)glfwGetTime());
		scene->render(camera);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// resize
/////////////////////////////////////////////////////////////////////////////////////////////
void resizeGL(QuatCamera camera, int w, int h ) {
    scene->resize(camera,w,h);
}


/////////////////////////////////////////////////////////////////////////////////////////////
// main entry point
/////////////////////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{

	string name ="Teapot with Diffuse Lighting";

	// Initialize GLFW
	if( !glfwInit() ) exit( EXIT_FAILURE );

	// Select OpenGL 4.3 with a forward compatible core profile.
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, TRUE);

	// Open the window
	string title = "imat2908 - " + name;
	window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL );
	if( ! window ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwMakeContextCurrent(window);

	//Key callback
	glfwSetKeyCallback(window,key_callback);

	//Mouse callback, not used at the moment
	//glfwSetMouseButtonCallback(window,mouse_callback);

	//Scroll callback
	glfwSetScrollCallback(window,scroll_callback);//Set callback

	// Load the OpenGL functions.
	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();

	if (!didLoad) {

		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Initialization
	initializeGL();

	resizeGL(camera,WIN_WIDTH,WIN_HEIGHT);

	// Enter the main loop
	mainLoop();

	// Close window and terminate GLFW
	glfwTerminate();

	delete scene;

	// Exit program
	exit( EXIT_SUCCESS );
}




