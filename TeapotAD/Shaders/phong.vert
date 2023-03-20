#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;


out struct LightData {
    vec3 vertPos; //Vertex position in eye coords
    vec3 N; //Transformed normal
    vec3 lightPos; //Light position in eye coords
};



uniform vec3 LightPosition; // Light position 
uniform mat3 NormalMatrix;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out LightData lightData;

void main()
{
   
   lightData.vertPos = vec3(V * M * vec4(VertexPosition,1.0)); 
     
   lightData.lightPos = vec3(V * M * vec4(LightPosition,1.0));  

   lightData.N = normalize( NormalMatrix * VertexNormal);
      
   gl_Position = gl_Position = P * V * M * vec4(VertexPosition,1.0);
}
