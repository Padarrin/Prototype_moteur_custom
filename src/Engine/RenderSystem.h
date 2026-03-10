#pragma once
#include "pch.h"
#include "../Render/Render_App.h"

class RenderSystem : public System
{
private :
	Render_App* m_pRenderer = nullptr;

	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();


	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

public :
	RenderSystem();
	~RenderSystem();
	void Init(ECS ecs) override;
	void Update(ECS ecs, float deltatime) override;
	void Build(std::vector<Vertex> _v, std::vector<std::uint16_t> _indices, std::string _submeshName);
	void RegisterMesh(std::string _name, Render_Mesh* _mesh);
	void SetRenderer(Render_App* _renderer);
	Render_Window* GetWindow();
	Render_Camera* GetCamera();

};

