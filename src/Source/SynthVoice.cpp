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
    modAdsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
    modAdsr.noteOff();
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
    // ProcessSpec osztály használata a könnyű specifikáció átadásnak. 
    // Segítségével átadjuk a paramétereket az oszcillátornak. 

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepareToPlay(spec);

    adsr.setSampleRate(sampleRate);

    filter.perpareToPlay(sampleRate, samplesPerBlock, outputChannels);
    modAdsr.setSampleRate(sampleRate);

    gain.prepare(spec);
    gain.setGainLinear(0.3f);

    isPrepared = true;
}

void SynthVoice::updateAdsr(const float attack, const float decay, const float sustain, const float release)
{
    adsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::updateFilter(const int filterType, const float filterCutoff, const float filterRes)
{
    float modulator = modAdsr.getNextSample();
    filter.updateParams(filterType, filterCutoff, filterRes, modulator);
}

void SynthVoice::updateModAdsr(const float attack, const float decay, const float sustain, const float release)
{
    modAdsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);

    if (!isVoiceActive())
    {
        return;
    }

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);

    //Ahhoz hogy működjön a mod, meg kell hívni az applyEnvelopeToBuffer fv-t. De mivel nem szeretnénk ilyen módon
    //használni, ezért üres buffer-re alkalmazzuk.
    modAdsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);
    synthBuffer.clear();

    // AudioBlock létrehozása úgy, hogy átadjuk neki az AudioBuffert.
    // Az AudioBlock itt nem hoz létre új adatot, az adat amit felhasznál azonos lesz a bufferrel, azaz pointerként működik.

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };

    // ProcessContextReplacing megadása -- Kicseréljük teljesen az inputoto új sample-ekre
    osc.getNextAudioBlock(audioBlock);
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    filter.process(synthBuffer);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    

    for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        if (!adsr.isActive())
        {
            clearCurrentNote();
        }
    }

    modAdsr.getNextSample();

}

