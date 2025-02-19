#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

#if defined(OPAQUE)
#undef OPAQUE
#endif

#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Core/EngineBuildPreferences.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Core/Image.hpp"
#include <vector>

#define DX_SAFE_RELEASE(dxObject)	\
{									\
	if ((dxObject) != nullptr)		\
	{								\
		(dxObject)->Release();		\
		(dxObject) = nullptr;		\
	} 								\
}		

static const int k_blurMaxSamples = 64;

static const int k_lightConstantsSlot = 1;
static const int k_cameraConstantsSlot = 2;
static const int k_modelConstantsSlot = 3;
static const int k_blurConstantsSlot = 5;

static const int g_textureNum = 4;

enum class BlendMode
{
	ALPHA,
	ADDITIVE,
	OPAQUE,
	COUNT
};

enum class RasterizerMode
{
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	SOLID_CULL_FRONT,
	WIREFRAME_CULL_NONE,
	WIREFRAME_CULL_BACK,
	COUNT
};
enum class SampleMode
{
	POINT_CLAMP,
	BILINEAR_WRAP,
	BILINEAR_CLAMP,
	COUNT
};
enum class DepthMode
{
	DISABLED,
	ENABLED,
	COUNT
};

struct RendererConfig
{
	Window* m_window = nullptr;
	bool m_renderEmissive = false;
};

struct CameraConstants
{
	Mat44 ProjectionMatrix;
	Mat44 ViewMatrix;
};

struct ModelConstants
{
	Mat44 ModelMatrix;
	float ModelColor[4];
};

struct LightingDebug
{
	int RenderAmbient = 1;
	int RenderDiffuse = 1;
	int RenderSpecular = 1;
	int RenderEmissive = 1;
	int UseDiffuseMap = 1;
	int UseNormalMap = 1;
	int UseSpecularMap = 1;
	int UseGlossinessMap = 1;
	int UseEmissiveMap = 1;
	int Padding[7];
};
struct LightConstants
{
	Vec3 SunDirection = Vec3(0.f, 0.f, -1.f);
	float SunIntensity = 1.f;
	float AmbientIntensity = 0.f;
	Vec3 WordEyePosition = Vec3::ZERO;

	float MinFallOff = 0.f;
	float MaxFallOff = 0.1f;
	float MinFallOffMultiplier = 0.f;
	float MaxFallOffMultiplier = 1.f;

	int RenderAmbient = 1;
	int RenderDiffuse = 1;
	int RenderSpecular = 1;
	int RenderEmissive = 1;
	int UseDiffuseMap = 1;
	int UseNormalMap = 1;
	int UseSpecularMap = 1;
	int UseGlossinessMap = 1;
	int UseEmissiveMap = 1;
	int Padding[7];
};

struct BlurSample
{
	Vec2 Offset;
	float Weight;
	int Padding;
};

struct BlurConstants
{
	Vec2 TexelSize;
	float LerpT;
	int NumSamples;
	BlurSample Samples[k_blurMaxSamples];
};

class Camera;

class Renderer
{
public:
	Renderer(RendererConfig	config);

	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void ClearScreen(const Rgba8& clearColor);
	void BeginCamera(const Camera& camera);
	void EndCamera(const Camera& camera);

	VertexBuffer* CreateVertexBuffer(unsigned int size);
	IndexBuffer* CreateIndexBuffer(size_t size);
	ConstantBuffer* CreateConstantBuffer(const size_t size);
	void CopyCPUToGPU(const void* data, unsigned int size, VertexBuffer*& vbo);
	void CopyCPUToGPU(const void* data, unsigned int size, IndexBuffer*& ibo);
	void CopyCPUToGPU(const void* data, unsigned int size, ConstantBuffer* cbo);
	void BindVertexBuffer(VertexBuffer* vbo, VertexType type = VertexType::Vertex_PCU);
	void BindIndexBuffer(IndexBuffer* ibo);
	void BindConstantBuffer(int slot, ConstantBuffer* cbo);

	void DrawVertexArray(size_t numVertexes, Vertex_PCU const* vertexArray, bool isLinePrimitive = false);
	void DrawVertexArray(size_t numVertexes, Vertex_PCUTBN const* vertexArray);
	void DrawVertexBuffer(VertexBuffer* vbo, size_t vertexCount, int vertexOffset = 0, VertexType type = VertexType::Vertex_PCU);
	void DrawIndexedBuffer(VertexBuffer* vbo, IndexBuffer* ibo, size_t indexCount, int indexOffset = 0, VertexType type = VertexType::Vertex_PCU);
	void DrawIndexedBuffer(std::vector<Vertex_PCUTBN> vertexes, std::vector<unsigned int> indexes, int indexOffset = 0);
	void DrawIndexedBuffer(std::vector<Vertex_PCU> vertexes, std::vector<unsigned int> indexes, int indexOffset = 0);

