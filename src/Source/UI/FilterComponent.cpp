/*
  ==============================================================================

    FilterComponent.cpp
    Created: 5 Jan 2022 2:01:36pm
    Author:  Ryvenay

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"
#include "Colours.h"
#include "StyleSheet.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterCutoffId, juce::String filterResonanceId)
{
    juce::StringArray options{ "Low pass", "Band pass", "High pass" };
    filterTypeSelector.addItemList(options, 1);
    addAndMakeVisible(filterTypeSelector);

    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorId, filterTypeSelector);

    filterSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterSelectorLabel.setFont(15.0f);
    filterSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(filterSelectorLabel);


    setSliderWithLabel(filterCutoffSlider, filterCutoffLabel, apvts, filterCutoffId, filterCutoffAttachment);
    setSliderWithLabel(filterResonanceSlider, filterResonanceLabel, apvts, filterResonanceId, filterResonanceAttachment);

}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    g.fillAll(OurColours::BaseColour);
}

void FilterComponent::resized()
{
    const auto startY = 10;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    filterTypeSelector.setBounds(30, startY + 10, 90, 30);
    filterSelectorLabel.setBounds(filterTypeSelector.getX(), filterTypeSelector.getY() - labelYOffset, 90, labelHeight);

    filterCutoffSlider.setBounds(filterTypeSelector.getX(), filterTypeSelector.getBottom() + 40, sliderWidth, sliderHeight);
    filterCutoffLabel.setBounds(filterCutoffSlider.getX(), filterCutoffSlider.getY() - labelYOffset, filterCutoffSlider.getWidth(), labelHeight);

    filterResonanceSlider.setBounds(filterCutoffSlider.getRight()+20, filterCutoffSlider.getY(), sliderWidth, sliderHeight);
    filterResonanceLabel.setBounds(filterResonanceSlider.getX(), filterResonanceSlider.getY() - labelYOffset, filterResonanceSlider.getWidth(), labelHeight);


}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
{
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);

    slider.setSliderStyle(juce::CustomDial2::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::CustomDial2::TextBoxBelow,true,80,25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramId, slider);

}