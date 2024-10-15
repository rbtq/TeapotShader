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
uniform bool isPhong; //enables the switching between phong and toon shaders
uniform int toonShaderShades; //stores the number of shades you can have in toon shader
in LightData lightData;

// complete shading
layout( location = 0 ) out vec4 FragColour;

//calculate the intensity of the diffuse light based off the direction of the light and the surface normal
float calculateDiffuseToon(vec3 L) {
    return max(dot(lightData.N,L), 0.0);
}
//calculate specular light intensity
float calculateSpecularToon(vec3 L) {
    return clamp(pow(dot(normalize(cameraDef.CPos - lightData.vertPos), reflect(-L, lightData.N)),materialDef.Lsp),0.0f,1.0f);
}

//calculate ambient light intensity - works for both toon and phong
vec3 calculateAmbient() {
    return materialDef.Ka * lightDef.La;
}
//calculate diffuse light intensity
vec3 calculateDiffusePhong(vec3 L) {
   //calculate Diffuse Light Intensity making sure it is not negative and is clamped 0 to 1  
   return clamp(materialDef.Kd*lightDef.Ld * calculateDiffuseToon(L), 0.0, 1.0);
}
//calculate specular light intensity
vec3 calculateSpecularPhong(vec3 L) {
   //calculate specular
   return clamp(materialDef.Ks * lightDef.Ls * calculateSpecularToon(L),0.0f,1.0f);
}
//calculate the normalised light vector - the distance from the point to the source
vec3 calculateLightVector() {
    return normalize(lightData.lightPos - lightData.vertPos);  //Calculating the normal between the teapot surface and the light source
}

//this is the light shader for a single point and is repeated
void main() {
   //Calculate the light vector
   vec3 L = calculateLightVector();
   if (isPhong) {
       //combine all 3 parts of the phong shader
       FragColour = vec4(calculateAmbient() + calculateDiffusePhong(L) + calculateSpecularPhong(L), 1.0f);
   }
   else {
       //light colour is white so intensity is the same for r g and b (x y and z respectively)
       float intensity = round((lightDef.La.x + lightDef.Ld.x * calculateDiffuseToon(L) + lightDef.Ls.x * calculateSpecularToon(L)) * toonShaderShades)/toonShaderShades;
       FragColour = vec4(intensity * materialDef.Ka, 1.0f);
   }
}

