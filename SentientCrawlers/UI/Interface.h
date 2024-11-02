#pragma once
#include <thread>
#include <memory>

#include "../ImGui/imgui.h"
#include "../Simulator/Simulator.h"

#include "Image.h"
#include "Renderer.h"
#include "TrainingRenderer.h"
#include "ShowcaseRenderer.h"

enum class SimulatorState
{
	Idle,
	RunningOneMinute,
	RunningOneGen,
	RunningAtMax
};

enum class InterfaceState
{
	Regular,
	ShowingBest
};

struct ShowBestState
{

};

class Interface
{
public:
	Interface();
	~Interface();

	void Render();

protected:
	bool open = true;
	std::unique_ptr<Image> mapImg;
	int crawlDuration = 300;
	std::unique_ptr<Renderer> renderer;

	// Statistics
	int maxBarsVisited = 0;

	// Simulator thread members
	volatile bool runSimThread = true;
	std::unique_ptr<Simulator> sim;
	volatile SimulatorState simState = SimulatorState::Idle;

	// Render helpers
	ImVec2 PointToScreen(ImVec2 canvasPos, ImVec2 canvasSize, Point p);
	void RenderBackdrop(ImDrawList* draw, ImVec2 pos, ImVec2 size);

	// Simulator thread methods
	void SimulatorThread();
	void RunOneMinute();
	void RunOneGen();
	void RunAtMax();

	std::thread simThread;
};