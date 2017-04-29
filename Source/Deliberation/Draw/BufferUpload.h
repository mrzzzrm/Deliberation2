#pragma once

#include <vector>



#include <Deliberation/Core/Blob.h>

namespace deliberation
{

class Buffer;
class DrawContext;

class BufferUpload final
{
public:
    BufferUpload(DrawContext & drawContext, Buffer & buffer, const Blob & data);
    BufferUpload(DrawContext & drawContext, Buffer & buffer, const Blob & data, unsigned int count);

    Buffer & buffer() const;
    const Blob & data() const;
    unsigned int count() const;

    void schedule();

private:
    DrawContext &   m_drawContext;
    Buffer &        m_buffer;
    Blob            m_blob;
    unsigned int    m_count;
};

}

