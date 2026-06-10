#include "graphics.hpp"

#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/utils/DebugRenderer.h>
#include <vector>
#include <cstdint>

using namespace reactphysics3d;

void setupShader(Shader shader, glm::vec3 pointLightPositions[], int numPointLights, glm::vec3 emissionPointLightPositions[], int numEmissionPointLights){

    //Let's activate the shader so that we can send stuff to it
    shader.use();

    //DirLight
    //Let's tell if we want to use the dirLight (Ambient Light) or not
    shader.setBool("dirLight.dirlightOn", ambientlightOn);

    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    //PointLights
    //We set up our upper light and our emission cubes in the dark room
    int origLightCount = 1;  // Upper floor light
    for (int i = 0; i < origLightCount; i++)
    {
        std::string base = "pointLights[" + std::to_string(i) + "]";
        shader.setBool(base + ".pointLightOn", pointlightOn); 
        shader.setBool(base + ".renderShadows", true);   // we tell if we want to render the shadows or not
        shader.setVec3(base + ".position", pointLightPositions[i]);
        shader.setVec3(base + ".ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3(base + ".diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3(base + ".specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat(base + ".constant", 1.0f);
        shader.setFloat(base + ".linear", 0.01f);
        shader.setFloat(base + ".quadratic", 0.0032f);
    }

    //Green emission cube lights
    int emissionStartIndex = origLightCount;
    for (int i = 0; i < numEmissionPointLights; i++)
    {
        int index = emissionStartIndex + i;
        std::string base = "pointLights[" + std::to_string(index) + "]";

        shader.setBool(base + ".pointLightOn", true);
        shader.setBool(base + ".renderShadows", false);   // we tell if we want to render the shadows or not
        shader.setVec3(base + ".position", emissionPointLightPositions[i]);
        shader.setVec3(base + ".ambient",  0.0f, 0.02f, 0.0f);
        shader.setVec3(base + ".diffuse",  0.2f, 1.0f,  0.2f);
        shader.setVec3(base + ".specular", 0.3f, 0.3f,  0.3f);
        shader.setFloat(base + ".constant",  1.0f);
        shader.setFloat(base + ".linear",    0.7f);
        shader.setFloat(base + ".quadratic", 1.8f);
    }

    //SpotLight
    //Let's tell the shader if we using the ambient and spotlight or not
    shader.setBool("spotLight.spotlightOn", spotlightOn);
    shader.setVec3("spotLight.position", camera.Position);
    shader.setVec3("spotLight.direction", camera.Front);
    shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));   

    shader.setVec3("viewPos", camera.Position);

    //material properties
    shader.setFloat("material.shininess", 256.0f);

    //We tell which Sampler2D is used in this case the 2 one cuz 0 and 1 is already used
    shader.setInt("material.diffuse", 2); 
    shader.setInt("material.specular", 3); 
    shader.setInt("material.emission", 4);
}

void renderCubesAndPlane(Shader shader, glm::vec3 cubePositions[], Mesh cubeMesh, 
    Mesh planeMesh, int numCubes, const std::vector<glm::mat4>& cubeModels, const std::vector<bool>& isBroken) {
    for(int i = 0; i < numCubes; i++){
        if (isBroken[i]) continue;
        shader.setMat4("model", cubeModels[i]);
        cubeMesh.Draw(shader);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
    shader.setMat4("model", model);
    planeMesh.Draw(shader);
}


void renderModels(Shader shader, glm::mat4& vaseModel, Model ourModel){

    //Let's draw our model
    shader.setMat4("model", vaseModel);
    ourModel.Draw(shader);
}

void renderScenePointLights(Shader shader,glm::vec3 pointLightPositions[],
    int numLights, glm::mat4 view, glm::mat4 projection, Mesh lightCubeMesh){

    //Light cubes
    if(pointlightOn){
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Light cube source, we define our model trasfromation to move and scale it, while
        // using the camera projection and view to move everyting around and stuff is define above
        for(int i = 0; i < numLights; i++)
        {
            //model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            shader.setMat4("model", model);
            lightCubeMesh.Draw(shader);
        }
    }


}

void renderSceneSkyBox(Shader shader, unsigned int skyboxVAO, unsigned int cubemapTexture){
    // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
    glDepthFunc(GL_LEQUAL);


    shader.use();
    // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
    // The last row and column affect the translation of the skybox (which we don't want to affect)
    glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up)));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/ WINDOW_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);
}


//Some bullshit to render the depth map
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// Utility function for loading a 2D texture from file
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Let's define our Level of Detail
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -2.0f);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void renderDebugPhysics(reactphysics3d::DebugRenderer& debugRenderer, Shader& debugShader,
                        const glm::mat4& view, const glm::mat4& projection) {

    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_POINT, true);
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    unsigned int nbLines = debugRenderer.getNbLines();

    if (nbLines == 0) return;

    debugShader.use();
    debugShader.setMat4("view", view);
    debugShader.setMat4("projection", projection);

    const reactphysics3d::DebugRenderer::DebugLine* lines = debugRenderer.getLinesArray();

    struct LineVertex { glm::vec3 pos; glm::vec3 color; };
    std::vector<LineVertex> vertices;
    vertices.reserve(nbLines * 2);

    for (unsigned int i = 0; i < nbLines; ++i) {
        const auto& line = lines[i];
        auto unpack = [](uint32_t c) -> glm::vec3 {
            // ReactPhysics3D stores color as 0xRRGGBBAA
            // This is just a fancy way to render lines with reactPhysics3d, i took it from the documentation
            float r = ((c >> 24) & 0xFF) / 255.0f;
            float g = ((c >> 16) & 0xFF) / 255.0f;
            float b = ((c >>  8) & 0xFF) / 255.0f;
            return glm::vec3(r, g, b);
        };
        vertices.push_back({glm::vec3(line.point1.x, line.point1.y, line.point1.z), unpack(line.color1)});
        vertices.push_back({glm::vec3(line.point2.x, line.point2.y, line.point2.z), unpack(line.color2)});
    }

    static unsigned int VAO = 0, VBO = 0;
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, color));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(LineVertex), vertices.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_LINES, 0, vertices.size());
    glBindVertexArray(0);
}



void renderBrokenModel(Shader shader, glm::mat4 modelPosition[], std::vector<Model> fragmentsModel){
    for (unsigned int i = 0; i < fragmentsModel.size(); i++)
    {
        renderModels(shader, modelPosition[i], fragmentsModel[i]);
    }
}


void renderCrosshair(Shader crosshairShader, unsigned int crosshairVAO){
    //Let's draw the crosshair
    glDisable(GL_DEPTH_TEST);
    crosshairShader.use();
    crosshairShader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
    glBindVertexArray(crosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}