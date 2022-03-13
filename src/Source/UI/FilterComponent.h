/*
  ==============================================================================

    FilterComponent.h
    Created: 5 Jan 2022 2:01:36pm
    Author:  Ryvenay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StyleSheet.h"

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterFreqId, juce::String filterResId);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::drawCustomComboBox filterTypeSelector;
    juce::CustomDial2 filterCutoffSlider;
    juce::CustomDial2 filterResonanceSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;


    std::unique_ptr<Attachment> filterCutoffAttachment;
    std::unique_ptr<Attachment> filterResonanceAttachment;

    juce::Label filterCutoffLabel{"fltr_cutoff_freq", "Filter cutoff freq"};
    juce::Label filterResonanceLabel{"fltr_res", "Filter resonance"};


    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts,
        juce::String paramId, std::unique_ptr<Attachment>& attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
