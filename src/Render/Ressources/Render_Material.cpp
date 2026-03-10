#include "Render_Material.h"

Render_Material::Render_Material(std::string _name, const std::wstring& _path)
{
	m_name = _name;
	m_pShader = new Render_Shader;
	m_pShader->CreateShader(_path);
	m_pShader->CreateInputLayout();

	m_diffuseAlbedo;
	m_fresnelR0;
	m_roughness;
}

Render_Material::~Render_Material()
{
	delete m_pShader;
	m_pShader = nullptr;
}
