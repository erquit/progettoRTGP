#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

//Let's define a struct so we can control better our material of the object for the Phong Lighting
struct Material{
    float shininess;
};
uniform Material material;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_emission1;
uniform bool useEmission;
uniform bool useSpecular;

//Let's define a struct for each type of Light: Directional, Point, Flashlight (SpotLight)

struct DirLight{
    bool dirlightOn;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight{
    bool pointLightOn;
    bool renderShadows;
    vec3 position;

    //These are use to calculate the fall-off of the point lights
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 16 //Number of lights we will have in our scene, 1 light + 15 emission cubes
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight{
    bool spotlightOn;

    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords; 
uniform vec3 viewPos;

//Shadow stuff
in vec4 FragPosLightSpace;
uniform sampler2D ShadowMap;
//Point light shadow stuff
uniform samplerCube depthMapPoint;
uniform float far_plane;
//Spotlight (Flashlight) shadow stuff
uniform sampler2D spotShadowMap;
uniform mat4 spotLightSpaceMatrix;

//Now we just need to define the function to calculate the impact of each light: Directiona, Point, Flashlight and
// use them to calculate our FragColor; (You can check a structure of the code to follow at the bottom of the code)

//Declaration of the functions (the definition of the function will be under the main):
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 FragPosLightSpace, DirLight light, vec3 normal);
float PointShadowCalculation(vec3 fragPos, vec3 lightPos, vec3 normal);
float SpotLightShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main()
{
    //Properties, we normalize our norms and our viewDir so that they are a unit vector
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    //Now our lighting is composed of: 1) Directional 2) Point Lights 3) SpotLight (Flashlight)

    //Directional
    if(dirLight.dirlightOn){
        vec3 dirLightColor = CalcDirLight(dirLight, norm, viewDir);
        float shadow = ShadowCalculation(FragPosLightSpace, dirLight, norm);
        result += (1.0f - shadow) * dirLightColor;
    }

    //Point Lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        if(pointLights[i].pointLightOn){
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }

    // //Spotlight
    if(spotLight.spotlightOn){
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    }

    //Emission map
    if(useEmission){
        vec3 emission = vec3(texture(texture_emission1, TexCoords));
        result += emission * 3.0f; //Boosting the emission for the hdr check
    }

    //this is our output
    FragColor = vec4(result, 1.0f);

    //Bloom
    float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 0.9f){ //Lower values contribute to more bloom
        BrightColor = vec4(FragColor.rgb, 1.0f);
    }else{
        BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }


}

//Definition of the functions:
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);

    //specular shading 

    //Phong Lighting
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    //Blinn-Phong Lighting
    float spec = 0.0f;
    if(diff > 0.0f){
        vec3 halfwayDir = normalize(lightDir + viewDir); 
        spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);
    }

    // Combining the results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);

    //specular shading Blinn-Phong Lighting
    float spec = 0.0f;
    if(diff > 0.0f){
        vec3 halfwayDir = normalize(lightDir + viewDir); 
        spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);
    }

    //Attenuation (light drop-off)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //combining the results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    
    //Applying the attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 lighting;
    //Point light shadows
    if(light.renderShadows){
        float shadow = PointShadowCalculation(fragPos, light.position, normal);
        lighting = (ambient + (1.0f - shadow) * (diffuse + specular));
    }else{
        lighting = (ambient + diffuse + specular);
    }
    return lighting;
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);

    //specular shading Blinn-Phong Lighting
    float spec = 0.0f;
    if(diff > 0.0f){
        vec3 halfwayDir = normalize(lightDir + viewDir); 
        spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);
    }


    //Attenuation (light drop-off)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Spotlight shadow
    vec4 fragPosSpotSpace = spotLightSpaceMatrix * vec4(fragPos, 1.0);
    float shadow = SpotLightShadowCalculation(fragPosSpotSpace, normal, lightDir);

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = vec3(0.0f);
    if(useSpecular){
        specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    }

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    // Shadow affects only diffuse + specular, not ambient
    return ambient + (1.0 - shadow) * (diffuse + specular);


}


float ShadowCalculation(vec4 FragPosLightSpace, DirLight light, vec3 normal){
    vec3 lightDir = normalize(-light.direction);

    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;
    float closestDepth = texture(ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    //We need to add a bias to fix the shadow acne
    float bias = max(0.05 * (1.0f - dot(normal, lightDir)), 0.005);

    //Check if it's in the shadow or nah, 1.0f =  no shadow, 0.0f = shadow
    //We'll be using PCF, we just render the depthmap from slightly different
    // angles and take the average to "smooth out the shadows and made them less blocky"

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0f){
        shadow = 0.0f;
    }

    return shadow;
}

float PointShadowCalculation(vec3 fragPos, vec3 lightPos, vec3 normal)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    // Bias to avoid shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, normalize(fragToLight))), 0.005);

    // PCF parameters
    float shadow = 0.0;
    float samples = 4.0;
    float offset = 0.1;

    // Step size for the loops
    float step = offset / (samples * 0.5);

    for(float x = -offset; x < offset; x += step)
    {
        for(float y = -offset; y < offset; y += step)
        {
            for(float z = -offset; z < offset; z += step)
            {
                // Sampling the cube map in a slightly offset directon
                float closestDepth = texture(depthMapPoint, fragToLight + vec3(x, y, z)).r;
                // convert back to world units
                closestDepth *= far_plane;

                if(currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }

    // Division total number of samples taken
    shadow /= (samples * samples * samples);

    return shadow;
}

float SpotLightShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // Perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(spotShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // Bias to avoid shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF 3x3
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(spotShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(spotShadowMap,
                            projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

//There are 3 types of lighting that combined make Phong lighting

// 1) Ambient lighting: All the items in the schene have a slight light to them
// We just add a ambientStrenght multiplied by our lightColor and applied it to our objectColor

// 2) Diffuse lighting: One of the sides is gonna be more "lit" (the one facing the light source)
// Here we need to introduce normals (which are just vectors perpendicular to our fragments)
// Using unit vectors we can calculate the Theta of our fragment to our light source (the angle between the normal and the light)
// And using that value we can tell of much of an impact that specific fragments should "light up"

// 3) Specular lighting: The shiny effect (the white ball of light), mainly used for 
//    glassy items and depend on the source's light value
