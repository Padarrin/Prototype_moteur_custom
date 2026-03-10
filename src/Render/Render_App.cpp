#include "Render_App.h"
#include <iostream>
#include "RessourceLoader.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

Render_App::Render_App()
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	{/*
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
		debugController->EnableDebugLayer();*/
	}
#endif
}

Render_App::~Render_App()
{
	m_pCommandObjects->FlushCommandQueue();

	for (auto& pair : m_meshBank)
	{
		delete pair.second;
	}
	m_meshBank.clear();
	mObjectCB.reset();
	mMaterialCB.reset();

	delete m_pCommandObjects;
	delete m_pSwapchain;
	delete m_pPso;
	delete m_pHeap;
	delete m_pRootSignature;
	delete m_pCamera;
	delete m_pColorMaterial;
	delete m_pDevice;
	m_pDevice = nullptr;
	m_pCommandObjects = nullptr;
	m_pSwapchain = nullptr;
	m_pPso = nullptr;
	m_pHeap = nullptr;
	m_pRootSignature = nullptr;
	m_pColorMaterial = nullptr;
	m_pCamera = nullptr;
}

void Render_App::Init()
{
	m_renderWindow.Init(GetModuleHandle(NULL));
	m_renderWindow.SetResizeCallback([this](int w, int h) {
		this->OnResize(w, h); });
	m_hWnd = m_renderWindow.GetHwnd();
	m_wndWidth = m_renderWindow.GetWidth();
	m_wndHeight = m_renderWindow.GetHeight();

	m_gameTimer.Reset();

	m_pDevice = new Render_Device;

	if (!m_pDevice->Create())
	{
		std::cout << "Device creation failed" << std::endl;
		return;
	}

	ID3D12Device* device = m_pDevice->GetDevice();
	if (!device)
	{
		OutputDebugStringA("Render_Device::GetDevice() a retourn� nullptr.\n");
		MessageBoxA(nullptr, "Device D3D12 null apr�s Create().", "Erreur", MB_OK | MB_ICONERROR);
		return;
	}

	m_pCommandObjects = new Render_CommandObjects;
	m_pCommandObjects->CreateCommandAllocator(device);
	m_pCommandObjects->CreateCommandQueue(device);
	m_pCommandObjects->CreateFence(device);
	ID3D12CommandQueue* commandQueue = m_pCommandObjects->GetCommandQueue();

	m_pSwapchain = new Render_Swapchain;
	m_pSwapchain->CreateFactory();
	m_pSwapchain->CreateSwapchain(m_hWnd, m_wndWidth, m_wndHeight, commandQueue);
	m_pSwapchain->CreateBuffer();

	static const int bufferCountSwapchain = m_pSwapchain->GetBufferCountSwapchain();
	IDXGISwapChain* swapchain = m_pSwapchain->GetSwapchain();

	m_pHeap = new Render_Heap;
	m_pHeap->CreateDescriptorHeaps(device, bufferCountSwapchain);

	m_pHeap->CreateRtv(device, swapchain, bufferCountSwapchain);
	m_pHeap->CreateDsv(device, m_wndWidth, m_wndHeight);
	m_pCommandObjects->CreateCommandList(device);
	LoadTexture();
	m_pHeap->CreateSrv(device, m_pFontTexture);
	m_pCommandObjects->CloseCommandList();

	m_pRootSignature = new Render_RootSignature;
	m_pRootSignature->CreateRootSignature(device);
	ID3D12RootSignature* rootSignature = m_pRootSignature->GetRootSignature();

	//Shader for 3D
	m_pColorMaterial = new Render_Material("color", L"..\\..\\Shaders\\ColorFix.hlsl");

	ID3DBlob* vsByteCode = m_pColorMaterial->m_pShader->GetShaderVsByteCode();
	ID3DBlob* psByteCode = m_pColorMaterial->m_pShader->GetShaderPsByteCode();

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = m_pColorMaterial->m_pShader->GetInputLayout();

	m_pPso = new Render_Pso;
	m_pPso->CreatePso(device, inputLayout, rootSignature,
		vsByteCode, psByteCode);

	//Shader for UI
	m_pUIShader = new Render_Shader;
	m_pUIShader->CreateShader(L"..\\..\\Shaders\\UI.hlsl");
	m_pUIShader->CreateInputLayoutUI();

	ID3DBlob* vsByteCodeUI = m_pUIShader->GetShaderVsByteCode();
	ID3DBlob* psByteCodeUI = m_pUIShader->GetShaderPsByteCode();

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutUI = m_pUIShader->GetInputLayoutUI();

	m_pPsoUI = new Render_Pso;
	m_pPsoUI->CreatePsoUI(device, inputLayoutUI, rootSignature,
		vsByteCodeUI, psByteCodeUI);

	ID3D12GraphicsCommandList* commandList = m_pCommandObjects->GetCommandList();
	ID3D12CommandList* cmdsLists[] = { commandList };
	m_pCommandObjects->GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	m_pCommandObjects->FlushCommandQueue();

	m_pCamera = new Render_Camera;
	m_pCamera->SetPosition(0.0f, 0.0f,  0.0f);
	m_pCamera->SetRotation(0.0f, 0.0f, 0.0f);
	m_pCamera->SetProjection(45.0f, (float)m_wndWidth / m_wndHeight, 0.1f, 100.0f);

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(m_wndWidth);
	mScreenViewport.Height = static_cast<float>(m_wndHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, m_wndWidth, m_wndHeight };

	mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, 10000, true); 
	mMaterialCB = std::make_unique<UploadBuffer<MaterialConstants>>(device, 10000, true);
	m_pUIVertexBuffer = std::make_unique<UploadBuffer<VertexUI>>(device, MaxUIVertices, false);
	m_uiVbv.BufferLocation = m_pUIVertexBuffer->Resource()->GetGPUVirtualAddress();
	m_uiVbv.StrideInBytes = sizeof(VertexUI);
	m_uiVbv.SizeInBytes = MaxUIVertices * sizeof(VertexUI);
}

