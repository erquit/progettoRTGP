#version 330 core

layout (location = 0) in vec3 aPos;
//We introduce normals
layout (location = 1) in vec3 aNormal;
//Let's add our texture coords
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//We need to propagate our normals to our fragment shader since all the calculation will occure there
out vec3 Normal;

//We need to pass the fragPos in the world coordinates for the diffused lighting 
out vec3 FragPos;

//We need to forward our aTexCoords to the fragment shader
out vec2 TexCoords;

//Shadow transformation and output
uniform mat4 lightSpaceMatrix;
out vec4 FragPosLightSpace;


void main()
{
    //Passing our texture coordinates to the fragment shader
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0f));

    //Propagating the normals
    Normal = mat3(transpose(inverse(model))) * aNormal; //We use this one for non uniform and uniform scaling 

    //Shadow stuff and propagation
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0f);

    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    //NOTE FOR THE FUTURE: calculating the inverse matrix is really heavy so it's best to do it on the CPU and send to the shaders
    // the done calculation to save on resources 


}

