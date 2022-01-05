/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SympathizerAudioProcessorEditor::SympathizerAudioProcessorEditor(SympathizerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), adsr(audioProcessor.apvts), 
    osc(audioProcessor.apvts, "OSC1WAVETYPE"), fm(audioProcessor.apvts), 
    filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(700, 300);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(fm);
    addAndMakeVisible(filter);
}

SympathizerAudioProcessorEditor::~SympathizerAudioProcessorEditor()
{
}

//==============================================================================
void SympathizerAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

}

void SympathizerAudioProcessorEditor::resized()
{
    adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);
    osc.setBounds(10, 10, 100, 30);
    fm.setBounds(0, osc.getBottom() + 10, getWidth() / 2, getHeight() / 2 - 10);
    filter.setBounds(adsr.getX(), adsr.getBottom(), adsr.getWidth(), adsr.getHeight());
}