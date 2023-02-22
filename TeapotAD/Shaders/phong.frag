#version 430

in vec3 vertPos;
in vec3 N; //normal
in vec3 lightPos;
/*TODO:: Complete your shader code for a full Phong shading*/ 

uniform vec3 Kd;            // Diffuse reflectivity - colour
uniform vec3 Ld;            // Diffuse light intensity - light source colour
uniform vec3 La;            // Ambient light intensity
uniform vec3 Ka;            // Ambient reflectivity - colour

// complete to a full phong shading
layout( location = 0 ) out vec4 FragColour;


//this is the light shader for a single point and is repeated
void main() {

    //calculate ambient light intensity
    vec3 ambient = Ka * La;

   //------- Diffuse lighting ----------//
   //Calculate the light vector
   vec3 L = normalize(lightPos - vertPos);  //Calculating the normal between the teapot surface and the light source
    
   //calculate Diffuse Light Intensity making sure it is not negative and is clamped 0 to 1  
   vec4 Id = vec4(Ld,1.0) * max(dot(N,L), 0.0);// For transformations
   Id = clamp(Id, 0.0, 1.0); // Clamp ensures the result is between 0 and 1 (in this case) 

   //Multiply the Reflectivity by the Diffuse intensity
   FragColour = vec4(ambient, 1.0) + vec4(Kd, 1.0) * Id;

}
