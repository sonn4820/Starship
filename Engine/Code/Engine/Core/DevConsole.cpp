#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Network/NetworkSystem.hpp"

#ifdef ERROR
#undef ERROR
#endif // ERROR

#include <algorithm>
#include <cctype>
#include <string>


DevConsole* g_theDevConsole = nullptr;

const Rgba8 DevConsole::ERROR = Rgba8(255, 70, 70);
const Rgba8 DevConsole::WARNING = Rgba8(255, 255, 0);
const Rgba8 DevConsole::SUCCESS = Rgba8(0, 255, 0);
const Rgba8 DevConsole::INFO_MAJOR = Rgba8(0, 255, 255);
const Rgba8 DevConsole::INFO_MINOR = Rgba8(255, 255, 255);
const Rgba8 DevConsole::INPUT_TEXT = Rgba8(0, 255, 0);
const Rgba8 DevConsole::INPUT_INSERTION_POINT = Rgba8(200, 255, 180);
const Rgba8 DevConsole::COMMAND_ECHO = Rgba8(255, 0, 255);
const Rgba8 DevConsole::COMMAND_REMOTE_ECHO = Rgba8(0, 0, 0);


bool DevConsole::Event_KeyPressed(EventArgs& args)
{
	if (!g_theDevConsole->IsOpen())
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	if (keyCode == KEYCODE_TIDLE)
	{
		g_theDevConsole->ToggleOpen();
		return false;
	}
	if (keyCode == KEYCODE_ENTER)
	{
		if (g_theDevConsole->m_inputText.size() <= 0)
		{
			g_theDevConsole->ToggleOpen();
			return false;
		}

		g_theDevConsole->Execute(g_theDevConsole->m_inputText, true);
		g_theDevConsole->m_commandHistory.push_back(g_theDevConsole->m_inputText);
		g_theDevConsole->m_insertionPointPosition = 0;
		g_theDevConsole->m_historyIndex = (int)g_theDevConsole->m_commandHistory.size();
		g_theDevConsole->m_inputText.clear();
	}
	if (keyCode == KEYCODE_LEFTARROW)
	{
		if (g_theDevConsole->m_insertionPointPosition <= 0)
		{
			return false;
		}
		g_theDevConsole->m_insertionPointPosition--;
	}
	if (keyCode == KEYCODE_RIGHTARROW)
	{
		if (g_theDevConsole->m_insertionPointPosition >= (int)g_theDevConsole->m_inputText.size())
		{
			return false;
		}
		g_theDevConsole->m_insertionPointPosition++;
	}
	if (keyCode == KEYCODE_UPARROW)
	{
		if ((int)g_theDevConsole->m_commandHistory.size() <= 0)
		{
			return false;
		}
		if (g_theDevConsole->m_historyIndex == 0)
		{
			return false;
		}
		g_theDevConsole->m_historyIndex--;
		g_theDevConsole->m_inputText = g_theDevConsole->m_commandHistory[g_theDevConsole->m_historyIndex];
		g_theDevConsole->m_insertionPointPosition = (int)g_theDevConsole->m_inputText.size();
	}
	if (keyCode == KEYCODE_DOWNARROW)
	{
		if ((int)g_theDevConsole->m_commandHistory.size() <= 0)
		{
			return false;
		}
		if (g_theDevConsole->m_historyIndex == (int)g_theDevConsole->m_commandHistory.size() - 1)
		{
			return false;
		}
		g_theDevConsole->m_historyIndex++;
		g_theDevConsole->m_inputText = g_theDevConsole->m_commandHistory[g_theDevConsole->m_historyIndex];
		g_theDevConsole->m_insertionPointPosition = (int)g_theDevConsole->m_inputText.size();

	}
	if (keyCode == KEYCODE_HOME)
	{
		g_theDevConsole->m_insertionPointPosition = 0;
	}
	if (keyCode == KEYCODE_END)
	{
		g_theDevConsole->m_insertionPointPosition = (int)g_theDevConsole->m_inputText.size();
	}
	if (keyCode == KEYCODE_DELETE)
	{
		if (g_theDevConsole->m_insertionPointPosition == (int)g_theDevConsole->m_inputText.size())
		{
			return false;
		}
		g_theDevConsole->m_inputText.erase(g_theDevConsole->m_inputText.begin() + g_theDevConsole->m_insertionPointPosition);
	}
	if (keyCode == KEYCODE_BACKSPACE)
	{
		if (g_theDevConsole->m_inputText.size() <= 0)
		{
			return false;
		}
		if (g_theDevConsole->m_insertionPointPosition != 0)
		{
			g_theDevConsole->m_inputText.erase(g_theDevConsole->m_inputText.begin() + g_theDevConsole->m_insertionPointPosition - 1);
			g_theDevConsole->m_insertionPointPosition--;
		}
	}
	if (keyCode == KEYCODE_ESCAPE)
	{
		if (g_theDevConsole->m_inputText.size() <= 0)
		{
			FireEvent("clear");
			g_theDevConsole->ToggleOpen();
			return false;
		}
		g_theDevConsole->m_inputText.clear();
		g_theDevConsole->m_insertionPointPosition = 0;
	}
	return true;
}

