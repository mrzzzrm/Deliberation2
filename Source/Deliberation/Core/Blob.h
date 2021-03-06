#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Deliberation/Core/Detail/IBlobImpl.h>

namespace deliberation
{
class Blob final
{
public:
    template<typename T>
    static Blob fromValue(const T & value);

    static Blob fromRawData(const void * src, size_t size);

public:
    Blob();

    Blob(std::size_t size);

    /*
        TODO
            port all these to static, named builders
    */

    template<typename T>
    Blob(const std::vector<T> & value);

    template<typename T>
    Blob(std::vector<T> && value);

    Blob(std::unique_ptr<detail::IBlobImpl> && impl);

    Blob(const Blob & blob);

    Blob(Blob && blob);

    std::size_t  size() const;
    void *       ptr(size_t offset = 0);
    const void * ptr(size_t offset = 0) const;
    void *       ptrRaw(size_t offset = 0);
    const void * ptrRaw(size_t offset = 0) const;

    template<typename T>
    void write(std::size_t offset, const T & value);

    void write(std::size_t offset, const void * src, size_t length);

    template<typename T>
    void read(std::size_t offset, T & value) const;

    template<typename T>
    T & access(size_t offset);

    template<typename T>
    const T & access(size_t offset) const;

    void resize(size_t size);
    void reserve(size_t size);

    std::string toString() const;

    Blob & operator=(const Blob & blob);
    Blob & operator=(Blob && blob);

private:
    std::unique_ptr<detail::IBlobImpl> m_impl;
};
}

#include <Deliberation/Core/Blob.inl>
