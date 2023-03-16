#ifndef SCENEDIFFUSE_H
#define SCENEDIFFUSE_H

#include "gl_core_4_3.hpp"

#include <glfw3.h>
#include "scene.h"
#include "glslprogram.h"

#include "vboteapot.h"
#include "vboplane.h"

#include <glm.hpp>

using glm::mat4;
//The value to increase with the +- keys
enum ThingToChange { AMBIENT_INTENSITY, DIFFUSE_INTENSITY, SPECULAR_INTENSITY };


namespace imat2908
{

//structure for light parameters
struct LightParams {
public:
    float Ls = 0.3f;
    float Ld = 0.9f;
    float La = 0.3f;

};

//structure for material parameters
struct MaterialParams {
public:
    vec3 Ks = vec3(0);
    vec3 Kd = vec3(0);
    vec3 Ka = vec3(0);
};


class SceneDiffuse : public Scene
{
private:
    GLSLProgram prog;

    int width, height;
 
	VBOTeapot *teapot;  //Teapot VBO
	VBOPlane *plane;  //Plane VBO
    LightParams lightParameters; //!<stores the parameters for the light
    MaterialParams planeMaterial; //!<stores parameters for the plane
    MaterialParams teapotMaterial; //!<stores parameters for the teapot

    mat4 model; //Model matrix

    //light intensity


    void setMatrices(QuatCamera camera); //Set the camera matrices

    void compileAndLinkShader(); //Compile and link the shader

public:
    SceneDiffuse(); //Constructor

	void setLightParams(QuatCamera camera); //Setup the lighting

    void initScene(QuatCamera camera); //Initialise the scene

    void update( float t ); //Update the scene

    void render(QuatCamera camera);	//Render the scene

    void resize(QuatCamera camera, int, int); //Resize

    void changeLightIntensity(ThingToChange type, float value); //!<change the light intensity of a type of light
    float getLightIntensity(ThingToChange type); //!<get the current light intensity for a type of light

    
};

}

#endif // SCENEDIFFUSE_H
