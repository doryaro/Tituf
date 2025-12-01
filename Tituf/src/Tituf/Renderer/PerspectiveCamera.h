#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Tituf {

    class PerspectiveCamera
    {
    public:
        PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void SetPosition(const glm::vec3& pos);
        void LookAt(const glm::vec3& target);

        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjection; }

    private:
        void RecalculateProjection();
        void RecalculateView();

    private:
        float m_FOV;
        float m_AspectRatio;
        float m_NearClip;
        float m_FarClip;

        glm::vec3 m_Position;
        glm::vec3 m_Target;

        glm::mat4 m_Projection{ 1.0f };
        glm::mat4 m_View{ 1.0f };
        glm::mat4 m_ViewProjection{ 1.0f };
    };

} // namespace Tituf
