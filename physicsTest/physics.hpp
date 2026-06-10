#ifndef PHYSICS_H
#define PHYSICS_H

#include <reactphysics3d/reactphysics3d.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "model.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "combinedListener.hpp"
#include <cstdlib>
#include <ctime>
#include <optional>

using namespace reactphysics3d;



void initPhysics(PhysicsCommon& physicsCommon, PhysicsWorld*& world);
void createRigidBodies(PhysicsCommon& physicsCommon, PhysicsWorld* world, const glm::vec3 cubePositions[], int numCubes, std::vector<RigidBody*>& rigidBodies);
void createRigidFragments(PhysicsCommon& physicsCommon, PhysicsWorld* world, const glm::vec3 fragmentPosition, int numFragments, std::vector<RigidBody*>& rigidFragments);
void updatePhysics(PhysicsCommon& physicsCommon,PhysicsWorld* world, std::vector<RigidBody*>& rigidBodies, bool applyGravity, float deltaTime, float& accumulator, float timeStep);
void updateCubesFromPhysics(const std::vector<RigidBody*>& rigidBodies, std::vector<glm::mat4>& cubeModels);
RigidBody* createRigidPlane(PhysicsCommon& physicsCommon, PhysicsWorld* world);
RigidBody* createRigidModels(PhysicsCommon& physicsCommon, PhysicsWorld* world, glm::vec3 vasePos, std::optional<Vector3> halfExtents = std::nullopt, std::optional<glm::vec2> capsuleSize = std::nullopt);
void updateModelFromPhysics(RigidBody* rigidVase, glm::mat4& vasePos, glm::vec3 scale);
void createRigidBox(PhysicsCommon& physicsCommon, PhysicsWorld* world, glm::vec3 wallpos[]);
void createRoom(PhysicsCommon& physicsCommon, PhysicsWorld* world,
                float width, float height, float depth, glm::vec3 roomCenter);
void updateEmissionLights(glm::vec3 emissionPointLightPositions[], std::vector<glm::mat4>& emissionRigidBodiesModels, int numEmissionCubes);

void createCubeFragments(PhysicsCommon& physicsCommon, PhysicsWorld* world,
                         const Transform& originalTransform, float breakSpeed,
                         std::vector<RigidBody*>& outFragments,
                         std::vector<glm::mat4>& outModels);

#endif