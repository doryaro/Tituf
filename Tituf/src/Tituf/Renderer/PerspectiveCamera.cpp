#include "tfpch.h"
#include "PerspectiveCamera.h"

namespace Tituf {

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
    {
        m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
        m_Target = glm::vec3(0.0f, 0.0f, 0.0f);
        RecalculateProjection();
        RecalculateView();
    }

    void PerspectiveCamera::SetPosition(const glm::vec3& pos)
    {
        m_Position = pos;
        RecalculateView();
    }

    void PerspectiveCamera::LookAt(const glm::vec3& target)
    {
        m_Target = target;
        RecalculateView();
    }

    void PerspectiveCamera::RecalculateProjection()
    {
        m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
        m_ViewProjection = m_Projection * m_View;
    }

    void PerspectiveCamera::RecalculateView()
    {
        m_View = glm::lookAt(m_Position, m_Target, glm::vec3(0.0f, 1.0f, 0.0f));
        m_ViewProjection = m_Projection * m_View;
    }

} // namespace Tituf
