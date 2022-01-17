/*
  ==============================================================================

    FilterData.cpp
    Created: 5 Jan 2022 1:01:36pm
    Author:  Ryvenay

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::perpareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
    filter.reset();
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;

    filter.prepare(spec);
    isPrepared = true;
}

void FilterData::process(juce::AudioBuffer<float>& buffer)
{

    jassert(isPrepared); //filter is not prepared, call prepareToPlay function!

    juce::dsp::AudioBlock<float> filterBlock{ buffer };


    filter.process(juce::dsp::ProcessContextReplacing<float>{ filterBlock });
}

void FilterData::updateParams(const int type, const float freq, const float resonance, const float mod)
{
    switch (type)
    {
    case 0:
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;

    case 1:
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;

    case 2:
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    }

    float modFreq = freq * mod;
    modFreq = std::fmax(modFreq, 20.f);
    modFreq = std::fmin(modFreq, 20000.0f);

    filter.setCutoffFrequency(modFreq);
    filter.setResonance(resonance);

}

void FilterData::reset()
{
    filter.reset();
}