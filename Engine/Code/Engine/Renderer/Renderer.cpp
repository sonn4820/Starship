#include <windows.h>
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Renderer/DefaultShader.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Renderer/Camera.hpp"
#include <vector>

Renderer::Renderer(RendererConfig config)
	:m_config(config)
{

}

void Renderer::Startup()
{
	//Create debug module
#if defined(ENGINE_DEBUG_RENDER)
	m_dxgiDebugModule = (void*)::LoadLibraryA("dxgidebug.dll");
	if (m_dxgiDebugModule == nullptr)
	{
		ERROR_AND_DIE("Couldn't load dxgidebug.dll");
	}
	typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
	((GetDebugModuleCB)::GetProcAddress((HMODULE)m_dxgiDebugModule, "DXGIGetDebugInterface"))
		(__uuidof(IDXGIDebug), &m_dxgiDebug);
	if (m_dxgiDebug == nullptr)
	{
		ERROR_AND_DIE("Couldn't load debug module.");
	}
#endif

	//Render Startup
	unsigned int devliceFlags = 0;
#if defined(ENGINE_DEBUG_RENDER)
	devliceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//Create device and swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = m_config.m_window->GetClientDimensions().x;
	swapChainDesc.BufferDesc.Height = m_config.m_window->GetClientDimensions().y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (HWND)m_config.m_window->GetHwnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, devliceFlags,
		nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&m_swapChain, &m_device, nullptr, &m_deviceContext);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Couldn't create D3D 11 device and swap chain");
	}

	//Get back buffer texture
	ID3D11Texture2D* backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Couldn't get swap chain buffer");
	}
	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_backbufferRTV);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Couldn't create render target view for swap chain buffer");
	}
	backBuffer->Release();

	m_defaultShader = CreateShader("Default", g_defaultShaderSource);
	BindShader(m_currentShader);

	m_immediateVBO = CreateVertexBuffer(sizeof(Vertex_PCU));
	m_immediateIBO = CreateIndexBuffer(sizeof(unsigned int));

	m_cameraCBO = CreateConstantBuffer(sizeof(CameraConstants));
	m_modelCBO = CreateConstantBuffer(sizeof(ModelConstants));
	m_lightCBO = CreateConstantBuffer(sizeof(LightConstants));
	m_blurCBO = CreateConstantBuffer(sizeof(BlurConstants));

	//Set rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDescSolidNone = { };
	rasterizerDescSolidNone.FillMode = D3D11_FILL_SOLID;
	rasterizerDescSolidNone.CullMode = D3D11_CULL_NONE;
	rasterizerDescSolidNone.FrontCounterClockwise = true;
	rasterizerDescSolidNone.DepthClipEnable = true;
	rasterizerDescSolidNone.AntialiasedLineEnable = true;
	rasterizerDescSolidNone.DepthBias = 0;
	rasterizerDescSolidNone.DepthBiasClamp = 0.0f;
	rasterizerDescSolidNone.SlopeScaledDepthBias = 0.0f;
	rasterizerDescSolidNone.ScissorEnable = false;
	rasterizerDescSolidNone.MultisampleEnable = false;

	hr = m_device->CreateRasterizerState(&rasterizerDescSolidNone, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_NONE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Rasterizer State for RasterizerMode::SOLID_CULL_NONE failed"));
	}

	D3D11_RASTERIZER_DESC rasterizerDescSolidBack = { };
	rasterizerDescSolidBack.FillMode = D3D11_FILL_SOLID;
	rasterizerDescSolidBack.CullMode = D3D11_CULL_BACK;
	rasterizerDescSolidBack.FrontCounterClockwise = true;
	rasterizerDescSolidBack.DepthClipEnable = true;
	rasterizerDescSolidBack.AntialiasedLineEnable = true;
	rasterizerDescSolidBack.DepthBias = 0;
	rasterizerDescSolidBack.DepthBiasClamp = 0.0f;
	rasterizerDescSolidBack.SlopeScaledDepthBias = 0.0f;
	rasterizerDescSolidBack.ScissorEnable = false;
	rasterizerDescSolidBack.MultisampleEnable = false;

	hr = m_device->CreateRasterizerState(&rasterizerDescSolidBack, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_BACK]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Rasterizer State for RasterizerMode::SOLID_CULL_BACK failed"));
	}

	D3D11_RASTERIZER_DESC rasterizerDescSolidFront = { };
	rasterizerDescSolidFront.FillMode = D3D11_FILL_SOLID;
	rasterizerDescSolidFront.CullMode = D3D11_CULL_FRONT;
	rasterizerDescSolidFront.FrontCounterClockwise = true;
	rasterizerDescSolidFront.DepthClipEnable = true;
	rasterizerDescSolidFront.AntialiasedLineEnable = true;
	rasterizerDescSolidFront.DepthBias = 0;
	rasterizerDescSolidFront.DepthBiasClamp = 0.0f;
	rasterizerDescSolidFront.SlopeScaledDepthBias = 0.0f;
	rasterizerDescSolidFront.ScissorEnable = false;
	rasterizerDescSolidFront.MultisampleEnable = false;

	hr = m_device->CreateRasterizerState(&rasterizerDescSolidFront, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_FRONT]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Rasterizer State for RasterizerMode::SOLID_CULL_FRONT failed"));
	}

	D3D11_RASTERIZER_DESC rasterizerDescWireframeNone = { };
	rasterizerDescWireframeNone.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDescWireframeNone.CullMode = D3D11_CULL_NONE;
	rasterizerDescWireframeNone.FrontCounterClockwise = true;
	rasterizerDescWireframeNone.DepthClipEnable = true;
	rasterizerDescWireframeNone.AntialiasedLineEnable = true;
	rasterizerDescWireframeNone.DepthBias = 0;
	rasterizerDescWireframeNone.DepthBiasClamp = 0.0f;
	rasterizerDescWireframeNone.SlopeScaledDepthBias = 0.0f;
	rasterizerDescWireframeNone.ScissorEnable = false;
	rasterizerDescWireframeNone.MultisampleEnable = false;

	hr = m_device->CreateRasterizerState(&rasterizerDescWireframeNone, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_NONE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Rasterizer State for RasterizerMode::WIREFRAME_CULL_NONE failed"));
	}

	D3D11_RASTERIZER_DESC rasterizerDescWireframeBack = { };
	rasterizerDescWireframeBack.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDescWireframeBack.CullMode = D3D11_CULL_BACK;
	rasterizerDescWireframeBack.FrontCounterClockwise = true;
	rasterizerDescWireframeBack.DepthClipEnable = true;
	rasterizerDescWireframeBack.AntialiasedLineEnable = true;
	rasterizerDescWireframeBack.DepthBias = 0;
	rasterizerDescWireframeBack.DepthBiasClamp = 0.0f;
	rasterizerDescWireframeBack.SlopeScaledDepthBias = 0.0f;
	rasterizerDescWireframeBack.ScissorEnable = false;
	rasterizerDescWireframeBack.MultisampleEnable = false;

	hr = m_device->CreateRasterizerState(&rasterizerDescWireframeBack, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_BACK]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Rasterizer State for RasterizerMode::WIREFRAME_CULL_BACK failed"));
	}

	SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);

	// OPAQUE set up
	D3D11_BLEND_DESC blendDescOPAQUE = {};
	blendDescOPAQUE.RenderTarget[0].BlendEnable = TRUE;
	blendDescOPAQUE.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDescOPAQUE.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDescOPAQUE.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescOPAQUE.RenderTarget[0].SrcBlendAlpha = blendDescOPAQUE.RenderTarget[0].SrcBlend;
	blendDescOPAQUE.RenderTarget[0].DestBlendAlpha = blendDescOPAQUE.RenderTarget[0].DestBlend;
	blendDescOPAQUE.RenderTarget[0].BlendOpAlpha = blendDescOPAQUE.RenderTarget[0].BlendOp;
	blendDescOPAQUE.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&blendDescOPAQUE, &m_blendStates[(int)BlendMode::OPAQUE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Create Blend State for BlendMode::OPAQUE failed")
	}

	// ALPHA set up
	D3D11_BLEND_DESC blendDescALPHA = {};
	blendDescALPHA.RenderTarget[0].BlendEnable = TRUE;
	blendDescALPHA.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDescALPHA.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDescALPHA.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescALPHA.RenderTarget[0].SrcBlendAlpha = blendDescALPHA.RenderTarget[0].SrcBlend;
	blendDescALPHA.RenderTarget[0].DestBlendAlpha = blendDescALPHA.RenderTarget[0].DestBlend;
	blendDescALPHA.RenderTarget[0].BlendOpAlpha = blendDescALPHA.RenderTarget[0].BlendOp;
	blendDescALPHA.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&blendDescALPHA, &m_blendStates[(int)BlendMode::ALPHA]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Create Blend State for BlendMode::ALPHA failed")
	}

	// ADDITIVE set up
	D3D11_BLEND_DESC blendDescADDITIVE = {};
	blendDescADDITIVE.RenderTarget[0].BlendEnable = TRUE;
	blendDescADDITIVE.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDescADDITIVE.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDescADDITIVE.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescADDITIVE.RenderTarget[0].SrcBlendAlpha = blendDescADDITIVE.RenderTarget[0].SrcBlend;
	blendDescADDITIVE.RenderTarget[0].DestBlendAlpha = blendDescADDITIVE.RenderTarget[0].DestBlend;
	blendDescADDITIVE.RenderTarget[0].BlendOpAlpha = blendDescADDITIVE.RenderTarget[0].BlendOp;
	blendDescADDITIVE.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&blendDescADDITIVE, &m_blendStates[(int)BlendMode::ADDITIVE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Create Blend State for BlendMode::ADDITIVE failed")
	}

	Image* image22white = new Image(IntVec2(2, 2), Rgba8::COLOR_WHITE);
	m_defaultTexture = CreateTextureFromImage(*image22white);
	BindTexture(m_defaultTexture);

	D3D11_SAMPLER_DESC samplerDescPC = {};
	samplerDescPC.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDescPC.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescPC.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescPC.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescPC.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescPC.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_device->CreateSamplerState(&samplerDescPC, &m_samplerStates[(int)SampleMode::POINT_CLAMP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerState for SampleMode::POINT_CLAMP failed")
	}

	D3D11_SAMPLER_DESC samplerDescBC = {};
	samplerDescBC.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescBC.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescBC.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescBC.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescBC.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescBC.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_device->CreateSamplerState(&samplerDescBC, &m_samplerStates[(int)SampleMode::BILINEAR_CLAMP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerState for SampleMode::BILINEAR_CLAMP failed")
	}

	D3D11_SAMPLER_DESC samplerDescBW = {};
	samplerDescBW.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescBW.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescBW.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescBW.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescBW.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescBW.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_device->CreateSamplerState(&samplerDescBW, &m_samplerStates[(int)SampleMode::BILINEAR_WRAP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerState for SampleMode::BILINEAR_WRAP failed")
	}

	SetSamplerMode(SampleMode::POINT_CLAMP);

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = m_config.m_window->GetClientDimensions().x;
	textureDesc.Height = m_config.m_window->GetClientDimensions().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.SampleDesc.Count = 1;

	hr = m_device->CreateTexture2D(&textureDesc, nullptr, &m_depthStencilTexture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Couldn't create texture for depth stencil")
	}

	hr = m_device->CreateDepthStencilView(m_depthStencilTexture, nullptr, &m_depthStencilView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Couldn't create texture for depth stencil")
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDescDisabled = {};
	depthStencilDescDisabled.DepthEnable = TRUE;
	depthStencilDescDisabled.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDescDisabled.DepthFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_device->CreateDepthStencilState(&depthStencilDescDisabled, &m_depthStates[(int)DepthMode::DISABLED]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateDepthStencilState for DepthMode::DISABLED failed")
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDescEnabled = {};
	depthStencilDescEnabled.DepthEnable = TRUE;
	depthStencilDescEnabled.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescEnabled.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_device->CreateDepthStencilState(&depthStencilDescEnabled, &m_depthStates[(int)DepthMode::ENABLED]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateDepthStencilState for DepthMode::DISABLED failed")
	}

	IntVec2 fullScreenSize = m_config.m_window->GetClientDimensions();

	if (m_config.m_renderEmissive)
	{
		m_emissiveRenderTexture = CreateRenderTexture(fullScreenSize, "EmissiveTexture");
		m_blurredEmissiveTexture = CreateRenderTexture(fullScreenSize, "BlurredEmissiveTexture");
		m_blurDownShader = CreateShader("Data/Shaders/BlurDown");
		m_blurUpShader = CreateShader("Data/Shaders/BlurUp");
		m_compositeShader = CreateShader("Data/Shaders/Composite");

		int width = 0;
		int height = 0;
		for (int i = 0; i < g_textureNum; i++)
		{
			height = fullScreenSize.y / (int)pow(2, i + 1);
			width = fullScreenSize.x / (int)pow(2, i + 1);
			m_blurDownTextures.push_back(CreateRenderTexture(IntVec2(width, height), Stringf("DownSampleTexture %i", i).c_str()));
		}
		for (int i = 0; i < g_textureNum; i++)
		{
			height = fullScreenSize.y / (int)pow(2, i);
			width = fullScreenSize.x / (int)pow(2, i);
			m_blurUpTextures.push_back(CreateRenderTexture(IntVec2(width, height), Stringf("UpSampleTexture %i", i).c_str()));
		}
	}

	// Save the viewport size for later
	m_originalViewport = { 0.0f, 0.0f, static_cast<float>(fullScreenSize.x), static_cast<float>(fullScreenSize.y), 0.0f, 1.0f };
}
void Renderer::BeginFrame()
{
	if (m_config.m_renderEmissive)
	{
		ID3D11RenderTargetView* RTVs[2] = {
			m_backbufferRTV,								// Back buffer should be the first render target
			m_emissiveRenderTexture->m_renderTargetView			// Emissive render texture should be the second render target
		};

		// Set the render targets
		m_deviceContext->OMSetRenderTargets(2, RTVs, m_depthStencilView);
	}
	else
	{
		ID3D11RenderTargetView* RTVs[1] = {
			m_backbufferRTV,							
		};
		m_deviceContext->OMSetRenderTargets(1, RTVs, m_depthStencilView);
	}
}
void Renderer::EndFrame()
{
	//Present
	HRESULT hr;
	hr = m_swapChain->Present(0, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		ERROR_AND_DIE("Device has been lost, application will now terminate.");
	}
}
void Renderer::Shutdown()
{
	delete m_fullScreenQuadVBO;
	m_fullScreenQuadVBO = nullptr;

	delete m_immediateIBO;
	m_immediateIBO = nullptr;

	delete m_immediateVBO;
	m_immediateVBO = nullptr;

	delete m_cameraCBO;
	m_cameraCBO = nullptr;

	delete m_modelCBO;
	m_modelCBO = nullptr;

	delete m_lightCBO;
	m_lightCBO = nullptr;

	delete m_blurCBO;
	m_blurCBO = nullptr;

	delete m_defaultTexture;
	m_defaultTexture = nullptr;

	for (size_t i = 0; i < m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i] != nullptr)
		{
			delete m_loadedTextures[i];
		}
		m_loadedTextures[i] = nullptr;
	}
	for (size_t i = 0; i < m_loadedFonts.size(); i++)
	{
		if (m_loadedFonts[i] != nullptr)
		{
			delete m_loadedFonts[i];
		}
		m_loadedFonts[i] = nullptr;
	}
	for (size_t i = 0; i < m_loadedShader.size(); i++)
	{
		if (m_loadedShader[i] != nullptr)
		{
			delete m_loadedShader[i];
		}
		m_loadedShader[i] = nullptr;
	}


	delete m_apiRederingContext;
	m_apiRederingContext = nullptr;

	DX_SAFE_RELEASE(m_device);
	DX_SAFE_RELEASE(m_deviceContext);
	DX_SAFE_RELEASE(m_backbufferRTV);
	DX_SAFE_RELEASE(m_swapChain);

	m_blendState = nullptr;
	m_rasterizerState = nullptr;
	m_samplerState = nullptr;

	for (int i = 0; i < (int)BlendMode::COUNT; i++)
	{
		DX_SAFE_RELEASE(m_blendStates[i]);
	}
	for (int i = 0; i < (int)SampleMode::COUNT; i++)
	{
		DX_SAFE_RELEASE(m_samplerStates[i]);
	}
	for (int i = 0; i < (int)RasterizerMode::COUNT; i++)
	{
		DX_SAFE_RELEASE(m_rasterizerStates[i]);
	}
	DX_SAFE_RELEASE(m_depthStencilTexture);
	DX_SAFE_RELEASE(m_depthStencilView);
	for (int i = 0; i < (int)DepthMode::COUNT; i++)
	{
		DX_SAFE_RELEASE(m_depthStates[i]);
	}

	//Report error leaks and release debug module
#if defined(ENGINE_DEBUG_RENDER)
	((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(
		DXGI_DEBUG_ALL,
		(DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
	);
	((IDXGIDebug*)m_dxgiDebug)->Release();
	m_dxgiDebug = nullptr;

	::FreeLibrary((HMODULE)m_dxgiDebugModule);
	m_dxgiDebugModule = nullptr;
#endif
}

void Renderer::ClearScreen(const Rgba8& clearColor)
{
	float colorAsFloat[4];
	clearColor.GetAsFloats(colorAsFloat);
	m_deviceContext->ClearRenderTargetView(m_backbufferRTV, colorAsFloat);

	if (m_config.m_renderEmissive)
	{
		float blackAsFloat[4] = { 0.f, 0.f, 0.f , 1.f };
		m_deviceContext->ClearRenderTargetView(m_emissiveRenderTexture->m_renderTargetView, blackAsFloat);
		m_deviceContext->ClearRenderTargetView(m_blurredEmissiveTexture->m_renderTargetView, blackAsFloat);

		for (auto& blurUpTexture : m_blurUpTextures)
		{
			m_deviceContext->ClearRenderTargetView(blurUpTexture->m_renderTargetView, blackAsFloat);
		}

		for (auto& blurDownTexture : m_blurDownTextures)
		{
			m_deviceContext->ClearRenderTargetView(blurDownTexture->m_renderTargetView, blackAsFloat);
		}
	}


	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Renderer::BeginCamera(const Camera& camera)
{
 	CameraConstants localCameraConstants;
	localCameraConstants.ProjectionMatrix = camera.GetProjectionMatrix();
	localCameraConstants.ViewMatrix = camera.GetViewMatrix();

	CopyCPUToGPU(&localCameraConstants, (unsigned int)m_cameraCBO->m_size, m_cameraCBO);

	AABB2 viewportUVs = camera.GetViewportUVs();
	//Set Viewport
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = viewportUVs.m_mins.x * (float)m_config.m_window->GetClientDimensions().x;
	viewport.TopLeftY = viewportUVs.m_mins.y * (float)m_config.m_window->GetClientDimensions().y;
	viewport.Width = (float)m_config.m_window->GetClientDimensions().x * (viewportUVs.m_maxs.x - viewportUVs.m_mins.x);
	viewport.Height = (float)m_config.m_window->GetClientDimensions().y * (viewportUVs.m_maxs.y - viewportUVs.m_mins.y);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	BindConstantBuffer(k_cameraConstantsSlot, m_cameraCBO);

	m_deviceContext->RSSetViewports(1, &viewport);
}
void Renderer::EndCamera(const Camera& camera)
{
	UNUSED(camera);
}
void Renderer::DrawVertexBuffer(VertexBuffer* vbo, size_t vertexCount, int vertexOffset, VertexType type)
{
	BindVertexBuffer(vbo, type);
	SetStatesIfChanged();
	m_deviceContext->Draw((UINT)vertexCount, vertexOffset);
}
void Renderer::DrawIndexedBuffer(VertexBuffer* vbo, IndexBuffer* ibo, size_t indexCount, int indexOffset, VertexType type)
{
	BindIndexBuffer(ibo);
	BindVertexBuffer(vbo, type);
	SetStatesIfChanged();
	m_deviceContext->DrawIndexed((UINT)indexCount, indexOffset, 0);
}

void Renderer::DrawIndexedBuffer(std::vector<Vertex_PCUTBN> vertexes, std::vector<unsigned int> indexes, int indexOffset)
{
	CopyCPUToGPU(vertexes.data(), (unsigned int)vertexes.size() * sizeof(Vertex_PCUTBN), m_immediateVBO);
	CopyCPUToGPU(indexes.data(), (unsigned int)indexes.size() * sizeof(unsigned int), m_immediateIBO);
	BindVertexBuffer(m_immediateVBO, VertexType::Vertex_PCUTBN);
	BindIndexBuffer(m_immediateIBO);
	SetStatesIfChanged();
	m_deviceContext->DrawIndexed((int)indexes.size(), indexOffset, 0);
}
void Renderer::DrawIndexedBuffer(std::vector<Vertex_PCU> vertexes, std::vector<unsigned int> indexes, int indexOffset)
{
	CopyCPUToGPU(vertexes.data(), (unsigned int)vertexes.size() * sizeof(Vertex_PCU), m_immediateVBO);
	CopyCPUToGPU(indexes.data(), (unsigned int)indexes.size() * sizeof(unsigned int), m_immediateIBO);
	BindVertexBuffer(m_immediateVBO);
	BindIndexBuffer(m_immediateIBO);
	SetStatesIfChanged();
	m_deviceContext->DrawIndexed((int)indexes.size(), indexOffset, 0);
}

void Renderer::RenderEmissive()
{
	if (!m_config.m_renderEmissive)
	{
		return;
	}

	//Set Blur Down
	BlurConstants blurDownConstants = SetBlurDownConstants();
	SetBlurConstantBuffer(blurDownConstants);
	m_deviceContext->OMSetRenderTargets(1, &m_blurDownTextures[0]->m_renderTargetView, nullptr);

	BindTexture(m_emissiveRenderTexture);
	BindShader(m_blurDownShader);
	SetModelConstants();
	SetRasterizerMode(RasterizerMode::SOLID_CULL_FRONT);
	SetSamplerMode(SampleMode::BILINEAR_CLAMP);
	SetBlendMode(BlendMode::OPAQUE);
	SetDepthStencilMode(DepthMode::DISABLED);

	//Draw Blur Down
	std::vector<Vertex_PCU> screenVerts;
	AddVertsForAABB2D(screenVerts, AABB2(Vec2(-1.f, 1.f), Vec2(0.f, 0.f)), Rgba8::COLOR_WHITE);
	DrawVertexArray((int)screenVerts.size(), screenVerts.data());
	for (int i = 1; i < g_textureNum; i++)
	{
		screenVerts.clear();
		AddVertsForAABB2D(screenVerts, AABB2(Vec2(-1.f, 1.f), Vec2(-1.f + (float)pow(0.5, i), 1.f - (float)pow(0.5, i))), Rgba8::COLOR_WHITE);
		blurDownConstants.TexelSize = Vec2(1.f / m_blurDownTextures[i - 1]->GetDimensions().x, 1.f / m_blurDownTextures[i - 1]->GetDimensions().y);
		SetBlurConstantBuffer(blurDownConstants);
		m_deviceContext->OMSetRenderTargets(1, &m_blurDownTextures[i]->m_renderTargetView, nullptr);
		BindTexture(m_blurDownTextures[i - 1]);
		DrawVertexArray((int)screenVerts.size(), screenVerts.data());
	}

	//Set Blur Up
	BlurConstants blurUpConstants = SetBlurUpConstants();
	SetBlurConstantBuffer(blurUpConstants);
	m_deviceContext->OMSetRenderTargets(1, &m_blurUpTextures[g_textureNum - 1]->m_renderTargetView, nullptr);
	BindTexture(m_blurDownTextures[g_textureNum - 1], 0);
	BindTexture(m_blurDownTextures[g_textureNum - 1], 1);
	BindShader(m_blurUpShader);

	//Draw Blur UP
	screenVerts.clear();
	AddVertsForAABB2D(screenVerts, AABB2(Vec2(-1.f, 1.f), Vec2(-1.f + (float)pow(0.5, g_textureNum - 2), 1.f - (float)pow(0.5, g_textureNum - 2))), Rgba8::COLOR_WHITE);
	DrawVertexArray((int)screenVerts.size(), screenVerts.data());
	for (int i = g_textureNum - 2; i >= 0; i--)
	{
		screenVerts.clear();
		AddVertsForAABB2D(screenVerts, AABB2(Vec2(-1.f, 1.f), Vec2(-1.f + (float)pow(0.5, i - 1), 1.f - (float)pow(0.5, i - 1))), Rgba8::COLOR_WHITE);
		blurUpConstants.TexelSize = Vec2(1.f / m_blurUpTextures[i + 1]->GetDimensions().x, 1.f / m_blurUpTextures[i + 1]->GetDimensions().y);
		SetBlurConstantBuffer(blurUpConstants);
		m_deviceContext->OMSetRenderTargets(1, &m_blurUpTextures[i]->m_renderTargetView, nullptr);
		BindTexture(m_blurDownTextures[i]);
		DrawVertexArray((int)screenVerts.size(), screenVerts.data());
	}

	//Set and Draw Composite
	m_deviceContext->OMSetRenderTargets(1, &m_backbufferRTV, m_depthStencilView);
	screenVerts.clear();
	AddVertsForAABB2D(screenVerts, AABB2(Vec2(-1.f, 1.f), Vec2(1.f, -1.f)), Rgba8::COLOR_WHITE);
	SetBlendMode(BlendMode::ADDITIVE);
	SetRasterizerMode(RasterizerMode::SOLID_CULL_FRONT);
	BindShader(m_compositeShader);
	BindTexture(m_blurUpTextures[0]);
	DrawVertexArray((int)screenVerts.size(), screenVerts.data());


	ID3D11RenderTargetView* RTVs[] =
	{
		m_backbufferRTV,
		m_emissiveRenderTexture->m_renderTargetView,
	};
	m_deviceContext->OMSetRenderTargets(2, RTVs, m_depthStencilView);
}

BlurConstants Renderer::SetBlurDownConstants()
{
	BlurConstants blurConstants;
	//Blur Down
	blurConstants.NumSamples = 13;
	blurConstants.TexelSize = Vec2(1.f / m_emissiveRenderTexture->GetDimensions().x, 1.f / m_emissiveRenderTexture->GetDimensions().y);
	blurConstants.LerpT = 1.f;
	blurConstants.Samples[0].Offset = Vec2();
	blurConstants.Samples[0].Weight = 0.0968f;
	blurConstants.Samples[1].Offset = Vec2(1.f, 1.f);
	blurConstants.Samples[1].Weight = 0.129f;
	blurConstants.Samples[2].Offset = Vec2(1.f, -1.f);
	blurConstants.Samples[2].Weight = 0.129f;
	blurConstants.Samples[3].Offset = Vec2(-1.f, -1.f);
	blurConstants.Samples[3].Weight = 0.129f;
	blurConstants.Samples[4].Offset = Vec2(-1.f, 1.f);
	blurConstants.Samples[4].Weight = 0.129f;
	blurConstants.Samples[5].Offset = Vec2(2.f, 0.f);
	blurConstants.Samples[5].Weight = 0.0645f;
	blurConstants.Samples[6].Offset = Vec2(-2.f, 0.f);
	blurConstants.Samples[6].Weight = 0.0645f;
	blurConstants.Samples[7].Offset = Vec2(0.f, 2.f);
	blurConstants.Samples[7].Weight = 0.0645f;
	blurConstants.Samples[8].Offset = Vec2(0.f, -2.f);
	blurConstants.Samples[8].Weight = 0.0645f;
	blurConstants.Samples[9].Offset = Vec2(2.f, 2.f);
	blurConstants.Samples[9].Weight = 0.0323f;
	blurConstants.Samples[10].Offset = Vec2(-2.f, 2.f);
	blurConstants.Samples[10].Weight = 0.0323f;
	blurConstants.Samples[11].Offset = Vec2(2.f, -2.f);
	blurConstants.Samples[11].Weight = 0.0323f;
	blurConstants.Samples[12].Offset = Vec2(-2.f, -2.f);
	blurConstants.Samples[12].Weight = 0.0323f;
	return blurConstants;
}

BlurConstants Renderer::SetBlurUpConstants()
{
	BlurConstants blurConstants;
	//Blur Up
	blurConstants.NumSamples = 9;
	blurConstants.LerpT = 0.85f;
	blurConstants.TexelSize = Vec2(1.f / m_blurDownTextures[4 - 1]->GetDimensions().x, 1.f / m_blurDownTextures[4 - 1]->GetDimensions().y);
	blurConstants.Samples[0].Offset = Vec2();
	blurConstants.Samples[0].Weight = 0.25f;
	blurConstants.Samples[1].Offset = Vec2(1.f, 0.f);
	blurConstants.Samples[1].Weight = 0.125f;
	blurConstants.Samples[2].Offset = Vec2(0.f, -1.f);
	blurConstants.Samples[2].Weight = 0.125f;
	blurConstants.Samples[3].Offset = Vec2(-1.f, 0.f);
	blurConstants.Samples[3].Weight = 0.125f;
	blurConstants.Samples[4].Offset = Vec2(0.f, 1.f);
	blurConstants.Samples[4].Weight = 0.125f;
	blurConstants.Samples[5].Offset = Vec2(1.f, 1.f);
	blurConstants.Samples[5].Weight = 0.0625f;
	blurConstants.Samples[6].Offset = Vec2(-1.f, 1.f);
	blurConstants.Samples[6].Weight = 0.0625f;
	blurConstants.Samples[7].Offset = Vec2(1.f, -1.f);
	blurConstants.Samples[7].Weight = 0.0625f;
	blurConstants.Samples[8].Offset = Vec2(1.f, -1.f);
	blurConstants.Samples[8].Weight = 0.0625f;
	return blurConstants;
}

void Renderer::DrawVertexArray(size_t numVertexes, Vertex_PCU const* vertexArray, bool isLinePrimitive)
{
	m_immediateVBO->SetIsLinePrimitive(isLinePrimitive);
	CopyCPUToGPU(vertexArray, (unsigned int)numVertexes * sizeof(Vertex_PCU), m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes, 0);
}

void Renderer::DrawVertexArray(size_t numVertexes, Vertex_PCUTBN const* vertexArray)
{
	CopyCPUToGPU(vertexArray, (unsigned int)numVertexes * sizeof(Vertex_PCUTBN), m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes, 0, VertexType::Vertex_PCUTBN);
}

//------------------------------------------------------------------------------------------------
Texture* Renderer::CreateOrGetTextureFromFile(char const* imageFilePath)
{
	// See if we already have this texture previously loaded
	Texture* existingTexture = GetTextureForFileName(imageFilePath);
	if (existingTexture)
	{
		return existingTexture;
	}

	// Never seen this texture before!  Let's load it.
	Texture* newTexture = CreateTextureFromFile(imageFilePath);
	newTexture->m_imageFilePath = imageFilePath;
	return newTexture;
}

Texture* Renderer::CreateTextureFromImage(const Image& image)
{
	HRESULT hr;

	Texture* newTexture = new Texture();
	newTexture->m_dimensions = image.GetDimensions();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = image.GetDimensions().x;
	textureDesc.Height = image.GetDimensions().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA textureData;
	textureData.pSysMem = image.GetRawData();
	textureData.SysMemPitch = 4 * image.GetDimensions().x;


	hr = m_device->CreateTexture2D(&textureDesc, &textureData, &newTexture->m_texture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateTextureFromImage failed for the image file \"%s\".", image.GetImageFilePath().c_str()));
	}

	hr = m_device->CreateShaderResourceView(newTexture->m_texture, NULL, &newTexture->m_shaderResourceView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateShaderResourceView failed for the image file \"%s\".", image.GetImageFilePath().c_str()));
	}

	return newTexture;
}


Texture* Renderer::CreateRenderTexture(const IntVec2& dimensions, const char* name)
{
	HRESULT hr;

	Texture* newTexture = new Texture();
	newTexture->m_dimensions = dimensions;
	newTexture->m_imageFilePath = name;

	D3D11_TEXTURE2D_DESC renderTextureDesc = {};
	renderTextureDesc.Width = dimensions.x; 
	renderTextureDesc.Height = dimensions.y;
	renderTextureDesc.MipLevels = 1;
	renderTextureDesc.ArraySize = 1;
	renderTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderTextureDesc.SampleDesc.Count = 1;

	hr = m_device->CreateTexture2D(&renderTextureDesc, NULL, &newTexture->m_texture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Render Texture failed \"%s\".", name));
	}

	// Create Render Target View

	hr = m_device->CreateRenderTargetView(newTexture->m_texture, NULL, &newTexture->m_renderTargetView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Render Target View failed \"%s\".", name));
	}

	// Create Shader Resource View

	hr = m_device->CreateShaderResourceView(newTexture->m_texture, NULL, &newTexture->m_shaderResourceView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Shader Resource View failed \"%s\".", name));
	}

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Shader* Renderer::CreateShader(char const* shaderName, char const* shaderSource, VertexType type)
{
	HRESULT hr;
	ShaderConfig newConfig;
	newConfig.m_name = shaderName;
	Shader* newShader = new Shader(newConfig);

	std::vector<unsigned char> vertexShaderByteCode;
	std::vector<unsigned char> pixelShaderByteCode;

	bool vertexShaderCompiledResult = CompileShaderToByteCode(vertexShaderByteCode, shaderName, shaderSource, newShader->m_config.m_vertexEntryPoint.c_str(), "vs_5_0");
	if (!vertexShaderCompiledResult)
	{
		ERROR_AND_DIE(Stringf("Could not compile vertex shader."));
	}
	hr = m_device->CreateVertexShader(
		vertexShaderByteCode.data(),
		vertexShaderByteCode.size(),
		NULL, &newShader->m_vertexShader
	);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create vertex shader."));
	}

	bool pixelShaderCompiledResult = CompileShaderToByteCode(pixelShaderByteCode, shaderName, shaderSource, newShader->m_config.m_pixelEntryPoint.c_str(), "ps_5_0");
	if (!pixelShaderCompiledResult)
	{
		ERROR_AND_DIE(Stringf("Could not compile pixel shader."));
	}
	hr = m_device->CreatePixelShader(
		pixelShaderByteCode.data(),
		pixelShaderByteCode.size(),
		NULL, &newShader->m_pixelShader
	);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create pixel shader."));
	}

	if (type == VertexType::Vertex_PCU)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		UINT numElements = ARRAYSIZE(inputElementDesc);
		hr = m_device->CreateInputLayout(
			inputElementDesc, numElements,
			vertexShaderByteCode.data(),
			vertexShaderByteCode.size(),
			&newShader->m_inputLayoutForVertex_PCU
		);
		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE(Stringf("Could not create vertex pcu layout."));
		}
	}
	else if (type == VertexType::Vertex_PCUTBN)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		UINT numElements = ARRAYSIZE(inputElementDesc);
		hr = m_device->CreateInputLayout(
			inputElementDesc, numElements,
			vertexShaderByteCode.data(),
			vertexShaderByteCode.size(),
			&newShader->m_inputLayoutForVertex_PCUTBN
		);
		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE(Stringf("Could not create vertex pcutbn layout."));
		}
	}

	m_loadedShader.push_back(newShader);

	return newShader;
}

