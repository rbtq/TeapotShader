#version 430

//structure that contains info on a point on the surface
in struct LightData {
    vec3 vertPos;
    vec3 N; //vertex normal
    vec3 lightPos;
    /*TODO:: Complete your shader code for a full Phong shading*/ 
};
uniform vec3 CPos; //camera position

//structure that defines a material
uniform struct MaterialDefinition {
    vec3 Ks;            // Specular reflectivity - colour
    vec3 Kd;            // Diffuse reflectivity - colour
    vec3 Ka;            // Ambient reflectivity - colour
};

//structure that defines a light source
uniform struct LightDefinition {
    vec3 Ls;            // Specular light intensity - light source colour
    vec3 Ld;            // Diffuse light intensity - light source colour
    vec3 La;            // Ambient light intensity
    float Lsp;   // Specular light power
};

//declare these structures
uniform LightDefinition lightDef;
uniform MaterialDefinition materialDef;
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
   return clamp(materialDef.Ks * lightDef.Ls * pow(dot(normalize(CPos - lightData.vertPos), reflect(-L, lightData.N)),lightDef.Lsp),0.0,1.0);
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

