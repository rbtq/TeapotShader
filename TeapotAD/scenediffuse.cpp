
#include "scenediffuse.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "defines.h"

using glm::vec3;


#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>


namespace imat2908
{
/////////////////////////////////////////////////////////////////////////////////////////////
// Default constructor
/////////////////////////////////////////////////////////////////////////////////////////////
SceneDiffuse::SceneDiffuse()
{
	/*
	//Set the plane's material properties in the shader and render
	prog.setUniform("lightDef.Ks", 0.10f, 0.1f, 0.10f); // What elements does Kd have?
	prog.setUniform("lightDef.Kd", 0.51f, 1.0f, 0.49f); // What elements does Kd have?
	prog.setUniform("lightDef.Ka", 0.51f, 1.0f, 0.49f); // What elements does Kd have?
	//Set the Teapot material properties in the shader and render
	prog.setUniform("lightDef.Ks", 0.29f, 0.29f, 0.29f); // What elements does Kd have?
	prog.setUniform("lightDef.Kd", 0.46f, 0.29f, 0.00f); // What elements does Kd have?
	prog.setUniform("lightDef.Ka", 0.46f, 0.29f, 0.00f); // What elements does Kd have?*/

	//setup structures
	planeMaterial.Ks = vec3(0.10f, 0.1f, 0.10f);
	planeMaterial.Kd = vec3(0.51f, 1.0f, 0.49f);
	planeMaterial.Ka = vec3(0.51f, 1.0f, 0.49f);

	teapotMaterial.Ks = vec3(0.29f, 0.29f, 0.29f);
	teapotMaterial.Kd = vec3(0.46f, 0.29f, 0.00f);
	teapotMaterial.Ka = vec3(0.46f, 0.29f, 0.00f);
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Initialise the scene
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::initScene(QuatCamera camera)
{
    //|Compile and link the shader  
	compileAndLinkShader();

    gl::Enable(gl::DEPTH_TEST);

 	//Set up the lighting
	setLightParams(camera);


	//Create the plane to represent the ground
	plane = new VBOPlane(100.0,100.0,100,100);



	//A matrix to move the teapot lid upwards
	glm::mat4 lid = glm::mat4(1.0);
	//lid *= glm::translate(vec3(0.0,1.0,0.0));

	//Create the teapot with translated lid
	teapot = new VBOTeapot(16,lid);


}

/////////////////////////////////////////////////////////////////////////////////////////////
//Update not used at present
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::update( float t )
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Set up the lighting variables in the shader
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::setLightParams(QuatCamera camera)
{

	vec3 worldLight = vec3(10.0f,10.0f,10.0f);
   
	
	prog.setUniform("lightDef.Ls", lightParameters.Ls, lightParameters.Ls, lightParameters.Ls); //set specular intensity
	prog.setUniform("lightDef.Ld", lightParameters.Ld, lightParameters.Ld, lightParameters.Ld); //set defuse intensity
	prog.setUniform("lightDef.La", lightParameters.La, lightParameters.La, lightParameters.La); //set ambient intensity
	
	prog.setUniform("LightPosition", worldLight );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Render the scene
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::render(QuatCamera camera)
{
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	setLightParams(camera); //update the light parameters

	//First deal with the plane to represent the ground

	//Initialise the model matrix for the plane
	model = mat4(1.0f);
	//Set the matrices for the plane although it is only the model matrix that changes so could be made more efficient
    setMatrices(camera);
	//Set the plane's material properties in the shader and render
	prog.setUniform("lightDef.Ks", planeMaterial.Ks); // What elements does Kd have?
	prog.setUniform("lightDef.Kd", planeMaterial.Kd); // What elements does Kd have?
	prog.setUniform("lightDef.Ka", planeMaterial.Ka); // What elements does Kd have?
	plane->render();// what does it do?



	//Now set up the teapot 
	 model = mat4(1.0f);
	 setMatrices(camera);
	 //Set the Teapot material properties in the shader and render
	 prog.setUniform("lightDef.Ks", teapotMaterial.Ks); // What elements does Kd have?
	 prog.setUniform("lightDef.Kd", teapotMaterial.Kd); // What elements does Kd have?
	 prog.setUniform("lightDef.Ka", teapotMaterial.Ka); // What elements does Kd have?
	 teapot->render(); // what does it do?
	
}


/////////////////////////////////////////////////////////////////////////////////////////////
//Send the MVP matrices to the GPU
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::setMatrices(QuatCamera camera)
{

    mat4 mv = camera.view() * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", camera.projection() * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));// What does this line do?
	prog.setUniform("M", model);
	prog.setUniform("V", camera.view() );
	prog.setUniform("P", camera.projection() );
	prog.setUniform("CPos", camera.position());

	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// resize the viewport
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::resize(QuatCamera camera,int w, int h)
{
    gl::Viewport(0,0,w,h);
    width = w;
    height = h;
	camera.setAspectRatio((float)w/h);

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Compile and link the shader
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::compileAndLinkShader()
{
   
	try {
    	prog.compileShader("Shaders/phong.vert");
    	prog.compileShader("Shaders/phong.frag");
    	prog.link();
    	prog.validate();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}

//changing the intensity of light
void SceneDiffuse::changeLightIntensity(ThingToChange type, float value) {
	switch (type) {
		case ThingToChange::AMBIENT_INTENSITY: { //ambient
			lightParameters.La += value;
			prog.setUniform("lightDef.La", lightParameters.La);
			break;
		}
		case ThingToChange::DIFFUSE_INTENSITY: { //diffuse
			lightParameters.Ld += value;
			prog.setUniform("lightDef.Ld", lightParameters.Ld);
			break;
		}
		case ThingToChange::SPECULAR_INTENSITY: { //specular
			lightParameters.Ls += value;
			prog.setUniform("lightDef.Ls", lightParameters.Ls);
			break;
		}
	}
}
//get the light intensity for a type of light
float SceneDiffuse::getLightIntensity(ThingToChange type) {
	switch(type) {
		case ThingToChange::AMBIENT_INTENSITY: return lightParameters.La;
		case ThingToChange::DIFFUSE_INTENSITY: return lightParameters.Ld;
		case ThingToChange::SPECULAR_INTENSITY: return lightParameters.Ls;
		default: {
			std::cout << "[Error] Invalid light type specified: " << type << std::endl;
			return 0.0f;
		}
	}
}

}
