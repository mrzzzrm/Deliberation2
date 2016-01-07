#include <Deliberation/Draw/Surface.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Texture.h>

#include "Detail/TextureImpl.h"

namespace deliberation
{

Surface::Surface():
    m_texture(nullptr),
    m_face(0u)
{

}

Texture Surface::texture() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return Texture(m_texture);
}

unsigned int Surface::face() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return m_face;
}

unsigned int Surface::width() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return m_texture->width; // No mipmaps yet
}

unsigned int Surface::height() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return m_texture->height; // No mipmaps yet
}

Surface::Surface(const std::shared_ptr<detail::TextureImpl> & texture,
                 unsigned int face):
    m_texture(texture),
    m_face(face)
{

}

}
