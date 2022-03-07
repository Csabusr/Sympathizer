/*
  ==============================================================================

    MidiKeyboardComponent.h
    Created: 7 Mar 2022 10:23:36am
    Author:  FCsaba

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "StyleSheet.h"
//#include "juce_MidiKeyboardComponent.h"

class MidiKeyboardComponent : public juce::Component
{
public:
    //MidiKeyboardComponent(juce::MidiKeyboardState& state, Orientation orientation);
    ~MidiKeyboardComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiKeyboardComponent)
};