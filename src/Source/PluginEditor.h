/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FMComponent.h"
#include "UI/StyleSheet.h"
#include "UI/FilterComponent.h"

//==============================================================================
/**
*/
class SympathizerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SympathizerAudioProcessorEditor(SympathizerAudioProcessor&);
    ~SympathizerAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::CustomLNF myCustomLNF;
    juce::CustomDial myDial;


    SympathizerAudioProcessor& audioProcessor;
    OscComponent osc1;
    OscComponent osc2;
    AdsrComponent adsr;
    FMComponent fm;
    FilterComponent filter;
    AdsrComponent modAdsr;

    juce::Slider osc1GainSlider;
    juce::Slider osc2GainSlider;

    juce::Slider osc2TuningSlider;

    juce::Slider gainSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2TuningAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SympathizerAudioProcessorEditor)
};
