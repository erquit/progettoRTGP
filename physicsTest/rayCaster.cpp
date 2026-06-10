#include "rayCaster.hpp"
#include <glm/glm.hpp>

reactphysics3d::decimal Raycaster::PickCallback::notifyRaycastHit(const reactphysics3d::RaycastInfo& info) {
    //We need to ignore all the bodies that are static
    auto* body = static_cast<reactphysics3d::RigidBody*>(info.body);
    if(body->getType() == reactphysics3d::BodyType::STATIC){
        return reactphysics3d::decimal(1.0f);
    }
    if( info.hitFraction < hitFraction){
        hit = true;
        hitBody = static_cast<reactphysics3d::RigidBody*>(info.body);
        hitPoint = info.worldPoint;
        hitFraction = info.hitFraction;
    }
    return 1.0f;
}

Raycaster::Raycaster(reactphysics3d::PhysicsWorld* world, const Camera& camera,
                     int /*windowWidth*/, int /*windowHeight*/, reactphysics3d::RigidBody* groundBody)
    : m_world(world), m_camera(camera), m_groundBody(groundBody) {}

void Raycaster::setGrabbedBody(reactphysics3d::RigidBody* body, const glm::vec3& localOffset) {
    if (m_grabbedBody) {
        m_grabbedBody->setType(reactphysics3d::BodyType::DYNAMIC);
        m_grabbedBody = nullptr;
    }
    if (body) {
        m_grabbedBody = body;
        m_grabbedBody->setLinearVelocity(reactphysics3d::Vector3(0, 0, 0));
        m_grabbedBody->setAngularVelocity(reactphysics3d::Vector3(0, 0, 0));
        m_grabbedBody->setType(reactphysics3d::BodyType::KINEMATIC);
        m_grabLocalOffset = localOffset;
        m_recordingAngular = true;
        m_recentYaw = m_camera.Yaw;
        m_recentPitch = m_camera.Pitch;
        m_lastFlickResetTime = glfwGetTime();
    }
}

void Raycaster::updateFlickReset(){
    if(!m_grabbedBody) return;
    double now = glfwGetTime();
    if(now - m_lastFlickResetTime >= flickWindow){
        m_recentYaw = m_camera.Yaw;
        m_recentPitch = m_camera.Pitch;
        m_lastFlickResetTime = now;
    }

}


bool Raycaster::castFromCenter(reactphysics3d::RigidBody*& outHitBody, reactphysics3d::Vector3& outHitPoint) const {
    reactphysics3d::Vector3 origin(m_camera.Position.x, m_camera.Position.y, m_camera.Position.z);
    reactphysics3d::Vector3 direction(m_camera.Front.x, m_camera.Front.y, m_camera.Front.z);
    reactphysics3d::Ray ray(origin, origin + direction * 100.0f);
    
    PickCallback callback;
    m_world->raycast(ray, &callback);
    
    if (callback.hit && callback.hitBody != m_groundBody) {
        outHitBody = callback.hitBody;
        outHitPoint = callback.hitPoint;
        return true;
    }
    return false;
}

void Raycaster::updateGrabbed() {
    if (!m_grabbedBody) return;
    
    glm::vec3 camPos(m_camera.Position.x, m_camera.Position.y, m_camera.Position.z);
    glm::vec3 target = camPos +
        m_camera.Front * m_grabLocalOffset.z +
        m_camera.Right * m_grabLocalOffset.x +
        m_camera.Up * m_grabLocalOffset.y;
    
    reactphysics3d::Transform transform = m_grabbedBody->getTransform();
    transform.setPosition(reactphysics3d::Vector3(target.x, target.y, target.z));
    m_grabbedBody->setTransform(transform);
}

void Raycaster::releaseGrabbed(float throwStrength, float totalTime,
                               const glm::vec3& lastCameraPos,
                               const glm::vec3& currentCameraPos) {
    if (!m_grabbedBody) return;

    glm::vec3 finalVelocity(0.0f);

    // Angular mouse movement for the flick
    if (m_recordingAngular) {
        double now = glfwGetTime();
        float flickDeltaTime = static_cast<float>(now - m_lastFlickResetTime);
        if (flickDeltaTime < 0.0001f) flickDeltaTime = 0.0001f;

        float deltaYaw = m_camera.Yaw - m_recentYaw;
        float deltaPitch = m_camera.Pitch - m_recentPitch;

        float angularSpeedYaw = glm::radians(deltaYaw) / flickDeltaTime;
        float angularSpeedPitch = glm::radians(deltaPitch) / flickDeltaTime;

        float radius = m_grabLocalOffset.z;
        glm::vec3 tangentialYaw = m_camera.Right * (radius * angularSpeedYaw);
        glm::vec3 tangentialPitch = m_camera.Up * (radius * angularSpeedPitch);

        finalVelocity += tangentialYaw + tangentialPitch;
        finalVelocity *= throwStrength;

        // If the angular speed is low we'll set velocty to zero
        float totalAngularSpeed = glm::length(glm::vec2(angularSpeedYaw, angularSpeedPitch));
        const float minAngularSpeed = 0.8f; 
        if (totalAngularSpeed < minAngularSpeed) {
            finalVelocity = glm::vec3(0.0f);
        }
    }

    reactphysics3d::Vector3 velocity(finalVelocity.x, finalVelocity.y, finalVelocity.z);
    m_grabbedBody->setLinearVelocity(velocity);

    m_grabbedBody->setType(reactphysics3d::BodyType::DYNAMIC);
    m_grabbedBody = nullptr;
    m_recordingAngular = false;

    std::cout << "Released with velocity: " << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
}

void Raycaster::throwForward(float speed) {
    if (!m_grabbedBody) return;
    
    // Direction from the camera's front
    glm::vec3 forwardDir = m_camera.Front;
    reactphysics3d::Vector3 velocity(forwardDir.x * speed,
                                     forwardDir.y * speed,
                                     forwardDir.z * speed);
    
    m_grabbedBody->setLinearVelocity(velocity);
    m_grabbedBody->setAngularVelocity(reactphysics3d::Vector3(0, 0, 0));
    m_grabbedBody->setType(reactphysics3d::BodyType::DYNAMIC);
    m_grabbedBody->setIsSleeping(false);
    m_grabbedBody = nullptr;
    m_recordingAngular = false;
    
    std::cout << "Forward throw with velocity: " << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
}