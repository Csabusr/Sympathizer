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
    osc(audioProcessor.apvts, "OSC1WAVETYPE"),
    adsr("Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"),
    fm(audioProcessor.apvts),
    filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES"),
    modAdsr("Mod Envelope", audioProcessor.apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);


    addAndMakeVisible(myDial);

    setSize(700, 600);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(fm);
    addAndMakeVisible(filter);
    addAndMakeVisible(modAdsr);
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

    adsr.setBounds(300, osc.getBottom() + 10, 400, 340);
    osc.setBounds(10, 10, 100, 30);
    fm.setBounds(0, osc.getBottom() + 10, 300, 300);
    filter.setBounds(10, adsr.getBottom() + 10, 350, 200);
    modAdsr.setBounds(filter.getRight(), adsr.getBottom() + 10, 350, 200);
}