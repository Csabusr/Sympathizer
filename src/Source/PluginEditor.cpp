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
    osc3(audioProcessor.apvts, "OSC3WAVETYPE"),
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

    osc3GainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc3GainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

    osc2TuningSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc2TuningSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

    osc3TuningSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc3TuningSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);


    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

    osc1GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC1GAIN", osc1GainSlider);
    osc2GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC2GAIN", osc2GainSlider);
    osc3GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC3GAIN", osc3GainSlider);

    osc2TuningAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC2TUNING", osc2TuningSlider);
    osc3TuningAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC3TUNING", osc3TuningSlider);

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);



    setSize(900, 800);
    //addAndMakeVisible(myDial);
    
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc1GainSlider);
    addAndMakeVisible(osc2);
    addAndMakeVisible(osc2GainSlider);
    addAndMakeVisible(osc2TuningSlider);
    addAndMakeVisible(osc3);
    addAndMakeVisible(osc3GainSlider);
    addAndMakeVisible(osc3TuningSlider);
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
    //right side
    osc1.setBounds(getWidth() / 2, 10, 100, 30);
    osc1GainSlider.setBounds(getWidth() / 2, osc1.getY() + 30, 150, 150);
    fm.setBounds(osc1GainSlider.getRight() + 10, osc1GainSlider.getY() , 300, 150);
    osc2.setBounds(getWidth() / 2, 160, 100, 30);
    osc2GainSlider.setBounds(getWidth() / 2, osc2.getY() + 30, 150, 150);
    osc2TuningSlider.setBounds(osc2GainSlider.getRight() + 10, osc2GainSlider.getY(), 150, 150);
    osc3.setBounds(getWidth() / 2, 310, 100, 30);
    osc3GainSlider.setBounds(getWidth() / 2, osc3.getY() + 30, 150, 150);
    osc3TuningSlider.setBounds(osc3GainSlider.getRight() + 10, osc3GainSlider.getY(), 150, 150);

    modAdsr.setBounds(getWidth() / 2, osc3TuningSlider.getBottom() + 30, 350, 200);

    //left side
    gainSlider.setBounds(200, 10, 100, 100);
    adsr.setBounds(10, 10, 200, 150);
    filter.setBounds(10, adsr.getBottom() + 10, 350, 200);
}