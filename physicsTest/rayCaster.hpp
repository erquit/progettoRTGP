#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include "camera.hpp"
#include "global.hpp"
#include <GLFW/glfw3.h>

class Raycaster {
public:
    Raycaster(reactphysics3d::PhysicsWorld* world, const Camera& camera,
              int windowWidth, int windowHeight,
              reactphysics3d::RigidBody* groundBody = nullptr);

    void setGrabbedBody(reactphysics3d::RigidBody* body, const glm::vec3& localOffset);
    reactphysics3d::RigidBody* getGrabbedBody() const { return m_grabbedBody; }

    bool castFromCenter(reactphysics3d::RigidBody*& outHitBody, reactphysics3d::Vector3& outHitPoint) const;
    void updateGrabbed();
    void releaseGrabbed(float throwStrength, float totalTime,
                        const glm::vec3& lastCameraPos,
                        const glm::vec3& currentCameraPos);
    void throwForward(float speed);

    void updateFlickReset();

private:
    reactphysics3d::PhysicsWorld* m_world;
    const Camera& m_camera;
    reactphysics3d::RigidBody* m_grabbedBody = nullptr;
    reactphysics3d::RigidBody* m_groundBody = nullptr;
    glm::vec3 m_grabLocalOffset;

    float m_startYaw = 0.0f;
    float m_startPitch = 0.0f;
    bool m_recordingAngular = false;

    class PickCallback : public reactphysics3d::RaycastCallback {
    public:
        reactphysics3d::RigidBody* hitBody = nullptr;
        reactphysics3d::Vector3 hitPoint;
        bool hit = false;
        float hitFraction = 1.0f;
        virtual reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo& info) override;
    };
};

#endif