bool DevConsole::Event_CharInput(EventArgs& args)
{
	if (!g_theInput || !g_theDevConsole->IsOpen())
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	if (keyCode < 32 || keyCode > 126)
	{
		return false;
	}
	g_theDevConsole->m_inputText.insert(g_theDevConsole->m_inputText.begin() + g_theDevConsole->m_insertionPointPosition, keyCode);
	g_theDevConsole->m_insertionPointPosition++;
	return true;
}

bool DevConsole::Command_Clear(EventArgs& args)
{
	UNUSED(args);
	g_theDevConsole->m_lines.clear();
	g_theDevConsole->m_scrollOffset = 0;
	return true;
}

bool DevConsole::Command_Echo(EventArgs& args)
{
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, args.GetValue("message", ""));

	return true;
}

bool DevConsole::Command_Help(EventArgs& args)
{
	UNUSED(args);
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "LIST OF REGISTERED EVENTS");
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "_____________________________");
	std::vector<std::string> eventList = g_theEventSystem->GetAllRegisteredEvent();
	for (size_t i = 0; i < eventList.size(); i++)
	{
		g_theDevConsole->AddLine(DevConsole::INFO_MINOR, std::to_string(i + 1) + ". " + eventList[i]);
	}
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "_____________________________");
	return true;
}

bool DevConsole::Command_SetTimeScale(EventArgs& args)
{
	if (!args.IsKeyNameValid("s"))
	{
		g_theDevConsole->AddLine(DevConsole::ERROR, "ERROR: Arguments are missing or incorrect, please use timescale s=<value>");
	}
	else 
	{
		float result = args.GetValue("s", 1.0f);
		Clock::s_theSystemClock->SetTimeScale(result);
		g_theDevConsole->AddLine(DevConsole::COMMAND_ECHO, "Time Scale is set to: " + std::to_string(result));
	}
	
	return true;
}

bool DevConsole::Command_Print(EventArgs& args)
{
	if (!args.IsKeyNameValid("p"))
	{
		g_theDevConsole->AddLine(DevConsole::ERROR, "ERROR: Arguments are missing or incorrect, please use p=<string>");
	}
	else
	{
		std::string result = args.GetValue("p", "");
		g_theDevConsole->AddLine(DevConsole::INFO_MINOR, result);
	}
	return true;
}

