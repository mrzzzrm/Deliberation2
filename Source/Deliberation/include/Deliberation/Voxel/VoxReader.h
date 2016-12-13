#pragma once

#include <array>
#include <fstream>
#include <vector>

#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Voxel/VoxelCluster.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API VoxReader final
{
public:
    struct Chunk
    {
        std::array<char, 4> type;
        u32 numBytes = 0;
        u32 numBytesChildren = 0;
    };

    struct Voxel
    {
        glm::u8vec3 position;
        u8 colorIndex;
    };

    struct VoxelModel
    {
        glm::u32vec3 size;
        std::vector<Voxel> voxels;
    };

    using Palette = std::array<u32, 256>;

public:
    std::vector<VoxelCluster<glm::vec3>> read(const std::string & path);

private:
    Chunk readChunkHeader(std::ifstream & file);
    Voxel readVoxel(std::ifstream & file);
};

};