void Render_App::Run()
{
	m_renderWindow.Update();
}

void Render_App::Update()
{
	m_gameTimer.Tick();
	m_pCamera->Render();
	for (int i = 0; i < m_renderQueue.size(); ++i)
	{
		const auto& item = m_renderQueue[i];
		if (m_meshBank.find(item.meshName) == m_meshBank.end())
			continue;
		Render_Mesh* mesh = m_meshBank[item.meshName];


		XMMATRIX world = XMLoadFloat4x4(&item.worldMatrix);
		XMMATRIX view = m_pCamera->GetViewMatrix();
		XMMATRIX proj = m_pCamera->GetProjectionMatrix();
		XMMATRIX wvp;

		if (item.isScreenSpace == true)
		{
		
			XMMATRIX world = XMLoadFloat4x4(&item.worldMatrix);
			wvp = world * proj;
		}
		else
		{
			wvp = world * view * proj;
		}

		ObjectConstants objConstants;
		objConstants.time = m_gameTimer.TotalTime();
		XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(wvp));
		mObjectCB->CopyData(i, objConstants);

		MaterialConstants matConstants;
		matConstants.DiffuseAlbedo = { 1.f, 1.f, 1.f, 1.0f };
		matConstants.FresnelR0 = { 0.01f, 0.01f, 0.01f };
		matConstants.Roughness = 0.25f;
		mMaterialCB->CopyData(i, matConstants);
	}


}