	void RenderEmissive();
	BlurConstants SetBlurDownConstants();
	BlurConstants SetBlurUpConstants();

	Texture* CreateOrGetTextureFromFile(char const* imageFilePath);
	Texture* CreateTextureFromImage(const Image& image);
	Texture* CreateRenderTexture(const IntVec2& dimensions, const char* name);
	BitmapFont* CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension);

	void BindTexture(const Texture* texture, unsigned int slot = 0);
	void SetBlendMode(BlendMode blendMode);
	void SetSamplerMode(SampleMode samplerMode);
	void SetRasterizerMode(RasterizerMode rasterizerrMode);
	void SetDepthStencilMode(DepthMode depthMode);
	void SetModelConstants(const Mat44& modelMatrix = Mat44(), const Rgba8& modelColor = Rgba8::COLOR_WHITE);
	void SetLightConstant(const LightConstants& lightconstant = LightConstants());
	void SetLightConstants(const Vec3& sunDirection = Vec3(2, 1, -1),
		const float sunIntensity = 0.85,
		const float ambientIntensity = 0.35,
		Vec3 wordEyePosition = Vec3(),
		float minFallOff = 0.f,
		float maxFallOff = 1.f,
		float minFallOffMultiplier = 1.f,
		float maxFallOffMultiplier = 1.f,
		LightingDebug lightDebug = LightingDebug());
	void SetLightConstants(const LightConstants lightConstant = LightConstants());
	void SetBlurConstantBuffer(BlurConstants blurConstatnt = BlurConstants());
	Shader* CreateShader(char const* shaderName, VertexType type = VertexType::Vertex_PCU);
	void BindShader(Shader* shader, VertexType type = VertexType::Vertex_PCU);

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

protected:
	RendererConfig					m_config;
	void* m_apiRederingContext = nullptr;
	std::vector<Texture*>			m_loadedTextures;
	std::vector<BitmapFont*>		m_loadedFonts;
	std::vector<Shader*>			m_loadedShader;
	Shader* m_currentShader = nullptr;
	Shader* m_defaultShader = nullptr;
	VertexBuffer* m_immediateVBO = nullptr;
	VertexBuffer* m_fullScreenQuadVBO = nullptr;
	IndexBuffer* m_immediateIBO = nullptr;
	ConstantBuffer* m_cameraCBO = nullptr;
	ConstantBuffer* m_modelCBO = nullptr;
	ConstantBuffer* m_lightCBO = nullptr;
	ConstantBuffer* m_blurCBO = nullptr;

	const Texture* m_defaultTexture = nullptr;
	const Texture* m_currentTexture = nullptr;

	const Texture* m_emissiveRenderTexture = nullptr;
	const Texture* m_blurredEmissiveTexture = nullptr;
	Shader* m_blurDownShader = nullptr;
	Shader* m_blurUpShader = nullptr;
	Shader* m_compositeShader = nullptr;

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_backbufferRTV = nullptr;

	std::vector<const Texture*> m_blurDownTextures;
	std::vector<const Texture*> m_blurUpTextures;
	D3D11_VIEWPORT m_originalViewport;

	ID3D11BlendState* m_blendState = nullptr;
	BlendMode m_desiredBlendMode = BlendMode::ALPHA;
	ID3D11BlendState* m_blendStates[(int)(BlendMode::COUNT)] = {};

	ID3D11SamplerState* m_samplerState = nullptr;
	SampleMode m_desiredSamplerMode = SampleMode::POINT_CLAMP;
	ID3D11SamplerState* m_samplerStates[(int)(SampleMode::COUNT)] = {};

	ID3D11RasterizerState* m_rasterizerState = nullptr;
	RasterizerMode m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	ID3D11RasterizerState* m_rasterizerStates[(int)(RasterizerMode::COUNT)] = {};

	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D* m_depthStencilTexture = nullptr;
	ID3D11DepthStencilState* m_depthState = nullptr;
	DepthMode						m_desiredDepthMode = DepthMode::DISABLED;
	ID3D11DepthStencilState* m_depthStates[(int)(DepthMode::COUNT)] = {};

	void* m_dxgiDebugModule = nullptr;
	void* m_dxgiDebug = nullptr;

protected:
	Shader* CreateShader(char const* shaderName, char const* shaderSource, VertexType type = VertexType::Vertex_PCU);
	bool CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target);
	void SetStatesIfChanged();

private:

	Texture* CreateTextureFromFile(char const* imageFilePath);
	Texture* CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData);
	Texture* GetTextureForFileName(char const* imageFilePath);

	BitmapFont* GetBitmapFontForFileName(const char* bitmapFontFilePathWithNoExtension);
};