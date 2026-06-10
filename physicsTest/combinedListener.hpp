#ifndef COMBINED_LISTENER_HPP
#define COMBINED_LISTENER_HPP

#include <reactphysics3d/reactphysics3d.h>
#include <vector>
#include <cstdint>
#include "rayCaster.hpp"

using namespace reactphysics3d;

class CombinedBreakListener : public EventListener {
public:
    CombinedBreakListener(RigidBody* vase, RigidBody* maoi,
                          const std::vector<RigidBody*>& cubes,
                          Raycaster* rc)
        : m_vaseBody(vase), m_maoiBody(maoi), m_cubeBodies(cubes),
          m_raycaster(rc),
          m_vaseCollided(false), m_maoiCollided(false),
          m_cubeCollided(cubes.size(), false) {}

    virtual void onContact(const CollisionCallback::CallbackData& callbackData) override {
        for (uint32_t i = 0; i < callbackData.getNbContactPairs(); ++i) {
            CollisionCallback::ContactPair pair = callbackData.getContactPair(i);
            RigidBody* b1 = static_cast<RigidBody*>(pair.getBody1());
            RigidBody* b2 = static_cast<RigidBody*>(pair.getBody2());

            // Vase collision (ignore if vase is grabbed)
            if ((b1 == m_vaseBody || b2 == m_vaseBody) &&
                !(m_raycaster && m_raycaster->getGrabbedBody() == m_vaseBody)) {
                m_vaseCollided = true;
            }

            // Maoi collision (ignore if Maoi is grabbed)
            if ((b1 == m_maoiBody || b2 == m_maoiBody) &&
                !(m_raycaster && m_raycaster->getGrabbedBody() == m_maoiBody)) {
                m_maoiCollided = true;
            }

            // Cubes collision (ignore if that cube is grabbed)
            for (size_t j = 0; j < m_cubeBodies.size(); ++j) {
                RigidBody* cube = m_cubeBodies[j];
                if ((b1 == cube || b2 == cube) &&
                    !(m_raycaster && m_raycaster->getGrabbedBody() == cube)) {
                    m_cubeCollided[j] = true;
                }
            }
        }
    }

    void reset() {
        m_vaseCollided = false;
        m_maoiCollided = false;
        std::fill(m_cubeCollided.begin(), m_cubeCollided.end(), false);
    }

    bool vaseCollided() const { return m_vaseCollided; }
    bool maoiCollided() const { return m_maoiCollided; }
    bool cubeCollided(size_t index) const { return m_cubeCollided[index]; }
    void invalidateCube(size_t index) {
        if (index < m_cubeBodies.size()) m_cubeBodies[index] = nullptr;
    }

private:
    RigidBody* m_vaseBody;
    RigidBody* m_maoiBody;
    std::vector<RigidBody*> m_cubeBodies;
    Raycaster* m_raycaster;
    bool m_vaseCollided, m_maoiCollided;
    std::vector<bool> m_cubeCollided;
};

extern CombinedBreakListener* combinedListener;
#endif