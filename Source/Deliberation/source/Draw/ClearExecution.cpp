#include "ClearExecution.h"

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/GL/GLStateManager.h>

#include "Detail/ClearImpl.h"

namespace deliberation
{

ClearExecution::ClearExecution(GLStateManager & glStateManager, const Clear & clear):
    m_glStateManager(glStateManager),
    m_clear(clear)
{
}

void ClearExecution::perform()
{
    m_clear.m_impl->framebuffer.bind(m_glStateManager);

    m_glStateManager.setDepthMask(true);
    m_glStateManager.setStencilMask(~0);
    m_glStateManager.setClearStencil(0);

    if (m_clear.m_impl->framebuffer.isBackbuffer())
    {
        clearBackbuffer();
    }
    else
    {
        clearFramebuffer();
    }
}

void ClearExecution::clearBackbuffer()
{
    auto flags = gl::ClearBufferMask::GL_NONE_BIT;

    if (m_clear.m_impl->clearColor)
    {
        auto & c = m_clear.m_impl->color;
        m_glStateManager.setClearColor(c.r, c.g, c.b, c.a);
        flags |= gl::GL_COLOR_BUFFER_BIT;
    }
    if (m_clear.m_impl->clearDepth)
    {
        m_glStateManager.setClearDepth(m_clear.m_impl->depth);
        flags |= gl::GL_DEPTH_BUFFER_BIT;
    }
    if (m_clear.m_impl->clearStencil)
    {
        m_glStateManager.setClearStencil(m_clear.m_impl->stencil);
        flags |= gl::GL_STENCIL_BUFFER_BIT;
    }

    if (flags != gl::ClearBufferMask::GL_NONE_BIT)
    {
        gl::glClear(flags);
    }
}

void ClearExecution::clearFramebuffer()
{
    for (auto b = 0u; b < m_clear.m_impl->framebuffer.surfaces().size(); b++)
    {
        gl::glClearBufferfv(gl::GL_COLOR, b, &m_clear.m_impl->color[0]);
    }
}

}