void Render_App::Draw()
{
	ID3D12Resource* backBuffer = m_pSwapchain->GetCurrentBackBuffer();
	if (!backBuffer)
	{
		return;
	}
	m_pCommandObjects->ResetCommandAllocator();
	m_pCommandObjects->ResetCommandList(m_pPso->GetPso());

	ID3D12GraphicsCommandList* commandList = m_pCommandObjects->GetCommandList();

	commandList->RSSetViewports(1, &mScreenViewport);
	commandList->RSSetScissorRects(1, &mScissorRect);

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffer,
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	commandList->ResourceBarrier(1, &barrier);
	commandList->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_pHeap->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
		m_pSwapchain->GetCurrentBackBufferIndex(),
		m_pHeap->GetRtvDescriptorSize()), Colors::LightBlue, 0, nullptr);

	commandList->ClearDepthStencilView(m_pHeap->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);


	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		m_pHeap->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
		m_pSwapchain->GetCurrentBackBufferIndex(),
		m_pHeap->GetRtvDescriptorSize());

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_pHeap->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(
		1,
		&rtvHandle,
		true,
		&dsvHandle
	);

	commandList->SetGraphicsRootSignature(m_pRootSignature->GetRootSignature());

	for (int i = 0; i < m_renderQueue.size(); ++i)
	{
		const auto& item = m_renderQueue[i];
		if (m_meshBank.find(item.meshName) == m_meshBank.end())
			continue;
		Render_Mesh* mesh = m_meshBank[item.meshName];

		auto cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
		cbAddress += i * d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
		auto matCbAddress = mMaterialCB->Resource()->GetGPUVirtualAddress();
		matCbAddress += i * d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

		commandList->SetGraphicsRootConstantBufferView(0, cbAddress);
		commandList->SetGraphicsRootConstantBufferView(1, matCbAddress);

		D3D12_VERTEX_BUFFER_VIEW vbv = mesh->GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW ibv = mesh->GetIndexBufferView();

		commandList->IASetVertexBuffers(0, 1, &vbv);
		commandList->IASetIndexBuffer(&ibv);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		commandList->DrawIndexedInstanced(mesh->GetIndexCount(), 1, 0, 0, 0);
	}

	UIConstants uiConsts;
	CreateOrthoProjMatrix(m_wndWidth, m_wndHeight, uiConsts);
	mObjectCB->CopyData(999, *(ObjectConstants*)&uiConsts);


	auto uiCbAddress = mObjectCB->Resource()->GetGPUVirtualAddress() + 999 * d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	commandList->SetPipelineState(m_pPsoUI->GetPso());
	commandList->SetGraphicsRootConstantBufferView(0, uiCbAddress);

	ID3D12DescriptorHeap* heaps[] = { m_pHeap->GetCbvSrvUavHeap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	commandList->SetGraphicsRootDescriptorTable(2, m_pHeap->GetCbvSrvUavHeap()->GetGPUDescriptorHandleForHeapStart());

	commandList->IASetVertexBuffers(0, 1, &m_uiVbv);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_uiVertexOffset > 0)
	{
		commandList->DrawInstanced(m_uiVertexOffset, 1, 0, 0);
		m_uiVertexOffset = 0;
	}

	auto _barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pSwapchain->GetCurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	commandList->ResourceBarrier(1, &_barrier);

	commandList->Close();

	ID3D12CommandList* cmdsLists[] = { commandList };
	m_pCommandObjects->GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	IDXGISwapChain* swapChain = m_pSwapchain->GetSwapchain();
	swapChain->Present(0, 0);
	int currentBackBufferIndex = m_pSwapchain->GetCurrentBackBufferIndex();
	currentBackBufferIndex = (currentBackBufferIndex + 1) % m_pSwapchain->GetBufferCountSwapchain();
	m_pSwapchain->SetCurrentBackBufferIndex(currentBackBufferIndex);

	m_pCommandObjects->FlushCommandQueue();

	m_renderQueue.clear();
}

void Render_App::RegisterMesh(std::string name, Render_Mesh* mesh)
{
	m_meshBank[name] = mesh;
}

void Render_App::Submit(std::string meshName, DirectX::XMFLOAT4X4 worldMatrix, bool _isScreenSpace)
{
	m_renderQueue.push_back({ meshName, worldMatrix, _isScreenSpace });
}

void Render_App::Build(std::vector<Vertex> _v, std::vector<std::uint16_t> _indices, std::string _submeshName)
{
	m_pCommandObjects->ResetCommandList(m_pPso->GetPso());
	Render_Mesh* mesh = new Render_Mesh;
	mesh->Build(m_pDevice->GetDevice(), m_pCommandObjects->GetCommandList(), _v, _indices, _submeshName);
	m_pCommandObjects->CloseCommandList();
	ID3D12CommandList* cmdsLists[] = { m_pCommandObjects->GetCommandList() };
	m_pCommandObjects->GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	m_pCommandObjects->FlushCommandQueue();
	RegisterMesh(_submeshName, mesh);
}

void Render_App::OnResize(int width, int height)
{
	if (width == 0 || height == 0) return;

	if (m_pSwapchain && m_pDevice && m_pCommandObjects)
	{
		m_pCommandObjects->FlushCommandQueue();


		m_pCommandObjects->ResetCommandAllocator();


		m_pCommandObjects->ResetCommandList(nullptr);
		m_pCommandObjects->CloseCommandList();


		m_pSwapchain->Resize(width, height);

		m_pHeap->CreateRtv(
			m_pDevice->GetDevice(),
			m_pSwapchain->GetSwapchain(),
			m_pSwapchain->GetBufferCountSwapchain()
		);

		m_pHeap->CreateDsv(m_pDevice->GetDevice(), width, height);
	}

	if (m_pCamera)
		m_pCamera->SetProjection(45.0f, (float)width / height, 0.1f, 100.0f);

	m_wndWidth = width;
	m_wndHeight = height;
	mScreenViewport.Width = (float)width;
	mScreenViewport.Height = (float)height;
	mScissorRect = { 0, 0, width, height };

}

