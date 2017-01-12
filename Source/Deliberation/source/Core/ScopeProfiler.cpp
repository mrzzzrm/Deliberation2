#include <Deliberation/Core/ScopeProfiler.h>

#include <iostream>

namespace deliberation
{

ScopeProfiler::ScopeProfiler(const std::string & name):
    m_name(name)
{
    m_start = CurrentMicros();
}

ScopeProfiler::~ScopeProfiler()
{
    auto end = CurrentMicros();

    auto micros = (end - m_start);
    auto millis = micros / 1000;
    micros -= millis * 1000;

    std::cout << m_name << ": " << millis << "," << micros << "ms" << std::endl;
}

}
