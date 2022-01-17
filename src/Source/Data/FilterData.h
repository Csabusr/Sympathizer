/*
  ==============================================================================

    FilterData.h
    Created: 5 Jan 2022 1:01:36pm
    Author:  Ryvenay

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FilterData
{
public:
    void perpareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParams(const int type, const float freq, const float resonance, const float mod = 1.0f);
    void reset();


private:
    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared = false;

};