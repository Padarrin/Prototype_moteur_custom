#include <iostream>
#include "main.h"
#include "../Game/pch.h"

#include "../Engine/Mesh.h"


int main()
{
    ECS ecs;

    RenderSystem sys;
    sys.Init(ecs);

    TransformSystem system;
    system.Init(ecs);

    Mesh* triangle = new Mesh();
    XMFLOAT3 a = { 0.0f, 0.5f, 0.0f };
    XMFLOAT3 b = { 0.5f, -0.5f, 0.0f };
    XMFLOAT3 c = { -0.5f, -0.5f, 0.0f };
    triangle->CreateTriangle(a, b, c, WHITE);

    Entity* player = ecs.createEntity();
    ecs.addComponent<MeshRenderer>(player);
    ecs.addComponent<Transform>(player);
    Entity* test = ecs.createEntity();

    Transform* pos = ecs.GetComponent<Transform>(*player);

    MeshRenderer* meshRenderer = ecs.GetComponent<MeshRenderer>(*player);
    meshRenderer->SetMesh(triangle);
    meshRenderer->SetMeshName("triangle");
    meshRenderer->SetMaterial("default");
    meshRenderer->SetVisible(true);

    while (true)
    {
        sys.Update(ecs, 0.0f);
        system.Update(ecs, 1.0f);
        pos = ecs.GetComponent<Transform>(*player);
        std::cout << pos->mPos.x << " , " << pos->mPos.y << " , " << pos->mPos.z << std::endl;
    }

    return 0;
}