void DevConsole::Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect) const
{
	renderer.SetModelConstants();
	renderer.SetBlendMode(BlendMode::ALPHA);
	renderer.SetDepthStencilMode(DepthMode::DISABLED);

	std::vector<Vertex_PCU> consoleBGVerts;
	consoleBGVerts.reserve(6);
	AddVertsForAABB2D(consoleBGVerts, bounds, Rgba8::DARK);
	renderer.BindTexture(nullptr);
	renderer.DrawVertexArray((int)consoleBGVerts.size(), consoleBGVerts.data());

	float textHeight = bounds.m_maxs.y / m_config.m_numLinesVisible;
	float shadowOffset = 2.2f * fontAspect;

	std::vector<Vertex_PCU> consoleTextVerts;
	consoleTextVerts.reserve(300);
	int totalLines = static_cast<int>(m_lines.size());
	int startLineIndex = IntMax(0, totalLines - (int)m_config.m_numLinesVisible - m_scrollOffset);
	int endLineIndex = IntMin(totalLines - 1, startLineIndex + (int)m_config.m_numLinesVisible - 1);

	for (int i = endLineIndex; i >= startLineIndex; i--)
	{
		int lineFromBottom = endLineIndex - i;
		Vec2 textBoundMins = Vec2(bounds.m_mins.x, bounds.m_mins.y + textHeight + textHeight * lineFromBottom);
		Vec2 textBoundMaxs = Vec2(bounds.m_maxs.x, textBoundMins.y + textHeight);
		AABB2 textBound = AABB2(textBoundMins, textBoundMaxs);

		std::string printLine;
		if (m_showFrameAndTime)
		{
			printLine = Stringf("(Frame: %i, Timestamp: %f) %s", m_lines[i].m_frameNumber, m_lines[i].m_timestamp, m_lines[i].m_text.c_str());
		}
		else
		{
			printLine = m_lines[i].m_text;
		}

		font.AddVertsForTextInBox2D(consoleTextVerts, AABB2(textBound.m_mins.x + shadowOffset, textBound.m_mins.y, textBound.m_maxs.x + shadowOffset, textBound.m_maxs.y), textHeight, printLine, Rgba8(0, 0, 0, 200), fontAspect, Vec2(0.f, 0.5f));
		font.AddVertsForTextInBox2D(consoleTextVerts, textBound, textHeight, printLine, m_lines[i].m_color, fontAspect, Vec2(0.f, 0.5f));
	}
	renderer.BindTexture(&font.GetTexture());
	renderer.DrawVertexArray((int)consoleTextVerts.size(), consoleTextVerts.data());

	std::vector<Vertex_PCU> consoleInputVerts;
	font.AddVertsForTextInBox2D(consoleInputVerts, AABB2(0 + shadowOffset, 0, bounds.m_maxs.x + shadowOffset, textHeight), textHeight, m_inputText, Rgba8::COLOR_BLACK, fontAspect, Vec2(0.f, 0.5f));
	font.AddVertsForTextInBox2D(consoleInputVerts, AABB2(0, 0, bounds.m_maxs.x, textHeight), textHeight, m_inputText, DevConsole::INPUT_TEXT, fontAspect, Vec2(0.f, 0.5f));
	renderer.BindTexture(&font.GetTexture());
	renderer.DrawVertexArray((int)consoleInputVerts.size(), consoleInputVerts.data());

	if (m_insertionPointVisible)
	{
		std::vector<Vertex_PCU> consoleInputInsertionPointVerts;

		AABB2 insertionBox;
		float insertionBoxWidth = textHeight * fontAspect / 4.5f;
		insertionBox.m_mins.x = bounds.m_mins.x + m_insertionPointPosition * textHeight * fontAspect;
		insertionBox.m_mins.y = bounds.m_mins.y;
		insertionBox.m_maxs.x = insertionBox.m_mins.x + insertionBoxWidth;
		insertionBox.m_maxs.y = insertionBox.m_mins.y + textHeight;

		if (insertionBox.m_mins.x >= bounds.m_maxs.x)
		{
			insertionBox.m_maxs.x = bounds.m_maxs.x;
			insertionBox.m_mins.x = insertionBox.m_maxs.x - insertionBoxWidth;
		}

		AddVertsForAABB2D(consoleInputInsertionPointVerts, insertionBox, DevConsole::INPUT_INSERTION_POINT);
	
		renderer.BindTexture(nullptr);
		renderer.DrawVertexArray((int)consoleInputInsertionPointVerts.size(), consoleInputInsertionPointVerts.data());
	}

}

DevConsole::DevConsole(DevConsoleConfig const& config)
	:m_config(config)
{
}

DevConsole::~DevConsole()
{
}

