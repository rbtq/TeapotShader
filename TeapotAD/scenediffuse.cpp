
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
	prog.setUniform("lightDef.Lsp", lightParameters.Lsp); //set specular power
	
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
	prog.setUniform("materialDef.Ks", planeMaterial.Ks); // What elements does Kd have?
	prog.setUniform("materialDef.Kd", planeMaterial.Kd); // What elements does Kd have?
	prog.setUniform("materialDef.Ka", planeMaterial.Ka); // What elements does Kd have?
	plane->render();// what does it do?



	//Now set up the teapot 
	 model = mat4(1.0f);
	 setMatrices(camera);
	 //Set the Teapot material properties in the shader and render
	 prog.setUniform("materialDef.Ks", teapotMaterial.Ks); // What elements does Kd have?
	 prog.setUniform("materialDef.Kd", teapotMaterial.Kd); // What elements does Kd have?
	 prog.setUniform("materialDef.Ka", teapotMaterial.Ka); // What elements does Kd have?
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
void SceneDiffuse::changeLightIntensity(WhatToChange type, float value) {
	switch (type) {
		case WhatToChange::AMBIENT_INTENSITY: { //ambient
			lightParameters.La += value;
			break;
		}
		case WhatToChange::DIFFUSE_INTENSITY: { //diffuse
			lightParameters.Ld += value;
			break;
		}
		case WhatToChange::SPECULAR_INTENSITY: { //specular
			lightParameters.Ls += value;
			break;
		}
		case WhatToChange::BACKGROUND_KA_R: { //background color red ambient
			planeMaterial.Ka.x += value;
			break;
		}
		case WhatToChange::BACKGROUND_KA_G: { //background color green ambient
			planeMaterial.Ka.y += value;
			break;
		}
		case WhatToChange::BACKGROUND_KA_B: { //background color blue ambient
			planeMaterial.Ka.z += value;
			break;
		}
		case WhatToChange::BACKGROUND_KD_R: { //background color red diffuse
			planeMaterial.Kd.x += value;
			break;
		}
		case WhatToChange::BACKGROUND_KD_G: { //background color green diffuse
			planeMaterial.Kd.y += value;
			break;
		}
		case WhatToChange::BACKGROUND_KD_B: { //background color blue diffuse
			planeMaterial.Kd.z += value;
			break;
		}
		case WhatToChange::BACKGROUND_KS_R: { //background color red specular
			planeMaterial.Ks.z += value;
			break;
		}
		case WhatToChange::BACKGROUND_KS_G: { //background color green specular
			planeMaterial.Ks.z += value;
			break;
		}
		case WhatToChange::BACKGROUND_KS_B: { //background color blue specular
			planeMaterial.Ks.z += value;
			break;
		}
		case WhatToChange::TEAPOT_KA_R: { //teapot color red ambient
			teapotMaterial.Ka.x += value;
			break;
		}
		case WhatToChange::TEAPOT_KA_G: { //teapot color green ambient
			teapotMaterial.Ka.y += value;
			break;
		}
		case WhatToChange::TEAPOT_KA_B: { //teapot color blue ambient
			teapotMaterial.Ka.z += value;
			break;
		}
		case WhatToChange::TEAPOT_KD_R: { //teapot color red diffuse
			teapotMaterial.Kd.x += value;
			break;
		}
		case WhatToChange::TEAPOT_KD_G: { //teapot color green diffuse
			teapotMaterial.Kd.y += value;
			break;
		}
		case WhatToChange::TEAPOT_KD_B: { //teapot color blue diffuse
			teapotMaterial.Kd.z += value;
			break;
		}
		case WhatToChange::TEAPOT_KS_R: { //teapot color red specular
			teapotMaterial.Ks.z += value;
			break;
		}
		case WhatToChange::TEAPOT_KS_G: { //teapot color green specular
			teapotMaterial.Ks.z += value;
			break;
		}
		case WhatToChange::TEAPOT_KS_B: { //teapot color blue specular
			teapotMaterial.Ks.z += value;
			break;
		}
		case WhatToChange::POWER_KS: { //power for specular
			lightParameters.Lsp += value;
			break;
		}
	}
}
//get the light intensity for a type of light
float SceneDiffuse::getValue(WhatToChange type) {
	switch(type) {
		case WhatToChange::AMBIENT_INTENSITY: return lightParameters.La;
		case WhatToChange::DIFFUSE_INTENSITY: return lightParameters.Ld;
		case WhatToChange::SPECULAR_INTENSITY: return lightParameters.Ls;
		case WhatToChange::BACKGROUND_KA_R: return planeMaterial.Ka.x;
		case WhatToChange::BACKGROUND_KA_G: return planeMaterial.Ka.y;
		case WhatToChange::BACKGROUND_KA_B: return planeMaterial.Ka.z;
		case WhatToChange::BACKGROUND_KD_R: return planeMaterial.Kd.x;
		case WhatToChange::BACKGROUND_KD_G: return planeMaterial.Kd.y;
		case WhatToChange::BACKGROUND_KD_B: return planeMaterial.Kd.z;
		case WhatToChange::BACKGROUND_KS_R: return planeMaterial.Ks.x;
		case WhatToChange::BACKGROUND_KS_G: return planeMaterial.Ks.y;
		case WhatToChange::BACKGROUND_KS_B: return planeMaterial.Ks.z;
		case WhatToChange::TEAPOT_KA_R: return teapotMaterial.Ka.x;
		case WhatToChange::TEAPOT_KA_G: return teapotMaterial.Ka.y;
		case WhatToChange::TEAPOT_KA_B: return teapotMaterial.Ka.z;
		case WhatToChange::TEAPOT_KD_R: return teapotMaterial.Kd.x;
		case WhatToChange::TEAPOT_KD_G: return teapotMaterial.Kd.y;
		case WhatToChange::TEAPOT_KD_B: return teapotMaterial.Kd.z;
		case WhatToChange::TEAPOT_KS_R: return teapotMaterial.Ks.x;
		case WhatToChange::TEAPOT_KS_G: return teapotMaterial.Ks.y;
		case WhatToChange::TEAPOT_KS_B: return teapotMaterial.Ks.z;
		case WhatToChange::POWER_KS: return lightParameters.Lsp;
		default: {
			std::cout << "[Error] Invalid light type specified: " << type << std::endl;
			return 0.0f;
		}
	}
}

}