bool Renderer::CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target)
{
	DWORD shaderFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined(ENGINE_DEBUG_RENDER)
	shaderFlags = D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob* shaderBlob = NULL;
	ID3DBlob* errorBlob = NULL;

	HRESULT hr = D3DCompile(
		source, strlen(source),
		name, nullptr, nullptr,
		entryPoint, target, shaderFlags, 0,
		&shaderBlob, &errorBlob
	);
	if (SUCCEEDED(hr))
	{
		outByteCode.resize(shaderBlob->GetBufferSize());
		memcpy(
			outByteCode.data(),
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize()
		);
	}
	else
	{
		if (errorBlob != NULL)
		{
			DebuggerPrintf((char*)errorBlob->GetBufferPointer());
		}
		return false;
	}
	shaderBlob->Release();
	if (errorBlob != NULL)
	{
		errorBlob->Release();
	}
	return true;
}

void Renderer::BindShader(Shader* shader, VertexType type)
{
	m_currentShader = (shader != nullptr) ? shader : m_defaultShader;
	m_deviceContext->VSSetShader(m_currentShader->m_vertexShader, nullptr, 0);
	m_deviceContext->PSSetShader(m_currentShader->m_pixelShader, nullptr, 0);
	if (type == VertexType::Vertex_PCU)
	{
		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayoutForVertex_PCU);
	}
	else if (type == VertexType::Vertex_PCUTBN)
	{
		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayoutForVertex_PCUTBN);
	}

}

