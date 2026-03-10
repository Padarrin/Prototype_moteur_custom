#include "Render_Shader.h"
#include "d3dUtil.h"
#include <iostream>
Render_Shader::Render_Shader()
{
}

Render_Shader::~Render_Shader()
{
	m_pVsByteCode->Release();
	m_pPsByteCode->Release();
	m_pVsByteCode = nullptr;
	m_pPsByteCode = nullptr;
}

void Render_Shader::CreateShader(const std::wstring& _path)
{
	HRESULT hr = S_OK;
	m_pVsByteCode = d3dUtil::CompileShader(_path, nullptr,
		"VS", "vs_5_0");

	m_pPsByteCode = d3dUtil::CompileShader(_path, nullptr,
		"PS", "ps_5_0");

	if (!m_pVsByteCode || !m_pPsByteCode) 
	{
		std::cout << "Shader not load" << std::endl;
		throw std::exception("Shader missing");
	}

	std::cout << "Shader compile" << std::endl;
}

void Render_Shader::CreateInputLayout()
{
	m_inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	std::cout << "Input layout create" << std::endl;
}

void Render_Shader::CreateInputLayoutUI()
{
	m_inputLayoutUI =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	std::cout << "Input layout UI create" << std::endl;
}

ID3DBlob* Render_Shader::GetShaderVsByteCode()
{
	return m_pVsByteCode;
}

ID3DBlob* Render_Shader::GetShaderPsByteCode()
{
	return m_pPsByteCode;
}

std::vector<D3D12_INPUT_ELEMENT_DESC> Render_Shader::GetInputLayout()
{
	return m_inputLayout;
}

std::vector<D3D12_INPUT_ELEMENT_DESC> Render_Shader::GetInputLayoutUI()
{
	return m_inputLayoutUI;
}
