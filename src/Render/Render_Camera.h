#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Render_Camera
{
private:
    XMFLOAT3 m_position;
    XMFLOAT3 m_rotation;

    XMMATRIX m_viewMatrix;
    XMMATRIX m_projectionMatrix;

  

public:
    Render_Camera();
    ~Render_Camera();
    void SetPosition(float x, float y, float z);
    void SetRotation(float pitch, float yaw, float roll); 
    void SetProjection(float fovDegrees, float aspectRatio, float nearZ, float farZ);

    void Render();

    XMFLOAT3 GetPosition();
    XMFLOAT3 GetRotation();
   

    XMMATRIX GetViewMatrix();
    XMMATRIX GetProjectionMatrix();
    XMFLOAT3 GetLookVector();
    XMFLOAT3 GetUpVector();
    XMFLOAT3 GetRightVector();
};
