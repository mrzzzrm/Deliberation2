#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/TextureLoader.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/SkyboxRenderer.h>
#include <Deliberation/Scene/Debug/DebugCubemapRenderer.h>
#include <Deliberation/Scene/Debug/DebugTexture2dRenderer.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/MeshCompiler.h>

using namespace deliberation;

class SceneSandbox:
    public Application
{
public:
    SceneSandbox():
        Application("SceneSandbox")
    {

    }

    virtual void onStartup() override
    {
       // EnableGLErrorChecksAndLogging();

        m_ground.reset(context(), m_camera);
        m_ground.get().setSize(30.0f);
        m_ground.get().setRadius(10.0f);

        m_camera.setPosition({0.0f, 1.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_clear = context().createClear();

        m_transform.setPosition({0.0f, 1.0f, 0.0f});
        m_navigator.reset(m_camera, input(), 10.0f);

        auto skyboxPaths = std::array<std::string, 6> {
            DeliberationDataPath("Data/Skybox/Cloudy/Right.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Left.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Top.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Bottom.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Back.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Front.png")
        };

        auto faceTexture = context().createTexture(
            TextureLoader(DeliberationDataPath("Data/Skybox/Debug/Right.png")).load());

        auto skyboxCubemapBinary = TextureLoader(skyboxPaths).load();
        auto skyboxCubemap = context().createTexture(skyboxCubemapBinary);

        m_skyboxRenderer.reset(context(), m_camera, skyboxCubemap);

        m_cubemapRenderer.reset(context(), m_camera, skyboxCubemap, DebugCubemapRenderer::MeshType::Sphere);
        m_cubemapRenderer->setPose(Pose3D::atPosition({10.0f, 10.0f, 0.0f}));
    }


    virtual void onFrame(float seconds) override
    {
        m_navigator.get().update(seconds);

        m_clear.render();

        m_cubemapRenderer->render();
        m_skyboxRenderer->render();
        m_ground.get().render();
    }

private:
    Draw                                m_draw;
    Clear                               m_clear;
    Optional<DebugGroundPlaneRenderer>  m_ground;
    Camera3D                            m_camera;
    Transform3D                         m_transform;
    Optional<DebugCameraNavigator3D>    m_navigator;
    Optional<SkyboxRenderer>            m_skyboxRenderer;
    Optional<DebugCubemapRenderer>      m_cubemapRenderer;
};

int main(int argc, char * argv[])
{
    return SceneSandbox().run(argc, argv);
}
