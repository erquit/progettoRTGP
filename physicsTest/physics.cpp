#include "physics.hpp"



void initPhysics(PhysicsCommon& physicsCommon, PhysicsWorld*& world) {
    PhysicsWorld::WorldSettings settings;
    settings.defaultVelocitySolverNbIterations = 20; //Bumed up from 10
    settings.defaultPositionSolverNbIterations = 10; //Bumed up from 5
    settings.isSleepingEnabled = true;
    settings.gravity = Vector3(0.0f, -9.81f, 0.0f);

    world = physicsCommon.createPhysicsWorld(settings);
    world->setIsDebugRenderingEnabled(true);
    DebugRenderer& debugRenderer = world->getDebugRenderer();
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_POINT, true);
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLIDER_AABB, true);
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE_NORMAL, true);

}

void createRigidBodies(PhysicsCommon& physicsCommon,PhysicsWorld* world, const glm::vec3 cubePositions[], 
    int numCubes, std::vector<RigidBody*>& rigidBodies) 
{

    for (int i = 0; i < numCubes; ++i) {

        Vector3 position(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
        Quaternion orientation = Quaternion::identity();
        Transform transform(position, orientation);

        RigidBody* body = world->createRigidBody(transform);
        body->setType(BodyType::DYNAMIC);
        body->enableGravity(true);
        body->setIsDebugEnabled(true);

        //Collider wrapper
        const Vector3 halfExtets(0.5f, 0.5f, 0.5f);
        BoxShape* boxShape = physicsCommon.createBoxShape(halfExtets);
        Collider* collider; 
        collider = body->addCollider(boxShape, Transform::identity());

        //Bounciness
        Material& material = collider->getMaterial();
        material.setBounciness(0.1f);
        material.setFrictionCoefficient(0.5f);

        //Set mass
        body->setMass(10.0f);

        rigidBodies.push_back(body);
    }
}

RigidBody* createRigidPlane(PhysicsCommon& physicsCommon, PhysicsWorld* world) {

        Vector3 position(0.0f, -0.5f, 0.0f);
        Quaternion orientation = Quaternion::identity();
        Transform transform(position, orientation);

        RigidBody* rigidPlane = world->createRigidBody(transform);
        rigidPlane->setType(BodyType::STATIC);
        rigidPlane->enableGravity(false);
        rigidPlane->setIsDebugEnabled(true);

        //Collider wrapper
        const Vector3 halfExtets(20.0f, 0.505f, 20.0f);
        BoxShape* boxShape = physicsCommon.createBoxShape(halfExtets);
        Collider* collider; 
        collider = rigidPlane->addCollider(boxShape, Transform::identity());

        //Bounciness
        Material& material = collider->getMaterial();
        material.setBounciness(0.0f);
        material.setFrictionCoefficient(0.5f);

        return rigidPlane;
}

void createRigidBox(PhysicsCommon& physicsCommon, PhysicsWorld* world, glm::vec3 wallpos[]) {

    float angle = 90.0f;
    Vector3 halfExtents;
    for (unsigned int i = 0; i < 3; i++)
    {
        Vector3 position(wallpos[i].x, wallpos[i].y, wallpos[i].z);
        Quaternion orientation = Quaternion::fromEulerAngles(0.0f, angle * (i + 1), 0.0f);
        Transform transform(position, orientation);

        RigidBody* rigidPlane = world->createRigidBody(transform);
        rigidPlane->setType(BodyType::STATIC);
        rigidPlane->enableGravity(false);

        //Collider wrapper
        if(i%2 == 1){
            halfExtents = Vector3(5.0f, 5.0f, 0.505f);
        }else{
            halfExtents = Vector3(0.505f, 5.0f, 10.0f);
        }
        BoxShape* boxShape = physicsCommon.createBoxShape(halfExtents);
        Collider* collider; 
        collider = rigidPlane->addCollider(boxShape, Transform::identity());

        //Bounciness
        Material& material = collider->getMaterial();
        material.setBounciness(0.0f);
        material.setFrictionCoefficient(0.5f);
    }
}

void createRoom(PhysicsCommon& physicsCommon, PhysicsWorld* world,
                float width, float height, float depth,
                glm::vec3 roomCenter)
{
    const float thickness = 0.05f;
    const float halfThick = thickness / 2.0f;

    struct WallDef {
        glm::vec3 positionOffset;
        float yaw;
        Vector3 halfExtents;
    };

    std::vector<WallDef> walls = {
        // back wall
        {{0, 0, depth/2}, 0, Vector3(width/2, height/2, halfThick)},
        // front wall
        {{0, 0, -depth/2}, 0, Vector3(width/2, height/2, halfThick)},
        // right wall
        {{width/2, 0, 0}, 90, Vector3(depth/2, height/2, halfThick)},
        // left wall
        {{-width/2, 0, 0}, 90, Vector3(depth/2, height/2, halfThick)},
    };

    for (const auto& w : walls) {
        glm::vec3 pos = roomCenter + w.positionOffset;
        Quaternion orientation = Quaternion::fromEulerAngles(0, glm::radians(w.yaw), 0);
        Transform transform(Vector3(pos.x, pos.y, pos.z), orientation);

        RigidBody* wall = world->createRigidBody(transform);
        wall->setType(BodyType::STATIC);
        wall->enableGravity(false);

        BoxShape* shape = physicsCommon.createBoxShape(w.halfExtents);
        wall->addCollider(shape, Transform::identity());
        wall->getCollider(0)->getMaterial().setBounciness(0.0f);
        wall->getCollider(0)->getMaterial().setFrictionCoefficient(0.5f);
    }

    // floor
    Vector3 floorPos(roomCenter.x, roomCenter.y - height/2, roomCenter.z);
    Transform floorTransform(floorPos, Quaternion::identity());
    RigidBody* floor = world->createRigidBody(floorTransform);
    floor->setType(BodyType::STATIC);
    floor->enableGravity(false);

    BoxShape* floorShape = physicsCommon.createBoxShape(
        Vector3(width/2, halfThick, depth/2));
    floor->addCollider(floorShape, Transform::identity());
    floor->getCollider(0)->getMaterial().setBounciness(0.0f);
    floor->getCollider(0)->getMaterial().setFrictionCoefficient(0.5f);

    // ceiling
    Vector3 ceilPos(roomCenter.x, roomCenter.y + height/2, roomCenter.z);
    Transform ceilTransform(ceilPos, Quaternion::identity());
    RigidBody* ceiling = world->createRigidBody(ceilTransform);
    ceiling->setType(BodyType::STATIC);
    ceiling->enableGravity(false);

    BoxShape* ceilShape = physicsCommon.createBoxShape(
        Vector3(width/2, halfThick, depth/2));
    ceiling->addCollider(ceilShape, Transform::identity());
    ceiling->getCollider(0)->getMaterial().setBounciness(0.0f);
    ceiling->getCollider(0)->getMaterial().setFrictionCoefficient(0.5f);
}

RigidBody* createRigidModels(PhysicsCommon& physicsCommon, PhysicsWorld* world, glm::vec3 vasePos, 
    std::optional<Vector3> halfExtents, std::optional<glm::vec2> capsuleSize)
{

    Vector3 position(vasePos.x, vasePos.y, vasePos.z);
    Quaternion orientation = Quaternion::identity();
    Transform transform(position, orientation);

    RigidBody* body = world->createRigidBody(transform);
    body->setType(BodyType::DYNAMIC);
    body->enableGravity(true);
    body->setIsDebugEnabled(true);
    body->setIsSleeping(false);
    body->setIsAllowedToSleep(false);

    Collider* collider; 
    if(halfExtents.has_value()){
        BoxShape* boxShape = physicsCommon.createBoxShape(halfExtents.value());
        collider = body->addCollider(boxShape, Transform::identity());

        //Bounciness
        Material& material = collider->getMaterial();
        material.setBounciness(0.1f);
        material.setFrictionCoefficient(0.5f);

        //Set mass
        body->setMass(5.0f);

    }else if(capsuleSize.has_value()){
        CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(capsuleSize.value().x, capsuleSize.value().y);
        collider = body->addCollider(capsuleShape, Transform::identity());

        //Bounciness
        Material& material = collider->getMaterial();
        material.setBounciness(0.1f);
        material.setFrictionCoefficient(0.9f);

        //Set mass
        body->setMass(15.0f);

    }else{
        std::cout << "Not a valid createRigidModels variables" << std::endl;
    }

    return body;
}

void updatePhysics(PhysicsCommon& physicsCommon, PhysicsWorld* world, std::vector<RigidBody*>& rigidBodies, 
    bool applyGravity, float deltaTime, float& accumulator, float timeStep) 
{
    if (!applyGravity) return;
    accumulator += deltaTime;
    while (accumulator >= timeStep) {
        world->update(timeStep);
        world->getDebugRenderer();
        accumulator -= timeStep;
    }
}

void updateCubesFromPhysics(const std::vector<RigidBody*>& rigidBodies, std::vector<glm::mat4>& cubeModels){

    cubeModels.resize(rigidBodies.size());

    for (unsigned int i = 0; i < rigidBodies.size(); i++)
    {
        if(rigidBodies[i] == nullptr){
            cubeModels[i] = glm::mat4(1.0f);
            continue;
        }
        const Transform& transform = rigidBodies[i]->getTransform();
        float matrix[16];
        transform.getOpenGLMatrix(matrix);
        cubeModels[i] = glm::make_mat4(matrix);
    }
    
}


void updateModelFromPhysics(RigidBody* rigidVase, glm::mat4& vasePos, glm::vec3 scale){
    if(rigidVase == nullptr) return;
    const Transform& transform = rigidVase->getTransform();
    float matrix[16];
    transform.getOpenGLMatrix(matrix);
    vasePos = glm::make_mat4(matrix);

    // Scaling down te model if too big
    vasePos = vasePos * glm::scale(glm::mat4(1.0f), scale);

}


void updateEmissionLights(glm::vec3 emissionPointLightPositions[], std::vector<glm::mat4>& emissionRigidBodiesModels, int numEmissionCubes){
    for (int i = 0; i < numEmissionCubes; i++){
        emissionPointLightPositions[i] = glm::vec3(emissionRigidBodiesModels[i][3]);
    }
}


void createCubeFragments(PhysicsCommon& physicsCommon, PhysicsWorld* world,
                         const Transform& originalTransform, float breakSpeed,
                         std::vector<RigidBody*>& outFragments,
                         std::vector<glm::mat4>& outModels) {

    const Vector3 boxHalfExtents(0.5f, 0.5f, 0.01f);
    BoxShape* shape = physicsCommon.createBoxShape(boxHalfExtents);

    // we'll create 6 fragmnts, one for each face direction
    std::vector<Vector3> directions = {
        Vector3( 1,  0,  0),  // right
        Vector3(-1,  0,  0),  // left
        Vector3( 0,  1,  0),  // up
        Vector3( 0, -1,  0),  // down
        Vector3( 0,  0,  1),  // front
        Vector3( 0,  0, -1)   // back
    };

    Vector3 breakPos = originalTransform.getPosition();
    for (int i = 0; i < 6; ++i) {
        RigidBody* fragment = world->createRigidBody(originalTransform);
        fragment->setType(BodyType::DYNAMIC);
        fragment->enableGravity(true);
        fragment->setMass(5.0f);
        fragment->setIsSleeping(false);
        fragment->setIsAllowedToSleep(false);
        fragment->setIsDebugEnabled(true);

        Collider* collider = fragment->addCollider(shape, Transform::identity());
        collider->getMaterial().setBounciness(0.2f);
        collider->getMaterial().setFrictionCoefficient(0.6f);

        // offset 
        Vector3 offset = directions[i] * 0.3f;
        Transform t = originalTransform;
        t.setPosition(breakPos + offset);
        fragment->setTransform(t);

        // Random velocity in random direction
        Vector3 dir = directions[i];
        float rx = ((rand() % 100) - 50) / 100.0f;
        float ry = ((rand() % 100) - 50) / 100.0f;
        float rz = ((rand() % 100) - 50) / 100.0f;
        dir = dir + Vector3(rx, ry, rz);
        dir.normalize();

        float speed = breakSpeed * 0.5f;
        fragment->setLinearVelocity(dir * speed);

        outFragments.push_back(fragment);

        glm::mat4 model(1.0f);
        outModels.push_back(model);
    }
}
