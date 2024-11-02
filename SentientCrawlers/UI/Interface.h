#pragma once
#include <thread>
#include <memory>

#include "Image.h"
#include "../Simulator/Simulator.h"

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
	std::unique_ptr<Image> mapImg;

	int crawlDuration = 300;
	std::unique_ptr<Simulator> sim;
	SimulatorState simState = SimulatorState::Idle;

	void SimulatorThread();
	void RunOneMinute();
	void RunOneGen();
	void RunAtMax();

	std::thread simThread;
};