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
#include "Data/FilterData.h"

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

    void updateAdsr(const float attack, const float decay, const float sustain, const float release);

    void updateFilter(const int filterType, const float filterCutoff, const float filterRes);
    void updateModAdsr(const float attack, const float decay, const float sustain, const float release);

    void updateOsc1Gain(const float gain);
    void updateOsc2Gain(const float gain);
    void updateOsc3Gain(const float gain);

    void updateOsc2Tuning(const int tuning);
    void updateOsc3Tuning(const int tuning);


    OscData& getOscillator1() { return osc1; }
    OscData& getOscillator2() { return osc2; }
    OscData& getOscillator3() { return osc3; }

private:
    juce::AudioBuffer<float> osc1Buffer;
    juce::AudioBuffer<float> osc2Buffer;
    juce::AudioBuffer<float> osc3Buffer;
    juce::AudioBuffer<float> synthBuffer;

    OscData osc1;
    OscData osc2;
    OscData osc3;
    
    AdsrData adsr;
    FilterData filter;
    AdsrData modAdsr;

    int osc2tuning;
    int osc3tuning;


    juce::dsp::Gain<float> osc1Gain;
    juce::dsp::Gain<float> osc2Gain;
    juce::dsp::Gain<float> osc3Gain;

    bool isPrepared{ false };
};