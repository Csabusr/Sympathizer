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
    SympathizerAudioProcessor& audioProcessor;
    AdsrComponent adsr;
    OscComponent osc;
    FMComponent fm;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SympathizerAudioProcessorEditor)
};
