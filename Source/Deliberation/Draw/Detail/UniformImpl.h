#pragma once

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/Types.h>

namespace deliberation
{
class UniformImpl final
{
public:
    UniformImpl(Type type, unsigned int location, unsigned int arraySize);

    Type         type;
    unsigned int location;
    bool         isAssigned;
    Blob         blob;
    unsigned int arraySize; // Maximum array size
    unsigned int count;     // Number of array slots in blob
};
}
