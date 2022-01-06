/*
  ==============================================================================

    FMComponent.h
    Created: 29 Dec 2021 1:03:56pm
    Author:  Ryvenay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StyleSheet.h"

//==============================================================================
/*
*/
class FMComponent : public juce::Component
{
public:
    FMComponent(juce::AudioProcessorValueTreeState& apvts);
    ~FMComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::CustomDial depthSlider;
    juce::CustomDial frequencySlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> depthAttachment;
    std::unique_ptr<SliderAttachment> freqAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FMComponent)
};
