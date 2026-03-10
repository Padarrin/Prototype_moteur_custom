#include "Render_Camera.h"

Render_Camera::Render_Camera()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_viewMatrix = XMMatrixIdentity();
	m_projectionMatrix = XMMatrixIdentity();
}

Render_Camera::~Render_Camera()
{

}

void Render_Camera::SetPosition(float _x, float _y, float _z)
{
	m_position = XMFLOAT3(_x,_y,_z);
}

void Render_Camera::SetRotation(float _pitch, float _yaw, float _roll)
{
	m_rotation = XMFLOAT3(_pitch, _yaw, _roll);
}

XMFLOAT3 Render_Camera::GetPosition()
{
	return m_position;
}

XMFLOAT3 Render_Camera::GetRotation()
{
	return m_rotation;
}

void Render_Camera::SetProjection(float _fovDegrees, float _aspectRatio, float _nearZ, float _farZ)
{
	float fovRadians = (_fovDegrees * 3.141592654f) / 180.0f;
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, _aspectRatio, _nearZ, _farZ);
}

void Render_Camera::Render()
{
    float pitch = m_rotation.x * 0.0174532925f;
    float yaw = m_rotation.y * 0.0174532925f;
    float roll = m_rotation.z * 0.0174532925f;

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
    XMFLOAT3 lookAtFloat(0.0f, 0.0f, 1.0f);
    XMVECTOR lookAt = XMLoadFloat3(&lookAtFloat);

    XMFLOAT3 upFloat(0.0f, 1.0f, 0.0f);
    XMVECTOR up = XMLoadFloat3(&upFloat); 

    lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
    up = XMVector3TransformCoord(up, rotationMatrix);

   
    XMVECTOR position = XMLoadFloat3(&m_position);
    lookAt = XMVectorAdd(position, lookAt);

    m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

XMMATRIX Render_Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

XMMATRIX Render_Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

XMFLOAT3 Render_Camera::GetLookVector() {
    XMMATRIX view = GetViewMatrix();
    XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, view);
    XMFLOAT3 look;
    look.x = XMVectorGetX(invView.r[2]);
    look.y = XMVectorGetY(invView.r[2]);
    look.z = XMVectorGetZ(invView.r[2]);

    return look;
}
XMFLOAT3 Render_Camera::GetUpVector() {
    XMMATRIX view = GetViewMatrix();
    XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, view);
    XMFLOAT3 up;
    XMStoreFloat3(&up, invView.r[1]);

    return up;
}
XMFLOAT3 Render_Camera::GetRightVector() {
    XMMATRIX view = GetViewMatrix();
    XMMATRIX invView = XMMatrixInverse(nullptr, view);
    XMFLOAT3 right;
    XMStoreFloat3(&right,invView.r[0]);

    return right;
}


