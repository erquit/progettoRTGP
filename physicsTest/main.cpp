#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <fstream>

//GLFW stuff
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Let's include our assimp (Model loader)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>
//We also need the mesh.hpp and model.hpp, model.hpp already includes mesh.hpp so it's fine not to include it here
#include "model.hpp"
#include "vertices.hpp"

#include "global.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "rayCaster.hpp"
#include "combinedListener.hpp"

using namespace reactphysics3d;

int main(){
    srand(static_cast<unsigned>(time(nullptr)));

    //Initializing and setting Version and Core profile of GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, samples);

    //Let's add a timer and the framecount for the fps counter:
    double prevTime = 0.0f;
    double currentTime = 0.0f;
    double timeDiff;
    unsigned int counter = 0;

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", NULL, NULL);
    if(window == NULL){
        std::cout << "BAD - Failed to create Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Let's make our windows the current context and add a function for dinamic resize
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Let's unlock the fps, to lock them back to the monitors refresh rate just put 1 instead of 0
    glfwSwapInterval(0);

    //Let's load GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "BAD - Failed to load GLAD" << std::endl;
        return -1;
    }
    //Let's define that OpenGL has to do Depth Test
    glEnable(GL_DEPTH_TEST);
    //Let's active the multi sampling MSAA
    glEnable(GL_MULTISAMPLE);
    //Also lets tell our application that it has to capture the mouse like in the games
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //Variables for the mouse_callback
    lastX = WINDOW_WIDTH/2;
    lastY = WINDOW_HEIGHT/2;

    //Also we need to register the callback function for the fov (zooming)
    glfwSetScrollCallback(window, scroll_callback);


    //-----------------------------------LOADING OF MODELS-------------------------

    stbi_set_flip_vertically_on_load(false);
    Model vaseIntact("Models/vase_reduced/vase/vase.obj");
    std::vector <Model> vaseFragments;
    for (int i = 0; i < 5; i++)
    {
        Model fragment("Models/vase_reduced/vase_fragments/frag_0" + std::to_string(i) + ".obj");
        vaseFragments.push_back(fragment);
    }

    Model maoiStatue("Models/maoi_statue/intact/model.obj");
    std::vector <Model> maoiFragments;

    for (int i = 0; i < 5; i++)
    {
        Model fragment("Models/maoi_statue/fragments/fragment" + std::to_string(i) + ".obj");
        maoiFragments.push_back(fragment);
    }
    
    stbi_set_flip_vertically_on_load(true);

    //-------------------------TEXTURES----------------------------

    unsigned int diffuseMap = loadTexture("Textures/container2.png");
    unsigned int blackdiffuseMap = loadTexture("Textures/blackcontainer2.png");
    unsigned int specularMap = loadTexture("Textures/container2_specular.png");
    unsigned int emissionMap = loadTexture("Textures/matrix.jpg");
    unsigned int diffusePlaneMap= loadTexture("Textures/Plane/brick1Color.png");
    unsigned int normalsPlaneMap= loadTexture("Textures/Plane/brick1Normal.png");

    std::vector<Texture> noTextures;
    std::vector<Texture> cubeTextures;
    std::vector<Texture> cubeEmissionTextures;
    std::vector<Texture> planeTextures;


    //Normal cube
    Texture diffuseTex;
    diffuseTex.id = diffuseMap;
    diffuseTex.type = "texture_diffuse";
    diffuseTex.path = "Textures/container2.png";
    cubeTextures.push_back(diffuseTex);

    Texture specularTex;
    specularTex.id = specularMap;
    specularTex.type = "texture_specular";
    specularTex.path = "Textures/container2_specular.png";
    cubeTextures.push_back(specularTex);

    Texture blackdiffuseTex;
    diffuseTex.id = blackdiffuseMap;
    diffuseTex.type = "texture_diffuse";
    diffuseTex.path = "Textures/blackcontainer2.png";

    //Emission cube
    Texture emissionTex;
    emissionTex.id = emissionMap;
    emissionTex.type = "texture_emission";
    emissionTex.path = "Textures/matrix.jpg";
    cubeEmissionTextures.push_back(emissionTex);


    //Plane
    Texture planeDiffuseTex;
    planeDiffuseTex.id = diffusePlaneMap;
    planeDiffuseTex.type = "texture_diffuse";
    planeDiffuseTex.path = "Textures/Plane/brick1Color.png";
    planeTextures.push_back(planeDiffuseTex);

    Texture planeNormalsTex;
    planeNormalsTex.id = normalsPlaneMap;
    planeNormalsTex.type = "texture_normal";
    planeNormalsTex.path = "Textures/Plane/brick1Normal.png";
    planeTextures.push_back(planeNormalsTex);

    //------------------- 1 && 2 && 3 Vertex shader, Fragment shader and Linking (Shader Program)---------------
    Shader lightingShader("Shaders/Lighting/vertexColors.vert", "Shaders/Lighting/fragmentColors.frag");
    Shader lightCubeShader("Shaders/Lighting/vertexLight.vert", "Shaders/Lighting/fragmentLight.frag");
    Shader modelShader("Shaders/Models/models.vert", "Shaders/Models/models.frag");
    Shader skyboxShader("Shaders/Skybox/skybox.vert", "Shaders/Skybox/skybox.frag");
    Shader depthMapShader("Shaders/Shadows/simpleDepthShader.vert", "Shaders/Shadows/simpleDepthShader.frag");
    Shader debugDepthQuad("Shaders/Shadows/Debug/quad.vert", "Shaders/Shadows/Debug/quad.frag");
    Shader debugShader("Shaders/Debug/debug.vert", "Shaders/Debug/debug.frag");
    Shader crosshairShader("Shaders/Crosshair/crosshair.vert", "Shaders/Crosshair/crosshair.frag");

    Shader pointShadowShader("Shaders/Shadows/PointShadow/depthShader.vert",
                        "Shaders/Shadows/PointShadow/depthShader.frag",
                        "Shaders/Shadows/PointShadow/depthShader.geom");
    //Bloom
    Shader blurShader("Shaders/Bloom/blur.vert", "Shaders/Bloom/blur.frag");
    Shader bloomFinalShader("Shaders/Bloom/bloom_final.vert", "Shaders/Bloom/bloom_final.frag");


	skyboxShader.use();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

    Mesh cubeMesh(cubeVertices, cubeIndicesVec, cubeTextures);
    Mesh cubeEmissionMesh(cubeVertices, cubeIndicesVec, cubeEmissionTextures);
    Mesh darkRoom(cubeVerticesInvertedNorm, cubeIndicesVec, cubeTextures);
    Mesh lightcubeMesh(cubeVertices, cubeIndicesVec, noTextures);
    Mesh planeMesh(planeVertices, planeIndicesVec, planeTextures);
    Mesh cubeside(cubeSideVertices, cubeIndicesVec, cubeTextures);

	// Skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Faces of the cubemap
	std::string facesCubemap[6] =
	{
		"Textures/skybox/right.png",
		"Textures/skybox/left.png",
		"Textures/skybox/top.png",
		"Textures/skybox/bottom.png",
		"Textures/skybox/front.png",
		"Textures/skybox/back.png",
	};


    //Crosshair
    unsigned int crosshairVAO, crosshairVBO;

    glGenVertexArrays(1, &crosshairVAO);
    glGenBuffers(1, &crosshairVBO);
    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

	// Cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Important parameters to prevent seams in our cubemap
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //Attaching the textures to the cubemap
	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

    //---------------SHADOWS (Using a depth map with the help of the framebuffer)------------------
    //Creating our FrameBuffer Object (FBO)
    unsigned int depthMapFBO; 
    glGenFramebuffers(1, &depthMapFBO);

    //Now we need to create a 2D texture and use it as our framebuffer's depth buffer
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    //We define the type of our Texture as GL_DEPTH_COMPONENT since we want to store the depth
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //Now we can attach it to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    //We don't need these 2 buffers but we need to specify it so that our frambuffer is complete, 
    // so we'll just put it to GL_NONE
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    //Check if the FRAMEBUFFER is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "BAD - Framebuffer not complete!" << std::endl;
    }else{
        std::cout << "GOOD - Framebuffer complete" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //We bind it to zero so that we don't override by mistake our depthMapFBO

    //Now we need to render the scene from the light's prospective and save it as a depth map which we'll use later
    // to check if a given fragment is occluded or not, if it's z value is higher then the z value stored in the depth map
    // then that fragment is in the shadow.


    //----------------------POINT SHADOW-------------------------
    //Creating our FrameBuffer Object (FBO)
    unsigned int depthMapPointFBO; 
    glGenFramebuffers(1, &depthMapPointFBO);

    //We'll be using a cubemap for our point lights
    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);

    //Now we need to bind our depthCubemap and assign a 2D depth-valued texture to each face
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for(int i = 0; i < 6; ++i){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    //We define our texture params
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //And our framebuffer params
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapPointFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Let's define the trasformation we need, we'll be using a perspective projection
    float aspect = (float)SHADOW_WIDTH/(float)SHADOW_WIDTH;
    float near = 1.0f;
    float far = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

    //Now we need to define all the 6 direction for our projection since we are dealing with a cubemap
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));


    // ---------- SpotLight Shadow Map ----------
    const unsigned int SPOT_SHADOW_SIZE = 1024;
    unsigned int spotDepthMapFBO, spotDepthMap;
    glGenFramebuffers(1, &spotDepthMapFBO);
    glGenTextures(1, &spotDepthMap);
    glBindTexture(GL_TEXTURE_2D, spotDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                SPOT_SHADOW_SIZE, SPOT_SHADOW_SIZE,
                0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float spotBorderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, spotBorderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, spotDepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                        GL_TEXTURE_2D, spotDepthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //---------------BLOOM SETUP (+ HDR setup)---------------

    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }

    // Depth renderbuffer for HDR FBO
    unsigned int hdrRBO;
    glGenRenderbuffers(1, &hdrRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, hdrRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRBO);

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "HDR Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ---------- Ping-pong framebuffers for blur ----------
    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffer[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Ping-pong Framebuffer " << i << " not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //------------WIREFRAME DRAWING / NORMAL DRAWING---------------

    //To draw in wireframe polygons use this
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //To draw polygons normally use this
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   
    //-----------------Physics-----------------------

    //Gravity stuff
    //Create a rigid body in the world
    const decimal timeStep = 1.0f / 250.0f;
    float accumulator = 0.0f;

    //Let's define and initialize our physicsCommon
    PhysicsCommon physicsCommon;
    PhysicsWorld* world; 
    std::vector<RigidBody*> rigidBodies;
    std::vector<glm::mat4> rigidBodiesModels;

    std::vector<RigidBody*> emissionRigidBodies;
    std::vector<glm::mat4> emissionRigidBodiesModels;

    glm::mat4 vaseModel = glm::mat4(1.0f); 
    initPhysics(physicsCommon, world);
   
    //Create the rigidbodies for our cube
    createRigidBodies(physicsCommon, world, cubePositions, numCubes, rigidBodies);
    createRigidBodies(physicsCommon, world, emissionCubePositions, numEmissionCubes, emissionRigidBodies);
    RigidBody* rigidPlane = createRigidPlane(physicsCommon, world);

    //Create rigidroom
    createRoom(physicsCommon, world, 20.0f, 20.0f, 20.0f, glm::vec3(0.0f, -15.0f, 0.0f));

    //Rigid Vase
    std::vector <RigidBody*> rigidVaseFragments;
    std::vector<glm::mat4> rigidVaseModels;
    bool vaseIsBroken = false;
    bool createFragments = false;
    Transform vaseTransform;
    float breakSpeed;

    //Rigid maoi
    bool maoiIsBroken = false;
    bool createMaoiFragments = false;
    RigidBody* rigidMaoi = nullptr;
    std::vector<RigidBody*> rigidMaoiFragments;
    glm::mat4 maoiModel = glm::mat4(1.0f);
    float maoiBreakSpeed;
    Transform maoiTransform;

    //Creating maoi rigid statue
    const Vector3 maoiHalfExtents(0.3f, 0.75f, 0.15f);
    rigidMaoi = createRigidModels(physicsCommon, world, maoiPos, maoiHalfExtents, std::nullopt);
    rigidMaoi->setMass(20.0f);

    //Creating rigid vase
    const Vector3 halfExtents(0.3f, 0.425f, 0.3f);
    RigidBody* rigidVase = createRigidModels(physicsCommon, world, vasePos, halfExtents, std::nullopt); 

    //Cube breaking handling
    std::vector<bool> cubeIsBroken(numCubes, false);
    std::vector<std::vector<RigidBody*>> cubeFragments(numCubes);
    std::vector<std::vector<glm::mat4>> cubeFragmentsModels(numCubes);


    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    raycaster = new Raycaster(world, camera, fbWidth, fbHeight, rigidPlane);

    //EvenListener for the model breaking
    CombinedBreakListener* combinedListener = nullptr;
    combinedListener = new CombinedBreakListener(rigidVase, rigidMaoi, rigidBodies, raycaster);
    world->setEventListener(combinedListener);

    //--------------- 10. Render Loop -------------------
    while(!glfwWindowShouldClose(window)){

        //Fps counter
        currentTime = glfwGetTime();
        timeDiff = currentTime - prevTime;
        counter++;
        if(timeDiff >= 1.0f / 5.0f){
            std::string FPS = std::to_string((1.0f/timeDiff) * counter);
            std::string ms = std::to_string((timeDiff/counter) * 1000);
            std::string title = "Progetto - " + FPS + "FPS / " + ms + "ms";
            glfwSetWindowTitle(window, title.c_str());
            prevTime = currentTime;
            counter = 0;
        }

        //Calculation the delta time and the last frame
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Physics
        if (raycaster->getGrabbedBody()){
            raycaster->updateGrabbed();
        }
        updatePhysics(physicsCommon, world, rigidBodies, applyGravity, deltaTime, accumulator, timeStep);
        updateCubesFromPhysics(rigidBodies, rigidBodiesModels);
        updateCubesFromPhysics(emissionRigidBodies, emissionRigidBodiesModels);
        //Update the position of the emission cubes so the light follows them
        updateEmissionLights(emissionPointLightPositions, emissionRigidBodiesModels, numEmissionPointLights);

        //Checks for breaking models

        if (!vaseIsBroken && rigidVase && combinedListener->vaseCollided()) {
            Vector3 vel = rigidVase->getLinearVelocity();
            float speed = vel.length();
            if (speed > 3.0f) {
                breakSpeed = speed;
                vaseTransform = rigidVase->getTransform();
                world->destroyRigidBody(rigidVase);
                rigidVase = nullptr;
                vaseIsBroken = true;
                createFragments = true;
            }
        }

        if (!maoiIsBroken && rigidMaoi && combinedListener->maoiCollided()) {
            Vector3 vel = rigidMaoi->getLinearVelocity();
            float speed = vel.length();
            if (speed > 3.0f) {
                maoiBreakSpeed = speed;
                maoiTransform = rigidMaoi->getTransform();
                world->destroyRigidBody(rigidMaoi);
                rigidMaoi = nullptr;
                maoiIsBroken = true;
                createMaoiFragments = true;
            }
        }

        for (int i = 0; i < numCubes; ++i) {
            if (!cubeIsBroken[i] && rigidBodies[i] && combinedListener->cubeCollided(i)) {
                Vector3 vel = rigidBodies[i]->getLinearVelocity();
                float speed = vel.length();
                if (speed > 3.0f) {
                    Transform cubeTransform = rigidBodies[i]->getTransform();
                    world->destroyRigidBody(rigidBodies[i]);
                    rigidBodies[i] = nullptr;
                    cubeIsBroken[i] = true;
                    combinedListener->invalidateCube(i);

                    createCubeFragments(physicsCommon, world, cubeTransform, speed,
                                        cubeFragments[i], cubeFragmentsModels[i]);
                }
            }
        }

        //Resetting the listeners flags
        combinedListener->reset();


        //Update vase pos or fragment pos
        if(!vaseIsBroken){
            updateModelFromPhysics(rigidVase, vaseModel, glm::vec3(0.1f, 0.1f, 0.1f));        
        }else{
            if(createFragments){

                Vector3 breakPos = vaseTransform.getPosition();
                glm::vec3 spawnPos(breakPos.x, breakPos.y, breakPos.z);

                for (unsigned int i = 0; i < vaseFragments.size(); i++)
                {
                    RigidBody* fragment = createRigidModels(physicsCommon, world,
                                                            spawnPos, std::nullopt,
                                                            fragmentHitBox[i]);

                    //Random direction for the fragments
                    float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
                    float phi   = static_cast<float>(rand()) / RAND_MAX * 3.14159f;
                    Vector3 dir(std::sin(phi) * std::cos(theta),
                                                std::cos(phi),
                                                std::sin(phi) * std::sin(theta));

                    //Small offset to avoid initial overlap
                    Transform offsetTransform = vaseTransform;
                    Vector3 newPos = breakPos + dir * 0.15f;
                    offsetTransform.setPosition(newPos);
                    fragment->setTransform(offsetTransform);

                    //Impulse
                    float impulseStrength = breakSpeed * 0.4f;
                    fragment->setLinearVelocity(dir * impulseStrength);

                    rigidVaseFragments.push_back(fragment);
                }
                createFragments = false;
            }

            //Update fragment position
            for (unsigned int i = 0; i < vaseFragments.size(); i++)
            {
                updateModelFromPhysics(rigidVaseFragments[i], vaseFragmentsModel[i], glm::vec3(0.5f, 0.5f, 0.5f));
            }
        }


        if (!maoiIsBroken) {
            updateModelFromPhysics(rigidMaoi, maoiModel, glm::vec3(0.1f));
        }else{

            if (createMaoiFragments) {
                Vector3 breakPos = maoiTransform.getPosition();
                glm::vec3 spawnPos(breakPos.x, breakPos.y, breakPos.z);

                for (unsigned int i = 0; i < maoiFragments.size(); i++) {
                    RigidBody* fragment = createRigidModels(physicsCommon, world,
                                                            spawnPos, maoiFragmentHitBox[i], std::nullopt);

                    float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
                    float phi   = static_cast<float>(rand()) / RAND_MAX * 3.14159f;
                    Vector3 dir(std::sin(phi)*std::cos(theta), std::cos(phi), std::sin(phi)*std::sin(theta));

                    Transform offsetTransform = maoiTransform;
                    Vector3 newPos = breakPos + dir * 0.2f;
                    offsetTransform.setPosition(newPos);
                    fragment->setTransform(offsetTransform);

                    float impulseStrength = maoiBreakSpeed * 0.4f;
                    fragment->setLinearVelocity(dir * impulseStrength);

                    rigidMaoiFragments.push_back(fragment);
                }
                createMaoiFragments = false;
            }

            for (size_t i = 0; i < rigidMaoiFragments.size(); i++) {
                updateModelFromPhysics(rigidMaoiFragments[i], maoiFragmentsModels[i], glm::vec3(0.1f));
            }
        }

        // Update cube fragment positions
        for (int i = 0; i < numCubes; ++i) {
            if (cubeIsBroken[i]) {
                for (size_t j = 0; j < cubeFragments[i].size(); ++j) {
                    if (cubeFragments[i][j]) {
                        updateModelFromPhysics(cubeFragments[i][j], cubeFragmentsModels[i][j], glm::vec3(1.0f));
                    }
                }
            }
        }
        

        //Input
        processInput(window);

        glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);
        if(ambientlightOn){
            //-------------DEPTH MAP (SHADOWS)-------------
            // First we need to render the scene from out light source (DirLight) prospective and calculate the depth map
            // we will define a location that stayes more or less in the direction of the DirLight we defined since we assume
            // it's infinitely far away, and also we'll use orthogonal projection with a frustrum big enough
            float near_plane = 1.0f, far_plane = 65.0f;
            float frustrum_size = 40.0f;
            glm::mat4 lightProjection = glm::ortho(-frustrum_size, frustrum_size, -frustrum_size, frustrum_size, near_plane, far_plane);

            //We need to define our pos of the light from which we'll look at the scene and render our depth map
            //This is the vec we defined for our lightDir and the one we pass to our shader later on, we'll use it to
            // calculate a position to approximate our lightPos
            glm::vec3 lightDir = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));
            float distance = 25.0f;
            glm::vec3 lightPos = -lightDir * distance;
            
            //Now we can use the glm::lookAt function to look from our light pos towards our scene (0.0f, 0.0f, 0.0f)
            glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            //Finally we can take our lightspcaematrix which will transform each vertex to the coords viewed from the lights prospective
            lightSpaceMatrix = lightProjection * lightView;

            //Rendering the depth map
            depthMapShader.use();
            depthMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            glViewport(0,0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);

            if (!vaseIsBroken) {
                renderModels(depthMapShader, vaseModel, vaseIntact);
            } else {
                renderBrokenModel(depthMapShader, vaseFragmentsModel, vaseFragments);
            }

            if (!maoiIsBroken) {
                renderModels(depthMapShader, maoiModel, maoiStatue);
            } else {
                renderBrokenModel(depthMapShader, maoiFragmentsModels, maoiFragments);
            }
    
            // Cube fragments
            for (int i = 0; i < numCubes; ++i) {
                if (cubeIsBroken[i]) {
                    for (size_t j = 0; j < cubeFragmentsModels[i].size(); ++j) {
                        depthMapShader.setMat4("model", cubeFragmentsModels[i][j]);
                        cubeside.Draw(depthMapShader);
                    }
                }
            }

            renderCubesAndPlane(depthMapShader, cubePositions, cubeMesh, planeMesh, numCubes, rigidBodiesModels, cubeIsBroken);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            //reset the viewPort
            glViewport(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);
            //----------------------SHADOW END-------------------------
        }

        float far_plane = 65.0f;
        if(pointlightOn){
            //-----------------Point shadow depth pass------------------
            pointShadowShader.use();

            // Set up the matricies
            for (int i = 0; i < 6; ++i) {
                std::string name = "shadowMatrices[" + std::to_string(i) + "]";
                pointShadowShader.setMat4(name, shadowTransforms[i]);
            }
            pointShadowShader.setFloat("far_plane", far_plane);
            pointShadowShader.setVec3("lightPos", pointLightPositions[0]); // light position

            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapPointFBO);
            glClear(GL_DEPTH_BUFFER_BIT);

            if (!vaseIsBroken) {
                renderModels(pointShadowShader, vaseModel, vaseIntact);
            } else {
                renderBrokenModel(pointShadowShader, vaseFragmentsModel, vaseFragments);
            }

            if (!maoiIsBroken) {
                renderModels(pointShadowShader, maoiModel, maoiStatue);
            } else {
                renderBrokenModel(pointShadowShader, maoiFragmentsModels, maoiFragments);
            }

            // Cube fragments
            for (int i = 0; i < numCubes; ++i) {
                if (cubeIsBroken[i]) {
                    for (size_t j = 0; j < cubeFragmentsModels[i].size(); ++j) {
                        pointShadowShader.setMat4("model", cubeFragmentsModels[i][j]);
                        cubeside.Draw(pointShadowShader);
                    }
                }
            }
            
            renderCubesAndPlane(pointShadowShader, cubePositions, cubeMesh,
                                planeMesh, numCubes, rigidBodiesModels, cubeIsBroken);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);

            //---------------------Point shadow end-----------------------
        }

        glm::mat4 spotLightSpaceMatrix = glm::mat4(1.0f);
        if(spotlightOn){
            // ---------- Spotlight shadow pass ----------
            glm::mat4 spotProj = glm::perspective(glm::radians(25.0f), 1.0f, 1.0f, 25.0f);
            glm::mat4 spotView = glm::lookAt(camera.Position,
                                            camera.Position + camera.Front,
                                            camera.Up);
            spotLightSpaceMatrix = spotProj * spotView;

            depthMapShader.use();
            depthMapShader.setMat4("lightSpaceMatrix", spotLightSpaceMatrix);

            glViewport(0, 0, SPOT_SHADOW_SIZE, SPOT_SHADOW_SIZE);
            glBindFramebuffer(GL_FRAMEBUFFER, spotDepthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);

                if (!vaseIsBroken)
                    renderModels(depthMapShader, vaseModel, vaseIntact);
                else
                    renderBrokenModel(depthMapShader, vaseFragmentsModel, vaseFragments);

                if (!maoiIsBroken) {
                    renderModels(depthMapShader, maoiModel, maoiStatue);
                } else {
                    renderBrokenModel(depthMapShader, maoiFragmentsModels, maoiFragments);
                }
    

                renderCubesAndPlane(depthMapShader, cubePositions, cubeMesh,
                                    planeMesh, numCubes, rigidBodiesModels, cubeIsBroken);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

            //---------------------Spotlight shadow end-----------------------
        }


        

        //-----------------------MAIN RENDER LOOP-----------------------

        //Setting up colors and cleaning the color and z depth buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(renderDebugOld){
            // std::cout << renderDebugOld<< std::endl;
            //Debug for depth map visualization
            /*
            debugDepthQuad.use();
            debugDepthQuad.setFloat("near_plane", near_plane);
            debugDepthQuad.setFloat("far_plane", far_plane);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            renderQuad();
            */
        }else{ 
            //Rendering of the whole scene
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Setup lighting shader with all lights and shadows
            int numPointLights = (int)(sizeof(pointLightPositions) / sizeof(glm::vec3));
            int numTotalLights = numPointLights + numEmissionPointLights;
            setupShader(lightingShader, pointLightPositions, numTotalLights, emissionPointLightPositions, numEmissionPointLights);
            lightingShader.use();

            //Shadows
            if(ambientlightOn){
                lightingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
                glActiveTexture(GL_TEXTURE5);
                glBindTexture(GL_TEXTURE_2D, depthMap);
                lightingShader.setInt("ShadowMap", 5);
            }

            if(pointlightOn){
                glActiveTexture(GL_TEXTURE6);
                glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
                lightingShader.setInt("depthMapPoint", 6);
                lightingShader.setFloat("far_plane", far_plane);
            }

            if(spotlightOn){
                glActiveTexture(GL_TEXTURE7);
                glBindTexture(GL_TEXTURE_2D, spotDepthMap);
                lightingShader.setInt("spotShadowMap", 7);
                lightingShader.setMat4("spotLightSpaceMatrix", spotLightSpaceMatrix);
            }

            // Camera matrices
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT), 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);

            //Vase (intact or fragments)
            if (!vaseIsBroken) {
                renderModels(lightingShader, vaseModel, vaseIntact);
            } else {
                renderBrokenModel(lightingShader, vaseFragmentsModel, vaseFragments);
            }


            if (!maoiIsBroken) {
                renderModels(lightingShader, maoiModel, maoiStatue);
            } else {
                renderBrokenModel(lightingShader, maoiFragmentsModels, maoiFragments);
            }

            // Cube fragments
            for (int i = 0; i < numCubes; ++i) {
                if (cubeIsBroken[i]) {
                    for (size_t j = 0; j < cubeFragmentsModels[i].size(); ++j) {
                        lightingShader.setMat4("model", cubeFragmentsModels[i][j]);
                        cubeside.Draw(lightingShader);
                    }
                }
            }

            // Dark room
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -15.0f, 0.0f));
            model = glm::scale(model, glm::vec3(20.0f));
            lightingShader.setMat4("model", model);
            darkRoom.Draw(lightingShader);

            //Emission cubes
            for (int i = 0; i < numEmissionCubes; i++) {
                lightingShader.setMat4("model", emissionRigidBodiesModels[i]);
                cubeEmissionMesh.Draw(lightingShader);
            }

            //Render stuff
            renderCubesAndPlane(lightingShader, cubePositions, cubeMesh, planeMesh, numCubes, rigidBodiesModels, cubeIsBroken);
            renderScenePointLights(lightCubeShader, pointLightPositions, numPointLights, view, projection, lightcubeMesh);

            // Skybox
            if (ambientlightOn) {
                renderSceneSkyBox(skyboxShader, skyboxVAO, cubemapTexture);
            }

            // Crosshair
            renderCrosshair(crosshairShader, crosshairVAO);

            // Raycaster update
            if (raycaster->getGrabbedBody()) {
                raycaster->updateFlickReset();
            }

            if(renderDebug){
                glEnable(GL_DEPTH_TEST);
                glDepthMask(GL_FALSE);
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT), 0.1f, 100.0f);
                glm::mat4 view = camera.GetViewMatrix();
                renderDebugPhysics(world->getDebugRenderer(), debugShader, view, projection);
                glDepthMask(GL_TRUE);
            }

            bool showBloom = false;

            //Check if we are close enough to render the bloom, this saves us a bit of performance
            if (glm::distance(camera.Position, glm::vec3(0.0f, -25.0f, 0.0f)) < 15.0f)
                showBloom = true;
            else
                showBloom = false;

            
            if(showBloom){
                // Rendering only the emission cubes
                glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                lightingShader.use();
                for (int i = 0; i < numEmissionCubes; i++) {
                    lightingShader.setMat4("model", emissionRigidBodiesModels[i]);
                    cubeEmissionMesh.Draw(lightingShader);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // Blur
                bool horizontal = true, first_iteration = true;
                int amount = 10;
                blurShader.use();
                for (int i = 0; i < amount; i++) {
                    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
                    blurShader.setInt("horizontal", horizontal);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffer[!horizontal]);
                    renderQuad();
                    horizontal = !horizontal;
                    first_iteration = false;
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                //Blending of the bloom
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                glDepthMask(GL_FALSE);
                glDisable(GL_DEPTH_TEST);

                bloomFinalShader.use();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
                renderQuad();

                glDepthMask(GL_TRUE);
                glDisable(GL_BLEND);
                glEnable(GL_DEPTH_TEST);
            }

       }

        //Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Terminate glfw
    glfwPollEvents();
    glfwTerminate();

    std::cout << "GOOD - Terminated GLFW" << std::endl;

    return 0;
}


