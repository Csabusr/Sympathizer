/*
  ==============================================================================

    FMComponent.cpp
    Created: 29 Dec 2021 1:03:56pm
    Author:  Ryvenay

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FMComponent.h"
#include "Colours.h"

//==============================================================================
FMComponent::FMComponent(juce::AudioProcessorValueTreeState& apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    depthAttachment = std::make_unique<SliderAttachment>(apvts, "FMDEPTH", depthSlider);
    freqAttachment = std::make_unique<SliderAttachment>(apvts, "FMFREQ", frequencySlider);

    depthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    frequencySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(depthSlider);
    addAndMakeVisible(frequencySlider);
}

FMComponent::~FMComponent()
{
}

void FMComponent::paint(juce::Graphics& g)
{

    g.fillAll(OurColours::BaseColour);   // clear the background
}

void FMComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;

    depthSlider.setBounds(0, 0, bounds.getWidth() / 2, bounds.getHeight());
    frequencySlider.setBounds(depthSlider.getRight() + padding, 0, bounds.getWidth() / 2, bounds.getHeight());


}
