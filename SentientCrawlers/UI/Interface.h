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
	RunningNonstop
};

using Percentiles = std::array<double, 11>;

struct GraphData
{
	double maxY = -INFINITY;
	Brain bestBrain;
	std::vector<std::pair<size_t, Percentiles>> points;
};

class Interface
{
public:
	Interface();
	~Interface();

	void Render();

protected:
	std::unique_ptr<Renderer> renderer;

	// UI state
	bool open = true;
	std::unique_ptr<Image> mapImg;
	int crawlDuration = 300, xStartPos = 620, yStartPos = 1030 - 950, startDir = 0;
	bool showNetworkVisualizer = false;

	// Statistics
	int maxBarsVisited = 0;
	GraphData graphData;

	// Simulator thread members
	volatile bool runSimThread = true;
	std::unique_ptr<Simulator> sim;
	volatile SimulatorState simState = SimulatorState::Idle;

	// UI Components
	void Sidebar();
	void Canvas();
	void NetworkVisualizer();

	// Render helpers
	ImVec2 PointToScreen(ImVec2 canvasPos, ImVec2 canvasSize, Point p);
	void RenderGraph(ImDrawList* draw, ImVec2 pos, ImVec2 size);
	void RenderBackdrop(ImDrawList* draw, ImVec2 pos, ImVec2 size);

	void UpdateStatistics();

	// Simulator thread methods
	void SimulatorThread();
	void RunOneMinute();
	void RunOneGen();
	void RunNonstop();

	// General Util
	static double ToRadians(int deg);

	std::thread simThread;
};