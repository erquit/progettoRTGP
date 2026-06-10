#include "input.hpp"

//Function to change the size of the Viewport if our window get resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width, height);
}

//Function to terminate the window if the Q key is pressed
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    //Forward
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);

    //Backward
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    //Left
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);

    //Right
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    //Up
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(FLY_UP, deltaTime);
    //Down
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(FLY_DOWN, deltaTime); 

    //Reset camera to initial position
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        camera.Position = cameraPos;
        camera.Front = cameraFront;
        camera.Up = cameraUp;
        camera.Yaw = yaw;
        camera.Pitch = pitch;
        camera.Zoom = fov;
        ambientlightOn = true;
        pointlightOn = true;
        spotlightOn = false;
    }

    //Tp to black room
    if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        glm::vec3 newPos(cameraPos.x, cameraPos.y - 25.0f, cameraPos.z - 2.0f);
        camera.Position = newPos;
        camera.Front = cameraFront;
        camera.Up = cameraUp;
        camera.Yaw = yaw;
        camera.Pitch = pitch;
        camera.Zoom = fov;
        ambientlightOn = false;
        pointlightOn = false;
        spotlightOn = true;
    }


    //Turn on and off the spotlight
    static bool fwasPressed = false;
    bool fisPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
    if(fisPressed && !fwasPressed){
        spotlightOn = !spotlightOn;
    }
    fwasPressed = fisPressed;

    //Turn on and off the pointLights
    static bool pwasPressed = false;
    bool pisPressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
    if(pisPressed && !pwasPressed){
        pointlightOn = !pointlightOn;
    }
    pwasPressed = pisPressed;

    //Turn on and off the ambientlight
    static bool mwasPressed = false;
    bool misPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
    if(misPressed && !mwasPressed){
        ambientlightOn = !ambientlightOn;
    }
    mwasPressed = misPressed;

    //Turn on and off the debug print
    static bool bwasPressed = false;
    bool bisPressed = glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS;
    if(bisPressed && !bwasPressed){
        renderDebug = !renderDebug;
    }
    bwasPressed = bisPressed;

    //Turn on and off the gravity for debug
    static bool gwasPressed = false;
    bool gisPressed = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;
    if(gisPressed && !gwasPressed){
        applyGravity = !applyGravity;
    }
    gwasPressed = gisPressed;

    //Throw objects
    static bool eWasPressed = false;
    bool eIsPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    if (eIsPressed && !eWasPressed && raycaster->getGrabbedBody()) {
        raycaster->throwForward(15.0f);
    }
    eWasPressed = eIsPressed;
}

//Function to process mouse inputs
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    //Let's calculate the offset for X and Y axis from the last know position of the mouse
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; //Reversed cuz the y goes from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}

//Let's also add the possibility of zooming by changin the value of our fov (which give the illusion of zooming)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float> (yoffset));
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    static double pressTime = 0.0;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            pressTime = glfwGetTime();
            reactphysics3d::RigidBody* hitBody = nullptr;
            reactphysics3d::Vector3 hitPoint;
            if (raycaster->castFromCenter(hitBody, hitPoint)) {
                glm::vec3 hitRel(hitPoint.x - camera.Position.x,
                                 hitPoint.y - camera.Position.y,
                                 hitPoint.z - camera.Position.z);
                glm::vec3 localOffset(
                    glm::dot(hitRel, camera.Right),
                    glm::dot(hitRel, camera.Up),
                    glm::dot(hitRel, camera.Front)
                );
                const float holdDistance = 0.5f; 
                localOffset.z += holdDistance;    
                raycaster->setGrabbedBody(hitBody, localOffset);
                lastCameraPos = camera.Position;
            }
        } else if (action == GLFW_RELEASE) {
            if (raycaster->getGrabbedBody()) {
                double releaseTime = glfwGetTime();
                float totalTime = static_cast<float>(releaseTime - pressTime);
                std::cout << totalTime << std::endl;
                if (totalTime < 0.0001f) totalTime = 0.0001f;
                raycaster->releaseGrabbed(0.5f, totalTime, lastCameraPos, camera.Position);
            }
        }
    }
}