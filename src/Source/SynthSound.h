/*
  ==============================================================================

    SynthSound.h
    Created: 20 Dec 2021 11:47:33am
    Author:  Ryvenay

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }

    bool appliesToChannel(int midiChannel) override { return true; }
};

