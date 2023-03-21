#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

//structure that contains all the data being outputted
out struct LightData {
    vec3 vertPos; //Vertex position in eye coords
    vec3 N; //Transformed normal
    vec3 lightPos; //Light position in eye coords
};

//structure that contains all the data to be set using prog.setuniform();
uniform struct InputData {
    vec3 LightPosition; // Light position 
    mat3 NormalMatrix; //surface normal matrix
    mat4 M;
    mat4 V;
    mat4 P;
};

//declare structures
out LightData lightData;
uniform InputData inputData;

void main()
{
   
   lightData.vertPos = vec3(inputData.V * inputData.M * vec4(VertexPosition,1.0)); 
     
   lightData.lightPos = vec3(inputData.V * inputData.M * vec4(inputData.LightPosition,1.0));  

   lightData.N = normalize( inputData.NormalMatrix * VertexNormal);
      
   gl_Position = gl_Position = inputData.P * inputData.V * inputData.M * vec4(VertexPosition,1.0);
}
