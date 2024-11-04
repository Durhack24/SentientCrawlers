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

Layer Brain::Think(const Layer& stimuli)
{
    auto h0 = ApplyLayer(stimuli, weights[0]);
    auto h1 = ApplyLayer(h0, weights[1]);
    auto output = ApplyLayer(h1, weights[2]);

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

Layer Brain::ApplyLayer(const Layer& nodes, const Layer& layer)
{
    size_t weightWidth = nodes.size() + 1;
    size_t weightHeight = layer.size() / weightWidth;

    Layer ret(weightHeight);
    for (size_t outIdx = 0; outIdx < weightHeight; outIdx++)
    {
        double nodeValue = layer[weightWidth * outIdx]; // Start with the bias (weight in the first column)

        // Sum the weights multiplied by node values
        for (size_t inputIdx = 0; inputIdx < weightWidth - 1; inputIdx++)
            nodeValue += nodes[inputIdx] * layer[weightWidth * outIdx + inputIdx + 1];

        ret[outIdx] = activation(nodeValue);
    }

    return ret;
}