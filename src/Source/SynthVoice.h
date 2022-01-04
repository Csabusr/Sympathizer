/*
  ==============================================================================

    SynthVoice.h
    Created: 20 Dec 2021 11:46:58am
    Author:  Ryvenay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/AdsrData.h"
#include "Data/OscData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int newPitchWheelValue) override;

    void controllerMoved(int controllerNumber, int newControllerValue) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void update(const float attack, const float decay, const float sustain, const float release);

    OscData& getOscillator() { return osc; }

private:

    juce::AudioBuffer<float> synthBuffer;
    OscData osc;


    // sine wave
    //juce::dsp::Oscillator<float> osc{ [](float x) {return std::sin(x); } };

    // saw wave
    //juce::dsp::Oscillator<float> osc{ [](float x) {return x / juce::MathConstants<float>::pi; } };

    // square wave
    //juce::dsp::Oscillator<float> osc{ [](float x) {return x < 0.0f ? -1.0f : 1.0f; } };
    juce::dsp::Gain<float> gain;
    AdsrData adsr;

    bool isPrepared{ false };
};