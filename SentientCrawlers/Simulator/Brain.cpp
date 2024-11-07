#include "Brain.h"

#include <random>

Brain Brain::Random()
{
    Brain brain;
    InitializeLayer(brain.weights[0], (NumInputs + 1) * NumHidden0);
    InitializeLayer(brain.weights[1], (NumHidden0 + 1) * NumHidden1);
    InitializeLayer(brain.weights[2], (NumHidden1 + 1) * NumOutputs);

	return brain;
}

Brain Brain::Mutate(const Brain& parent)
{
    Brain mutated = parent;
    for (auto& layer : mutated.weights)
        MutateLayer(layer);
    
    return mutated;
}

std::array<double, NumOutputs> Brain::Think(const Layer& stimuli)
{
    std::array<double, NumHidden0> h0;
    std::array<double, NumHidden1> h1;
    std::array<double, NumOutputs> output;

    ApplyLayer(h0.data(), stimuli.data(), stimuli.size(), weights[0]);
    ApplyLayer(h1.data(), h0.data(), h0.size(), weights[1]);
    ApplyLayer(output.data(), h1.data(), h1.size(), weights[2]);

    return output;
}

std::vector<size_t> Brain::GetArchitecture()
{
    return { NumInputs, NumHidden0, NumHidden1, NumOutputs };
}

const std::array<Layer, 3>& Brain::GetWeights() const
{
    return weights;
}

void Brain::InitializeLayer(Layer& layer, size_t size)
{
    static std::mt19937_64 gen{ std::random_device{}() };
    static std::uniform_real_distribution<double> dist{ -1, 1 };

    layer.reserve(size);
    for (size_t i = 0; i < size; ++i)
        layer.push_back(dist(gen));
}

void Brain::MutateLayer(Layer& layer)
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
        layer[i] *= 1 + (dist(gen) * 0.05);
    }
}

static inline double activation(double x)
{
    return 1.0 / (1.0 + exp(-x));
    //return std::max(-1.0, x);
    //return tanh(x);
}

void Brain::ApplyLayer(double* out, const double* nodes, size_t inputNum, const Layer& weights)
{
    size_t weightWidth = inputNum + 1;
    size_t weightHeight = weights.size() / weightWidth;

    for (size_t outIdx = 0; outIdx < weightHeight; outIdx++)
    {
        double nodeValue = weights[weightWidth * outIdx]; // Start with the bias (weight in the first column)

        // Sum the weights multiplied by node values
        for (size_t inputIdx = 0; inputIdx < weightWidth - 1; inputIdx++)
            nodeValue += nodes[inputIdx] * weights[weightWidth * outIdx + inputIdx + 1];

        out[outIdx] = activation(nodeValue);
    }
}