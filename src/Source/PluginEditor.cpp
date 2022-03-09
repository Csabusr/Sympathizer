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
    modAdsr("Mod Envelope", audioProcessor.apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE"),
    midiKeyboardComponent(audioProcessor.keyboardState,juce::MidiKeyboardComponent::Orientation::horizontalKeyboard)
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



    setSize(770, 820);
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
    addAndMakeVisible(midiKeyboardComponent);
}

SympathizerAudioProcessorEditor::~SympathizerAudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void SympathizerAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(OurColours::BaseColour);
    g.setColour(OurColours::HighlightColor);

    g.setFont(20.0f);
    g.drawText("FILTER", 345, 440, 60, 20, juce::Justification::left);
    g.drawRoundedRectangle(20, 430, 730, 260, 5.0f, 2.0f);

    g.drawText("FILTER", 345, 440, 60, 20, juce::Justification::left);
    g.drawRoundedRectangle(20, 10, 410, 370, 5.0f, 2.0f);

    g.drawText("OSC2", 565, 20, 50, 20, juce::Justification::left);
    g.drawRoundedRectangle(430, 10, 320, 210, 5.0f, 2.0f);

    g.drawText("OSC3", 565, 230, 50, 20, juce::Justification::left);
    g.drawRoundedRectangle(430, 220, 320, 210, 5.0f, 2.0f);

    g.setFont(40.0f);

    g.drawRoundedRectangle(20, 380, 410, 50, 5.0f, 2.0f);
    g.drawText("SYMPATHIZER", 100, 375, 250, 60, juce::Justification::left);

}


void SympathizerAudioProcessorEditor::resized()
{
    juce::CustomLNF myCustomLNF;

    //myDial.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
    osc1.setBounds(30, 20, 100, 30);
    adsr.setBounds(osc1.getX()+10, osc1.getBottom() + 10, 200, 150);
    osc1GainSlider.setBounds(30, adsr.getBottom()+ 10, 150, 150);
    gainSlider.setBounds(adsr.getRight() + 20, 80, 100, 100);
    fm.setBounds(osc1GainSlider.getRight() + 40, osc1GainSlider.getY(), 200, 150);

    osc2.setBounds(getWidth() / 2+50, 20, 100, 30);
    osc2GainSlider.setBounds((getWidth() / 2)+50, osc2.getBottom() + 10, 150, 150);
    osc2TuningSlider.setBounds(osc2GainSlider.getRight() + 10, osc2GainSlider.getY(), 150, 150);

    osc3.setBounds(getWidth() / 2+60, osc2TuningSlider.getBottom()+20, 100, 30);
    osc3GainSlider.setBounds((getWidth() / 2)+50, osc3.getBottom() + 10, 150, 150);
    osc3TuningSlider.setBounds(osc3GainSlider.getRight() + 10, osc3GainSlider.getY(), 150, 150);

    modAdsr.setBounds((getWidth() / 2), osc3TuningSlider.getBottom() + 50, 350, 200);

    filter.setBounds(25, osc3TuningSlider.getBottom() + 40, 300, 190);

    midiKeyboardComponent.setBounds(20, 700, 730, 100);
}