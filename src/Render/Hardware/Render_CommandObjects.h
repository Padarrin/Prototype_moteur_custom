#pragma once
#include "d3dx12.h"

class Render_CommandObjects
{

private:
	ID3D12CommandQueue* m_pCommandQueue = nullptr;
	ID3D12CommandAllocator* m_pCommandAllocator = nullptr;

	ID3D12Fence* m_pFence = nullptr;
	UINT64 m_currentFence = 0;

	ID3D12GraphicsCommandList* m_pCommandList = nullptr;


public :

	Render_CommandObjects();
	~Render_CommandObjects();

	//Create Command Object
	void CreateCommandQueue(ID3D12Device* _device);
	void CreateCommandAllocator(ID3D12Device* _device);
	void CreateFence(ID3D12Device* _device);
	void CreateCommandList(ID3D12Device* _device);

	ID3D12CommandQueue* GetCommandQueue() const;

	//Command Allocator
	void ResetCommandAllocator();

	//Command List
	void ResetCommandList(ID3D12PipelineState* _pso);
	void CloseCommandList();
	ID3D12GraphicsCommandList* GetCommandList() const;

	//Flush Command Queue
	void FlushCommandQueue();


};

