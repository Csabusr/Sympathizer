/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthSound.h"
#include "Data/FilterData.h"


//==============================================================================
SympathizerAudioProcessor::SympathizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
}

SympathizerAudioProcessor::~SympathizerAudioProcessor()
{

}

//==============================================================================
const juce::String SympathizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SympathizerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SympathizerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SympathizerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SympathizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SympathizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SympathizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SympathizerAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SympathizerAudioProcessor::getProgramName(int index)
{
    return {};
}

void SympathizerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void SympathizerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..


    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void SympathizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SympathizerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void SympathizerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            auto& osc1WaveType = *apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& osc2WaveType = *apvts.getRawParameterValue("OSC2WAVETYPE");
            auto& osc3WaveType = *apvts.getRawParameterValue("OSC3WAVETYPE");


            //Volume
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");

            //Modulation
            auto& modAttack = *apvts.getRawParameterValue("MODATTACK");
            auto& modDecay = *apvts.getRawParameterValue("MODDECAY");
            auto& modSustain = *apvts.getRawParameterValue("MODSUSTAIN");
            auto& modRelease = *apvts.getRawParameterValue("MODRELEASE");

            auto& fmDepth = *apvts.getRawParameterValue("FMDEPTH");
            auto& fmFreq = *apvts.getRawParameterValue("FMFREQ");

            //Filter
            auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
            auto& filterCutoff = *apvts.getRawParameterValue("FILTERCUTOFF");
            auto& filterRes = *apvts.getRawParameterValue("FILTERRES");

            //Gain
            auto& osc1Gain = *apvts.getRawParameterValue("OSC1GAIN");
            auto& osc2Gain = *apvts.getRawParameterValue("OSC2GAIN");
            auto& osc3Gain = *apvts.getRawParameterValue("OSC3GAIN");

            //Tuning
            auto& osc2Tuning = *apvts.getRawParameterValue("OSC2TUNING");
            auto& osc3Tuning = *apvts.getRawParameterValue("OSC3TUNING");

            voice->getOscillator1().setFmParams(fmDepth, fmFreq);
            voice->getOscillator1().setWaveType(osc1WaveType);
            voice->getOscillator2().setFmParams(fmDepth, fmFreq);
            voice->getOscillator2().setWaveType(osc2WaveType);
            voice->getOscillator3().setFmParams(fmDepth, fmFreq);
            voice->getOscillator3().setWaveType(osc3WaveType);

            voice->updateOsc2Tuning(osc2Tuning);
            voice->updateOsc3Tuning(osc3Tuning);


            voice->updateOsc1Gain(osc1Gain);
            voice->updateOsc2Gain(osc2Gain);
            voice->updateOsc3Gain(osc3Gain);

            voice->updateAdsr(attack.load(), decay.load(), sustain.load(), release.load());
            voice->updateFilter(filterType.load(), filterCutoff.load(), filterRes.load());
            voice->updateModAdsr(modAttack.load(), modDecay.load(), modSustain.load(), modRelease.load());
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    buffer.applyGain(*apvts.getRawParameterValue("GAIN"));
}

//==============================================================================
bool SympathizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SympathizerAudioProcessor::createEditor()
{
    return new SympathizerAudioProcessorEditor(*this);
}

//==============================================================================
void SympathizerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SympathizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SympathizerAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SympathizerAudioProcessor::createParameters()
{

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //osc
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 wave type", juce::StringArray{ "Sine", "Saw", "Square", "Triangle" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2WAVETYPE", "Osc 2 wave type", juce::StringArray{ "Sine", "Saw", "Square", "Triangle" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC3WAVETYPE", "Osc 3 wave type", juce::StringArray{ "Sine", "Saw", "Square", "Triangle" }, 0));

    //OSC Gain
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1GAIN", "Osc 1 gain", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2GAIN", "Osc 2 gain", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 0.8f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3GAIN", "Osc 3 gain", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 0.25f));

    //OSC Tuning
    params.push_back(std::make_unique<juce::AudioParameterInt>("OSC2TUNING", "Osc 2 tuning", 0, 48, 24));
    params.push_back(std::make_unique<juce::AudioParameterInt>("OSC3TUNING", "Osc 3 tuning", 0, 48, 0));

    //FM 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ", "FM Frequency", juce::NormalisableRange<float> {0.0f, 10.0f, 0.01f, 0.5f}, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH", "FM Depth", juce::NormalisableRange<float> {0.0f, 100.0f, 0.01f, 0.5f}, 0.0f));

    //Amplitude ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.0f, 1.0f, 0.001f, 0.6f }, 0.01f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.0f, 1.0f, 0.001f, 0.6f}, 0.01f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.0f,  1.0f, 0.001f, 0.6f}, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.0f, 3.0f, 0.001f, 0.6f}, 0.1f));

    //Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter type", juce::StringArray{ "Low pass", "Band pass", "High pass" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter cutoff freq", juce::NormalisableRange<float> {20.0f, 20000.0f, 1.0f, 0.5f}, 20000.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter resonance", juce::NormalisableRange<float> {1.0f, 10.0f, 0.1f}, 1.0f));

    //Filter ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODATTACK", "Mod Attack", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODDECAY", "Mod Decay", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODSUSTAIN", "Mod Sustain", juce::NormalisableRange<float> {0.1f, 1.0f}, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODRELEASE", "Mod Release", juce::NormalisableRange<float> {0.1f, 3.0f}, 0.4f));


    //Master Gain
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 0.5f));


    return { params.begin(), params.end() };
}