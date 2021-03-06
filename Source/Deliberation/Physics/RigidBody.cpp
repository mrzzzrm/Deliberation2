#include <Deliberation/Physics/RigidBody.h>

#include <sstream>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btDefaultMotionState.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/GLMUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/StreamUtils.h>

namespace
{
class BulletCollisionShapeWrapper:
    public btCollisionShape
{
public:
    BulletCollisionShapeWrapper(deliberation::RigidBody & body):
        m_rigidBody(body)
    {}

    void getAabb(const btTransform& bulletTransform, btVector3& aabbMin, btVector3& aabbMax) const override
    {
        auto transform = deliberation::BulletPhysicsConvert(bulletTransform);
        transform.setScale(m_rigidBody.transform().scale());
        transform.setScale(m_rigidBody.transform().scale());
        auto bounds = m_rigidBody.shape()->bounds(transform);
        aabbMin = deliberation::BulletPhysicsConvert(bounds.llf());
        aabbMax = deliberation::BulletPhysicsConvert(bounds.urb());
    }

    void setLocalScaling(const btVector3& scaling) override {
        m_localScaling = scaling;
    }

    const btVector3& getLocalScaling() const override {
        return m_localScaling;
    }

    void calculateLocalInertia(btScalar mass, btVector3& inertia) const override {
        Fail("What does this even do")
    }

    const char*	getName() const override {
        return "WrappedShape";
    }

    btScalar getMargin() const override {
        return m_collisionMargin;
    }

    void setMargin(btScalar collisionMargin) override {
        m_collisionMargin = collisionMargin;
    }

private:
    deliberation::RigidBody & m_rigidBody;
    btVector3 m_localScaling;
    btScalar m_collisionMargin = 0.0f;
};
}

namespace deliberation
{
RigidBody::RigidBody(
    const std::shared_ptr<CollisionShape> & shape):
    m_shape(shape)
{
    m_btCollisionShape = std::make_shared<BulletCollisionShapeWrapper>(
        *this
    );

    m_btMotionState = std::make_shared<btDefaultMotionState>(btTransform());

    btRigidBody::btRigidBodyConstructionInfo constructionInfo(
        shape->mass(1.0f),
        m_btMotionState.get(),
        m_btCollisionShape.get(),
        BulletPhysicsConvert(shape->localInertia(1.0f))
    );

    m_btRigidBody = std::make_shared<btRigidBody>(constructionInfo);
    m_btRigidBody->setUserPointer(this);
    m_btRigidBody->setActivationState(DISABLE_DEACTIVATION);

    m_transform.setCenterAndAdjustPosition(m_shape->centerOfMass());
}

const Transform3D & RigidBody::transform() const
{
    btTransform bulletTransform = m_btRigidBody->getWorldTransform();
    //m_btMotionState->getWorldTransform(bulletTransform);

    m_transform.setPosition(BulletPhysicsConvert(bulletTransform.getOrigin()));
    m_transform.setOrientation(BulletPhysicsConvert(bulletTransform.getRotation()));

    return m_transform;
}

void RigidBody::predictTransform(float seconds, Transform3D & prediction) const {
    btTransform btPrediction;
    m_btRigidBody->predictIntegratedTransform(seconds, btPrediction);
    prediction = BulletPhysicsConvert(btPrediction);
}

void RigidBody::setPosition(const glm::vec3 & position)
{
    m_transform.setPosition(position);
    m_btRigidBody->proceedToTransform(BulletPhysicsConvert(m_transform));
}

void RigidBody::setOrientation(const glm::quat & orientation)
{
    m_transform.setOrientation(orientation);
    m_btRigidBody->proceedToTransform(BulletPhysicsConvert(m_transform));
}

void RigidBody::setScale(float scale)
{
    if (m_transform.scale() == scale) return;

    m_transform.setScale(scale);
    updateMassAndInertia();
}

//void RigidBody::setTransform(const Transform3D & transform)
//{
//    const auto centerCorrection = (shape()->centerOfMass() - transform.center()) * transform.scale();
//
//    setPosition(transform.position() + transform.orientation() * centerCorrection);
//    setOrientation(transform.orientation());
//    setScale(transform.scale());
//}

void RigidBody::adaptShape()
{
    shape()->updateMassProperties();
    updateCenterOfMass();
    updateMassAndInertia();
}

void RigidBody::updateMassAndInertia()
{
    m_btRigidBody->setMassProps(m_shape->mass(m_transform.scale()),
                                BulletPhysicsConvert(m_shape->localInertia(m_transform.scale())));
    m_btRigidBody->updateInertiaTensor();
}

void RigidBody::updateCenterOfMass()
{
    if (m_transform.center() == m_shape->centerOfMass()) return;

    auto delta = m_shape->centerOfMass() - m_transform.center();

    m_transform.setCenter(m_shape->centerOfMass());
    m_transform.worldTranslate(
        m_transform.orientation() * (delta * m_transform.scale()));

    m_btRigidBody->proceedToTransform(BulletPhysicsConvert(m_transform));
}


}
