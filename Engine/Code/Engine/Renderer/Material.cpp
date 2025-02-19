#include "Material.hpp"
#include "Engine/Core/XmlUtils.hpp"

Material::Material(Renderer* renderer)
	:m_renderer(renderer)
{

}

Material::~Material()
{

}

bool Material::LoadXML(const std::string& XMLFileName)
{
	XmlDocument file;
	XmlError result = file.LoadFile(XMLFileName.c_str());
	GUARANTEE_OR_DIE(result == tinyxml2::XML_SUCCESS, "FILE IS NOT LOADED");

	XmlElement* rootElement = file.RootElement();
	GUARANTEE_OR_DIE(rootElement, "Root Element is null");

	std::string name = rootElement->Name();
	GUARANTEE_OR_DIE(name == "Material", "Root child element in Material is in the wrong format");

	while (rootElement)
	{
		m_shaderName = ParseXmlAttribute(*rootElement, "shader", "");
		m_vertexTypeName = ParseXmlAttribute(*rootElement, "vertexType", "");
		m_diffuseTextureName = ParseXmlAttribute(*rootElement, "diffuseTexture", "");
		m_normalTextureName = ParseXmlAttribute(*rootElement, "normalTexture", "");
		m_specGlossEmitTexureName = ParseXmlAttribute(*rootElement, "specGlossEmitTexture", "");
		m_color = ParseXmlAttribute(*rootElement, "color", Rgba8::COLOR_WHITE);

		rootElement = rootElement->NextSiblingElement();
	}

	if (m_vertexTypeName == "Vertex_PCUTBN")
	{
		m_vertexType = VertexType::Vertex_PCUTBN;
	}
	else
	{
		m_vertexType = VertexType::Vertex_PCU;
	}

	std::string splitter;
	Strings fileNameSplit = SplitStringOnDelimiter(XMLFileName, "/", true);
	if (fileNameSplit.size() > 1)
	{
		splitter = "/";
	}
	else
	{
		splitter = "\\";
	}

	std::string currentPath = XMLFileName.substr(0, XMLFileName.find_last_of(splitter));
	std::string dataPath = currentPath.substr(0, currentPath.find_last_of(splitter));
	std::string runPath = dataPath.substr(0, dataPath.find_last_of(splitter));
	if (runPath == "Data")
	{
		runPath = "";
	}

	if (m_shaderName != "")
	{
		Strings splitNameShaders = SplitStringOnDelimiter(m_shaderName, "/", true);
		std::string shadersPath = runPath;
		for (size_t i = 0; i < splitNameShaders.size(); i++)
		{
			if (shadersPath.empty())
			{
				shadersPath += splitNameShaders[i];
			}
			else
			{
				shadersPath += splitter + splitNameShaders[i];
			}

		}
		m_shader = m_renderer->CreateShader(shadersPath.c_str(), m_vertexType);
	}

	if (m_diffuseTextureName != "")
	{
		std::string diffusePath = runPath;
		Strings splitNameTextureD = SplitStringOnDelimiter(m_diffuseTextureName, "/", true);
		for (size_t i = 0; i < splitNameTextureD.size(); i++)
		{
			if (diffusePath.empty())
			{
				diffusePath += splitNameTextureD[i];
			}
			else
			{
				diffusePath += splitter + splitNameTextureD[i];
			}
		}
		m_diffuseTexture = m_renderer->CreateOrGetTextureFromFile(diffusePath.c_str());
	}

	if (m_normalTextureName != "")
	{
		std::string normalPath = runPath;
		Strings splitNameTexureN = SplitStringOnDelimiter(m_normalTextureName, "/", true);
		for (size_t i = 0; i < splitNameTexureN.size(); i++)
		{
			if (normalPath.empty())
			{
				normalPath += splitNameTexureN[i];
			}
			else
			{
				normalPath += splitter + splitNameTexureN[i];
			}
		}
		m_normalTexure = m_renderer->CreateOrGetTextureFromFile(normalPath.c_str());
	}

	if (m_specGlossEmitTexureName != "")
	{
		std::string glossPath = runPath;
		Strings splitNameTextureG = SplitStringOnDelimiter(m_specGlossEmitTexureName, "/", true);
		for (size_t i = 0; i < splitNameTextureG.size(); i++)
		{
			if (glossPath.empty())
			{
				glossPath += splitNameTextureG[i];
			}
			else
			{
				glossPath += splitter + splitNameTextureG[i];
			}
		}
		m_specGlossEmitTexure = m_renderer->CreateOrGetTextureFromFile(glossPath.c_str());
	}


	return true;
}
