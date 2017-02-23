#include <Deliberation/Scene/Camera3D.h>

#include <sstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{

Camera3D::Camera3D():
    m_zNear(0.1f),
    m_zFar(9999.0f),
    m_yFoV(glm::radians(70.0f)),
    m_aspectRatio(1.0f),
    m_viewDirty(true),
    m_projectionDirty(true)
{
}

const glm::vec3 & Camera3D::position() const
{
    return m_pose.position();
}

const glm::quat & Camera3D::orientation() const
{
    return m_pose.orientation();
}

float Camera3D::zNear() const
{
    return m_zNear;
}

float Camera3D::zFar() const
{
    return m_zFar;
}

float Camera3D::yFoV() const
{
    return m_yFoV;
}

float Camera3D::aspectRatio() const
{
    return m_aspectRatio;
}

const Pose3D & Camera3D::pose() const
{
    return m_pose;
}

void Camera3D::setPosition(const glm::vec3 & position)
{
    m_pose.setPosition(position);
    m_viewDirty = true;
}

void Camera3D::setOrientation(const glm::quat & orientation)
{
    m_pose.setOrientation(orientation);
    m_viewDirty = true;
}

void Camera3D::setZNear(float zNear)
{
    Assert(zNear > 0.0f, "");

    m_zNear = zNear;
    m_projectionDirty = true;
}

void Camera3D::setZFar(float zFar)
{
    Assert(zFar > 0.0f, "");

    m_zFar = zFar;
    m_projectionDirty = true;
}

void Camera3D::setYFoV(float yFoV)
{
    Assert(yFoV > 0.0f, "");

    m_yFoV = yFoV;
    m_projectionDirty = true;
}

void Camera3D::setAspectRatio(float aspectRatio)
{
    Assert(aspectRatio > 0.0f, "");

    m_aspectRatio = aspectRatio;
    m_projectionDirty = true;
}

const glm::mat4 & Camera3D::view() const
{
    if (m_viewDirty)
    {
        m_view = glm::lookAt(m_pose.position(),
                             m_pose.position() + m_pose.orientation() * glm::vec3(0.0f, 0.0f, -1.0f),
                             m_pose.orientation() * glm::vec3(0.0f, 1.0f, 0.0f));
        m_viewDirty = false;
    }

    return m_view;
}

const glm::mat4 & Camera3D::projection() const
{
    if (m_projectionDirty)
    {
	    m_projection = glm::perspective(m_yFoV, m_aspectRatio, m_zNear, m_zFar);
	    m_projectionDirty = true;
    }

    return m_projection;
}

const glm::mat4 & Camera3D::viewProjection() const
{
    if (m_viewDirty || m_projectionDirty)
    {
        m_viewProjection = projection() * view();
    }

    return m_viewProjection;
}

Rect3D Camera3D::nearPlane() const
{
    /*
        TODO
            Remove code duplication with farPlane()
    */

    auto halfYFoV = m_yFoV / 2.0f;
    auto height = 2.0f * (std::tan(halfYFoV) * m_zNear);
    auto width = m_aspectRatio * height;

    auto origin = glm::vec3(-width/2.0f, -height/2.0f, -m_zNear);
    origin = m_pose.position() + m_pose.orientation() * origin;

    auto right = m_pose.orientation() * glm::vec3(width, 0.0f, 0.0f);
    auto up = m_pose.orientation() * glm::vec3(0.0f, height, 0.0f);

    return {origin, right, up};
}

Rect3D Camera3D::farPlane() const
{
    /*
        TODO
            Remove code duplication with nearPlane()
    */

    auto halfYFoV = m_yFoV / 2.0f;
    auto height = 2.0f * (std::tan(halfYFoV) * m_zFar);
    auto width = m_aspectRatio * height;

    auto origin = glm::vec3(-width/2.0f, -height/2.0f, -m_zFar);
    origin = m_pose.position() + m_pose.orientation() * origin;

    auto right = m_pose.orientation() * glm::vec3(width, 0.0f, 0.0f);
    auto up = m_pose.orientation() * glm::vec3(0.0f, height, 0.0f);

    return {origin, right, up};
}

std::string Camera3D::toString() const
{
    std::stringstream stream;

    stream << "{Position: " << m_pose.position() << "; ";
    stream << "Orientation: " << m_pose.orientation() << "; ";
    stream << "zNear: " << m_zNear << "; ";
    stream << "zFar: " << m_zFar << "; ";
    stream << "yFoV: " << m_yFoV << "; ";
    stream << "Aspect Ratio: " << m_aspectRatio << "}";

    return stream.str();

}

}

