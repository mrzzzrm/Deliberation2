#include <iostream>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Box.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/Physics/Contacts/BoxContact.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/DebugGeometryRenderer.h>
#include <Deliberation/Scene/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/DebugCameraNavigator3D.h>

using namespace deliberation;

void TestBoxes(const Box & boxA, const Box & boxB)
{
//    std::cout << "  -------------- TestBoxes ----------------" << std::endl;
//
//
//
//    auto r = CollideBoxBox(boxA, boxB, manifold);
//
//    if (r)
//    {
//        std::cout << "    Collision";
//    }
//    else
//    {
//        std::cout << "    Separated";
//    }
}

void AABBTest()
{
    std::cout << "---------------- AABBTest ----------------" << std::endl;

    Box originUnitBox({0, 0, 0}, {0.5f, 0, 0}, {0, 0.5f, 0}, {0, 0, 0.5f});
    Box testBox0({0.3, 0, 0}, {0.5f, 0, 0}, {0, 0.5f, 0}, {0, 0, 0.5f});

    TestBoxes(originUnitBox, testBox0);
}

void BoxBoxExample()
{
//    Manifold manifold;

//    Box a({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
//    Box b({1.4f, 1.4f, 0.0f}, {0.71f, 0.71f, 0.0f}, {-0.71f, 0.71f, 0.0f}, {0.0f, 0.0f, 1.0f});

//    auto r = CollideBoxBox(a, b, manifold);

//    if (r)
//    {
//        std::cout << "Manifold: " << manifold.toString() << std::endl;
//    }
//    else
//    {
//        std::cout << "No collision" << std::endl;
//    }
}


class BoxBoxSandbox:
    public Application
{
public:
    BoxBoxSandbox():
        Application("BoxBoxSandbox")
    {

    }

    virtual void onStartup() override
    {
        m_grid.reset(context(), 2.5f, m_camera);

        m_camera.setPosition({0.0f, 5.0f, 15.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / (float)context().backbuffer().height());

        m_clear = context().createClear();

        m_navigator.reset(m_camera, inputAdapter(), 10.0f);

        m_geometryRenderer.reset(context(), m_camera);

        m_debugInfoRenderer.reset(context(), m_camera);
        m_debugInfoRenderer.get().allocatePoints(4, {1.0f, 0.5f, 0.5f}, true); // Reference Face
        m_debugInfoRenderer.get().allocatePoints(4, {0.5f, 1.0f, 0.5f}, true); // Incident Face
        m_debugInfoRenderer.get().allocatePoints(8, {0.5f, 0.5f, 1.0f}, false);
        m_debugInfoRenderer.get().allocateArrows(2, {0.2f, 0.9f, 0.2f}, false);
        m_debugInfoRenderer.get().allocateArrows(4, {0.9f, 0.9f, 0.2f}, false); // Contacts

        m_boxB = m_geometryRenderer.get().addBox({5.0f, 1.0f, 1.0f}, {1.0f, 0.6f, 0.6f}, true);
        m_geometryRenderer.get().box(m_boxB).transform().setPosition({10.9006f,0.191372f,6.29913e-09f});
        m_geometryRenderer.get().box(m_boxB).transform().setOrientation({0.528583,0.0519542,0.109405,-0.840297});

        m_boxA = m_geometryRenderer.get().addBox({10.0f, 1.0f, 6.0f}, {0.0f, 1.0f, 0.6f}, true);
        m_geometryRenderer.get().box(m_boxA).transform().setPosition({1.98752e-06,-0.000352063,-1.2599e-13});
        m_geometryRenderer.get().box(m_boxA).transform().setOrientation({1,5.162e-08,-3.177e-08,-5.23162e-05});
//        m_geometryRenderer.get().box(m_boxB).transform().setOrientation(
//            glm::angleAxis(glm::quarter_pi<float>(),
//                           glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f))));

        auto box = m_geometryRenderer.get().box(m_boxA).toBox();
        auto p = box.p();
        m_halfExtent[0] = m_geometryRenderer.get().addArrow(p, box.x(), {0.8f, 0.2f, 0.2f});
        m_halfExtent[1] = m_geometryRenderer.get().addArrow(p, box.y(), {0.2f, 0.8f, 0.2f});
        m_halfExtent[2] = m_geometryRenderer.get().addArrow(p, box.z(), {0.2f, 0.2f, 0.8f});
    }

    virtual void onFrame(float seconds) override
    {
        seconds = 0.002f;

        m_navigator.get().update(seconds);

        {
            auto box = m_geometryRenderer.get().box(m_boxA).toBox();
            auto p = box.p();
             m_geometryRenderer.get().arrow(m_halfExtent[0]).reset(p, box.x() * box.halfExtent().x);
             m_geometryRenderer.get().arrow(m_halfExtent[1]).reset(p, box.y() * box.halfExtent().y);
             m_geometryRenderer.get().arrow(m_halfExtent[2]).reset(p, box.z() * box.halfExtent().z);
        }

        auto prevTransform = m_geometryRenderer.get().box(0).transform();

        {
            glm::vec3 t;
            glm::vec3 a;
            auto speed = 5.0f;
            auto r = glm::pi<float>();

            t.x += -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_J) ? 1 : 0);
            t.x -= -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_L) ? 1 : 0);
            t.y += -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_K) ? 1 : 0);
            t.y -= -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_I) ? 1 : 0);
            t.z += -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_U) ? 1 : 0);
            t.z -= -speed * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_O) ? 1 : 0);

            a.x += -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_Y) ? 1 : 0);
            a.x -= -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_X) ? 1 : 0);
            a.y += -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_C) ? 1 : 0);
            a.y -= -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_V) ? 1 : 0);
            a.z += -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_B) ? 1 : 0);
            a.z -= -r * seconds * (inputAdapter().keyDown(InputAdapterBase::Key_N) ? 1 : 0);

            m_geometryRenderer.get().box(0).transform().worldTranslate(t);
            m_geometryRenderer.get().box(0).transform().worldRotate(glm::quat(a));

            std::cout << m_geometryRenderer.get().box(0).transform().position() << " " <<
                                                                                       m_geometryRenderer.get().box(0).transform().orientation() << std::endl;
        }

        CollideBox3DDebugInfo debugInfo;

        CollideBox3D collision(m_geometryRenderer.get().box(m_boxA).toBox(),
                               m_geometryRenderer.get().box(m_boxB).toBox(),
                               &debugInfo);
        auto r = collision.execute();

