/*
  ==============================================================================

    OscData.cpp
    Created: 4 Jan 2022 4:11:34pm
    Author:  Ryvenay

  ==============================================================================
*/

#include "OscData.h"

#include "OscData.h"

void OscData::setWaveType(const int oscType)
{

    switch (oscType)
    {
    case 0:
        // Sine
        initialise([](float x) {return std::sin(x); });
        break;
    case 1:
        // Saw
        initialise([](float x) {return x / juce::MathConstants<float>::pi; });
        break;
    case 2:
        // square
        initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; });
        break;
    default:
        jassertfalse;
        break;
    }
}

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    fmOsc.prepare(spec);
    prepare(spec);
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    currentMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); s++)
        {
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OscData::setFmParams(const float depth, const float freqency)
{
    fmOsc.setFrequency(freqency);
    fmDepth = depth;
    auto currentFrequency = juce::MidiMessage::getMidiNoteInHertz(currentMidiNote) + fmMod;
    setFrequency(currentFrequency > 0.0f ? currentFrequency : 0.0f);
}