#pragma once

#include "Engine/Core/FileUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

class Material
{
public:
	Material(Renderer* renderer);
	virtual ~Material();

	bool LoadXML(const std::string& XMLFileName);


	Renderer* m_renderer = nullptr;

	std::string m_shaderName;
	std::string m_vertexTypeName;
	std::string m_diffuseTextureName;
	std::string m_normalTextureName;
	std::string m_specGlossEmitTexureName;

	Shader* m_shader = nullptr;
	VertexType m_vertexType = VertexType::Vertex_PCUTBN;
	Texture* m_diffuseTexture = nullptr;
	Texture* m_normalTexure = nullptr;
	Texture* m_specGlossEmitTexure = nullptr;
	Rgba8 m_color = Rgba8::COLOR_WHITE;
};

