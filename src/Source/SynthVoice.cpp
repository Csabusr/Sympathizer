/*
  ==============================================================================

    SynthVoice.cpp
    Created: 20 Dec 2021 11:46:58am
    Author:  Ryvenay

  ==============================================================================
*/

#include "SynthVoice.h"

// Returns true, if we have sound loaded and we are able to play
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
    if (!allowTailOff || !adsr.isActive())
    {
        clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);

    // ProcessSpec osztály használata a könnyű specifikáció átadásnak. 
    // Segítségével átadjuk a paramétereket az oszcillátornak. 

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepareToPlay(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.3f);

    isPrepared = true;
}

void SynthVoice::update(const float attack, const float decay, const float sustain, const float release)
{
    adsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);

    if (!isVoiceActive())
    {
        return;
    }

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // AudioBlock létrehozása úgy, hogy átadjuk neki az AudioBuffert.
    // Az AudioBlock itt nem hoz létre új adatot, az adat amit felhasznál azonos lesz a bufferrel, azaz pointerként működik.

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };

    // ProcessContextReplacing megadása -- Kicseréljük teljesen az inputoto új sample-ekre
    osc.getNextAudioBlock(audioBlock);

    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        if (!adsr.isActive())
        {
            clearCurrentNote();
        }
    }
}

