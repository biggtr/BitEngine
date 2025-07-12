#pragma once

#include "Bit/Math/Matrix.h"
#include "Bit/Renderer/RendererAPI.h"
namespace BitEngine
{

class OrthographicCamera
{

private:
    BitMath::Matrix4x4 m_ProjectionMatrix;
    BitMath::Matrix4x4 m_ViewMatrix;
    BitMath::Matrix4x4 m_ViewProjection;
    BitMath::Vector3 m_Position;
    BitMath::Vector3 m_Rotation;
public: 
    OrthographicCamera(float left, float right , float bottom, float top, float zNear, float zFar)
        : m_Position(1.0f, 1.0f, 1.0f), m_Rotation(1.0f, 1.0f, 1.0f)
    {
        m_ProjectionMatrix = BitMath::Matrix4x4::Ortho(left, right, bottom, top, zNear, zFar);
        m_ViewMatrix = {};
        m_ViewProjection = {};
        RecalculateViewProjectionMatrix();
    }

    BitMath::Matrix4x4 GetProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }

    BitMath::Matrix4x4 GetViewProjectionMatrix()
    {
        return m_ViewProjection;
    }

    BitMath::Vector3 GetPosition()
    {
        return m_Position;
    }
    void SetPosition(BitMath::Vector3 newPosition)
    {
        m_Position = newPosition;
        RecalculateViewProjectionMatrix();
    }

    BitMath::Vector3 GetRotation()
    {
        return m_Rotation;
    }
    void SetRotation(BitMath::Vector3 newRotation)
    {
        m_Rotation = newRotation;
        RecalculateViewProjectionMatrix();
    }

    void RecalculateViewProjectionMatrix()
    {
        m_ViewMatrix = BitMath::Matrix4x4::Rotate(-m_Rotation.x, -m_Rotation.y, -m_Rotation.z);
        m_ViewMatrix = m_ViewMatrix * BitMath::Matrix4x4::Translate(-m_Position.x, -m_Position.y, -m_Position.z);
        
        m_ViewProjection = m_ProjectionMatrix * m_ViewMatrix;
    }
    
};

}
