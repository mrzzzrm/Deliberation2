#include <Deliberation/Draw/Sampler.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Texture.h>

#include "Detail/SamplerImpl.h"

namespace deliberation
{

Sampler::Sampler():
    m_impl(nullptr)
{

}

void Sampler::setTexture(const Texture & texture)
{
    Assert(m_impl, "Hollow Sampler can't be set");
    Assert(texture.type() == m_impl->textureType(), "Texture type mismatch");

    m_impl->texture = texture.m_impl.get();
}

void Sampler::setWrap(gl::GLenum wrap)
{
    Assert(m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(0, wrap);
    m_impl->glSampler.setWrap(1, wrap);
    m_impl->glSampler.setWrap(2, wrap);
}

void Sampler::setWrapS(gl::GLenum wrap)
{
    Assert(m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(0, wrap);
}

void Sampler::setWrapT(gl::GLenum wrap)
{
    Assert(m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(1, wrap);
}

void Sampler::setWrapR(gl::GLenum wrap)
{
    Assert(m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(2, wrap);
}

Sampler::Sampler(detail::SamplerImpl & impl):
    m_impl(&impl)
{

}

}
