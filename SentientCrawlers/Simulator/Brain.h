#pragma once
#include <array>
#include <vector>

static constexpr size_t NumInputs = 6;
static constexpr size_t NumHidden0 = 8;
static constexpr size_t NumHidden1 = 8;
static constexpr size_t NumOutputs = 3;

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