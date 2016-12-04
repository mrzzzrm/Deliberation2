#include <bitset>
#include <iostream>
#include <set>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Voxel/VoxelCluster.h>
#include <Deliberation/Voxel/VoxelClusterMarchingCubes.h>

#include "SceneExampleApplication.h"

using namespace deliberation;

class VoxelExample:
    public SceneExampleApplication
{
public:
    VoxelExample():
        SceneExampleApplication("VoxelExample")
    {

    }

    virtual void onStartup() override
    {
        SceneExampleApplication::onStartup();

        auto cluster = VoxelCluster<glm::vec3>({1, 1, 1});
        cluster.set({0, 0, 0}, {1.0f, 0.8f, 0.2f});

        auto marchingCubes = VoxelClusterMarchingCubes(cluster);
        marchingCubes.run();


    }


    virtual void onFrame(float seconds) override
    {
        SceneExampleApplication::onFrame(seconds);
    }

private:
};

std::bitset<8> rotateX(const std::bitset<8> & config) {
    std::bitset<8> result;
    result.set(0, config[4]);
    result.set(1, config[5]);
    result.set(2, config[1]);
    result.set(3, config[0]);
    result.set(4, config[7]);
    result.set(5, config[6]);
    result.set(6, config[2]);
    result.set(7, config[3]);
    return result;
}

std::bitset<8> rotateY(const std::bitset<8> & config) {
    std::bitset<8> result;
    result.set(0, config[3]);
    result.set(1, config[0]);
    result.set(2, config[1]);
    result.set(3, config[2]);
    result.set(4, config[7]);
    result.set(5, config[4]);
    result.set(6, config[5]);
    result.set(7, config[6]);
    return result;
}

std::bitset<8> rotateZ(const std::bitset<8> & config) {
    std::bitset<8> result;
    result.set(0, config[4]);
    result.set(1, config[0]);
    result.set(2, config[3]);
    result.set(3, config[7]);
    result.set(4, config[5]);
    result.set(5, config[1]);
    result.set(6, config[2]);
    result.set(7, config[6]);
    return result;
}

struct ClassConfig {
    u8 equivalenceClass = 255;
    glm::u8vec3 rotation;
    bool inverse;
};

