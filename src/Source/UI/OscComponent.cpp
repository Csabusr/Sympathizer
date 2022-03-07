/*
  ==============================================================================

    OscComponent.cpp
    Created: 28 Dec 2021 5:16:23pm
    Author:  Ryvenay

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"
#include "Colours.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    juce::StringArray choices{ "Sine", "Saw", "Square", "Triangle"};
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);

}

OscComponent::~OscComponent()
{
}

void OscComponent::paint(juce::Graphics& g)
{
    g.fillAll(OurColours::BaseColour);

}

void OscComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    oscWaveSelector.setBounds(0, 0, 90, 20);
}
