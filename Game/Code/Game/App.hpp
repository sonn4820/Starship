#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
class App {

public:
	App();
	~App();

	void Startup();
	void Shutdown();
	void Run();
	void RunFrame();

	bool IsQuitting() const { return m_isQuitting; }
	bool HandleQuitRequested();

public:
	void BeginFrame();
	void Update(float deltaSeconds);
	void Render() const;
	void EndFrame();
	static bool Event_Quit(EventArgs& args);

	bool m_isQuitting = false;
	
	Clock* m_gameClock = nullptr;
	float accumulator;
private:

	Game* m_game = nullptr;

	void ConsoleTutorial();
};