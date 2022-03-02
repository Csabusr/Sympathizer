/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 28 Dec 2021 11:44:10am
    Author:  Ryvenay

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Colours.h"
#include "AdsrComponent.h"
#include "StyleSheet.h"

//==============================================================================

juce::AudioProcessorValueTreeState& rounde(juce::AudioProcessorValueTreeState& var)
{
    //TODO KEREKÍTÉS
    return var;
}


AdsrComponent::AdsrComponent(juce::String adsrName, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    juce::String name = adsrName;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    attackAttachment = std::make_unique<SliderAttachment>(rounde(apvts), attackId, attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(rounde(apvts), decayId, decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(rounde(apvts), sustainId, sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(rounde(apvts), releaseId, releaseSlider);



    setSliderParams(attackSlider);
    setSliderParams(decaySlider);
    setSliderParams(sustainSlider);
    setSliderParams(releaseSlider);
}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint(juce::Graphics& g)
{
    g.fillAll(OurColours::BaseColour);
}

void AdsrComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 15;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight();
    const auto sliderStartX = 0;
    const auto sliderStartY = 0;

    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
}

void AdsrComponent::setSliderParams(juce::Slider& slider)
{
    addAndMakeVisible(slider);
}

