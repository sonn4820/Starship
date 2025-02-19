#include "Engine/Renderer/DebugRender.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Camera.hpp"

struct DebugWorldData
{
	std::vector<Vertex_PCU> m_verts;
	Mat44 m_transform = Mat44();
	Rgba8 m_startColor = Rgba8::COLOR_WHITE;
	Rgba8 m_endColor = Rgba8::COLOR_WHITE;
	DebugRenderMode m_mode = DebugRenderMode::USE_DEPTH;
	RasterizerMode m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	const Texture* m_texture = nullptr;
	Shader* m_shader = nullptr;
	Timer* m_timer = nullptr;
	bool m_isBillboardText = false;
};

struct DebugScreenData
{
	std::vector<Vertex_PCU> m_verts;
	int m_numStaticIndex = -1;
	Rgba8 m_startColor = Rgba8::COLOR_WHITE;
	Rgba8 m_endColor = Rgba8::COLOR_WHITE;
	Timer* m_timer = nullptr;
	Shader* m_shader = nullptr;
};

class DebugRender
{
public:
	DebugRender() = default;
	~DebugRender();
	Renderer* m_renderer = nullptr;
	std::string m_fontName;
	bool m_isHidden = false;

	Clock* m_clock = nullptr;
	std::vector<DebugWorldData> m_debugWorldDataList;
	std::vector<DebugScreenData> m_debugScreenDataList;
	std::vector<DebugScreenData> m_debugMessagesDataList;
	BitmapFont* m_debugFont = nullptr;
	Mat44 m_cameraTransform;
	int m_numStaticMessage = 0;
	mutable std::mutex m_debugRenderMutex;
};

DebugRender::~DebugRender()
{
	m_renderer = nullptr;
	m_clock = nullptr;
	m_debugFont = nullptr;
}

DebugRender* g_theDebugRender;

void DebugRenderSystemStartUp(const DebugRenderConfig& config)
{
	g_theDebugRender = new DebugRender();
	g_theDebugRender->m_renderer = config.m_renderer;
	g_theDebugRender->m_fontName = config.m_fontName;
	g_theDebugRender->m_clock = new Clock(*Clock::s_theSystemClock);
	g_theDebugRender->m_debugFont = g_theDebugRender->m_renderer->CreateOrGetBitmapFont("Data/Fonts/SquirrelFixedFont");
	g_theEventSystem->SubscribeEventCallbackFunction("debugclear", Command_DebugRenderClear);
	g_theEventSystem->SubscribeEventCallbackFunction("debugtoggle", Command_DebugRenderToggle);
}

void DebugRenderSystemShutdown()
{
	delete g_theDebugRender;
	g_theDebugRender = nullptr;
}

