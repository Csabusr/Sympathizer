/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "UI/Colours.h"
using namespace juce;

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
    g.drawRoundedRectangle(20, 20, 410, 360, 5.0f, 2.0f);


    //FILTER
    g.setColour(OurColours::HighlightColor);
    int f1sx = 20;
    int f1sy = 440;
    int f1ex = 730;
    int f1yx = 360;

    g.drawRoundedRectangle(f1sx, f1sy, f1ex, f1yx, 5.0f, 2.0f);
    g.drawRoundedRectangle(f1sx + 10, f1sy, f1ex - 20, 20, 5.0f, 20.0f);
    g.drawRoundedRectangle(f1sx + 10, f1sy + 10, f1ex - 20, 10, 0.0f, 20.0f);

    g.setColour(OurColours::ScrewHeadBase);
    g.drawRoundedRectangle(f1ex - 10, f1sy + 5, 10, 10, 5.0f, 10.0f);

    g.setColour(OurColours::ScrewHeadBase2);
    Path ScrewCross;
    ScrewCross.startNewSubPath((float)f1ex - 10, (float)f1sy + 6);
    ScrewCross.lineTo((float)f1ex, (float)f1sy + 15);
    g.strokePath(ScrewCross, PathStrokeType(2.0f));
    Path ScrewCross2;
    ScrewCross2.startNewSubPath((float)f1ex, (float)f1sy + 6);
    ScrewCross2.lineTo((float)f1ex -10, (float)f1sy + 15);
    g.strokePath(ScrewCross2, PathStrokeType(2.0f));

    g.setColour(OurColours::BaseColour);
    g.setFont(30.0f);
    g.drawText("FILTER", 325, 440, 100, 20, juce::Justification::left);

    //OSC 1
    g.setColour(OurColours::HighlightColor);
    int o1sx = 20;
    int o1sy = 20;
    int o1ex = 410;
    int o1yx = 200;


    g.drawRoundedRectangle(o1sx, o1sy, o1ex, o1yx, 5.0f, 2.0f);
    g.drawRoundedRectangle(o1sx + 10, o1sy, o1ex - 20, o1yx - 180, 5.0f, 20.0f);
    g.drawRoundedRectangle(o1sx + 10, o1sy + 10, o1ex - 20, o1yx - 190, 0.0f, 20.0f);

    g.setColour(OurColours::BaseColour);
    g.setFont(30.0f);
    g.drawText("OSC1", 200, 20, 70, 20, juce::Justification::left);

    //OSC 2
    g.setColour(OurColours::HighlightColor);
    int o2sx = 430;
    int o2sy = 20;
    int o2ex = 320;
    int o2yx = 200;


    g.drawRoundedRectangle(o2sx, o2sy, o2ex, o2yx, 5.0f, 2.0f);
    g.drawRoundedRectangle(o2sx + 10, o2sy, o2ex - 20, o2yx - 180, 5.0f, 20.0f);
    g.drawRoundedRectangle(o2sx + 10, o2sy + 10, o2ex - 20, o2yx - 190, 0.0f, 20.0f);


    g.setColour(OurColours::BaseColour);
    g.setFont(30.0f);
    g.drawText("OSC2", 555, 20, 70, 20, juce::Justification::left);


    //OSC 3
    g.setColour(OurColours::HighlightColor);
    int o3sx = 430;
    int o3sy = 230;
    int o3ex = 320;
    int o3yx = 200;

    g.drawRoundedRectangle(o3sx, o3sy, o3ex, o3yx, 5.0f, 2.0f);
    g.drawRoundedRectangle(o3sx+10, o3sy, o3ex-20, o3yx-180, 5.0f, 20.0f);
    g.drawRoundedRectangle(o3sx+10, o3sy+10, o3ex-20, o3yx-190, 0.0f, 20.0f);

    g.setColour(OurColours::BaseColour);
    g.setFont(30.0f);
    g.drawText("OSC3", 555, 230, 70, 20, juce::Justification::left);

    //SYMPATHIZER

    g.setColour(OurColours::HighlightColor);
    g.setFont(40.0f);
    g.drawRoundedRectangle(20, 380, 410, 50, 5.0f, 2.0f);
    g.drawText("SYMPATHIZER", 100, 375, 250, 60, juce::Justification::left);

}


void SympathizerAudioProcessorEditor::resized()
{
    juce::CustomLNF myCustomLNF;

    //myDial.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
    osc1.setBounds(30, 20, 100, 30);
    adsr.setBounds(40, 235, 250, 160);
    osc1GainSlider.setBounds(30, 60, 150, 150);

    gainSlider.setBounds(adsr.getRight() + 20, 250 , 100, 100);
    fm.setBounds(osc1GainSlider.getRight() + 40, osc1GainSlider.getY(), 200, 150);

    osc2.setBounds(getWidth() / 2+50, 20, 100, 30);
    osc2GainSlider.setBounds((getWidth() / 2)+50, osc2.getBottom() + 10, 150, 150);
    osc2TuningSlider.setBounds(osc2GainSlider.getRight() + 10, osc2GainSlider.getY(), 150, 150);

    osc3.setBounds(getWidth() / 2+60, osc2TuningSlider.getBottom()+20, 100, 30);
    osc3GainSlider.setBounds((getWidth() / 2)+50, osc3.getBottom() + 10, 150, 150);
    osc3TuningSlider.setBounds(osc3GainSlider.getRight() + 10, osc3GainSlider.getY(), 150, 150);

    modAdsr.setBounds((getWidth() / 2), osc3TuningSlider.getBottom() + 70, 350, 200);

    filter.setBounds(25, osc3TuningSlider.getBottom(), 300, 190);

    midiKeyboardComponent.setBounds(20, 700, 730, 100);
}