int main(int argc, char * argv[])
{
    std::array<ClassConfig, 256> classByConfig;
    std::set<u8> coveredConfigs;

    std::array<std::bitset<8>, 18> equivalenceClasses = {
        0b00000000, // 0
        0b00001000, // 1
        0b01001000, // 2
        0b00001001, // 3
        0b00101000, // 4
        0b00001110, // 5
        0b01001001, // 6
        0b01011000, // 7
        0b00011110, // 8
        0b01101001, // 9
        0b01011010, // 10
        0b00001111, // 11
        0b10001110, // 12
        0b01001101, // 13
        0b01001110, // 14
        0b11011110, // 15
        0b10011110, // 16
        0b01011110, // 17
    };

    std::array<std::vector<int>, 18> equivalenceClassMeshes = {
        std::vector<int>(), // 0
        std::vector<int>{{3,2,7}}, // 1
        std::vector<int>{{3,2,7,10,6,9}}, // 2
        std::vector<int>{{0,7,4,0,2,7}}, // 3
        std::vector<int>{{3,2,7,8,9,5}}, // 4
        std::vector<int>{{0,5,3,3,5,7,5,6,7}}, // 5
        std::vector<int>{{3,2,7,8,9,5,9,10,6}}, // 6
        std::vector<int>{{3,2,7,10,6,9,11,8,4}}, // 7
        std::vector<int>{{0,5,3,3,5,7,5,6,7,11,8,4}}, // 8
        std::vector<int>{{0,7,4,0,2,7,5,8,10,5,10,6}}, // 9
        std::vector<int>{{3,2,7,10,6,9,11,8,4,0,5,1}}, // 10
        std::vector<int>{{4,5,7,5,6,7}}, // 11
        std::vector<int>{{3,0,11,0,5,11,5,10,11,5,6,10}}, // 12
        std::vector<int>{{10,7,4,4,9,10,4,0,9,0,1,9}}, // 13
        std::vector<int>{{3,10,7,3,9,10,3,0,9,0,5,9}}, // 14
        std::vector<int>{{4,3,8,3,9,8,3,5,9,3,0,5}}, // 15
        std::vector<int>{{4,3,8,8,3,10,3,6,10,3,5,6,3,0,5}}, // 16
        std::vector<int>{{3,10,7,3,9,10,3,0,9,0,5,9,4,11,8}}, // 17
    };

    std::array<glm::vec3, 12> edgePositions = {{
                                                   {0.0f, -0.5f, 0.5f},
                                                   {0.5f, -0.5f, 0.0f},
                                                   {0.0f, -0.5f, -0.5f},
                                                   {-0.5f, -0.5f, 0.0f},
                                                   {-0.5f, 0.0f, 0.5f},
                                                   {0.5f, 0.0f, 0.5f},
                                                   {0.5f, 0.0f, -0.5f},
                                                   {-0.5f, 0.0f, -0.5f},
                                                   {0.0f, 0.5f, 0.5f},
                                                   {0.5f, 0.5f, 0.0f},
                                                   {0.0f, 0.5f, -0.5f},
                                                   {-0.5f, 0.5f, 0.0f},
                                               }};

    for (size_t e = 0; e < equivalenceClasses.size(); e++) {
        auto & equivalenceClass = equivalenceClasses[e];

        std::bitset<8> baseClassX = equivalenceClass;

        for (auto x = 0; x < 4; x++) {
            std::bitset<8> baseClassY = baseClassX;

            for (auto y = 0; y < 4; y++) {
                std::bitset<8> baseClassZ = baseClassY;

                for (auto z = 0; z < 4; z++) {

                    auto configID = baseClassZ.to_ullong();

                    std::cout << e << " " << configID << std::endl;


                    if (coveredConfigs.count(configID) == 0) {
                        coveredConfigs.insert(configID);

                        ClassConfig config;
                        config.equivalenceClass = e;
                        config.rotation = {x, y, z};
                        config.inverse = false;

                        classByConfig[configID] = config;
                    }

                    auto flipped = baseClassZ;
                    flipped.flip();

                    configID = flipped.to_ullong();

                    if (coveredConfigs.count(configID) == 0) {
                        coveredConfigs.insert(configID);

                        ClassConfig config;
                        config.equivalenceClass = e;
                        config.rotation = {x, y, z};
                        config.inverse = true;

                        classByConfig[configID] = config;
                    }

                    baseClassZ = rotateZ(baseClassZ);
                }

                baseClassY = rotateY(baseClassY);
            }

            baseClassX = rotateX(baseClassX);
        }

        std::cout << std::endl;
    }

    std::cout << "Num configs: " << coveredConfigs.size() << std::endl;

    for (size_t c = 0; c < classByConfig.size(); c++) {
        auto & _class = classByConfig[c];

//        std::cout << c << ": " << std::bitset<8>(c).to_string() << ": " << (int)_class.equivalenceClass << " {" <<
//                  (int)_class.rotation.x << ", " <<
//                  (int)_class.rotation.y << ", " <<
//                  (int)_class.rotation.z << "} " << _class.inverse << std::endl;

        auto & edgeIndices = equivalenceClassMeshes[_class.equivalenceClass];

        std::cout << c << "(" << (int)_class.equivalenceClass << ", " << edgeIndices.size() << ") {";

        for (auto & index : edgeIndices) {
            auto position = edgePositions[index];

//            for (auto i = 0; i < _class.rotation.x; i++)
//            {
//                position.z = position.y;
//                position.y = -position.z;
//            }
//            for (auto i = 0; i < _class.rotation.y; i++)
//            {
//                position.x = position.z;
//                position.z = -position.x;
//            }
//            for (auto i = 0; i < _class.rotation.z; i++)
//            {
//                position.x = -position.y;
//                position.y = position.x;
//            }

//            position = glm::angleAxis(glm::half_pi<float>() * _class.rotation.x, glm::vec3{1.0f, 0.0f, 0.0}) * position;
//            position = glm::angleAxis(glm::half_pi<float>() * _class.rotation.y, glm::vec3{0.0f, 1.0f, 0.0}) * position;
//            position = glm::angleAxis(glm::half_pi<float>() * _class.rotation.z, glm::vec3{0.0f, 0.0f, 1.0}) * position;

            std::cout << position  << ", ";
        }
        std::cout << "}" << std::endl;
    }



   // return VoxelExample().run(argc, argv);
}
