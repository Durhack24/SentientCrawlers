#include "Brain.h"

#include <random>

Brain Brain::Random()
{
    std::vector<double> inputs;
    std::vector<double> layer0;
    std::vector<double> layer1;

    InitializeLayer(inputs, (NumInputs + 1) * NumHidden0);
    InitializeLayer(inputs, (NumHidden0 + 1) * NumHidden1);
    InitializeLayer(inputs, (NumHidden1 + 1) * NumOutputs);

    Brain brain;
    brain.weights[0] = inputs;
    brain.weights[1] = layer0;
    brain.weights[2] = layer1;
	return brain;
}

Brain Brain::Mutate(const Brain& parent)
{
    Brain mutated = parent;
    for (auto& layer : mutated.weights)
        MutateLayer(layer);

	return mutated;
}

std::vector<double> Brain::Think(const std::vector<double>& stimuli)
{
	return std::vector<double>();
}

void Brain::InitializeLayer(std::vector<double>& layer, size_t size)
{
    static std::mt19937_64 gen{ std::random_device{}() };
    static std::uniform_real_distribution<double> dist{ -1, 1 };

    layer.reserve(size);
    for (size_t i = 0; i < size; ++i)
        layer.push_back(dist(gen));
}

void Brain::MutateLayer(std::vector<double>& layer)
{
    static std::mt19937_64 gen{ std::random_device{}() };
    static std::uniform_real_distribution<double> dist{-1, 1 };

    static std::uniform_real_distribution<float> probability{0.0f, 1.0f };

    for (size_t i = 0; i < layer.size(); ++i)
    {
        // 25% chance of mutating a weight
        if (probability(gen) > 0.25)
            continue;
        // 5% chance that we completely randomize the weight
        if (probability(gen) > 0.95)
        {
            layer[i] = dist(gen);
            continue;
        }
        // We just shift the weight a little bit
        layer[i] += dist(gen) * 0.05;
    }
}