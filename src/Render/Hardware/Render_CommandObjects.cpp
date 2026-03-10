#include "Render_CommandObjects.h"

#include <iostream>

Render_CommandObjects::Render_CommandObjects()
{
}

Render_CommandObjects::~Render_CommandObjects()
{
	
	m_pCommandAllocator->Release();
	m_pCommandQueue->Release();
	m_pCommandList->Release();
	m_pFence->Release();
	m_pCommandAllocator = nullptr;
	m_pCommandQueue = nullptr;
	m_pFence = nullptr;
	m_pCommandList = nullptr;

}

void Render_CommandObjects::CreateCommandQueue(ID3D12Device* _device)
{

	if (m_pCommandQueue)
	{
		std::cout << "Command Queue already created" << std::endl;
		return;
	}

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));

	std::cout << "Command Queue created" << std::endl;
}

void Render_CommandObjects::CreateCommandAllocator(ID3D12Device* _device)
{
	if (m_pCommandAllocator)
	{
		std::cout << "Command Allocator already created" << std::endl;
		return;
	}
	_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));

	std::cout << "Command Allocator Created" << std::endl;
}

void Render_CommandObjects::CreateFence(ID3D12Device* _device)
{
	if (m_pFence)
	{
		std::cout << "Fence already created" << std::endl;
		return;
	}

	_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));

	std::cout << "Fence created" << std::endl;
}

void Render_CommandObjects::CreateCommandList(ID3D12Device* _device)
{
	if (m_pCommandAllocator == nullptr)
	{
		std::cout << "Command Allocator -> Null" << std::endl;
		return;
	}

	if (m_pCommandList)
	{
		std::cout << "Command List already created" << std::endl;
		return;
	}

	_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT
		, m_pCommandAllocator, nullptr, IID_PPV_ARGS(&m_pCommandList));

	std::cout << "Command List created" << std::endl;
}

ID3D12CommandQueue* Render_CommandObjects::GetCommandQueue() const
{
	return m_pCommandQueue;
}

void Render_CommandObjects::ResetCommandAllocator()
{
	if (m_pCommandAllocator == nullptr)
	{
		std::cout << "Command Allocator -> Null" << std::endl;
		return;
	}

 	m_pCommandAllocator->Reset();
}

void Render_CommandObjects::ResetCommandList(ID3D12PipelineState* _pso)
{
	if (m_pCommandList == nullptr)
	{
		std::cout << "Command List -> Null" << std::endl;
		return;
	}

	if (m_pCommandAllocator == nullptr)
	{
		std::cout << "Command Allocator -> Null" << std::endl;
		return;
	}

	m_pCommandList->Reset(m_pCommandAllocator, _pso);
}

void Render_CommandObjects::CloseCommandList()
{
	if (m_pCommandList == nullptr)
	{
		std::cout << "Command List -> Null" << std::endl;
		return;
	}

	m_pCommandList->Close();
}

ID3D12GraphicsCommandList* Render_CommandObjects::GetCommandList() const
{
	if (m_pCommandList == nullptr)
	{
		std::cout << "Command List -> Null" << std::endl;
	}

	return m_pCommandList;
}

void Render_CommandObjects::FlushCommandQueue()
{
	if (m_pCommandQueue == nullptr || m_pFence == nullptr) return;

	m_currentFence++;
	HRESULT hr = m_pCommandQueue->Signal(m_pFence, m_currentFence);
	if (FAILED(hr))
	{
		std::cout << "Erreur critique : Signal a échoué !" << std::endl;
		return;
	}

	if (m_pFence->GetCompletedValue() < m_currentFence)
	{
		HANDLE eventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);

		if (eventHandle == nullptr)
		{
			std::cout << "Erreur critique : Impossible de créer l'event !" << std::endl;
			return;
		}

		hr = m_pFence->SetEventOnCompletion(m_currentFence, eventHandle);
		if (FAILED(hr))
		{
			std::cout << "Erreur critique : SetEventOnCompletion a échoué !" << std::endl;
			CloseHandle(eventHandle);
			return;
		}

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

