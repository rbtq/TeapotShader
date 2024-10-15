#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

//structure that contains all the data being outputted
out struct LightData {
    vec3 vertPos;  //Vertex position in eye coords
    vec3 N;        //unit normal from the point to the source
    vec3 lightPos; //Light position in eye coords
};

//structure that contains all the data to be set using prog.setuniform();
uniform struct InputData {
    vec3 LightPosition; //Light source position 
    mat3 NormalMatrix; //surface normal matrix
    mat4 M; //Model to be displayed
    mat4 V; //Current camera view
    mat4 P; //current camera projection
};

//structure that contains all the data cached during processing
struct ProcessData {
    mat4 VM; //model transformed by camera view
    vec4 vertexPositionHomogenous; //the vertex position converted to homogenous coords
};

//declare structures
out LightData lightData;
ProcessData processData;
uniform InputData inputData;


//function that calculates the model transformed by camera view
mat4 calculateVM() {
    return inputData.V * inputData.M;
}
//function that converts a vector into a homogenous vector for matrix multiplication
vec4 convertToHomogenous(vec3 value) {
    return vec4(value,1.0f);
}
//function that calculates the vertex position
vec3 calculateVertexPosition() {
    return vec3(processData.VM * processData.vertexPositionHomogenous);
}
//function that calculates the light sources position
vec3 calculateLightPosition() {
    return vec3(processData.VM * vec4(inputData.LightPosition,1.0));
}
//function that calculates the vertex normal
vec3 calculateVertexNormal() {
    return normalize( inputData.NormalMatrix * VertexNormal);
}
//function that sets the position of the model on the screen
void setModelPositionOnScreen() {
    gl_Position = inputData.P * processData.VM * processData.vertexPositionHomogenous;
}

void main()
{
   //calculate data used multiple times
   processData.VM = calculateVM();
   processData.vertexPositionHomogenous = convertToHomogenous(VertexPosition);

   //calculate everything
   lightData.vertPos = calculateVertexPosition();      
   lightData.lightPos = calculateLightPosition();  
   lightData.N = calculateVertexNormal();
   //set the models position on-screen
   setModelPositionOnScreen();
}
