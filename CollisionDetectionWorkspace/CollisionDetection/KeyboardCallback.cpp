#include "KeyboardCallback.h"
#include "GlobalObjects.h"

extern glm::vec3 gCameraEye;
extern glm::vec3 gCameraTarget;
extern glm::vec3 gDronePosition;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Close window if escape key is pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        editMode = MOVING_CAMERA;
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        editMode = MOVING_LIGHT;
    }

    switch (editMode) {

    case MOVING_CAMERA: {
        const float CAMERA_MOVE_SPEED = 1.1f;
        const float DRONE_MOVE_SPEED = 1.0f;

        if (key == GLFW_KEY_A) { // Left
            ::gCameraEye.x -= CAMERA_MOVE_SPEED;
            //::gDronePosition.x -= DRONE_MOVE_SPEED;
        }  

        if (key == GLFW_KEY_D) { // Right
            //::gDronePosition.x += DRONE_MOVE_SPEED;
            ::gCameraEye.x += CAMERA_MOVE_SPEED;
        }      

        if (key == GLFW_KEY_W) { // Forward
            //::gDronePosition.z += DRONE_MOVE_SPEED;
            ::gCameraEye.z += CAMERA_MOVE_SPEED;
        }  

        if (key == GLFW_KEY_S) { // Backwards
            //::gDronePosition.z -= DRONE_MOVE_SPEED;
            ::gCameraEye.z -= CAMERA_MOVE_SPEED;
        }  

        if (key == GLFW_KEY_Q) { // Down
            //::gDronePosition.y -= DRONE_MOVE_SPEED;
            ::gCameraEye.y -= CAMERA_MOVE_SPEED;
        }   
            
        if (key == GLFW_KEY_E) { // Up
            //::gDronePosition.y += DRONE_MOVE_SPEED;
            ::gCameraEye.y += CAMERA_MOVE_SPEED;
        }     

        if (key == GLFW_KEY_1)
            ::gCameraEye = glm::vec3(-5.5f, -3.4f, 15.0f);
    } 
                      break;

    case MOVING_LIGHT: {
        const float LIGHT_MOVE_SPEED = 3.0f;
        if (key == GLFW_KEY_A)     // Left
        {
            ::gLightManager->vLights[selectedLightIndex].position.x -= LIGHT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_D)     // Right
        {
            ::gLightManager->vLights[selectedLightIndex].position.x += LIGHT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_W)     // Forward
        {
            ::gLightManager->vLights[selectedLightIndex].position.z += LIGHT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_S)     // Backwards
        {
            ::gLightManager->vLights[selectedLightIndex].position.z -= LIGHT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_Q)     // Down
        {
            ::gLightManager->vLights[selectedLightIndex].position.y -= LIGHT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_E)     // Up
        {
            ::gLightManager->vLights[selectedLightIndex].position.y += LIGHT_MOVE_SPEED;
        }

        if (key == GLFW_KEY_PAGE_DOWN)
        {
            // Select previous light
            if (selectedLightIndex > 0)
            {
                selectedLightIndex--;
            }
        }
        if (key == GLFW_KEY_PAGE_UP)
        {
            if (selectedLightIndex < (::gLightManager->vLights.size() - 1))
            {
                // Select previous light
                selectedLightIndex++;
            }
        }

        // Change attenuation
        if (key == GLFW_KEY_1)
        {
            // Linear Decrease by 1% 
            ::gLightManager->vLights[selectedLightIndex].atten.y *= 0.99f;
        }
        if (key == GLFW_KEY_2)
        {
            // Linear Increase by 1%
            ::gLightManager->vLights[selectedLightIndex].atten.y *= 1.01f;
        }
        if (key == GLFW_KEY_3)
        {
            // Quadratic Decrease by 0.01% 
            ::gLightManager->vLights[selectedLightIndex].atten.z *= 0.999f;
        }
        if (key == GLFW_KEY_4)
        {
            // Quadratic Increase by 0.01%
            ::gLightManager->vLights[selectedLightIndex].atten.z *= 1.001f;
        }
    }
    break;
    }
}