void DebugRenderSetVisible()
{
	g_theDebugRender->m_debugRenderMutex.lock();
	g_theDebugRender->m_isHidden = false;
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugRenderSetHidden()
{
	g_theDebugRender->m_debugRenderMutex.lock();
	g_theDebugRender->m_isHidden = true;
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugRenderClear()
{
	g_theDebugRender->m_debugRenderMutex.lock();
	g_theDebugRender->m_debugWorldDataList.clear();
	g_theDebugRender->m_debugScreenDataList.clear();
	g_theDebugRender->m_debugMessagesDataList.clear();
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugRenderBeginFrame()
{
	g_theDebugRender->m_numStaticMessage = 0;
}

void DebugRenderWorld(const Camera& camera)
{
	//g_theDebugRender->m_debugRenderMutex.lock();
	if (g_theDebugRender->m_isHidden)
	{
		return;
	}
	g_theDebugRender->m_renderer->BeginCamera(camera);
	g_theDebugRender->m_cameraTransform = camera.GetModelMatrix();

	for (int i = 0; i < (int)g_theDebugRender->m_debugWorldDataList.size(); i++)
	{
		std::vector<Vertex_PCU> vertsToDraw;
		Rgba8 color;

		vertsToDraw = g_theDebugRender->m_debugWorldDataList[i].m_verts;

		if (g_theDebugRender->m_debugWorldDataList[i].m_timer == nullptr
			|| g_theDebugRender->m_debugWorldDataList[i].m_timer->m_period == 0)
		{
			color = g_theDebugRender->m_debugWorldDataList[i].m_startColor;
		}
		else
		{
			color = Interpolate(g_theDebugRender->m_debugWorldDataList[i].m_startColor,
				g_theDebugRender->m_debugWorldDataList[i].m_endColor,
				g_theDebugRender->m_debugWorldDataList[i].m_timer->GetElapsedFraction());
		}


		if (g_theDebugRender->m_debugWorldDataList[i].m_mode != DebugRenderMode::XRAY)
		{
			if (g_theDebugRender->m_debugWorldDataList[i].m_mode == DebugRenderMode::ALWAYS)
			{
				g_theDebugRender->m_renderer->SetDepthStencilMode(DepthMode::DISABLED);
			}
			else if (g_theDebugRender->m_debugWorldDataList[i].m_mode == DebugRenderMode::USE_DEPTH)
			{
				g_theDebugRender->m_renderer->SetDepthStencilMode(DepthMode::ENABLED);
			}
			g_theDebugRender->m_renderer->SetBlendMode(BlendMode::ALPHA);
			g_theDebugRender->m_renderer->SetSamplerMode(SampleMode::POINT_CLAMP);
			g_theDebugRender->m_renderer->SetRasterizerMode(g_theDebugRender->m_debugWorldDataList[i].m_rasterizerMode);
			g_theDebugRender->m_renderer->SetModelConstants(g_theDebugRender->m_debugWorldDataList[i].m_transform, color);
			g_theDebugRender->m_renderer->BindTexture(g_theDebugRender->m_debugWorldDataList[i].m_texture);
			g_theDebugRender->m_renderer->BindShader(g_theDebugRender->m_debugWorldDataList[i].m_shader);
			g_theDebugRender->m_renderer->DrawVertexArray((int)vertsToDraw.size(), vertsToDraw.data());
		}

	}
	for (int i = 0; i < (int)g_theDebugRender->m_debugWorldDataList.size(); i++)
	{
		if (g_theDebugRender->m_debugWorldDataList[i].m_mode == DebugRenderMode::XRAY)
		{
			std::vector<Vertex_PCU> vertsToDraw;
			vertsToDraw = g_theDebugRender->m_debugWorldDataList[i].m_verts;
			Rgba8 color;

			if (g_theDebugRender->m_debugWorldDataList[i].m_timer == nullptr
				|| g_theDebugRender->m_debugWorldDataList[i].m_timer->m_period == 0)
			{
				color = g_theDebugRender->m_debugWorldDataList[i].m_startColor;
			}
			else
			{
				color = Interpolate(g_theDebugRender->m_debugWorldDataList[i].m_startColor,
					g_theDebugRender->m_debugWorldDataList[i].m_endColor,
					g_theDebugRender->m_debugWorldDataList[i].m_timer->GetElapsedFraction());
			}
			g_theDebugRender->m_renderer->SetRasterizerMode(g_theDebugRender->m_debugWorldDataList[i].m_rasterizerMode);
			g_theDebugRender->m_renderer->SetSamplerMode(SampleMode::POINT_CLAMP);
			// FIRST PASS
			g_theDebugRender->m_renderer->SetBlendMode(BlendMode::ALPHA);
			g_theDebugRender->m_renderer->SetDepthStencilMode(DepthMode::DISABLED);
			Rgba8 firstPassColor = color;
			firstPassColor.r = (firstPassColor.r + 40 > 255) ? 255 : firstPassColor.r + 40;
			firstPassColor.b = (firstPassColor.b + 40 > 255) ? 255 : firstPassColor.b + 40;
			firstPassColor.g = (firstPassColor.g + 40 > 255) ? 255 : firstPassColor.g + 40;
			firstPassColor.a = 120;
			g_theDebugRender->m_renderer->SetModelConstants(g_theDebugRender->m_debugWorldDataList[i].m_transform, firstPassColor);
			g_theDebugRender->m_renderer->BindTexture(g_theDebugRender->m_debugWorldDataList[i].m_texture);
			g_theDebugRender->m_renderer->BindShader(g_theDebugRender->m_debugWorldDataList[i].m_shader);
			g_theDebugRender->m_renderer->DrawVertexArray((int)vertsToDraw.size(), vertsToDraw.data());
			// SECOND PASS
			g_theDebugRender->m_renderer->SetBlendMode(BlendMode::OPAQUE);
			g_theDebugRender->m_renderer->SetDepthStencilMode(DepthMode::ENABLED);
			g_theDebugRender->m_renderer->SetModelConstants(g_theDebugRender->m_debugWorldDataList[i].m_transform, color);
			g_theDebugRender->m_renderer->BindTexture(g_theDebugRender->m_debugWorldDataList[i].m_texture);
			g_theDebugRender->m_renderer->BindShader(g_theDebugRender->m_debugWorldDataList[i].m_shader);
			g_theDebugRender->m_renderer->DrawVertexArray((int)vertsToDraw.size(), vertsToDraw.data());
		}
	}
	g_theDebugRender->m_renderer->EndCamera(camera);
	//g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugRenderScreen(const Camera& camera)
{
	//g_theDebugRender->m_debugRenderMutex.lock();
	if (g_theDebugRender->m_isHidden)
	{
		g_theDebugRender->m_debugRenderMutex.unlock();
		return;
	}
	g_theDebugRender->m_renderer->BeginCamera(camera);
	g_theDebugRender->m_renderer->SetDepthStencilMode(DepthMode::DISABLED);
	g_theDebugRender->m_renderer->SetBlendMode(BlendMode::ALPHA);
	g_theDebugRender->m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	g_theDebugRender->m_renderer->SetSamplerMode(SampleMode::POINT_CLAMP);

	for (int i = 0; i < (int)g_theDebugRender->m_debugScreenDataList.size(); i++)
	{
		std::vector<Vertex_PCU> vertsToDraw;
		Rgba8 color;

		vertsToDraw = g_theDebugRender->m_debugScreenDataList[i].m_verts;

		if (g_theDebugRender->m_debugScreenDataList[i].m_timer == nullptr
			|| g_theDebugRender->m_debugScreenDataList[i].m_timer->m_period == 0)
		{
			color = g_theDebugRender->m_debugScreenDataList[i].m_startColor;
		}
		else
		{
			color = Interpolate(g_theDebugRender->m_debugScreenDataList[i].m_startColor,
				g_theDebugRender->m_debugScreenDataList[i].m_endColor,
				g_theDebugRender->m_debugScreenDataList[i].m_timer->GetElapsedFraction());
		}

		g_theDebugRender->m_renderer->SetModelConstants(Mat44(), color);
		g_theDebugRender->m_renderer->BindTexture(&g_theDebugRender->m_debugFont->GetTexture());
		g_theDebugRender->m_renderer->BindShader(g_theDebugRender->m_debugScreenDataList[i].m_shader);
		g_theDebugRender->m_renderer->DrawVertexArray((int)vertsToDraw.size(), vertsToDraw.data());
	}

	int clientTopY = Window::GetMainWindowInstance()->GetClientDimensions().y;
	for (int i = 0; i < (int)g_theDebugRender->m_debugMessagesDataList.size(); i++)
	{
		std::vector<Vertex_PCU> messageToDraw;
		Rgba8 color;
		Mat44 transform;

		messageToDraw = g_theDebugRender->m_debugMessagesDataList[i].m_verts;

		if (g_theDebugRender->m_debugMessagesDataList[i].m_timer == nullptr
			|| g_theDebugRender->m_debugMessagesDataList[i].m_timer->m_period == 0)
		{
			color = g_theDebugRender->m_debugMessagesDataList[i].m_startColor;
			transform.SetTranslation2D(Vec2(10.f, (float)clientTopY - 30.f - (g_theDebugRender->m_debugMessagesDataList[i].m_numStaticIndex * 22.5f)));
		}
		else
		{
			color = Interpolate(g_theDebugRender->m_debugMessagesDataList[i].m_startColor,
				g_theDebugRender->m_debugMessagesDataList[i].m_endColor,
				g_theDebugRender->m_debugMessagesDataList[i].m_timer->GetElapsedFraction());
			transform.SetTranslation2D(Vec2(10.f, (float)clientTopY - 30.f - (i * 22.5f) - (g_theDebugRender->m_numStaticMessage * 22.5f)));
		}

		g_theDebugRender->m_renderer->SetModelConstants(transform, color);
		g_theDebugRender->m_renderer->BindTexture(&g_theDebugRender->m_debugFont->GetTexture());
		g_theDebugRender->m_renderer->BindShader(g_theDebugRender->m_debugMessagesDataList[i].m_shader);
		g_theDebugRender->m_renderer->DrawVertexArray((int)messageToDraw.size(), messageToDraw.data());
	}
	g_theDebugRender->m_renderer->EndCamera(camera);
	//g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugRenderEndFrame()
{

	for (int i = 0; i < (int)g_theDebugRender->m_debugWorldDataList.size(); i++)
	{
		if (g_theDebugRender->m_debugWorldDataList[i].m_isBillboardText)
		{
			g_theDebugRender->m_debugWorldDataList[i].m_transform = GetBillboardMatrix(BilboardType::FULL_CAMERA_OPPOSING, g_theDebugRender->m_cameraTransform, g_theDebugRender->m_debugWorldDataList[i].m_transform.GetTranslation3D());
		}
		if (g_theDebugRender->m_debugWorldDataList[i].m_timer != nullptr && g_theDebugRender->m_debugWorldDataList[i].m_timer->HasPeriodElapsed())
		{
			g_theDebugRender->m_debugWorldDataList.erase(g_theDebugRender->m_debugWorldDataList.begin() + i);
			i = -1;
		}
	}
	for (int i = 0; i < (int)g_theDebugRender->m_debugScreenDataList.size(); i++)
	{
		if (g_theDebugRender->m_debugScreenDataList[i].m_timer != nullptr && g_theDebugRender->m_debugScreenDataList[i].m_timer->HasPeriodElapsed())
		{
			g_theDebugRender->m_debugScreenDataList.erase(g_theDebugRender->m_debugScreenDataList.begin() + i);
			i = -1;
		}
	}
	for (int i = 0; i < (int)g_theDebugRender->m_debugMessagesDataList.size(); i++)
	{
		if (g_theDebugRender->m_debugMessagesDataList[i].m_timer != nullptr && g_theDebugRender->m_debugMessagesDataList[i].m_timer->HasPeriodElapsed())
		{
			g_theDebugRender->m_debugMessagesDataList.erase(g_theDebugRender->m_debugMessagesDataList.begin() + i);
			i = -1;
		}
	}
}

void DebugAddWorldPoint(const Vec3& pos, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	data.m_mode = mode;

	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}

	AddVertsForSphere(data.m_verts, pos, radius, Rgba8::COLOR_WHITE);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddWorldBasis(const Mat44& transform, float duration, DebugRenderMode mode)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_mode = mode;
	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}

	Vec3 pos = transform.GetTranslation3D();
	Vec3 forward = transform.GetIBasis3D();
	Vec3 left = transform.GetJBasis3D();
	Vec3 up = transform.GetKBasis3D();

	Vec3 connectorForward = pos + 0.7f * forward;
	AddVertsForCone3D(data.m_verts, connectorForward, pos + forward, 0.2f, Rgba8::COLOR_RED);
	AddVertsForCylinder3D(data.m_verts, pos, connectorForward, 0.12f, Rgba8::COLOR_RED);

	Vec3 connectorLeft = pos + 0.7f * left;
	AddVertsForCone3D(data.m_verts, connectorLeft, pos + left, 0.2f, Rgba8::COLOR_GREEN);
	AddVertsForCylinder3D(data.m_verts, pos, connectorLeft, 0.12f, Rgba8::COLOR_GREEN);

	Vec3 connectorUp = pos + 0.7f * up;
	AddVertsForCone3D(data.m_verts, connectorUp, pos + up, 0.2f, Rgba8::COLOR_BLUE);
	AddVertsForCylinder3D(data.m_verts, pos, connectorUp, 0.12f, Rgba8::COLOR_BLUE);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddWorldLine(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	data.m_mode = mode;

	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}

	AddVertsForCylinder3D(data.m_verts, start, end, radius, Rgba8::COLOR_WHITE);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddWorldWireCylinder(const Vec3& base, const Vec3& top, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	data.m_mode = mode;

	data.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;

	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}

	AddVertsForCylinder3D(data.m_verts, base, top, radius, Rgba8::COLOR_WHITE);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddWorldWireSphere(const Vec3& center, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	data.m_mode = mode;

	data.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;

	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}

	AddVertsForSphere(data.m_verts, center, radius, Rgba8::COLOR_WHITE);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddWorldArrow(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	data.m_mode = mode;

	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}

	AddVertsForArrow3D(data.m_verts, start, end, radius, Rgba8::COLOR_WHITE);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddWorldWireArrow(const Vec3& start, const Vec3& end, float radius /*= 0.025f*/, float duration /*= 0.f*/, const Rgba8& startColor /*= Rgba8::COLOR_WHITE*/, const Rgba8& endColor /*= Rgba8::COLOR_WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	data.m_mode = mode;

	data.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;

	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}

	AddVertsForArrow3D(data.m_verts, start, end, radius, Rgba8::COLOR_WHITE);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddWorldText(const std::string& text, const Mat44& transform, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	data.m_mode = mode;
	data.m_transform = transform;
	data.m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	data.m_texture = &g_theDebugRender->m_debugFont->GetTexture();
	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}
	g_theDebugRender->m_debugFont->AddVertsForText3DAtOriginXForward(data.m_verts, textHeight, text, Rgba8::COLOR_WHITE, 1.f, alignment);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddWorldBillboardText(const std::string& text, const Vec3& origin, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugWorldData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	data.m_mode = mode;
	data.m_isBillboardText = true;
	data.m_transform = GetBillboardMatrix(BilboardType::FULL_CAMERA_OPPOSING, g_theDebugRender->m_cameraTransform, origin);
	data.m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	data.m_texture = &g_theDebugRender->m_debugFont->GetTexture();
	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}
	g_theDebugRender->m_debugFont->AddVertsForText3DAtOriginXForward(data.m_verts, textHeight, text, Rgba8::COLOR_WHITE, 1.f, alignment);

	g_theDebugRender->m_debugWorldDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}


void DebugAddScreenText(const std::string& text, const Vec2& position, float size, const Rgba8& startColor, const Rgba8& endColor, float duration, const Vec2& alignment)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugScreenData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}
	float width = g_theDebugRender->m_debugFont->GetTextWidth(size, text);
	AABB2 box(position, Vec2(position.x + width, position.y + size));

	g_theDebugRender->m_debugFont->AddVertsForTextInBox2D(data.m_verts, box, size, text, Rgba8::COLOR_WHITE, 1.f, alignment);

	g_theDebugRender->m_debugScreenDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

void DebugAddMessage(const std::string& text, float duration, const Rgba8& startColor, const Rgba8& endColor)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	DebugScreenData data;
	data.m_startColor = startColor;
	data.m_endColor = endColor;
	if (duration != -1)
	{
		data.m_timer = new Timer(duration, g_theDebugRender->m_clock);
		data.m_timer->Start();
	}
	if (duration == -1 || duration == 0)
	{
		g_theDebugRender->m_numStaticMessage++;
		data.m_numStaticIndex = g_theDebugRender->m_numStaticMessage - 1;
	}
	g_theDebugRender->m_debugFont->AddVertsForText2D(data.m_verts, Vec2(0.f, 0.f), 15.f, text);

	g_theDebugRender->m_debugMessagesDataList.push_back(data);
	g_theDebugRender->m_debugRenderMutex.unlock();
}

bool Command_DebugRenderClear(EventArgs& args)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	UNUSED(args);
	DebugRenderClear();
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "Clear Debug Render");
	g_theDebugRender->m_debugRenderMutex.unlock();
	return true;
}

bool Command_DebugRenderToggle(EventArgs& args)
{
	g_theDebugRender->m_debugRenderMutex.lock();
	UNUSED(args);
	g_theDebugRender->m_isHidden = !g_theDebugRender->m_isHidden;
	if (g_theDebugRender->m_isHidden)
	{
		g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "Debug Render is off");
	}
	else
	{
		g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "Debug Render is on");
	}
	g_theDebugRender->m_debugRenderMutex.unlock();
	return true;
}
