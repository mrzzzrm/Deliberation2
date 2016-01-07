#include <Deliberation/Draw/TextureBinary.h>

#include <iostream>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

TextureBinary TextureBinary::emptyTexture2D(unsigned int width,
                                            unsigned int height,
                                            PixelFormat format)
{
    TextureBinary binary;

    binary.m_width = width;
    binary.m_height = height;
    binary.m_type = gl::GL_TEXTURE_2D;
    binary.m_faces = {SurfaceBinary::empty(width, height, format)};

    return binary;
}

TextureBinary::TextureBinary():
    m_type(gl::GL_NONE)
{
}

TextureBinary::TextureBinary(std::vector<float> && pixels,
                             unsigned int width,
                             unsigned int height,
                             PixelFormat format):
    m_type(gl::GL_TEXTURE_2D),
    m_width(width),
    m_height(height)
{
    m_faces.push_back(SurfaceBinary(std::move(pixels), m_width, m_height, format));
}

TextureBinary::TextureBinary(std::array<SurfaceBinary, 6> && cubeFaces):
    m_type(gl::GL_TEXTURE_CUBE_MAP)
{
    m_width = cubeFaces[0].width();
    m_height = cubeFaces[0].height();

    m_faces.reserve(6);
    for (auto & face : cubeFaces)
    {
        Assert(face.width() == m_width && face.height() == m_height, "");
        m_faces.push_back(std::move(face));
    }
}

unsigned int TextureBinary::width() const
{
    return m_width;
}

unsigned int TextureBinary::height() const
{
    return m_height;
}

unsigned int TextureBinary::numFaces() const
{
    return m_faces.size();
}

gl::GLenum TextureBinary::type() const
{
    return m_type;
}

const SurfaceBinary & TextureBinary::surface(unsigned int face) const
{
    Assert(face < m_faces.size(), "Surface '" + std::to_string(face) + "' out of range '" +
                                  std::to_string(m_faces.size()) + "'");
    return m_faces[face];
}

}
