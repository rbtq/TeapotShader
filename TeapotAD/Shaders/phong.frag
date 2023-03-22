#version 430

//structure that contains info on a point on the surface
in struct LightData {
    vec3 vertPos;       //vertex position
    vec3 N;             //vertex normal
    vec3 lightPos;      //light position
    /*TODO:: Complete your shader code for a full Phong shading*/ 
};

//structure that defines a material
uniform struct MaterialDefinition {
    vec3 Ks;            // Specular reflectivity - colour
    vec3 Kd;            // Diffuse reflectivity - colour
    vec3 Ka;            // Ambient reflectivity - colour
    float Lsp;          // Specular light power
};

//structure that defines a light source
uniform struct LightDefinition {
    vec3 Ls;            // Specular light intensity - light source colour
    vec3 Ld;            // Diffuse light intensity - light source colour
    vec3 La;            // Ambient light intensity
};

//structure that defines a camera
uniform struct CameraDefinition {
    vec3 CPos; //camera position
};

//declare these structures
uniform LightDefinition lightDef;
uniform MaterialDefinition materialDef;
uniform CameraDefinition cameraDef;
in LightData lightData;

// complete to a full phong shading
layout( location = 0 ) out vec4 FragColour;


//calculate ambient light intensity
vec3 calculateAmbient() {
    return materialDef.Ka * lightDef.La;
}
//calculate diffuse light intensity
vec3 calculateDiffuse(vec3 L) {
   //calculate Diffuse Light Intensity making sure it is not negative and is clamped 0 to 1  
   return clamp(materialDef.Kd*lightDef.Ld* max(dot(lightData.N,L), 0.0), 0.0, 1.0);
}
//calculate specular light intensity
vec3 calculateSpecular(vec3 L) {
   //calculate specular
   return clamp(materialDef.Ks * lightDef.Ls * pow(dot(normalize(cameraDef.CPos - lightData.vertPos), reflect(-L, lightData.N)),materialDef.Lsp),0.0,1.0);
}
//calculate the normalised light vector - the distance from the point to the source
vec3 calculateLightVector() {
    return normalize(lightData.lightPos - lightData.vertPos);  //Calculating the normal between the teapot surface and the light source
}

//this is the light shader for a single point and is repeated
void main() {

   //Calculate the light vector
   vec3 L = calculateLightVector();

   //calculate the light intensity
   FragColour = vec4(calculateAmbient() + calculateDiffuse(L) + calculateSpecular(L), 1.0);

}

