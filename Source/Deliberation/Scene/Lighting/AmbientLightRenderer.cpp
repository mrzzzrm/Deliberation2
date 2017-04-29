#include <Deliberation/Scene/Lighting/AmbientLightRenderer.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/ScreenSpaceEffect.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

namespace deliberation
{

class AmbientLightNode:
    public RenderNode
{
public:
    AmbientLightNode(AmbientLightRenderer & renderer):
        m_renderer(renderer),
        RenderNode(renderer.renderManager())
    {
        auto & drawContext = m_renderer.renderManager().drawContext();

        m_effect = ScreenSpaceEffect(drawContext, {DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
            DeliberationDataPath("Data/Shaders/AmbientLight.frag")}, "AmbientLight");

        m_effect.draw().setFramebuffer(m_renderer.renderManager().hdrBuffer());
        m_effect.draw().sampler("Ssao").setTexture(m_renderer.renderManager().ssaoBuffer().renderTarget(0)->texture());
        m_effect.draw().sampler("Diffuse").setTexture(m_renderer.renderManager().gbuffer().renderTarget(0)->texture());
    }

    void render() override
    {
        m_effect.schedule();
    }

private:
    AmbientLightRenderer &  m_renderer;
    ScreenSpaceEffect       m_effect;
};

AmbientLightRenderer::AmbientLightRenderer(RenderManager & renderManager):
    Renderer(renderManager)
{
}

void AmbientLightRenderer::registerRenderNodes()
{
    m_renderManager.registerRenderNode(std::make_shared<AmbientLightNode>(*this), RenderPhase::Lighting);
}

}