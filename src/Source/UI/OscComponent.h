/*
  ==============================================================================

    OscComponent.h
    Created: 28 Dec 2021 5:16:23pm
    Author:  Ryvenay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StyleSheet.h"

//==============================================================================
/*
*/
class OscComponent : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId);
    ~OscComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::drawCustomComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent)
};
