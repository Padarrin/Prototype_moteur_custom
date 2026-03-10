#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
	m_pRenderer = nullptr;
}

void RenderSystem::Init(ECS ecs)
{
}

void RenderSystem::Update(ECS ecs, float deltatime)
{
	if (m_pRenderer == nullptr)
		return; 

	auto entities = ecs.GetEntitiesWith<Transform, MeshRenderer>();
    for (auto entity : entities)
    {
        Transform* transform = ecs.GetComponent<Transform>(*entity);
        MeshRenderer* renderer = ecs.GetComponent<MeshRenderer>(*entity);

		if (transform && renderer && renderer->IsVisible() && transform->isScreenSpace == true)
		{
			m_pRenderer->Submit(renderer->GetMeshName(), transform->mWorldMatrix, true );
		}
		else if (transform && renderer && renderer->IsVisible())
        {
            m_pRenderer->Submit(renderer->GetMeshName(), transform->mWorldMatrix, false );
        }
    }
	
}
void RenderSystem::Build(std::vector<Vertex> _v, std::vector<std::uint16_t> _indices ,std::string _submeshName)
{
	m_pRenderer->Build(_v, _indices, _submeshName);
}

void RenderSystem::SetRenderer(Render_App* _renderer)
{
	m_pRenderer = _renderer;
}

Render_Window* RenderSystem::GetWindow()
{
	return m_pRenderer->GetWindow();
}

Render_Camera* RenderSystem::GetCamera()
{
	return m_pRenderer->GetCamera();
}