void Render_App::CreateOrthoProjMatrix(int _windowWidth, int _windowHeight, UIConstants& _outBuffer)
{
	XMMATRIX orthoMatrix = XMMatrixOrthographicOffCenterLH(
		0.0f,
		(float)_windowWidth,
		(float)_windowHeight,
		0.0f,
		0.0f,
		1.0f
	);

	XMStoreFloat4x4(&_outBuffer.OrthoProjMatrix, XMMatrixTranspose(orthoMatrix));
}

void Render_App::LoadTexture()
{
	int texWidth = 0;
	int texHeight = 0;

	std::string path = "../../Textures/font.bmp";
	unsigned char* pixels = RessourceLoader::LoadBMP32(path, texWidth, texHeight);

	if (!pixels) {
		std::cout << "Erreur: Pixels non charges." << std::endl;
		return;
	}

	ID3D12Device* device = m_pDevice->GetDevice();

	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = texWidth;
	textureDesc.Height = texHeight;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	auto heapPropsDefault = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	device->CreateCommittedResource(
		&heapPropsDefault,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_pFontTexture));

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_pFontTexture, 0, 1);
	auto heapPropsUpload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

	device->CreateCommittedResource(
		&heapPropsUpload,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pFontTextureUpload));

	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = pixels;
	textureData.RowPitch = texWidth * 4;
	textureData.SlicePitch = textureData.RowPitch * texHeight;

	ID3D12GraphicsCommandList* cmdList = m_pCommandObjects->GetCommandList();

	UpdateSubresources(cmdList, m_pFontTexture, m_pFontTextureUpload, 0, 0, 1, &textureData);

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pFontTexture,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	cmdList->ResourceBarrier(1, &barrier);
	delete[] pixels;
}

void Render_App::DrawTextUI(std::string _text, float _x, float _y, float _size, XMFLOAT4 _color)
{
	const int numColumns = 16;
	const int numRows = 16;
	const float uvWidth = 1.0f / numColumns;
	const float uvHeight = 1.0f / numRows;

	float currentX = _x;

	for (char c : _text)
	{ 

		int asciiCode = (int)c;
		int col = asciiCode % numColumns;
		int row = 15 - (asciiCode / 16);

		float u = col * uvWidth;
		float v = row * uvHeight;

		float left = currentX;
		float right = currentX + _size;
		float top = _y;
		float bottom = _y + _size;

		float u0 = col * uvWidth;
		float v0 = row * uvHeight;
		float u1 = u0 + uvWidth;
		float v1 = v0 + uvHeight;

		VertexUI v1_vert;
		v1_vert.pos = { left, top };
		v1_vert.uv = { u0, v1 };
		v1_vert.color = _color;

		VertexUI v2_vert;
		v2_vert.pos = { right, top };
		v2_vert.uv = { u1, v1 };         
		v2_vert.color = _color;

		VertexUI v3_vert;
		v3_vert.pos = { left, bottom };
		v3_vert.uv = { u0, v0 };         
		v3_vert.color = _color;

		VertexUI v4_vert;
		v4_vert = v3_vert;
		VertexUI v5_vert;
		v5_vert = v2_vert;

		VertexUI v6_vert;
		v6_vert.pos = { right, bottom };
		v6_vert.uv = { u1, v0 };
		v6_vert.color = _color;

		if (m_uiVertexOffset + 6 <= MaxUIVertices)
		{
			m_pUIVertexBuffer->CopyData(m_uiVertexOffset + 0, v1_vert);
			m_pUIVertexBuffer->CopyData(m_uiVertexOffset + 1, v2_vert);
			m_pUIVertexBuffer->CopyData(m_uiVertexOffset + 2, v3_vert);
			m_pUIVertexBuffer->CopyData(m_uiVertexOffset + 3, v4_vert);
			m_pUIVertexBuffer->CopyData(m_uiVertexOffset + 4, v5_vert);
			m_pUIVertexBuffer->CopyData(m_uiVertexOffset + 5, v6_vert);
			m_uiVertexOffset += 6;
		}

		currentX += _size * 0.6f;
	}
}

Render_Window* Render_App::GetWindow()
{
	return &m_renderWindow;
}

Render_Camera* Render_App::GetCamera()
{
	return m_pCamera;
}