ID3D11Device* Renderer::GetDevice() const
{
	return m_device;
}

ID3D11DeviceContext* Renderer::GetDeviceContext() const
{
	return m_deviceContext;
}

Shader* Renderer::CreateShader(char const* shaderName, VertexType type)
{
	std::string defaultString = "Default";
	if (shaderName == defaultString.c_str())
	{
		return m_defaultShader;
	}

	std::string fileName = Stringf("%s.hlsl", shaderName);

	std::string shaderSource;
	FileReadToString(shaderSource, fileName);

	return CreateShader(fileName.c_str(), shaderSource.c_str(), type);
}

VertexBuffer* Renderer::CreateVertexBuffer(unsigned int size)
{
	VertexBuffer* vbo = new VertexBuffer(size);

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, &vbo->m_buffer);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create vertex buffer."));
	}

	return vbo;
}

IndexBuffer* Renderer::CreateIndexBuffer(size_t size)
{
	IndexBuffer* ibo = new IndexBuffer(size);

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, &ibo->m_buffer);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create index buffer."));
	}

	return ibo;
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, VertexBuffer*& vbo)
{
	if (vbo->m_size < size)
	{
		VertexBuffer* tempVbo = vbo;
		vbo = CreateVertexBuffer(size);
		delete tempVbo;
	}
	//Copy vertices
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(vbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(vbo->m_buffer, 0);
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, IndexBuffer*& ibo)
{
	if (ibo->m_size < size)
	{
		IndexBuffer* tempIbo = ibo;
		ibo = CreateIndexBuffer(size);
		delete tempIbo;
	}
	//Copy vertices
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(ibo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(ibo->m_buffer, 0);
}

void Renderer::BindVertexBuffer(VertexBuffer* vbo, VertexType type)
{
	m_deviceContext->IASetPrimitiveTopology( vbo->m_isLinePrimitive 
		? D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	    : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (type == VertexType::Vertex_PCU)
	{
		UINT stride = sizeof(Vertex_PCU);
		UINT startOffset = 0;
		m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &startOffset);
	}
	else if (type == VertexType::Vertex_PCUTBN)
	{
		UINT stride = sizeof(Vertex_PCUTBN);
		UINT startOffset = 0;
		m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &startOffset);
	}

}

void Renderer::BindIndexBuffer(IndexBuffer* ibo)
{
	m_deviceContext->IASetIndexBuffer(ibo->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

ConstantBuffer* Renderer::CreateConstantBuffer(const size_t size)
{
	ConstantBuffer* cbo = new ConstantBuffer(size);

	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, &cbo->m_buffer);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create constant buffer."));
	}

	return cbo;
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, ConstantBuffer* cbo)
{
	//Copy vertices
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(cbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(cbo->m_buffer, 0);
}

void Renderer::BindConstantBuffer(int slot, ConstantBuffer* cbo)
{
	m_deviceContext->VSSetConstantBuffers(slot, 1, &cbo->m_buffer);
	m_deviceContext->PSSetConstantBuffers(slot, 1, &cbo->m_buffer);
}

void Renderer::SetStatesIfChanged()
{
	if (m_blendState != m_blendStates[(int)m_desiredBlendMode])
	{
		m_blendState = m_blendStates[(int)m_desiredBlendMode];
		float blendFactor[4] = { 0.0f, 0.f, 0.0f, 0.f };
		UINT sampleMask = 0xffffffff;
		m_deviceContext->OMSetBlendState(m_blendState, blendFactor, sampleMask);
	}
	else
	{
		m_blendState = m_blendStates[(int)m_desiredBlendMode];
	}

	if (m_samplerState != m_samplerStates[(int)m_desiredSamplerMode])
	{
		m_samplerState = m_samplerStates[(int)m_desiredSamplerMode];
		m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	}

	if (m_rasterizerState != m_rasterizerStates[(int)m_desiredRasterizerMode])
	{
		m_rasterizerState = m_rasterizerStates[(int)m_desiredRasterizerMode];
		m_deviceContext->RSSetState(m_rasterizerState);
	}
	if (m_depthState != m_depthStates[(int)m_desiredDepthMode])
	{
		m_depthState = m_depthStates[(int)m_desiredDepthMode];
		m_deviceContext->OMSetDepthStencilState(m_depthState, 0);
	}
}

//------------------------------------------------------------------------------------------------
Texture* Renderer::CreateTextureFromFile(char const* imageFilePath)
{
	Image* image = new Image(imageFilePath);
	Texture* newTexture = CreateTextureFromImage(*image);
	m_loadedTextures.push_back(newTexture);
	return newTexture;
}


//------------------------------------------------------------------------------------------------
Texture* Renderer::CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData)
{
	Image imageData = Image(name);
	imageData.m_dimensions = dimensions;
	// OLD FUNCTION WHICH IS NO LONGER USED, REFACTOR IF NEEDED
	UNUSED(bytesPerTexel);
	UNUSED(texelData);
	return CreateTextureFromImage(imageData);
}


//-----------------------------------------------------------------------------------------------
void Renderer::BindTexture(const Texture* texture, unsigned int slot)
{
	if (texture != nullptr)
	{
		m_currentTexture = texture;
	}
	else
	{
		m_currentTexture = m_defaultTexture;
	}
	m_deviceContext->PSSetShaderResources(slot, 1, &m_currentTexture->m_shaderResourceView);
}

void Renderer::SetBlendMode(BlendMode blendMode)
{
	m_desiredBlendMode = blendMode;
}

void Renderer::SetSamplerMode(SampleMode samplerMode)
{
	m_desiredSamplerMode = samplerMode;
}

void Renderer::SetRasterizerMode(RasterizerMode rasterizerrMode)
{
	m_desiredRasterizerMode = rasterizerrMode;
}

void Renderer::SetDepthStencilMode(DepthMode depthMode)
{
	m_desiredDepthMode = depthMode;
}

void Renderer::SetModelConstants(const Mat44& modelMatrix, const Rgba8& modelColor)
{
	ModelConstants localModelConstants;
	localModelConstants.ModelMatrix = modelMatrix;
	modelColor.GetAsFloats(localModelConstants.ModelColor);
	CopyCPUToGPU(&localModelConstants, sizeof(ModelConstants), m_modelCBO);
	BindConstantBuffer(k_modelConstantsSlot, m_modelCBO);
}

void Renderer::SetLightConstant(const LightConstants& lightconstant /*= LightConstants()*/)
{
	CopyCPUToGPU(&lightconstant, sizeof(LightConstants), m_lightCBO);
	BindConstantBuffer(k_lightConstantsSlot, m_lightCBO);
}

void Renderer::SetLightConstants(LightConstants lightConstant /*= LightConstants()*/)
{
	CopyCPUToGPU(&lightConstant, sizeof(LightConstants), m_lightCBO);
	BindConstantBuffer(k_lightConstantsSlot, m_lightCBO);
}

void Renderer::SetLightConstants(const Vec3& sunDirection /*= Vec3(2, 1, -1)*/, const float sunIntensity /*= 0.85*/, const float ambientIntensity /*= 0.35*/, Vec3 wordEyePosition /*= Vec3()*/, float minFallOff /*= 0.f*/, float maxFallOff /*= 1.f*/, float minFallOffMultiplier /*= 1.f*/, float maxFallOffMultiplier /*= 1.f*/, LightingDebug lightDebug)
{
	LightConstants LightConstants;
	LightConstants.SunDirection = sunDirection;
	LightConstants.SunDirection.Normalize();
	LightConstants.SunIntensity = Clamp(sunIntensity, 0.f, 1.f);
	LightConstants.AmbientIntensity = Clamp(ambientIntensity, 0.f, 1.f);
	LightConstants.WordEyePosition = wordEyePosition;
	LightConstants.MinFallOff = minFallOff;
	LightConstants.MaxFallOff = maxFallOff;
	LightConstants.MinFallOffMultiplier = minFallOffMultiplier;
	LightConstants.MaxFallOffMultiplier = maxFallOffMultiplier;
	LightConstants.RenderAmbient = lightDebug.RenderAmbient;
	LightConstants.RenderDiffuse = lightDebug.RenderDiffuse;
	LightConstants.RenderSpecular = lightDebug.RenderSpecular;
	LightConstants.RenderEmissive = lightDebug.RenderEmissive;
	LightConstants.UseDiffuseMap = lightDebug.UseDiffuseMap;
	LightConstants.UseSpecularMap = lightDebug.UseSpecularMap;
	LightConstants.UseGlossinessMap = lightDebug.UseGlossinessMap;
	LightConstants.UseNormalMap = lightDebug.UseNormalMap;

	CopyCPUToGPU(&LightConstants, sizeof(LightConstants), m_lightCBO);
	BindConstantBuffer(k_lightConstantsSlot, m_lightCBO);
}

void Renderer::SetBlurConstantBuffer(BlurConstants blurConstatnt /*= BlurConstants()*/)
{
	CopyCPUToGPU(&blurConstatnt, sizeof(BlurConstants), m_blurCBO);
	BindConstantBuffer(k_blurConstantsSlot, m_blurCBO);
}

Texture* Renderer::GetTextureForFileName(char const* imageFilePath)
{
	for (int i = 0; i < (int)m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i]->GetImageFilePath() == imageFilePath)
		{
			return m_loadedTextures[i];
		}
	}
	return nullptr;
}

BitmapFont* Renderer::CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension)
{
	BitmapFont* bitmapFont = GetBitmapFontForFileName(bitmapFontFilePathWithNoExtension);
	if (bitmapFont)
	{
		return bitmapFont;
	}
	std::string bitmapFontFilePath = bitmapFontFilePathWithNoExtension;
	bitmapFontFilePath += ".png";
	Texture* existingTexture = CreateOrGetTextureFromFile(bitmapFontFilePath.c_str());
	BitmapFont* newBitmapFont = new BitmapFont(bitmapFontFilePathWithNoExtension, *existingTexture);
	m_loadedFonts.push_back(newBitmapFont);
	return newBitmapFont;
}
BitmapFont* Renderer::GetBitmapFontForFileName(const char* bitmapFontFilePathWithNoExtension)
{
	for (int i = 0; i < (int)m_loadedFonts.size(); i++)
	{
		if (m_loadedFonts[i]->m_fontFilePathNameWithNoExtension == bitmapFontFilePathWithNoExtension)
		{
			return m_loadedFonts[i];
		}
	}
	return nullptr;
}
