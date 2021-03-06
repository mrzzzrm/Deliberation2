#include <Deliberation/Resource/ResourceManager.h>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Platform/App.h>

#include <Deliberation/Scene/MeshData.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>
#include <Deliberation/Scene/UVSphere.h>

namespace deliberation {

DELIBERATION_DEFINE_GLOBAL(ResourceManager);

ResourceManager::ResourceManager()
{

}

void ResourceManager::registerBuildIns() {
    /**
     * Init base particle got nowhere else to put this right now
     */
    {
        const auto layout =
            DataLayout{{{"Position", Type_Vec3}, {"UV", Type_Vec2}}};

        LayoutedBlob vertices(layout, 4);
        auto positions = vertices.field<glm::vec3>("Position");
        auto uvs = vertices.field<glm::vec2>("UV");

        positions.assign({glm::vec3(-0.5f, -0.5f, 0.0f),
                          glm::vec3(0.5f, -0.5f, 0.0f),
                          glm::vec3(0.5f, 0.5f, 0.0f),
                          glm::vec3(-0.5f, 0.5f, 0.0f)});

        uvs.assign({glm::vec2(0.0f, 0.0f),
                    glm::vec2(1.0f, 0.0f),
                    glm::vec2(1.0f, 1.0f),
                    glm::vec2(0.0f, 1.0f)});

        LayoutedBlob indicesBlob(DataLayout("Index", Type_U32), 6);
        auto indices = indicesBlob.field<u32>("Index");

        indices.assign({0, 1, 3, 1, 2, 3});

        const auto textureBinary =
            TextureLoader(GameDataPath("Data/Particles/BaseParticle.png"))
                .load();
        const auto texture = GetGlobal<DrawContext>()->createTexture(textureBinary);

        auto meshData = std::make_shared<MeshData>(std::move(vertices), std::move(indicesBlob),
                                                   std::vector<Texture>{texture});

        addResource<std::shared_ptr<MeshData>>("buildin:BaseParticle", std::move(meshData));
    }

    /**
     * Init Square Buildin
     */
    {
        const auto layout =
            DataLayout{"Position", Type_Vec3};

        LayoutedBlob vertices(layout, 4);
        auto positions = vertices.field<glm::vec3>("Position");

        positions.assign({glm::vec3(-0.5f, -0.5f, 0.0f),
                          glm::vec3(0.5f, -0.5f, 0.0f),
                          glm::vec3(0.5f, 0.5f, 0.0f),
                          glm::vec3(-0.5f, 0.5f, 0.0f)});

        LayoutedBlob indicesBlob(DataLayout("Index", Type_U32), 6);
        auto indices = indicesBlob.field<u32>("Index");

        indices.assign({0, 1, 3, 1, 2, 3});

        auto meshData = std::make_shared<MeshData>(std::move(vertices), std::move(indicesBlob));

        addResource<std::shared_ptr<MeshData>>("buildin:OpaqueSquare", std::move(meshData));
    }

    /**
     * Init bullet particle
     */
    {
        auto meshData = std::make_shared<MeshData>(UVSphere(5, 5).generateMesh2());
        addResource<std::shared_ptr<MeshData>>("buildin:Bullet", std::move(meshData));
    }

    /**
     * Init crosshair
     */
    {
        const auto layout =
            DataLayout{{{"Position", Type_Vec2}, {"UV", Type_Vec2}}};

        LayoutedBlob vertices(layout, 4);
        auto positions = vertices.field<glm::vec2>("Position");
        auto uvs = vertices.field<glm::vec2>("UV");

        positions.assign({glm::vec2(-0.5f, -0.5f),
                          glm::vec2(0.5f, -0.5f),
                          glm::vec2(0.5f, 0.5f),
                          glm::vec2(-0.5f, 0.5f)});

        uvs.assign({glm::vec2(0.0f, 0.0f),
                    glm::vec2(1.0f, 0.0f),
                    glm::vec2(1.0f, 1.0f),
                    glm::vec2(0.0f, 1.0f)});

        LayoutedBlob indicesBlob(DataLayout("Index", Type_U32), 6);
        auto indices = indicesBlob.field<u32>("Index");

        indices.assign({0, 1, 3, 1, 2, 3});

        const auto textureBinary =
            TextureLoader(GameDataPath("Data/Ui/Crosshairs.png")).load();
        const auto texture = GetGlobal<DrawContext>()->createTexture(textureBinary);

        MeshData mesh(std::move(vertices), std::move(indicesBlob), {texture});
        m_meshByResourceId.emplace(
            (size_t) R::UiCrosshairMesh, std::make_shared<MeshData>(mesh));
    }

    /**
     * Init HudEntityMarkerUpperLeft
     */
    {
        const auto layout =
            DataLayout{{{"Position", Type_Vec2}, {"UV", Type_Vec2}}};

        LayoutedBlob vertices(layout, 4);
        auto positions = vertices.field<glm::vec2>("Position");
        auto uvs = vertices.field<glm::vec2>("UV");

        positions.assign({glm::vec2(-0.5f, -0.5f),
                          glm::vec2(0.5f, -0.5f),
                          glm::vec2(0.5f, 0.5f),
                          glm::vec2(-0.5f, 0.5f)});

        uvs.assign({glm::vec2(0.0f, 0.0f),
                    glm::vec2(1.0f, 0.0f),
                    glm::vec2(1.0f, 1.0f),
                    glm::vec2(0.0f, 1.0f)});

        LayoutedBlob indicesBlob(DataLayout("Index", Type_U32), 6);
        auto indices = indicesBlob.field<u32>("Index");

        indices.assign({0, 1, 3, 1, 2, 3});

        const auto textureBinary =
            TextureLoader(GameDataPath("Data/Ui/HudEntityMarkerUpperLeft.png"))
                .load();
        const auto texture = GetGlobal<DrawContext>()->createTexture(textureBinary);

        MeshData mesh(std::move(vertices), std::move(indicesBlob), {texture});
        m_meshByResourceId.emplace(
            (size_t) R::HudEntityMarkerUpperLeft,
            std::make_shared<MeshData>(mesh));
    }

    {
        auto program = GetGlobal<DrawContext>()->createProgram(
            {GameDataPath("Data/Shaders/HudElement.vert"),
             GameDataPath("Data/Shaders/HudElement.frag")});
        m_programByResourceId.emplace((size_t) R::HudElement, program);
    }
}

const MeshData &ResourceManager::mesh(LegacyResourceId resourceId) const {
    const auto iter = m_meshByResourceId.find((size_t) resourceId);
    AssertM(
        iter != m_meshByResourceId.end(),
        "Couldn't find resource " + std::to_string((size_t) resourceId));

    return *iter->second;
}

const Program &ResourceManager::program(LegacyResourceId resourceId) const {
    const auto iter = m_programByResourceId.find((size_t) resourceId);
    AssertM(
        iter != m_programByResourceId.end(),
        "Couldn't find resource " + std::to_string((size_t) resourceId));

    return iter->second;
}

}