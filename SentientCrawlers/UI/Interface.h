#pragma once
#include <thread>
#include <memory>

#include "Image.h"
#include "../Simulator/Simulator.h"

#include "../ImGui/imgui.h"

enum class SimulatorState
{
	Idle,
	RunningOneMinute,
	RunningOneGen,
	RunningAtMax
};

class Interface
{
public:
	Interface();
	~Interface();

	void Render();

protected:
	bool open = true;
	volatile bool runSimThread = true;
	std::unique_ptr<Image> mapImg;

	int crawlDuration = 300;
	std::unique_ptr<Simulator> sim;
	volatile SimulatorState simState = SimulatorState::Idle;

	ImVec2 PointToScreen(ImVec2 canvasPos, ImVec2 canvasSize, Point p);

	void SimulatorThread();
	void RunOneMinute();
	void RunOneGen();
	void RunAtMax();

	std::thread simThread;
};