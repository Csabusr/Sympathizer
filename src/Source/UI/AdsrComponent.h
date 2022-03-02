/*
  ==============================================================================

    AdsrComponent.h
    Created: 28 Dec 2021 11:44:10am
    Author:  Ryvenay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StyleSheet.h"

//==============================================================================
/*
*/
class AdsrComponent : public juce::Component
{
public:
    AdsrComponent(juce::String adsrName, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId);
    ~AdsrComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::String name = "";

    juce::CustomLinearSliderWithLabel attackSlider;
    juce::CustomLinearSliderWithLabel decaySlider;
    juce::CustomLinearSliderWithLabel sustainSlider;
    juce::CustomLinearSliderWithLabel releaseSlider;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    void setSliderParams(juce::Slider& slider);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdsrComponent)
};
