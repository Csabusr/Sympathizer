/*
  ==============================================================================

	StyleSheet.h
	Created: 4 Jan 2022 6:11:56pm
	Author:  FCsaba

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Colours.h"

namespace juce
{

	class CustomLNF : public juce::LookAndFeel_V4
	{
	public:
		void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider);
		void drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider);
		void drawRotarySliderSmall(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider);
	};





	class CustomDial : public juce::Slider
	{
	public:
		CustomDial()
		{
			auto rotaryParams = getRotaryParameters();
			rotaryParams.startAngleRadians = juce::MathConstants<float>::pi * 1.5f;
			rotaryParams.endAngleRadians = juce::MathConstants<float>::pi * 3.0f;
			setRotaryParameters(rotaryParams);
			setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
			setRange(0.0f, 100.0f);
			setValue(25.0f);
			setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);

			setColour(juce::Slider::ColourIds::rotarySliderFillColourId, OurColours::HighlightColor);
			setColour(juce::Slider::ColourIds::thumbColourId, OurColours::HighlightColor);
		}

	private:
	};

	class CustomDial2 :public juce::Slider
	{
	public:
		CustomDial2()
		{
			auto rotaryParams = getRotaryParameters();
			rotaryParams.startAngleRadians = juce::MathConstants<float>::pi * 1.5f;
			rotaryParams.endAngleRadians = juce::MathConstants<float>::pi * 3.0f;
			setRotaryParameters(rotaryParams);
			setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
			setRange(0.0f, 100.0f);
			setValue(25.0f);
			setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);

			setColour(juce::Slider::ColourIds::rotarySliderFillColourId, OurColours::HighlightColor);
			setColour(juce::Slider::ColourIds::thumbColourId, OurColours::HighlightColor);
			setColour(juce::Slider::ColourIds::trackColourId, OurColours::BaseColourBrighter);
			setColour(juce::Slider::ColourIds::backgroundColourId, OurColours::BaseColourBrightest);
		}

	private:
	};

	class CustomLinearSliderWithLabel :public juce::Slider
	{
	public:
		CustomLinearSliderWithLabel()
		{
			setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
			setRange(0.0f, 100.0f);
			setValue(25.0f);
			setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);

			setColour(juce::Slider::ColourIds::rotarySliderFillColourId, OurColours::HighlightColor);
			setColour(juce::Slider::ColourIds::thumbColourId, OurColours::HighlightColor);
			setColour(juce::Slider::ColourIds::trackColourId, OurColours::BaseColourBrighter);
			setColour(juce::Slider::ColourIds::backgroundColourId, OurColours::BaseColourBrightest);
		}
	private:
	};
}