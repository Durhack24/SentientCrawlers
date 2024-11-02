#pragma once
#include <thread>
#include <memory>

#include "../Simulator/Simulator.h"

enum class SimulatorState
{
	Idle,
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
	int crawlDuration = 300;
	std::unique_ptr<Simulator> sim;
	SimulatorState simState = SimulatorState::Idle;

	void SimulatorThread();
	void RunOneGen();
	void RunAtMax();

	std::thread simThread;
};