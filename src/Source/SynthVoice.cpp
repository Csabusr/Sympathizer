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
    osc1.setWaveFrequency(midiNoteNumber);
    osc2.setWaveFrequency(midiNoteNumber + osc2tuning - 24);
    osc3.setWaveFrequency(midiNoteNumber + osc3tuning - 24);
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

    osc1.prepareToPlay(spec);
    osc2.prepareToPlay(spec);
    osc3.prepareToPlay(spec);

    adsr.setSampleRate(sampleRate);

    filter.perpareToPlay(sampleRate, samplesPerBlock, outputChannels);
    modAdsr.setSampleRate(sampleRate);

    osc1Gain.prepare(spec);
    osc2Gain.prepare(spec);
    osc3Gain.prepare(spec);
    

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

void SynthVoice::updateOsc1Gain(const float gain)
{
    osc1Gain.setGainLinear(gain);
}

void SynthVoice::updateOsc2Gain(const float gain)
{
    osc2Gain.setGainLinear(gain);
}

void SynthVoice::updateOsc3Gain(const float gain)
{
    osc3Gain.setGainLinear(gain);
}

void SynthVoice::updateOsc2Tuning(const int tuning)
{
    osc2tuning = tuning;
}

void SynthVoice::updateOsc3Tuning(const int tuning)
{
    osc3tuning = tuning;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);

    if (!isVoiceActive())
    {
        return;
    }

    osc1Buffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    osc2Buffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    osc3Buffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);

    //Ahhoz hogy működjön a mod, meg kell hívni az applyEnvelopeToBuffer fv-t. De mivel nem szeretnénk ilyen módon
    //használni, ezért üres buffer-re alkalmazzuk.
    modAdsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);


    osc1Buffer.clear();
    osc2Buffer.clear();
    osc3Buffer.clear();
    synthBuffer.clear();


    // AudioBlock létrehozása úgy, hogy átadjuk neki az AudioBuffert.
    // Az AudioBlock itt nem hoz létre új adatot, az adat amit felhasznál azonos lesz a bufferrel, azaz pointerként működik.

    juce::dsp::AudioBlock<float> osc1AudioBlock{ osc1Buffer };
    juce::dsp::AudioBlock<float> osc2AudioBlock{ osc2Buffer };
    juce::dsp::AudioBlock<float> osc3AudioBlock{ osc3Buffer };
    juce::dsp::AudioBlock<float> synthAudioBlock{ synthBuffer };

    // ProcessContextReplacing megadása -- Kicseréljük teljesen az inputotot új sample-ekre
    
    osc1.getNextAudioBlock(osc1AudioBlock);
    osc1Gain.process(juce::dsp::ProcessContextReplacing<float>(osc1AudioBlock));
    osc2.getNextAudioBlock(osc2AudioBlock);
    osc2Gain.process(juce::dsp::ProcessContextReplacing<float>(osc2AudioBlock));
    osc3.getNextAudioBlock(osc3AudioBlock);
    osc3Gain.process(juce::dsp::ProcessContextReplacing<float>(osc3AudioBlock));

    synthAudioBlock.add(osc1AudioBlock);
    synthAudioBlock.add(osc2AudioBlock);
    synthAudioBlock.add(osc3AudioBlock);


    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    filter.process(synthBuffer);
    

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

