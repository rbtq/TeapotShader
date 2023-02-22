#version 430

in vec3 vertPos;
in vec3 N; //normal
in vec3 lightPos;
/*TODO:: Complete your shader code for a full Phong shading*/ 

uniform vec3 Ks;            // Specular reflectivity - colour
uniform vec3 Kd;            // Diffuse reflectivity - colour
uniform vec3 Ka;            // Ambient reflectivity - colour

uniform vec3 Ls;            // Specular light intensity - light source colour
uniform vec3 Ld;            // Diffuse light intensity - light source colour
uniform vec3 La;            // Ambient light intensity

uniform vec3 CPos;

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
   vec3 D = Kd*Ld* max(dot(N,L), 0.0); // For transformations
   D = clamp(D, 0.0, 1.0);

   //---Specular lighting----//
   vec3 lookVec = normalize(CPos - vertPos);
   vec3 lightReflectVec = reflect(-L, N);
   float viewAngle = dot(lookVec, lightReflectVec);
   vec3 specular = clamp(Ks * Ls * pow(viewAngle,2),0.0,1.0);



   FragColour = vec4(ambient + D + specular, 1.0);

}

