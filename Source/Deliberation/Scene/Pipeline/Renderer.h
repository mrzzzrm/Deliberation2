#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Draw/Framebuffer.h>

namespace deliberation
{
class DrawContext;
class RenderManager;

class Renderer : public std::enable_shared_from_this<Renderer>
{
public:
    Renderer(
        const std::string & name = {"Unnamed Renderer"})
        : m_name(name)
    {
    }

    virtual ~Renderer() = default;

    std::string     name() const { return m_name; }
    bool enabled() const { return m_enabled; }

    void setName(const std::string & name) { m_name = name; }
    void setEnabled(bool enabled) { m_enabled = enabled; }

    virtual void onRegisterRenderNodes() = 0;
    virtual void onBeforeRender() {};
    virtual void onCreated() {}

    void renderImGui();

protected:
    virtual void onRenderImGui() {}

private:
    std::string m_name;
    bool m_enabled = true;
};
}