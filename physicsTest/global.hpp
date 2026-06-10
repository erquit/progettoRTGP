#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <glm/glm.hpp>
#include "camera.hpp"
#include "rayCaster.hpp"
class Raycaster;

extern const unsigned int WINDOW_WIDTH;
extern const unsigned int WINDOW_HEIGHT;

//Shadow map (Depth map) size
extern const unsigned int SHADOW_WIDTH;
extern const unsigned int SHADOW_HEIGHT;

//MSAA 8
extern unsigned int samples;

//Our camera position
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;  

extern float lastX, lastY;
extern float yaw, pitch, fov;
extern bool firstMouse;

//Let's define our camera
extern Camera camera;

//To balance out the Camera speed cuz some users might call processInput more often then other
// we use "deltaTime" and "lastFrame" as global value and use them to balance out the speed of the user (camera)
extern float deltaTime;
extern float lastFrame;

//Bool for the spotlight, pointLight and ambientLight
extern bool spotlightOn;
extern bool pointlightOn;
extern bool ambientlightOn;

//Gravity turn on off
extern bool applyGravity;

//Debug
extern bool renderDebugOld;
extern bool renderDebug;

//Raycasting
extern glm::vec3 grabLocalOffset;
extern glm::vec3 lastCameraPos;
extern Raycaster* raycaster;
extern glm::vec3 cameraVelocity;

extern float m_recentYaw;
extern float m_recentPitch;
extern double m_lastFlickResetTime;
extern const double flickWindow;
extern float BREAK_SPEED;




#endif