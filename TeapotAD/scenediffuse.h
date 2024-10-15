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


namespace imat2908
{
//enum that contains everything that can be changed
enum WhatToChange { AMBIENT_INTENSITY, DIFFUSE_INTENSITY, SPECULAR_INTENSITY, 
    TEAPOT_KA_R, TEAPOT_KA_G, TEAPOT_KA_B,
    TEAPOT_KD_R, TEAPOT_KD_G, TEAPOT_KD_B,
    TEAPOT_KS_R, TEAPOT_KS_G, TEAPOT_KS_B,
    BACKGROUND_KA_R, BACKGROUND_KA_G, BACKGROUND_KA_B,
    BACKGROUND_KD_R, BACKGROUND_KD_G, BACKGROUND_KD_B,
    BACKGROUND_KS_R, BACKGROUND_KS_G, BACKGROUND_KS_B,
    POWER_KS_TEAPOT, POWER_KS_BACKGROUND, TOON_SHADE_COUNT, LIGHT_POSITION
};
//class for selecting what to change
class ThingToChange {
private:
    //The value to increase with the +- keys
    unsigned int toChangeInt = 0U;
    WhatToChange toChange = WhatToChange::AMBIENT_INTENSITY;

public:
    //operator functions to change what to change
    void operator++(int) {
        if (toChangeInt != 24) {
            toChangeInt++;
            toChange = (WhatToChange)toChangeInt;
        }
    }
    void operator--(int) {
        //can only decrease to zero at lowest
        if (toChangeInt > 0) {
            toChangeInt--;
            toChange = (WhatToChange)toChangeInt;
        }
    }
    //getter function
    WhatToChange getWhatToChange() { return toChange; };

};

//structure for light parameters
struct LightParams {
public:
    float Ls = 0.3f; //intensity for specular light
    float Ld = 0.9f; //intensity for diffuse light
    float La = 0.3f; //intenstity for ambient
    int numberOfShades = 50; //number of shades for toon shading
    vec3 distance = vec3(10.0f); //distance from 0,0,0 the source is

};

//structure for material parameters
struct MaterialParams {
public:
    vec3 Ks = vec3(0); //specular reflectivity
    vec3 Kd = vec3(0); //diffuser reflectivity
    vec3 Ka = vec3(0); //ambient reflectivity
    float Lsp = 0.0f; //specular shinyness factor
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
    bool isUsingPhong; //!<stores if phong shading or toon shading should be used

    mat4 model; //Model matrix

    //light intensity


    void setMatrices(QuatCamera camera); //Set the camera matrices

    void compileAndLinkShader(); //Compile and link the shader

    void renderObject(QuatCamera camera, Drawable* object, MaterialParams materialDef); //renders an item to the screen

public:
    SceneDiffuse(); //Constructor

	void setLightParams(QuatCamera camera); //Setup the lighting

    void initScene(QuatCamera camera); //Initialise the scene

    void update( float t ); //Update the scene

    void render(QuatCamera camera);	//Render the scene

    void resize(QuatCamera camera, int, int); //Resize

    void changeValue(WhatToChange type, float value); //!<change the light intensity of a type of light
    float getValue(WhatToChange type); //!<get the current light intensity for a type of light
    void printSceneValues(); //!<print all scene values
    void togglePhong() { 
        isUsingPhong = !isUsingPhong; 
        prog.setUniform("isPhong", isUsingPhong);
        if (isUsingPhong) {
            std::cout << "Phong shader enabled!" << std::endl;
        }
        else {
            std::cout << "Toon shader enabled!" << std::endl;
        }
    }; //switch between toon and phong shaders

    
};

}

#endif // SCENEDIFFUSE_H
