#pragma once
#include <array>
#include <vector>

static constexpr size_t NumInputs = 3;
/*
0. Distance to the closest bar
1. Angle to the closest bar (relative to facing)
2. Time spent at bar
*/

static constexpr size_t NumHidden0 = 8;
static constexpr size_t NumHidden1 = 8;
static constexpr size_t NumOutputs = 3;
/*
0. Turn left
1. Turn right
2. Speed
*/

using Layer = std::vector<double>;

class Brain
{
public:
	static Brain Random();
	static Brain Mutate(const Brain& parent);

	Layer Think(const Layer& stimuli);

	static std::vector<size_t> GetArchitecture();
	const std::array<Layer, 3>& GetWeights() const;

protected:
	std::array<Layer, 3> weights;

    static void InitializeLayer(Layer& layer, size_t size);
    static void MutateLayer(Layer& layer);
	static void ApplyLayer(Layer& out, const Layer& nodes, const Layer& layer);
};