#include <Deliberation/Draw/Buffer.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include "Detail/BufferImpl.h"

namespace deliberation
{

Buffer::Buffer():
    m_impl(nullptr)
{

}

const DataLayout & Buffer::layout() const
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return m_impl->layout;
}

unsigned int Buffer::count() const
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return m_impl->count;
}

unsigned int Buffer::size() const
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return count() * layout().stride();
}

std::string Buffer::toString() const
{
    return
        "Layout: " + layout().toString() + "\n"
        "Count: " + std::to_string(count());
}

Buffer::Buffer(const std::shared_ptr<detail::BufferImpl> & impl):
    m_impl(impl)
{
}

BufferUpload Buffer::createUpload(const Blob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return BufferUpload(m_impl->context, *this, data);
}

BufferUpload Buffer::createRawUpload(const Blob & data, unsigned int count)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return BufferUpload(m_impl->context, *this, data, count);
}

void Buffer::scheduleUpload(const Blob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    BufferUpload(m_impl->context, *this, data).schedule();
}

void Buffer::scheduleRawUpload(const Blob & data, unsigned int count)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    BufferUpload(m_impl->context, *this, data, count).schedule();
}

void Buffer::scheduleUpload(const LayoutedBlob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(layout().equals(data.layout()), "Layout mismatch");

    BufferUpload(m_impl->context, *this, data.rawData(), data.count()).schedule();
}

}

