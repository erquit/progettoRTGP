#include "global.hpp"

const unsigned int WINDOW_WIDTH = 1920;
const unsigned int WINDOW_HEIGHT = 1080;

//Shadow map (Depth map) size
const unsigned int SHADOW_WIDTH = 1024 * 2;
const unsigned int SHADOW_HEIGHT = 1024 * 2;

//MSAA 8
unsigned int samples = 8;

//Our camera position
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastX, lastY;
float yaw = -90.f, pitch = 0.0f, fov = 45.0f;
bool firstMouse = true;

//Let's define our camera
Camera camera(cameraPos, cameraUp, yaw, pitch);

//To balance out the Camera speed cuz some users might call processInput more often then other
// we use "deltaTime" and "lastFrame" as global value and use them to balance out the speed of the user (camera)
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Bool for the spotlight, pointLight and ambientLight
bool spotlightOn = true;
bool pointlightOn = true;
bool ambientlightOn = true;

//Gravity turn on off
bool applyGravity = true;

//Debug
bool renderDebugOld = false;
bool renderDebug = false;

//Raycasting
glm::vec3 grabLocalOffset(0.0f);
glm::vec3 lastCameraPos(0.0f);
Raycaster* raycaster = nullptr;
glm::vec3 cameraVelocity(0.0f);

double lastCameraResetTime = 0.0f;
const double resetInterval = 0.3f;

float m_recentYaw;
float m_recentPitch;
double m_lastFlickResetTime;
const double flickWindow = 0.3;


float BREAK_SPEED = 2.0f;


