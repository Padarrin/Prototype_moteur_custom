#include "pch.h"

TransformSystem::TransformSystem()
{
}

TransformSystem::~TransformSystem()
{
    m_entities.clear();
    vTransform.clear();
    vVelocity.clear();
}

void TransformSystem::Init(ECS ecs)
{
}

void TransformSystem::Update(ECS ecs, float deltatime)
{
    m_entities = ecs.GetEntitiesWith<Transform>();

    for (Entity* entity : m_entities)
    {
        Transform* transform = ecs.GetComponent<Transform>(*entity);

        XMMATRIX matScale = XMMatrixScaling(transform->mSca.x, transform->mSca.y, transform->mSca.z);
        XMMATRIX matRot = XMMatrixRotationRollPitchYaw(transform->mRot.x, transform->mRot.y, transform->mRot.z);
        XMMATRIX matTrans = XMMatrixTranslation(transform->mPos.x, transform->mPos.y, transform->mPos.z);

        XMMATRIX worldMatrix = matScale * matRot * matTrans;

        XMStoreFloat4x4(&transform->mWorldMatrix, worldMatrix);
    }
}