//        std::cout << "Collision: " << r << " " << manifold.toString() << std::endl;

        if (r)
        {
            m_debugInfoRenderer.get().setVisible(!debugInfo.edgeCollision);
            if (!debugInfo.edgeCollision)
            {
                for (int v = 0; v < 4; v++)
                {
                    m_debugInfoRenderer.get().point(v).setPosition(debugInfo.referenceFace[v]);
                    m_debugInfoRenderer.get().point(v + 4).setPosition(debugInfo.incidentFace[v]);
                }
                for (uint v = 0; v < debugInfo.numClipPoints; v++)
                {
                    m_debugInfoRenderer.get().point(v + 8).setPosition(debugInfo.clipPoints[v]);
                    m_debugInfoRenderer.get().point(v + 8).setVisible(true);
                }
                for (uint v = debugInfo.numClipPoints; v < 8; v++)
                {
                    m_debugInfoRenderer.get().point(v + 8).setVisible(false);
                }
                m_debugInfoRenderer.get().arrow(0).reset(debugInfo.incidentFaceCenter, debugInfo.incidentFaceNormal);
                m_debugInfoRenderer.get().arrow(0).setVisible(true);
                m_debugInfoRenderer.get().arrow(1).reset(debugInfo.referenceFaceCenter, debugInfo.referenceFaceNormal);
                m_debugInfoRenderer.get().arrow(1).setVisible(true);
            }

            for (auto a = 0; a < collision.numIntersections; a++)
            {
                m_debugInfoRenderer.get().arrow(a + 2).setVisible(true);
                m_debugInfoRenderer.get().arrow(a + 2).reset(collision.intersections[a].position, collision.intersections[a].normal);
            }

            for (auto a = collision.numIntersections; a < 4; a++)
            {
                m_debugInfoRenderer.get().arrow(a + 2).setVisible(false);
            }

            m_geometryRenderer.get().box(0).transform() = prevTransform;

            auto origin = collision.intersections[0].position - collision.intersections[0].normal / 2.0f;
            auto delta = collision.intersections[0].normal;

            if (!m_contactSeparationIndex.engaged())
            {
                m_contactSeparationIndex.reset(
                    m_geometryRenderer.get().addArrow(origin, delta, glm::vec3(1.0f, 1.0f, 1.0f))
                );
            }
            else
            {
                m_geometryRenderer.get().arrow(m_contactSeparationIndex.get()).reset(origin, delta);
            }


            if (!m_contactPointIndex.engaged())
            {
                m_contactPointIndex.reset(
                    m_geometryRenderer.get().addPoint(collision.intersections[0].position, glm::vec3(1.0f, 1.0f, 1.0f))
                );
            }
            else
            {
                m_geometryRenderer.get().point(m_contactPointIndex.get()).setPosition(collision.intersections[0].position);
            }
        }
        else
        {
            m_debugInfoRenderer.get().setVisible(false);

            if (m_contactSeparationIndex.engaged())
            {
                m_geometryRenderer.get().removeArrow(m_contactSeparationIndex.get());
                m_contactSeparationIndex.disengage();
            }

            if (m_contactPointIndex.engaged())
            {
                m_geometryRenderer.get().removePoint(m_contactPointIndex.get());
                m_contactPointIndex.disengage();
            }
        }

        m_clear.schedule();
        m_grid.get().draw();
        m_geometryRenderer.get().schedule();
        m_debugInfoRenderer.get().schedule();
    }

private:
    Optional<DebugGeometryRenderer>     m_geometryRenderer;
    Optional<DebugGeometryRenderer>     m_debugInfoRenderer;
    Clear                               m_clear;
    Optional<DebugGrid3DRenderer>       m_grid;
    Camera3D                            m_camera;
    Uniform                             m_viewProjectionHandle;
    Uniform                             m_transformHandle;
    Optional<DebugCameraNavigator3D>    m_navigator;

    Optional<size_t>                    m_contactPointIndex;
    Optional<size_t>                    m_contactSeparationIndex;

    size_t                              m_boxA;
    size_t                              m_boxB;
    size_t                              m_halfExtent[3];
};

int main()
{
  //  AABBTest();
  //  BoxBoxExample(); return 0;

    return BoxBoxSandbox().run();
}