#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "global.hpp"
#include "myShader.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include <glad/glad.h>
#include "Textures/Headers/stb_image.h"

void setupShader(Shader shader, glm::vec3 pointLightPositions[], int numPointLights, 
    glm::vec3 emissionPointLightPositions[], int numEmissionPointLights);
void renderSceneSkyBox(Shader shader, unsigned int skyboxVAO, unsigned int cubemapTexture);
void renderScenePointLights(Shader shader,glm::vec3 pointLightPositions[],
    int numLights, glm::mat4 view, glm::mat4 projection, Mesh lightCubeMesh);
void renderQuad();
unsigned int loadTexture(char const * path);

void renderCubesAndPlane(Shader shader, glm::vec3 cubePositions[], Mesh cubeMesh, 
    Mesh planeMesh, int numCubes, const std::vector<glm::mat4>& cubeModels, const std::vector<bool>& isBroken);

void renderDebugPhysics(reactphysics3d::DebugRenderer& debugRenderer, 
    Shader& debugShader, const glm::mat4& view, const glm::mat4& projection);
void renderModels(Shader shader, glm::mat4& modelPosition, Model ourModel);
void renderBrokenModel(Shader, glm::mat4[], std::vector<Model> );
void renderCrosshair(Shader crosshairShader, unsigned int crosshairVAO);

#endif

