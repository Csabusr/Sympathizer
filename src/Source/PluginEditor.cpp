/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "UI/Colours.h"

//==============================================================================
SympathizerAudioProcessorEditor::SympathizerAudioProcessorEditor(SympathizerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    osc1(audioProcessor.apvts, "OSC1WAVETYPE"),
    osc2(audioProcessor.apvts, "OSC2WAVETYPE"),
    adsr("Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"),
    fm(audioProcessor.apvts),
    filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES"),
    modAdsr("Mod Envelope", audioProcessor.apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);

    osc1GainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc1GainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

    osc2GainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc2GainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

    osc2TuningSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc2TuningSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);


    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

    osc1GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC1GAIN", osc1GainSlider);
    osc2GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC2GAIN", osc2GainSlider);

    osc2TuningAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC2TUNING", osc2TuningSlider);

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);



    setSize(800, 700);
    //addAndMakeVisible(myDial);
    
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc1GainSlider);
    addAndMakeVisible(osc2);
    addAndMakeVisible(osc2GainSlider);
    addAndMakeVisible(osc2TuningSlider);
    addAndMakeVisible(adsr);
    addAndMakeVisible(fm);
    addAndMakeVisible(filter);
    addAndMakeVisible(modAdsr);
    addAndMakeVisible(gainSlider);
}

SympathizerAudioProcessorEditor::~SympathizerAudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void SympathizerAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(OurColours::BaseColour);

}

void SympathizerAudioProcessorEditor::resized()
{
    juce::CustomLNF myCustomLNF;

    //myDial.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);

    osc1.setBounds(10, 10, 100, 30);
    osc2.setBounds(getWidth() / 2, 10, 100, 30);
    adsr.setBounds(10, osc1.getBottom() + 10, 200, 150);
    osc1GainSlider.setBounds(adsr.getRight() + 10, adsr.getY(), 150, 150);
    osc2GainSlider.setBounds(getWidth() /2, adsr.getY(), 150, 150);
    osc2TuningSlider.setBounds(getWidth() /2 + 150, adsr.getY(), 150, 150);

    fm.setBounds(10, adsr.getBottom() + 10, 300, 150);
    filter.setBounds(10, fm.getBottom() + 10, 350, 200);
    modAdsr.setBounds(filter.getRight(), fm.getBottom() + 10, 350, 200);
    gainSlider.setBounds(50, 600, 100, 100);
}