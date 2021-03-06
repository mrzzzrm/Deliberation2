#include <Deliberation/Draw/Surface.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

#include "Detail/SurfaceDownloadImpl.h"
#include "Detail/SurfaceImpl.h"
#include "Detail/TextureImpl.h"

namespace deliberation
{
Surface::Surface(const std::shared_ptr<SurfaceImpl> & impl) : Base(impl) {}

unsigned int Surface::face() const
{
    AssertM(m_impl.get(), "Surface object is hollow");

    return m_impl->face;
}

unsigned int Surface::width() const
{
    AssertM(m_impl.get(), "Surface object is hollow");

    return m_impl->textureImpl->width; // No mipmaps yet
}

unsigned int Surface::height() const
{
    AssertM(m_impl.get(), "Surface object is hollow");

    return m_impl->textureImpl->height; // No mipmaps yet
}

PixelFormat Surface::format() const { return m_impl->textureImpl->format; }

SurfaceDownload Surface::download() const
{
    AssertM(m_impl.get(), "Surface object is hollow");

    return SurfaceDownload(std::make_shared<SurfaceDownloadImpl>(*this));
}

std::string Surface::toString() const
{
    AssertM(m_impl.get(), "Surface object is hollow");
    return std::string("Surface {GL-Object: ") +
           std::to_string(m_impl->textureImpl->glName) +
           "; Format: " + m_impl->textureImpl->format.toString() + "}";
}
}
