#pragma once
#include "GameTimer.h"
#include "Render_Window.h"
#include "Hardware/Render_Device.h"
#include "Hardware/Render_CommandObjects.h"
#include "Hardware/Render_Heap.h"
#include "Hardware/Render_Swapchain.h"
#include "Hardware/Render_Pso.h"
#include "Hardware/Render_RootSignature.h"
#include "Ressources/Render_Material.h"
#include "Ressources/Render_Mesh.h"
#include "Render_Item.h"
#include "Vertex.h"
#include "VertexUI.h"
#include "d3dUtil.h"
#include "MathHelper.h"
#include "Render_Camera.h"
#include <vector>
#include <map>
#include "UploadBuffer.h"

using namespace DirectX;

//struct Vertex
//{
//	XMFLOAT3 pos;
//	XMFLOAT4 color;
//};
struct ObjectConstants
{
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	float time;
};

struct UIConstants
{
	XMFLOAT4X4 OrthoProjMatrix = MathHelper::Identity4x4();;
	float Padding[48];
};

struct MaterialConstants
{
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };  
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };  
	float Roughness = 0.25f;

	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};

class Render_App
{
private :
	Render_Window m_renderWindow;
	GameTimer m_gameTimer;
	
	Render_Camera* m_pCamera = nullptr;

	HWND m_hWnd;
	int m_wndWidth;
	int m_wndHeight;

	//Device & hardware..
	Render_Device* m_pDevice = nullptr;
	Render_CommandObjects* m_pCommandObjects = nullptr;
	Render_Heap* m_pHeap = nullptr;
	Render_Swapchain* m_pSwapchain = nullptr;
	Render_RootSignature* m_pRootSignature = nullptr;
	Render_Pso* m_pPso = nullptr;
	Render_Pso* m_pPsoUI = nullptr;

	Render_Material* m_pColorMaterial = nullptr;
	Render_Shader* m_pUIShader = nullptr;

	ID3D12Resource* m_pFontTexture = nullptr;
	ID3D12Resource* m_pFontTextureUpload = nullptr;


	std::unique_ptr<UploadBuffer<ObjectConstants>>mObjectCB = nullptr;
	std::unique_ptr<UploadBuffer<MaterialConstants>>mMaterialCB = nullptr;
	std::unique_ptr<UploadBuffer<VertexUI>> m_pUIVertexBuffer = nullptr;

	
	D3D12_VERTEX_BUFFER_VIEW m_uiVbv;

	const int MaxUIVertices = 6000;

	int m_uiVertexOffset = 0;

	std::map<std::string, Render_Mesh*> m_meshBank;
	std::vector<Item> m_renderQueue;

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;


public : 

	Render_App();
	~Render_App();
	void Init();
	void Run();
	void Update();
	void Draw();
	void RegisterMesh(std::string name, Render_Mesh* mesh);
	void Submit(std::string meshName, DirectX::XMFLOAT4X4 worldMatrix, bool _isScreenSpace);
	void Build(std::vector<Vertex> _v, std::vector<std::uint16_t> _indices, std::string _submeshName);
	void OnResize(int width, int height);
	void CreateOrthoProjMatrix(int _windowWidth, int _windowHeight, UIConstants& _outBuffer);
	void LoadTexture();
	void DrawTextUI(std::string _text, float _x, float _y, float _size, XMFLOAT4 _color);

	Render_Window* GetWindow();
	Render_Camera* GetCamera();
};