void DevConsole::Startup()
{
	if (m_config.m_renderer != nullptr)
	{
		m_font = m_config.m_renderer->CreateOrGetBitmapFont(m_config.m_fontFilePath.c_str());
	}
	m_camera = m_config.m_camera;
	g_theEventSystem->SubscribeEventCallbackFunction("CharInput", DevConsole::Event_CharInput);
	g_theEventSystem->SubscribeEventCallbackFunction("KeyPressed", DevConsole::Event_KeyPressed);
	g_theEventSystem->SubscribeEventCallbackFunction("help", DevConsole::Command_Help);
	g_theEventSystem->SubscribeEventCallbackFunction("clear", DevConsole::Command_Clear);
	g_theEventSystem->SubscribeEventCallbackFunction("echo", DevConsole::Command_Echo);
	g_theEventSystem->SubscribeEventCallbackFunction("timescale", DevConsole::Command_SetTimeScale);
	g_theEventSystem->SubscribeEventCallbackFunction("print", DevConsole::Command_Print);
	m_showFrameAndTime = false;
	m_insertionPointBlinkTimer = new Timer(0.5f, Clock::s_theSystemClock);
	m_insertionPointBlinkTimer->Start();
}

void DevConsole::BeginFrame()
{
	if (m_insertionPointBlinkTimer->DecrementPeriodIfElapsed())
	{
		m_insertionPointVisible = !m_insertionPointVisible;
	}
	m_frameNumber += 1;
	if (g_theInput->WasMouseWheelScrolledUp())
	{
		if (m_lines.size() < m_config.m_numLinesVisible) return;
		m_scrollOffset++;
	}
	if (g_theInput->WasMouseWheelScrolledDown())
	{
		if (m_lines.size() < m_config.m_numLinesVisible) return;
		m_scrollOffset--;
		if (m_scrollOffset < 0) m_scrollOffset = 0;
	}
}

void DevConsole::EndFrame()
{
}

void DevConsole::Shutdown()
{

}

void DevConsole::Execute(std::string const& consoleCommandText, bool echoCommand)
{
	EventArgs args;

	Strings lineList = SplitStringOnDelimiter(consoleCommandText, '\n');
	for (int lineIndex = 0; lineIndex < (int)lineList.size(); lineIndex++)
	{
		Strings pairList = SplitStringWithQuotes(lineList[lineIndex], ' ');
		std::string keyvalueHistory;
		for (int pairIndex = 1; pairIndex < (int)pairList.size(); pairIndex++)
		{
			Strings pairElements = SplitStringWithQuotes(pairList[pairIndex], '=');
			if (pairElements.size() > 1)
			{
				args.SetValue(pairElements[0], pairElements[1]);
				if (!pairElements[0].empty() && !pairElements[1].empty())
				{
					keyvalueHistory = pairElements[0] + "=" + pairElements[1];
				}
			}
		}

		FireEvent(ToLower(pairList[0]), args);

		if (echoCommand)
		{
			g_theDevConsole->AddLine(DevConsole::COMMAND_ECHO, consoleCommandText);
		}
	}	

	m_scrollOffset = 0;
}

void DevConsole::AddLine(Rgba8 const& color, std::string const& text)
{
	DevConsoleLine newLine;
	newLine.m_color = color;
	newLine.m_text = "> " + text;
	newLine.m_frameNumber = m_frameNumber;
	newLine.m_timestamp = GetCurrentTimeSeconds();
	m_lines.push_back(newLine);
}

void DevConsole::Render(AABB2 const& bounds, Renderer* rendererOverride) const
{
	if (!m_isOpen)
	{
		return;
	}
	m_camera->SetOrthographicView(bounds.m_mins, bounds.m_maxs);
	rendererOverride->BeginCamera(*m_camera);
	float aspect = 1.f / Window::GetMainWindowInstance()->GetAspect();
	Render_OpenFull(bounds, *rendererOverride, *m_font, aspect);
	rendererOverride->EndCamera(*m_camera);
}

int DevConsole::GetCurrentFrame()
{
	int frame = m_frameNumber;
	return frame;
}

void DevConsole::ToggleOpen()
{
	m_isOpen = !m_isOpen;
}

bool DevConsole::IsOpen()
{
	bool open = m_isOpen;
	return open;
}



