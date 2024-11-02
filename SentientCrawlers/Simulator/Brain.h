#pragma once
#include <array>
#include <vector>

static constexpr size_t NumInputs = 7;
/*
0. Distance to closest bar
1. Angle to closest bar (relative to facing)
2. Time spent at bar
3. Distance to river
4. Angle to river (relative to facing)
5. Distance to closest bridge
6. Angle to closest bridge (relative to facing)
*/

static constexpr size_t NumHidden0 = 8;
static constexpr size_t NumHidden1 = 8;
static constexpr size_t NumOutputs = 3;
/*
0. Turn left
1. Turn right
2. Speed
*/

class Brain
{
protected:
	using Layer = std::vector<double>;

public:
	static Brain Random();
	static Brain Mutate(const Brain& parent);

	Layer Think(const Layer& stimuli);

protected:
	std::array<Layer, 3> weights;

    static void InitializeLayer(Layer& layer, size_t size);
    static void MutateLayer(Layer& layer);
	static Layer ApplyLayer(const Layer& nodes, const Layer& layer);
};