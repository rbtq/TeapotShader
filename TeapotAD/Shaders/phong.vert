#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

//structure that contains all the data being outputted
out struct OutputData {
    vec3 vertPos; //Vertex position in eye coords
    vec3 N; //Transformed normal
    vec3 lightPos; //Light position in eye coords
};

//structure that contains all the data to be set using prog.setuniform();
uniform struct InputData {
    vec3 LightPosition; // Light position 
    mat3 NormalMatrix; //surface normal matrix
    mat4 M; //Model to be displayed
    mat4 V; //Current camera view
    mat4 P; //current camera projection
};

//structure that contains all the data cached during processing
struct ProcessData {
    mat4 MV; //model transformed by camera view
    vec4 vertexPositionHomogenous; //the vertex position converted to homogenous coords
};

//declare structures
out OutputData outputData;
ProcessData processData;
uniform InputData inputData;


//function that calculates the model transformed by camera view
mat4 calculateMV() {
    return inputData.M * inputData.V;
}

vec4 convertToHomogenous(vec3 value) {
    return vec4(value,1.0f);
}

//function that calculates the vertex position
vec3 calculateVertexPosition() {
    return vec3(processData.MV * processData.vertexPositionHomogenous);
}

//function that calculates the light sources position
vec3 calculateLightPosition() {
    return vec3(processData.MV * vec4(inputData.LightPosition,1.0));
}

//function that calculates the vertex normal
vec3 calculateVertexNormal() {
    return normalize( inputData.NormalMatrix * VertexNormal);
}
//function that sets the position of the model on the screen
void setModelPositionOnScreen() {
    gl_Position = inputData.P * processData.MV * processData.vertexPositionHomogenous;
}

void main()
{
   //calculate data used multiple times
   processData.MV = calculateMV();
   processData.vertexPositionHomogenous = convertToHomogenous(VertexPosition);

   outputData.vertPos = calculateVertexPosition(); 
     
   outputData.lightPos = calculateLightPosition();  

   outputData.N = calculateVertexNormal();
      
   setModelPositionOnScreen();
}
